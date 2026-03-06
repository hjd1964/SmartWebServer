// Version 3.1 Kenihi Aihara 20 JAN 2026--------------------------------
// Locale Japanese (custom), ISO ISO639-1 language code "ja"
#pragma once

// General
#define L_UPLOAD "アップロード"
#define L_DOWNLOAD "ダウンロード"
#define L_ACCEPT "承認"
#define L_CONTINUE "続行"
#define L_ARE_YOU_SURE "本当によろしいですか"
#define L_LOGOUT "ログアウト"

#define L_UT "グリニッジ標準時(UT)"
#define L_LST "ローカル恒星時(LST)"
#define L_LONG "経度"
#define L_LAT "緯度"
#define L_EAST "東"
#define L_WEST "西"
#define L_ZENITH "天頂"
#define L_NCP "NCP"
#define L_SCP "SCP"

#define L_NONE "なし"
#define L_AUTO "自動"
#define L_SET_LABEL "導入(Goto)"
#define L_OK "OK"
#define L_ON "ON"
#define L_OFF "OFF"
#define L_START "開始"
#define L_DELTA "差分(Delta)"
#define L_DP_MSG "相対"
#define L_DP_ZERO "以下でフルパワー"
#define L_DP_SPAN "以上でヒーターオフ"
#define L_POWER "電源"
#define L_GUIDE "Guide"
#define L_CENTER "センタリング"
#define L_BLUE "青"
#define L_GREEN "緑"
#define L_RATE "速度"
#define L_RANGE "範囲"
#define L_UNKNOWN "不明"
#define L_FAULT "障害"
#define L_POINT "ポイント"
#define L_COMPLETE "完了"
#define L_ACTIVE "Active"
#define L_INACTIVE "Inactive"
#define L_BEST "最適"
#define L_ALTERNATE "代替"

// ---------------------------- Index ----------------------------
// date/time
#define L_TIME_BROWSER_UT "UT(ブラウザ取得)"

// weather
#define L_TEMPERATURE "気温"
#define L_AMBIENT_TEMPERATURE "環境温度"
#define L_PRESSURE "気圧"
#define L_HUMIDITY "湿度"
#define L_DEW_POINT "露点温度"

// operation
#define L_TRACKING "追尾"
#define L_SITE "観測地"
#define L_COORDINATES "座標"
#define L_CURRENT "現在位置"
#define L_TARGET "目標位置"
#define L_ENCODER "エンコーダ"
#define L_POLAR_ALIGN "極軸合わせ"

// pier side
#define L_PIER_SIDE "ピアサイド"
#define L_MERIDIAN_FLIPS "子午線反転"
#define L_ALIGN_MESSAGE "アライメント情報"
#define L_MERIDIAN_FLIP_W_TO_E "子午線反転（西→東）"
#define L_MERIDIAN_FLIP_E_TO_W "子午線反転（東→西）"

#define L_OPERATIONS "操作"

// parking
#define L_PARKING "パーク動作中"
#define L_PARKED "パーク済み"
#define L_NOT_PARKED "未パーク"
#define L_PARKING "パーク動作中"
#define L_PARK_FAILED "パーク失敗"

// home
#define L_HOMING "ホームへ移動中"
#define L_AT_HOME "ホーム位置"
#define L_SLEWING "Slewing"
#define L_PPS_SYNC "PPS同期"
#define L_HOME_REV "反転"
#define L_HOME_OFFSET "ホーム検知位置からのオフセット:"
#define L_HOME_OFFSET_RANGE_AXIS1 "軸1, +/-" STR(HOME_OFFSET_RANGE_AXIS1) " 秒角"
#define L_HOME_OFFSET_RANGE_AXIS2 "軸2, +/-" STR(HOME_OFFSET_RANGE_AXIS2) " 秒角"
#define L_HOME_AUTO "起動時に自動ホーム設定:"

