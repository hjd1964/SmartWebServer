// Added align to M button, direction button swap, changed guide rate handling, focus parking
//  and sound function 
// -----------------------------------------------------------------------------------
// BLE Gamepad support v2.0
// by Drew Bolce'

#include "BleGamepad.h"

#if defined(BLE_GAMEPAD) && (BLE_GAMEPAD == ON && ESP32)

  #include <BLEDevice.h>
  #include "../cmd/Cmd.h"
  #include "../status/Status.h"

  //******************************************************************************
  // Configuration settings
  //******************************************************************************

  #define REV -2 // macro definition

  // Direction button swaps
  #define NS_SWAP              OFF      // ON reverses direction buttons from OnStep standard
  #define EW_SWAP              OFF      // East/West swap

  #define LED_ENABLE            ON      // LED on/off behavior, OFF disables LED BLE connected/disconnected
                                        // indication. ON = LED off when connected, on when disconnected
                                        // REV = LED on when connected, off when disconnected

  #define FOCUS_INIT            ON      // Focuser moves to half-position on M button "Start tracking" press,
                                        // moves to "0" position on "M" button "Park". OFF disables feature

  #define SOUND                 ON      // ON Enables beeps and alerts

  //******************************************************************************

  // ===== GamePad Commands =====
  // Only commandBlind() is used to process these since it is ESP32 thread safe 
  // (unlike commandBool() etc.) and still reads any response as appropriate.
  #define FOCUS_IN          ":F-#"
  #define FOCUS_OUT         ":F+#"
  #define FOCUS_LOW         ":F3#"     // Set speed 3 (100um) for visual
  #define FOCUS_HIGH        ":F4#"     // High speed, 0.5x
  #define FOCUS_STOP        ":FQ#"
  #define SPIRAL            ":Mp#"     // Spiral search
  #define HOMEFOCUS         ":Fh#"     // Move focus to Home (1/2 max position)
  #define HOMEZERO          ":FS0#"    // Move focus to 0 position
  #define PARKFOCUS         ":FZ#"     // Set focus position to 0
  #define PARK              ":hP#"  
  #define UNPARK            ":hR#"
  #define TRACK_ON          ":Te#"
  #define TRACK_OFF         ":Td#"
  #define STOP_ALL           ":Q#"     // Emergency stop
  #define BEEP          ":SX97,2#"     // Short beep
  #define ALERT         ":SX97,3#"     // Alert, full 1 second blast
  #define CLICK         ":SX97,4#"     // Click
  #define GOTO_CURRENT      ":MS#"     // Returns to last Goto location
  #define ALIGN             ":A+#"     // Accept alignment location

  #define JoyStickDeadZone       0     // Increase if motors not stopping when in center position

  // ===== Timer values in milliseconds ======
  #define JOYTIMEOUT           100     // Joystick no activity timeout
  #define SCANTIMER          15000     // Number of ms before rescanning
  #define FOCUSTIMER          5000     // Focus speedup timer

  //------ GamePad Definitions -----
  enum
  {
    GP_TRIGGERS = 0,
    GP_JOYX,
    GP_JOYY,
    GP_BTNAB,
    GP_BTNCDM,
    GP_NUMBYTES
  };

  // ===== GamePad Button Masks =====

  #define GP_BUTTON_UP        0x00
  #define GP_LOW_TRIGGER      0x01
  #define GP_UPR_TRIGGER      0x02
  #define GP_BUTTON_A         0x01
  #define GP_BUTTON_B         0x02
  #define GP_BUTTON_C         0x01
  #define GP_BUTTON_D         0x02
  #define GP_BUTTON_M         0x00
  #define FRESHFLAG           0x80

  // We will connect to server(GamePad) by MAC address
  static BLEAddress *Server_BLE_Address; 
  String My_BLE_Address; 
  String My_BLE_Address1;
  String Scaned_BLE_Address;

  // All four modes send data. However each mode uses different byte positions and
  // values for each of the switches. We are using Mode "D". This allows use of all buttons.
  
  typedef void (*NotifyCallback)(BLERemoteCharacteristic*, uint8_t*, size_t, bool);

  // This is the service UUID of the mouse/joystick device (HID)
  static BLEUUID serviceUUID("00001812-0000-1000-8000-00805f9b34fb");

  // Battery Service UUID - not used for now
  //static BLEUUID BatteryServiceUUID("0000180F-0000-1000-8000-00805f9b34fb");

  // This characteristic UUID works for joystick & triggers (report)
  static BLEUUID ReportCharUUID("00002A4D-0000-1000-8000-00805f9b34fb"); 

  // Control flags
  static bool movingNorth = false;
  static bool movingSouth = false;
  static bool movingWest = false;
  static bool movingEast = false;
  static bool triggerPress = false; // Focus triggers
  static bool timerReturn = false; // Focus timer for high speed
  static bool pressedOnce = false; // We use the second signal
  static bool FocusSpd = false; // Focus speed toggle flag
  static bool SpiralInProgess = false; // Spiral search start/stop flag
  static bool CDMpressedOnce = false; // A sort of debounce
  static bool doConnect = false; // Run the connection routine
  static bool Connected = false; // Fully connected to the GamePad
  static bool doScan = false; // Run the scan routine
  static bool havedevice = false; // Found one of our GamePads
  static bool firstrate = true; // Allow rate change during alignment

  // BLE device model
  static BLEAdvertisedDevice* myDevice;
  static BLEClient *pClient = NULL;

  // Pointer to a list of characteristics of the active service,
  // sorted by characteristic UUID
  std::map<std::string, BLERemoteCharacteristic*> *pmap;
  std::map<std::string, BLERemoteCharacteristic*> :: iterator itr;

  // Pointer to a list of characteristics of the active service,
  // sorted by characteristic handle
  std::map<std::uint16_t, BLERemoteCharacteristic*> *pmapbh;
  std::map<std::uint16_t, BLERemoteCharacteristic*> :: iterator itrbh;

  // Storage for pointers to characteristics we want to work with
  BLERemoteCharacteristic *bleRcs[4];

  // This is where we store the data from the buttons and joystick
  volatile byte   VrBoxData[GP_NUMBYTES];

  // ------ Timers, millisecond re-triggerable ------ 

  // joyTimer sets the joystick back to center if there is
  // no activity on the joystick or trigger buttons. 
  volatile uint32_t joyTimer = millis();

  // pushTimer sets the amount of time a key must be pressed 
  // before a button state change is triggered
  volatile uint32_t pushTimer = millis();

  // scanTimer sets the amount of time between rescans on 
  // device disconnect
  volatile uint32_t scanTimer = millis();

  // Task handles  
  TaskHandle_t HandleJS = NULL;   // handle of the joystick task
  TaskHandle_t HandleAB = NULL;   // handle of the A/B button task
  TaskHandle_t HandleCDM = NULL;  // handle of the C/D/M button task

  // GuideRate up/down
  int activeGuideRate = 7;        // Scope, 2 to 9, 7 = 48X

  // Character buffer
  char bfr[80];

  // Task Stack memory allocation size
  #define TaskStackSize     4096

  // ----- Focuser functons -----
  void StartFocus()
  {
    if (FOCUS_INIT == ON) 
    {           
    onStep.commandBlind(PARKFOCUS);  //Set 0 position  
    onStep.commandBlind(HOMEFOCUS); // Move half way
    }
  }
  void ParkFocus()
  {
    if (FOCUS_INIT == ON) 
    {           
    onStep.commandBlind(HOMEZERO); //Move focus to 0 position   
    onStep.commandBlind(PARKFOCUS); // Set 0 position
    }
  }

  // ------ LED functions -----
  void LED_STATE_A()
  {
    if (LED_ENABLE == ON) digitalWrite(LED_STATUS_PIN, LED_STATUS_OFF_STATE);
    else if (LED_ENABLE == REV) digitalWrite(LED_STATUS_PIN, LED_STATUS_ON_STATE);
  }
  void LED_STATE_B()
  {
    if (LED_ENABLE == ON) digitalWrite(LED_STATUS_PIN, LED_STATUS_ON_STATE);
    else if (LED_ENABLE == REV) digitalWrite(LED_STATUS_PIN, LED_STATUS_OFF_STATE);
  }
 
  // ------ Sound functions -----
  void Beep()
  {
    if (SOUND == ON) onStep.commandBlind(BEEP);
  }
  void Alert() 
  {
    if (SOUND == ON) onStep.commandBlind(ALERT);
  }
    void Click()
  {
    if (SOUND == ON) onStep.commandBlind(CLICK);
  }

  // ------ Direction functions ------
  void MoveNorth() // Move North
  {
      if (!movingNorth)
      {
        movingNorth = ON;
        onStep.commandBlind(":Mn#");
      }
  }
  void MoveSouth() // Move South 
  {
      if (!movingSouth)
      {
        movingSouth = ON;
        onStep.commandBlind(":Ms#");
      }
  }
  void MoveEast() // Move East 
  {
      if (!movingEast)
      {
        movingEast = ON;
        onStep.commandBlind(":Me#");
      }
  }
  void MoveWest() // Move West 
  {
      if (!movingWest)
      {
        movingWest = ON;
        onStep.commandBlind(":Mw#");
      }
  }

  //******************************************************************************
  // HID notification callback handler.
  //******************************************************************************
  static void notifyCallback(
    BLERemoteCharacteristic* pBLERemoteCharacteristic,
    uint8_t* pData,
    size_t length,
    bool isNotify)
  {
    // We are getting the two trigger buttons in the first byte, joyX & joyY in 2nd & 3rd bytes
    // A four byte report is the joystick/trigger buttons.
    // A two byte report is either the A/B buttons or the C/D buttons
    // Low nibble equal to 0x50 indicates A/B buttons.
    if (4 == length)
    {
      // Copy data to VrBoxData
      for (int i = GP_TRIGGERS; i < GP_BTNAB; i++)
        VrBoxData[i] = pData[i];
  
      // Wake up the joystick/trigger buttons handler task
      if (HandleJS)
        vTaskResume(HandleJS);
        
      // Restart the timers
      joyTimer = millis() + JOYTIMEOUT;
    }
    else if (2 == length)
    {
      // Show the received data
      if (0x50 == pData[1])
      {
        // A/B button report, wake the A/B button handler task
        VrBoxData[GP_BTNAB] = pData[0];
        if (HandleAB)
          vTaskResume(HandleAB);
      }
      else
      {
        // C/D button report, wake the C/D/M button handler task
        VrBoxData[GP_BTNCDM] = pData[0];
        if (HandleCDM)
          vTaskResume(HandleCDM);
      }
    }
  }
  // End of notifyCallback

  //******************************************************************************
  // Connection state change event callback handler.
  //******************************************************************************
  class MyClientCallback: public BLEClientCallbacks
  {
    void onConnect(BLEClient* pclient)
    {
      scanTimer = 0;                                      // Reinitalize the scan timer
      LED_STATE_A();
      VLF("MSG: BLE GamePad Connected");
    }
    
    void onDisconnect(BLEClient* pclient)
    {
      havedevice = false;
      Connected = false;
      delete myDevice; 
      scanTimer = millis() + SCANTIMER;                  // Restart the scan timer
      LED_STATE_B();
      VLF("MSG: BLE GamePad Disconnected");
    }
  };
  // End of MyClientCallback

  //******************************************************************************
  // Connect to a service, register for notifications from Report Characteristics.
  //******************************************************************************
  bool setupCharacteristics(BLERemoteService* pRemoteService, NotifyCallback pNotifyCallback)
  {
    // Get all the characteristics of the service using the handle as the key
    pmapbh = pRemoteService->getCharacteristicsByHandle();
    
    // Only interested in report characteristics that have the notify capability
    for (itrbh = pmapbh->begin(); itrbh != pmapbh->end(); itrbh++)
    {
      BLEUUID x = itrbh->second->getUUID();
      UNUSED(x);
      // The uuid must match the report uuid
      if (ReportCharUUID.equals(itrbh->second->getUUID()))
      {
        // Found a report characteristic
        if (itrbh->second->canNotify())
        {
          // Register for notifications from this characteristic
          itrbh->second->registerForNotify(pNotifyCallback);
        }
      }
    }
    return true;
  }
  // End of setupCharacteristics

  //******************************************************************************
  // Validate the server has the correct name and services we are looking for.
  // The server must have the HID service.
  //******************************************************************************
  bool connectToServer()
  {

    if (pClient == NULL) pClient = BLEDevice::createClient();
    pClient->setClientCallbacks(new MyClientCallback());

    // Connect to the remote BLE Server
    bool _connected = pClient->connect(myDevice); // If you pass BLEAdvertisedDevice instead of address, 
                                                  // it will be recognized type of peer device address (public or private)
    if(!_connected)
    {
      delete pClient;
      delete myDevice;
      return false;
    } 
       
    // BLE servers may offer several services, each with unique characteristics
    // we can identify the type of service by using the service UUID
    // Obtain a reference to the service we are after in the remote BLE server.
    // This will return a pointer to the remote service if it has a matching service UUID
    
    BLERemoteService* pRemoteService = pClient->getService(serviceUUID);
    if (pRemoteService == nullptr)
    {
      pClient->disconnect();
      return false;
    }
    setupCharacteristics(pRemoteService, notifyCallback);

    doConnect = false;
    Connected = true;
    return true; 
  }
  // End of connectToServer

  //******************************************************************************
  // Scan for BLE servers and find the first one that advertises the service we are looking for.
  //******************************************************************************
  class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks
  {
    // Called for each advertising BLE server.
    void onResult(BLEAdvertisedDevice advertisedDevice)
    {
      // We have found a server, see if it has the name we are looking for
      if (!havedevice) 
      {
        if (advertisedDevice.haveName())
        {
          Server_BLE_Address = new BLEAddress(advertisedDevice.getAddress()); 
          Scaned_BLE_Address = Server_BLE_Address->toString().c_str();
          if ((Scaned_BLE_Address == (My_BLE_Address)) || (Scaned_BLE_Address == (My_BLE_Address1)))
          {
            // We found a server with the correct Mac, see if it has the service we are
            // interested in (HID)
            if (advertisedDevice.haveServiceUUID() && advertisedDevice.isAdvertisingService(serviceUUID))
            {
              doConnect = true;
              doScan = true;
              havedevice = true;
              scanTimer = 0; // reinitalize the scan timer
              BLEDevice::getScan()->stop();
              myDevice = new BLEAdvertisedDevice(advertisedDevice);
            }           
          } 
        }
      }
    }
  };
  // End of MyAdvertisedDeviceCallbacks

  // All of these tasks are designed to run forever. The tasks are resumed when
  // a notification message is received with new data.
  //******************************************************************************
  // Joystick handler Task.
  // Moving the joystick off center causes this task to be resumed about every
  // 15ms. Press of either trigger button will also resume this task.
  //******************************************************************************
  void taskJoyStick(void *parameter)
  {
    int8_t  x;
    int8_t  y;
    uint8_t triggers;

    // Forever loop
    while(true)
    {
      // give up the CPU, wait for new data
      vTaskSuspend(NULL);

      // We just woke up, new data is available, convert joystick data to
      // signed 8 bit integers
      x = (int8_t)VrBoxData[GP_JOYX];
      y = (int8_t)VrBoxData[GP_JOYY];
      triggers = VrBoxData[GP_TRIGGERS];

      if (!status.aligning) firstrate = true; // Reset for multiple aligns

      // set guide rate = 48X for align
      if (status.aligning && firstrate)
      {
        firstrate = false; // Ignore after starting align to allow rate changes        
        activeGuideRate = 7; // 48X
        char cmd[5] = ":Rn#"; cmd[2] = '0' + activeGuideRate; // set initial GuideRate
        onStep.commandBlind(cmd);
      }

      // Trigger released, stop focuser 
      if (triggerPress && (VrBoxData[GP_TRIGGERS] == 0))
      {
        onStep.commandBlind(FOCUS_STOP);        
        pushTimer = 0;
        triggerPress = false;   
        FocusSpd = false;
        continue;          
      }
      
      if (triggers & GP_LOW_TRIGGER)
      {
      // The lower trigger button is pressed
      if (FocusSpd && triggerPress)
        {
        onStep.commandBlind(FOCUS_HIGH); 
        FocusSpd = false;
        }
      else 
        {
        onStep.commandBlind(FOCUS_LOW);
        pushTimer = millis() + FOCUSTIMER; 
        }
         onStep.commandBlind(FOCUS_IN);         
        triggerPress = true;
        pressedOnce = true;
        continue;
      }
      
      if (triggers & GP_UPR_TRIGGER)
      {
        // The upper trigger button is pressed
        if (FocusSpd && triggerPress)
          {
          onStep.commandBlind(FOCUS_HIGH); 
          FocusSpd = false;
          }
       else 
         {
         onStep.commandBlind(FOCUS_LOW);
         pushTimer = millis() + FOCUSTIMER; 
         }
         onStep.commandBlind(FOCUS_OUT);         
         triggerPress = true;
         pressedOnce = true;
         continue;
      }

      if (pressedOnce)
      {
        pressedOnce = false;
        continue;
      }

      // Joystick has been centered for JOYTIMEOUT ms 
      if (timerReturn) 
      {
        if (movingNorth || movingSouth || movingEast || movingWest)
        {
          if (movingNorth) {movingNorth = false; onStep.commandBlind(":Qn#");}
          if (movingSouth) {movingSouth = false; onStep.commandBlind(":Qs#");}
          if (movingEast) {movingEast = false; onStep.commandBlind(":Qe#");}
          if (movingWest) {movingWest = false; onStep.commandBlind(":Qw#");}
          delay(25);
          if (movingNorth || movingSouth || movingEast || movingWest)
          { 
            onStep.commandBlind(STOP_ALL); 
            delay(50);
          } 
          timerReturn = false; 
          continue;          
        }        
      }

      // Joystick directional moves
      if (y < -JoyStickDeadZone)
      {
        if (NS_SWAP == OFF) MoveNorth();
        else MoveSouth();
      }
      else if (y > JoyStickDeadZone)
      {
        if (NS_SWAP == OFF) MoveSouth(); 
        else MoveNorth();
      }
          
      if (x < -JoyStickDeadZone)
      {
        if (EW_SWAP == OFF) MoveEast();
        else MoveWest();
      }
      else if (x > JoyStickDeadZone)
      {
        if (EW_SWAP == OFF) MoveWest();
        else MoveEast();
      }
    }
  }
  // End of taskJoyStick

  //******************************************************************************
  // A & B Buttons handler Task.
  // Press of either the A or B button will resume this task. 
  //******************************************************************************
  void taskButtonAB(void *parameter)
  {
    uint8_t buttons;
  
    while(true)
    {
      // Give up the CPU, wait for new data
      vTaskSuspend(NULL);
      
      // We just woke up, new data is available
      buttons = VrBoxData[GP_BTNAB];

      if (buttons & GP_BUTTON_A) onStep.commandBlind(GOTO_CURRENT); // Button A pressed, goto last location

      if (buttons & GP_BUTTON_B)
      {
        // Button B pressed, Spiral search
        if (SpiralInProgess == false)
        {
          SpiralInProgess = true;
          onStep.commandBlind(SPIRAL);
        }
        else
        {
          SpiralInProgess = false; 
          onStep.commandBlind(STOP_ALL);
        }
      }
    }
  } 
  //  End of taskButtonAB

  //******************************************************************************
  // C, D & M Buttons handler Task. 
  // Press of either the C, D or M button will resume this task. 
  //******************************************************************************
  void taskButtonCDM(void *parameter)
  {
    uint8_t buttons;
    
    while(true)
    {
      // Give up the CPU
      vTaskSuspend(NULL);

      // we just woke up, new data is available
      buttons = VrBoxData[GP_BTNCDM];

      if (CDMpressedOnce) // Ignore first callback 
      {
        CDMpressedOnce = false;        
        continue;
      }

      if (buttons & GP_BUTTON_C)
      {
        // button C pressed
        CDMpressedOnce = true; 
        (activeGuideRate--);
        if (activeGuideRate > 2) Click(); 
        if (activeGuideRate < 3) activeGuideRate = 3;
        if (activeGuideRate > 9) activeGuideRate = 9;
        char cmd[5] = ":Rn#"; cmd[2] = '0' + activeGuideRate;
        onStep.commandBlind(cmd);
      }

      if (buttons & GP_BUTTON_D)
      {
        // Button D pressed
        CDMpressedOnce = true; 
        (activeGuideRate++);
        if (activeGuideRate < 10) Click();
        if (activeGuideRate < 3) activeGuideRate = 3;
        if (activeGuideRate > 9) activeGuideRate = 9;
        char cmd[5] = ":Rn#"; cmd[2] = '0' + activeGuideRate;
        onStep.commandBlind(cmd);
      } 

      if (GP_BUTTON_M == buttons) // Button M pressed, conditional response to state
      {
         // Emergency stop
        if (status.inGoto || movingNorth || movingSouth || movingEast || movingWest)
        {   
          onStep.commandBlind(STOP_ALL);
          continue;
        }
        // Set align point when doing an alignment
        else if (status.aligning)
        {
          onStep.commandBlind(ALIGN);
          Click(); 
          continue;
        }
        // Unpark mount if parked        
        else if (status.parked) 
        {
          onStep.commandBlind(UNPARK);
          StartFocus();          
          Beep();          
        }
        // Start tracking if "At home"        
        else if (status.atHome && !status.tracking)
        {
          onStep.commandBlind(TRACK_ON);
          StartFocus();
          Alert();                   
        }
        // Start tracking if tracking off
        else if (!status.tracking)
        {
          onStep.commandBlind(TRACK_ON);
          Beep();          
        }
        // Park the scope                
        else if (!status.parked)
        {
          ParkFocus();
          onStep.commandBlind(PARK);                
        }
      }
    } 
  }
  //  End of taskButtonCDM

  //******************************************************************************
  // Initial and Service routines
  //******************************************************************************

  void bleInit()
  {
    // Enable BT ble only (no classic)
    esp_bt_controller_enable(ESP_BT_MODE_BLE);
    // Free memory used by BT Classic to prevent out of memory crash
    esp_bt_controller_mem_release(ESP_BT_MODE_CLASSIC_BT);
  }

  void DoScan() 
  {
    if (scanTimer && (scanTimer < millis()))
    {
      // Retrieve a GATT Scanner and set the callback we want to use to be informed 
      // when we have detected a new device.  Specify that we want active scanning
      // and start the scan.
    
      BLEScan* pBLEScan = BLEDevice::getScan();
      pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
      pBLEScan->setInterval(1349);
      pBLEScan->setWindow(499);
      pBLEScan->setActiveScan(true);
      pBLEScan->start(1); // scan 1 second
        
      pBLEScan->clearResults(); // Delete results fromBLEScan buffer to release memory, solves heap problem
    
      // Restart the scan timer
      scanTimer = millis() + SCANTIMER;
      VLF("MSG: Scanning for BLE GamePad");
    }
  }
  // End of DoScan.

  //******************************************************************************
  // This is the first (and after disconnect) server connection
  //******************************************************************************
  void bleConnTest() 
  {
    // If the flag "doConnect" is true then we have scanned for and found the desired
    // BLE Server with which we wish to connect.  Now we connect to it.

    if (doConnect == true) connectToServer(); 
    else
    {
      doConnect = false;
      doScan = true;
      DoScan();
    }
  }
  // End of bleConnTest

  void bleSetup()
  {
    My_BLE_Address = BLE_GP_ADDR;
    My_BLE_Address1 = BLE_GP_ADDR1;
    joyTimer = 0;  
    pushTimer = 0;
    activeGuideRate = status.guideRate;
    BaseType_t xReturned;

    VLF("MSG: Starting BLE GamePad Services"); 

    // Create tasks to handle the joystick and buttons
    xReturned = xTaskCreate(taskJoyStick,             // task to handle activity on the joystick.
                            "Joystick",               // String with name of task.
                            TaskStackSize,            // Stack size in 32 bit words.
                            NULL,                     // Parameter passed as input of the task
                            1,                        // Priority of the task.
                            &HandleJS);               // Task handle.
  
    xReturned = xTaskCreate(taskButtonAB,             // task to handle activity on the A & B buttons.
                            "ButtonsAB",              // String with name of task.
                            TaskStackSize,            // Stack size in 32 bit words.
                            NULL,                     // Parameter passed as input of the task
                            1,                        // Priority of the task.
                            &HandleAB);               // Task handle.

    xReturned = xTaskCreate(taskButtonCDM,             // task to handle activity on the C, D & M buttons.
                            "ButtonsCDM",              // String with name of task.
                            TaskStackSize,            // Stack size in 32 bit words.
                            NULL,                     // Parameter passed as input of the task
                            1,                        // Priority of the task.
                            &HandleCDM);               // Task handle.

    UNUSED(xReturned);

    BLEDevice::init("");

    if (!Connected)
    {
      // Retrieve a GATT Scanner and set the callback we want to use to be informed 
      // when we have detected a new device.  Specify that we want active scanning
      // and start the scan.
      
      BLEScan* pBLEScan = BLEDevice::getScan();
      pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
      pBLEScan->setInterval(1349);
      pBLEScan->setWindow(150);  
      pBLEScan->setActiveScan(true);
      pBLEScan->start(2, false); // Scan
      delay(3000); // Delay for scan
    }

    if (!Connected)
    {
      doScan = true;
      scanTimer = millis() + SCANTIMER; // Restart the scan timer
    }

    bleConnTest();     
  }
  // End of bleSetup

  void bleTimers() 
  {
    if (Connected)
    {
      if (joyTimer && (joyTimer < millis()) && (movingNorth || movingSouth || movingEast || movingWest))
      { 
        // No joystick notification for JOYTIMEOUT mS, center the joystick
        timerReturn = true;
        joyTimer = 0; 
        VrBoxData[GP_JOYX] = VrBoxData[GP_JOYY] = 0;               
        vTaskResume(HandleJS);
      }
            
      if (pushTimer && (pushTimer < millis()) && triggerPress)
      {
        // Focus button held down for FOCUSTIMER, speed up focuser
        FocusSpd = true;
        timerReturn = true;
        pushTimer = 0;                  
        vTaskResume(HandleJS);
      }
      else FocusSpd = false;
    }
  }
  // End of bleTimers

#endif 