// tracking
#define L_REFR_COMP_RA "RC(大気差)"
#define L_REFR_COMP_BOTH "RCD(大気差+モデル)"
#define L_FULL_COMP_RA "FC(全補正)"
#define L_FULL_COMP_BOTH "FCD(全補正+モデル)"
#define L_TRACKING "追尾"
#define L_CURRENT_MAXRATE "現在の最大速度"
#define L_DEFAULT_MAXRATE "デフォルト最大速度"
#define L_CURRENT_MAXSLEW "最大スルー速度"

#define L_FREQUENCY_ADJUST "周波数調整"

// driver status
#define L_DRIVER "ドライバ"
#define L_STATUS "ステータス"
#define L_STATE "状態"
#define L_COMMS_FAILURE "通信エラー"
#define L_STANDSTILL "停止中"
#define L_OPEN_LOAD "負荷開放"
#define L_SHORT_GND "GND短絡"
#define L_SHUTDOWN_OVER "シャットダウン"
#define L_PRE_WARNING "事前警告"
#define L_DRIVER_FAULT "ドライバ障害"

// misc. errors/workload
#define L_HARDWARE "ハードウェア"
#define L_INTERNAL_TEMP "コントローラー温度"
#define L_LAST_GENERAL_ERROR "最後のエラー"
#define L_WORKLOAD "負荷率"
#define L_WIRELESS_SIGNAL_STRENGTH "無線信号強度"

// --------------------------- Control ---------------------------

#if DISPLAY_SPECIAL_CHARS == ON
  #define HOME_CH "&#x1F3E0;"
  #define ARROW_DR "&#x27A5;"
  #define ARROW_UR "&#x27A6;"
  #define ARROW_R2 "&#x27A4;"
  #define CAUTION_CH "&#9888;"
  #define CLOCK_CH "&#x1F565;"
  #define ARROW_LL "&lt;&lt;"
  #define ARROW_L "&lt;"
  #define ARROW_R "&gt;"
  #define ARROW_RR "&gt;&gt;"
  #define ARROW_DD "&lt;&lt;"
  #define ARROW_D "&lt;"
  #define ARROW_U "&gt;"
  #define ARROW_UU "&gt;&gt;"
  #define SIDEREAL_CH "&#9733;"
  #define LUNAR_CH "&#9790;"
  #define SOLAR_CH "&#9737;"
  #define KING_CH "&#9812;"
#else
  #define HOME_CH "H"
  #define ARROW_DR "-&gt;"
  #define ARROW_UR "-&gt;"
  #define ARROW_R2 "&gt;"
  #define CAUTION_CH "!"
  #define CLOCK_CH "T"
  #define ARROW_LL "&lt;&lt;"
  #define ARROW_L "&lt;"
  #define ARROW_R "&gt;"
  #define ARROW_RR "&gt;&gt;"
  #define ARROW_DD "&lt;&lt;"
  #define ARROW_D "&lt;"
  #define ARROW_U "&gt;"
  #define ARROW_UU "&gt;&gt;"
  #define SIDEREAL_CH "*"
  #define LUNAR_CH "("
  #define SOLAR_CH "O"
  #define KING_CH "K"
#endif

#define BUTTON_N "N"
#define BUTTON_S "S"
#define BUTTON_E "E"
#define BUTTON_W "W"
#define BUTTON_SYNC "@"

#define L_HINT_CLOCK "現在時刻の同期"
#define L_HINT_CONTINUE_GOTO "自動導入を続行 (ホーム位置の場合)"
#define L_HOME_PARK_TITLE "ホームとパーク"
#define L_RETURN_HOME "ホームへ戻る"
#define L_RESET_HOME "ホーム位置リセット"
#define L_UNPARK "パーク解除"
#define L_PARK "パーク"
#define L_HINT_EMERGENCY_STOP "緊急停止！"
#define L_HINT_RATE_SIDEREAL "恒星時レート"
#define L_HINT_RATE_LUNAR "月追尾レート"
#define L_HINT_RATE_SOLAR "太陽追尾レート"
#define L_HINT_RATE_KING "キング・レート"
#define L_HINT_FOC_SEL "フォーカーサ選択"
#define L_HINT_DEROTATE_ON "回転防止オン"
#define L_HINT_ROTATOR_REV "回転方向を反転"
#define L_HINT_ROTATOR_PAR "赤道角に回転器を移動"
#define L_HINT_DEROTATE_OFF "回転防止オフ"

#define L_ALIGN "アライメント"
#define L_AUX_FEATURES "補助機能"

#define L_FOCUSER "フォーカーサ"
#define L_ROTATOR "ローテータ"
#define L_DEROTATOR "デローテータ"
#define L_CAMERA_EXPOSURE "露光時間"
#define L_CAMERA_DELAY "遅延"
#define L_CAMERA_COUNT "枚数"

// ---------------------------- Library --------------------------

#define L_CATALOG "カタログ"
#define L_CATALOGS "カタログ一覧"
#define L_RECS_AVAIL "件が利用可能"
#define L_CAT_DATA "データ (ダウンロードまたはアップロード用)"

#define L_CAT_EXAMPLE1 "フィールド:<br /><pre>"
#define L_CAT_EXAMPLE2 "  1行目にはカタログ名レコードが必要です。<br />"
#define L_CAT_EXAMPLE3 "  これは '$' に続く最大10文字で構成されます。<br /><br />"
#define L_CAT_EXAMPLE4 "  天体名は最大11文字の文字列です。<br /><br />"
#define L_CAT_EXAMPLE5 "  Catは以下のカテゴリコードです:<br />"
#define L_CAT_EXAMPLE6 "  赤経 RA (Jnow) は、時、分、秒です。<br />"
#define L_CAT_EXAMPLE7 "  赤緯 Dec (Jnow) は、+/- 度、分、秒です。<br />"

#define L_CAT_NO_NAME "カタログが選択されましたが、名前がありません。"
#define L_CAT_GET_NAME_FAIL "カタログ選択、名前の取得に失敗しました。"
#define L_CAT_NO_CAT "カタログが未選択"
#define L_CAT_NO_OBJECT "天体が選択されていません。"
#define L_SELECTED "選択中"
#define L_CAT_DOWNLOAD_FAIL "ダウンロード失敗、不正なデータです。"
#define L_CAT_DOWNLOAD_SUCCESS "ダウンロード成功。"
#define L_CAT_DOWNLOAD_INDEX_FAIL "ダウンロード失敗、カタログのインデックスが作成できませんでした。"
#define L_CAT_DATA_REMOVED "カタログデータが削除されました"
#define L_CAT_UPLOAD_FAIL "アップロード失敗、カタログ名が不正です。"
#define L_CAT_UPLOAD_INDEX_FAIL "アップロード失敗、カタログ名レコードのインデックス作成中。"
#define L_CAT_DELETE_FAIL "アップロード失敗、カタログ名レコードの削除中。"
#define L_CAT_WRITE_NAME_FAIL "アップロード失敗、カタログ名レコードの書き込み中。"
#define L_CAT_UPLOAD_NO_NAME_FAIL "アップロード失敗、1行目にはカタログ名が必要です。"
#define L_CAT_BAD_FORM "アップロード失敗、フォーマット不正 行番号: "
#define L_CAT_UPLOAD_BAD_OBJECT_NAME "アップロード失敗、天体名が不正 行番号: "
#define L_CAT_BAD_CATEGORY "アップロード失敗、カテゴリが不正 行番号: "
#define L_CAT_BAD_RA "アップロード失敗、赤経(RA)フォーマット不正 行番号: "
#define L_CAT_BAD_DEC "アップロード失敗、赤緯(Dec)フォーマット不正 行番号: "
#define L_CAT_UPLOAD_RA_FAIL "アップロード失敗、赤経(RA)送信中 行番号: "
#define L_CAT_UPLOAD_DEC_FAIL "アップロード失敗、赤緯(Dec)送信中 行番号: "
#define L_CAT_UPLOAD_LINE_FAIL "アップロード失敗、行送信中 番号: "
#define L_CAT_UPLOAD_SUCCESS "アップロード成功"
#define L_CAT_UPLOAD_LINES_WRITTEN "行書き込み完了"
#define L_CAT_UPLOAD_SELECT_FAIL "アップロード失敗、カタログを選択できません。"
#define L_CAT_UPLOAD_NO_CAT "アップロード失敗、カタログが選択されていません。"
#define L_CAT_CLEAR "カタログ消去"
#define L_CAT_CLEAR_LIB "ライブラリ消去"

// --------------------------- Encoders --------------------------

// general settings
#define L_ENC_SYNC_TO_ENC "同期: OnStep -> エンコーダ"
#define L_ENC_SYNC_TO_ONS "同期: エンコーダ -> OnStep"
#define L_ENC_ORIGIN_TO_ONS "OnStep設定 -> エンコーダ原点"
#define L_ENC_AUTO_SYNC "自動同期: エンコーダ -> OnStep"
#define L_ENC_CONF "エンコーダ設定"
#define L_ENC_SET_TPD "1度あたりのステップ数(Counts/deg)"
#define L_ENC_SYNC_THLD "自動同期の閾値"
#define L_ENC_SYNC_THLD_UNITS "0 ～ 9999 秒角"
#define L_ENC_AUTO_RATE "自動速度制御"
#define L_NOW "今すぐ実行"

// ------------------------------ PEC ----------------------------

#define L_PEC_STATUS "ステータス"
#define L_PEC_CONTROL "PEC制御"
#define L_PEC_PLAY "再生"
#define L_STOP "停止"
#define L_PEC_CLEAR "消去"
#define L_PEC_REC "記録"
#define L_PEC_CLEAR_MESSAGE "「消去」はEEPROMではなくメモリバッファを消去します。記録中はバッファが優先され3:1で平均化されますが、消去された場合は完全な補正値が使用されます。"
#define L_PEC_EEWRITE "EEPROMへ書き込み"
#define L_PEC_EEWRITE_MESSAGE "PECデータをEEPROMに書き込み、OnStep再起動後も保持されるようにします。書き込みには数秒かかります。"
#define L_DISABLED_MESSAGE "無効"
#define L_PEC_IDLE "待機中"
#define L_PEC_WAIT_PLAY "再生待機中"
#define L_PEC_PLAYING "再生中"
#define L_PEC_WAIT_REC "記録待機中（インデックス待ち）"
#define L_PEC_RECORDING "記録中"
#define L_PEC_UNK "不明"
#define L_PEC_EEWRITING "EEPROM書き込み中"
#define L_PEC_STEPS_PER_WORM_ROTATION "ウォーム1回転あたりのステップ数"

// --------------------------- Settings --------------------------

#define L_REFINE_POLAR_ALIGN "極軸の追い込み"
#define L_REFINE_PA "極軸の微調整"
#define L_REFINE_MESSAGE1 "赤道儀の設定と3つ以上の星を基準にアライメントを実施。北極または南極付近の明るい星を、50～80度の範囲で選択してください。"
#define L_REFINE_MESSAGE2 "次に[極軸の微調整]ボタンを押します。方向キーを使用して目視で星を再配置してください。"
#define L_REFINE_MESSAGE3 "（オプション）赤道儀を再調整します。"

#define L_PARK "パーク"
#define L_SET_PARK "パーク設定"
#define L_SET_PARK_CURRENT_COORDS "現在の座標をパーク位置に設定"

#define L_TRK_FASTER "追尾速度を上げる"
#define L_TRK_SLOWER "追尾速度を下げる"
#define L_TRK_RESET "リセット（デフォルト）"
#define L_TRK_COMP "追尾速度の補正（ポインティングモデル/屈折）"
#define L_TRK_FULL "全軸"
#define L_TRK_REFR "屈折のみ"
#define L_TRK_DUAL "2軸"
#define L_TRK_SINGLE "1軸"

#define L_BUZZER "警報ブザー"

#define L_MERIDIAN_FLIP_AUTO "境界を超えた際に自動で子午線反転"
#define L_MERIDIAN_FLIP_NOW "今すぐ実行"
#define L_MERIDIAN_FLIP_PAUSE "子午線反転、ホームで一時停止"
#define L_MERIDIAN_FLIP_PPS "子午線反転、優先ピアサイド"

#define L_ORIENTATION_AUTO "方向転換"
#define L_ORIENTATION_CHANGE_NOW L_MERIDIAN_FLIP_NOW
#define L_ORIENTATION_CHANGE_PAUSE "反転時、ホームで一時停止"
#define L_ORIENTATION_CHANGE_PPS "反転時、優先する向き"

// ------------------------ Configuration ------------------------

#define L_BASIC_SET_TITLE "基本設定:"

#define L_MAX_SLEW_SPEED "最大導入速度"

// goto Rate
#define L_VSLOW "非常に遅い 0.5x"
#define L_SLOW "遅い 0.75x"
#define L_NORMAL "通常 1x"
#define L_FAST "速い 1.5x"
#define L_VFAST "非常に速い 2x"

// backlash
#define L_BACKLASH "バックラッシュ"
#define L_BACKLASH_RANGE_AXIS1 "軸1, 0 ～ 3600 秒角"
#define L_BACKLASH_RANGE_AXIS2 "軸2, 0 ～ 3600 秒角"
#define L_BACKLASH_RANGE_AXIS3UP "バックラッシュ, 0 ～ 32767 ステップ"

// deadband/TCF
#define L_DEADBAND "TCF不感帯(Deadband)"
#define L_TCF_COEF "TCF係数"
#define L_TCF_COEF_EN "TCF有効化"
#define L_DEADBAND_RANGE_AXIS4UP "TCF不感帯, 1 ～ 32767 ステップ"
#define L_TCF_COEF_RANGE_AXIS4UP "TCF係数, &pm;999.0 ミクロン/度C"
#define L_TCF_COEF_EN_AXIS4UP "TCF有効化, 0:偽 または 1:真"

// limits
#define L_LIMITS_TITLE "地平線および天頂リミット"
#define L_LIMITS_RANGE_HORIZON "地平線(Horizon), 最小高度 &pm;30&deg;"
#define L_LIMITS_RANGE_OVERHEAD "天頂(Overhead), 最大高度 60 ～ 90&deg;"
#define L_LIMITS_RANGE_MERIDIAN_E "子午線越え, ピアの東側 &pm;270&deg;"
#define L_LIMITS_RANGE_MERIDIAN_W "子午線越え, ピアの西側 &pm;270&deg;"

// location
#define L_LOCATION_TITLE "時刻と場所"
#define L_LOCATION_LONG "経度 度 分 秒 &pm;180, 西経は+（日本は-）"
#define L_LOCATION_LAT  "緯度 度 分 秒 &pm;90, 北緯は+"
#define L_LOCATION_RANGE_UTC_OFFSET "UTCオフセット, 時間と分 -14～+12（日本は-9）"
#define L_LOCATION_MESSAGE_UTC_OFFSET "タイムゾーン値の逆符号です。標準時用です（夏時間ではありません）。"

// advanced configuration

// AxisToTile.cpp has a count and array matching these values
#define L_AXPN_STEPS_PER_DEG "ステップ数/度"      // "$1" (first axis parameter)
#define L_AXPN_LIMIT_DEGS_MIN "最小リミット, 度"  // "$2"
#define L_AXPN_LIMIT_DEGS_MAX "最大リミット, 度"  // "$3"
#define L_AXPN_STEPS_PER_UM "ステップ数/um"           // "$4"
#define L_AXPN_LIMIT_UM_MIN "最小リミット, um"      // "$5"
#define L_AXPN_LIMIT_UM_MAX "最大リミット, um"      // "$6"
#define L_AXPN_REVERSE "反転(Reverse)"                 // "$7" (first motor parameter)
#define L_AXPN_MICROSTEPS "マイクロステップ"           // "$8"
#define L_AXPN_MICROSTEPS_GOTO "導入時マイクロステップ" // "$9"
#define L_AXPN_DECAY_MODE "Decayモード"           // "$10"
#define L_AXPN_DECAY_MODE_GOTO "Decayモード(Goto時)" // "$11"
#define L_AXPN_CURRENT_HOLD "電流(mA) 保持時"            // "$12"
#define L_AXPN_CURRENT_RUN "電流(mA) 追尾時"              // "$13"
#define L_AXPN_CURRENT_GOTO "電流(mA) 導入時"            // "$14"
#define L_AXPN_INTERPOLATE "256x 補間"    // "$15"
#define L_AXPN_PID_P "<big><code>P</code></big> 追尾" // "$16"
#define L_AXPN_PID_I "<big><code>I</code></big> 追尾" // "$17"
#define L_AXPN_PID_D "<big><code>D</code></big> 追尾" // "$18"
#define L_AXPN_PID_SLEWING_P "<big><code>P</code></big> スルー" // "$19"
#define L_AXPN_PID_SLEWING_I "<big><code>I</code></big> スルー" // "$20"
#define L_AXPN_PID_SLEWING_D "<big><code>D</code></big> スルー" // "$21"
#define L_AXPN_RADS_PER_COUNT "Rads/カウント"       // "$22"
#define L_AXPN_STEPS_TO_COUNTS "ステップ/カウント比" // "$23"
#define L_AXPN_MAX_ACCEL "最大加速度, %/s/s"      // "$24"
#define L_AXPN_MIN_PWR "最小パワー, %"            // "$25"
#define L_AXPN_MAX_PWR "最大パワー, %"            // "$26"

#define L_ADV_DECAY_SLOW "Slow"
#define L_ADV_DECAY_FAST "Fast"
#define L_ADV_DECAY_MIXED "Mixed"
#define L_ADV_DECAY_SPREADCYCLE "SpreadCycle"
#define L_ADV_DECAY_STEALTHCHOP "StealthChop"

#define L_ADV_MOTOR "モーター"
#define L_REVERT "デフォルトに戻す"
#define L_ADV_SET_TITLE "詳細設定"
#define L_ADV_SET_HEADER_MSG "特に記載がない限り、以下の変更はOnStepの再起動後に有効になります:"
#define L_ADV_MOUNT_TYPE " 選択: 1 ドイツ式赤道儀(GEM), 2 フォーク式赤道儀, 3 経緯台(Alt/Azm)"
#define L_ADV_BOOL "0:偽 または 1:真"

#define L_ADV_SET_REV "方向を反転"

#define L_ADV_ENABLE "詳細設定を有効にする"
#define L_ADV_DISABLE "詳細設定を無効にする (再起動ですべてデフォルトに戻ります)"
#define L_ADV_SET_AXIS_NO_EDIT "この軸の編集は無効化されています。"
#define L_ADV_SET_NO_EDIT "現在、すべての軸の編集が無効です。この機能が無効化されている（関連する設定デフォルトがアクティブ）か、OnStepを再起動する必要があります。"
#define L_ADV_SET_FOOTER_MSG0 "軸設定の変更を有効にするには、再起動が必要です。"
#define L_ADV_SET_FOOTER_MSG1 "[アップロード] を押した後、不正なパラメータが原因でアップロードが拒否された可能性があるため、上記の変更点を確認してください。"
#define L_ADV_SET_FOOTER_MSG2 "[デフォルトに戻す] を選択すると、OnStepを再起動するまでそのグループは編集できなくなります。"
#define L_ADV_SET_FOOTER_MSG4 "軸1の「ウォーム1回転あたりのステップ数」と「1度あたりのステップ数」の比率を変更すると、ライブラリのNVメモリ領域が破損する可能性があります。必要に応じて、事前にカタログをバックアップしてください。"

// reset control
#define L_RESET_TITLE "OnStepX 再起動制御:"
#define L_WIPE_RESET "全設定を消去して OnStepX を再起動"
#define L_RESET "OnStepX を再起動"
#define L_RESET_FWU "ファームウェア更新のために OnStepX を再起動"
#define L_RESET_MSG1 "OnStepXは「ホーム位置」または「パーク済み」である必要があります。そうでない場合、再起動要求は無視されます。"

// ----------------------------  SETUP ----------------------------

#define L_NETWORK_PERFORMANCE "パフォーマンスと互換性"
#define L_NETWORK_CMD_TO "コマンドチャネル シリアル読み取りタイムアウト"
#define L_NETWORK_WWW_TO "Webチャネル シリアル読み取りタイムアウト"

#define L_NETWORK_STA_TITLE "ステーションモード (アクセスポイントに接続)"
#define L_NETWORK_PWD "パスワード"
#define L_NETWORK_PWD_MSG "最小8文字"
#define L_NETWORK_EN_DHCP "DHCPを有効化"
#define L_NETWORK_EN_DHCP_MSG "注意: DHCPが有効な場合、上記のアドレスは無視されます"
#define L_NETWORK_EN_STA "ステーションモードを有効化"
#define L_IP_ADDRESS "IPアドレス"
#define L_GATEWAY "ゲートウェイ"
#define L_SUBNET "サブネットマスク"
#define L_NETWORK_CHA "チャンネル"
#define L_NETWORK_EN_AP_MODE "アクセスポイントモードを有効化"
#define L_NETWORK_OTA_TITLE "OTA SWS firmware update"
#define L_NETWORK_EN_OTA "Enable OTA Mode"
#define L_REBOOT_OTA "Reboot into OTA mode"
#define L_NETWORK_CONFIG_SECURITY "ネットワーク設定セキュリティ"


#define L_NETWORK_AP "アクセスポイントモード"

#define L_NETWORK_RESTART_MSG "変更を反映するには <u>手動で</u> 再起動する必要があります。"

#define L_NETWORK_TITLE "設定を変更するにはパスワードを入力してください:"

#define L_NETWORK_ADVICE1 "セットアップ:<br/><br/>"
#define L_NETWORK_ADVICE2 "ステーションモード <b>または</b> アクセスポイントモードのいずれかを有効にしてください。両方を有効にするとパフォーマンスの問題が発生する可能性があります。<br/><br/>"
#define L_NETWORK_ADVICE3 "OnStepを自宅のWiFiネットワークに接続させたい場合は、ステーションモードのみを有効にし、"
#define L_NETWORK_ADVICE4 "アクセスポイントモードを無効にしてください。OnStepが自宅のWiFiに接続できない場合、"
#define L_NETWORK_ADVICE5 "自動的にアクセスポイントモードを有効にします。これは、遠征先（暗い場所）にいる場合や、"
#define L_NETWORK_ADVICE6 "締め出し（ロックアウト）を防ぐのに役立ちます。<br/><br/>"
#define L_NETWORK_ADVICE7 "WiFiから締め出された場合は、Arduino IDEを使用してESPのフラッシュメモリをリセット（つまり"
#define L_NETWORK_ADVICE8 "デフォルトに戻す）できます。Arduinoのツールメニューから、Erase Flashを 'Only Sketch' "
#define L_NETWORK_ADVICE9 "から 'All Flash Contents' に変更し、WiFi-Bluetoothスケッチを再度アップロードしてください。"

// --------------------------- Constants -------------------------

#define L_DOWN_TITLE "OnStepへのシリアルインターフェースがダウンしています！"
#define L_DOWN_MESSAGE1 "OnStepを再起動したばかりの場合は、すべてが起動するまで数秒待ってから、WiFiに接続されているか確認してください。"
#define L_DOWN_MESSAGE2 "問題が解決しない場合、以下の原因が考えられます:<br /><ul>"
#define L_DOWN_MESSAGE3 "OnStepのみの再起動によるボーレートの不一致。すべての電源を一度切り、入れ直してください。"
#define L_DOWN_MESSAGE4 "SWSのボーレート設定が間違っています。"
#define L_DOWN_MESSAGE5 "OnStepのボーレート設定が間違っています。"
#define L_DOWN_MESSAGE6 "COMポートの配線が間違っています。"
#define L_DOWN_MESSAGE7 "電源ピンの配線が間違っています。GNDはOnStepのMCUとアドオンのMCUの間で共有されている必要があります。"

#define L_CONTROLLER "コントローラ" // was Status
#define L_PAGE_MOUNT "マウント" // was Control
#define L_PAGE_AUXILIARY "補助"
#define L_PAGE_LIBRARY "ライブラリ"
#define L_PAGE_ENCODERS "エンコーダ"
#define L_PAGE_PEC "PEC"
#define L_SETTINGS "設定"
#define L_CONTROLS "操作"
#define L_PAGE_CONFIG "構成"
#define L_PAGE_NETWORK "ネットワーク"

// ------------------------- MountStatus -------------------------

// general (background) errors
#define L_GE_NONE "なし"
#define L_GE_MOTOR_FAULT "モーター/ドライバ障害"
#define L_GE_ALT_MIN "地平線リミット未満" 
#define L_GE_LIMIT_SENSE "リミット検知"
#define L_GE_DEC "赤緯リミット超過"
#define L_GE_AZM "方位リミット超過"
#define L_GE_UNDER_POLE "極下リミット超過"
#define L_GE_MERIDIAN "子午線リミット超過"
#define L_GE_SYNC "Sync安全リミット超過"
#define L_GE_PARK "パーク失敗"
#define L_GE_GOTO_SYNC "Goto sync失敗"
#define L_GE_UNSPECIFIED "不明なエラー"
#define L_GE_ALT_MAX "天頂リミット超過"
#define L_GE_WEATHER_INIT "気象センサー初期化失敗"
#define L_GE_SITE_INIT "時刻または場所が未更新"
#define L_GE_NV_INIT "NV/EEPROM初期化エラー"
#define L_GE_OTHER "不明なエラー、コード"

// command errors
#define L_CE_NONE "エラーなし"
#define L_CE_0 "応答 0"
#define L_CE_CMD_UNKNOWN "不明なコマンド"
#define L_CE_REPLY_UNKNOWN "無効な応答"
#define L_CE_PARAM_RANGE "パラメータ範囲外"
#define L_CE_PARAM_FORM "パラメータ形式不正"
#define L_CE_ALIGN_FAIL "アライメント失敗"
#define L_CE_ALIGN_NOT_ACTIVE "アライメント非アクティブ"
#define L_CE_NOT_PARKED_OR_AT_HOME "非パーク または 非ホーム"
#define L_CE_PARKED "すでにパーク済み"
#define L_CE_PARK_FAILED "パーク失敗"
#define L_CE_NOT_PARKED "パークされていません"
#define L_CE_NO_PARK_POSITION_SET "パーク位置が未設定"
#define L_CE_GOTO_FAIL "自動導入(Goto)失敗"
#define L_CE_LIBRARY_FULL "ライブラリが一杯です"
#define L_CE_GOTO_ERR_BELOW_HORIZON "地平線以下のため導入不可"
#define L_CE_GOTO_ERR_ABOVE_OVERHEAD "天頂リミット超過"
#define L_CE_SLEW_ERR_IN_STANDBY "スタンバイ中のスルー操作"
#define L_CE_SLEW_ERR_IN_PARK "パーク中のスルー操作"
#define L_CE_GOTO_ERR_GOTO "すでに自動導入動作中"
#define L_CE_GOTO_ERR_OUTSIDE_LIMITS "リミット外への導入"
#define L_CE_SLEW_ERR_HARDWARE_FAULT "ハードウェア障害"
#define L_CE_MOUNT_IN_MOTION "架台が動作中"
#define L_CE_GOTO_ERR_UNSPECIFIED "その他"
#define L_CE_UNK "不明"