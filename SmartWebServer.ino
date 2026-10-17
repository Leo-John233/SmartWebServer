/*
 * 标题       OnStep Smart Web Server (智能网页服务器)
 * 作者       Howard Dutton
 *
 * Copyright (C) 2016 to 2026 Howard Dutton
 *
 * 本程序为自由软件：您可以根据自由软件基金会发布的 GNU 通用公共许可证（GNU GPL）
 * 的条款重新分发和/或修改它，无论是许可证的第 3 版，还是（由您选择）任何更高版本。
 *
 * 本程序的发布是希望它能有用，但【没有任何担保】；
 * 甚至没有对【适销性】或【特定用途适用性】的暗示性担保。
 * 有关更多详细信息，请参阅 GNU 通用公共许可证。
 *
 * 您应该随本程序一起收到了一份 GNU 通用公共许可证的副本。
 * 如果没有，请参阅 <http://www.gnu.org/licenses/>。
 *
 * 修订历史：请参阅 GitHub
 *
 * 作者: Howard Dutton
 * http://www.stellarjourney.com
 * hjd1964@gmail.com
 *
 * 描述
 *
 * 用于 OnStep 和 OnStepX 的网页和 IP 服务器
 *
 */

#define Product "Smart Web Server"
#define FirmwareVersionMajor  "2"
#define FirmwareVersionMinor  "10"
#define FirmwareVersionPatch  "h"

// 请使用 Config.h 根据您的要求配置 SWS (智能网页服务器)

#include "src/Common.h"

#ifdef OTA_PRESENT
  #include <ArduinoOTA.h>

  typedef struct OtaSettings {
    char password[64];
    bool enabled;
  } OtaSettings;
#endif

bool otaEnabled = false;

#include "src/lib/tasks/OnTask.h"
#include "src/lib/nv/Nv.h"
#include "src/lib/ethernet/cmdServer/CmdServer.h"
#include "src/lib/ethernet/webServer/WebServer.h"
#include "src/lib/wifi/cmdServer/CmdServer.h"
#include "src/lib/wifi/webServer/WebServer.h"

#include "src/libApp/cmd/Cmd.h"
#include "src/libApp/bleGamepad/BleGamepad.h"
#include "src/libApp/encoders/Encoders.h"
#include "src/libApp/status/Status.h"
#include "src/libApp/boardIo/BoardIo.h"

#include "src/pages/Pages.h"

#if DEBUG == PROFILER
  extern void profiler();
#endif

#if COMMAND_SERVER == PERSISTENT || COMMAND_SERVER == BOTH
  // 持久连接命令服务器 (通常用于 Planetarium 软件)
  CmdServer persistentCmdSvr1(9996, 10L*1000L, true);
  #if OPERATIONAL_MODE != ETHERNET_W5100
    CmdServer persistentCmdSvr2(9997, 10L*1000L, true);
  #endif
  CmdServer persistentCmdSvr3(9998, 10L*1000L, true);
#endif

#if COMMAND_SERVER == STANDARD || COMMAND_SERVER == BOTH
  // 标准命令服务器 (通常用于 App，端口 9999)
  CmdServer cmdSvr(9999, 1L*1000L);
#endif

void pollWebSvr() {
  www.handleClient(); // 处理网页客户端请求
}

void pollCmdSvr() {
  if (otaEnabled) return;

  #if COMMAND_SERVER == PERSISTENT || COMMAND_SERVER == BOTH
    persistentCmdSvr1.handleClient(); Y;

    #if OPERATIONAL_MODE != ETHERNET_W5100
      persistentCmdSvr2.handleClient(); Y;
    #endif
    persistentCmdSvr3.handleClient(); Y;
  #endif

  #if COMMAND_SERVER == STANDARD || COMMAND_SERVER == BOTH
    cmdSvr.handleClient();
    Y;
  #endif
}

void setup(void) {
  boardIo.initPins();
  #if OPERATIONAL_MODE == WIFI
    WiFi.disconnect();
    WiFi.softAPdisconnect(true); // 断开之前的 WiFi 连接，重置状态
  #endif

  sstrcpy(firmwareVersion.str, FirmwareVersionMajor "." FirmwareVersionMinor FirmwareVersionPatch);

  // 启动调试串口 (如果开启了调试模式)
  if (DEBUG == ON || DEBUG == VERBOSE || DEBUG == PROFILER) SERIAL_DEBUG.begin(SERIAL_DEBUG_BAUD);
  delay(2000);

  #ifdef PIN_INIT
    PIN_INIT();
  #endif

  VF("MSG: SmartWebServer "); VL(firmwareVersion.str);
  VF("MSG: MCU = "); VF(MCU_STR); V(", ");
  VF("Pinmap = "); VLF(PINMAP_STR); // 打印引脚映射信息

  delay(1000);

  // 调用蓝牙手柄初始化
  #if (BLE_GAMEPAD == ON && ESP32)
    VLF("MSG: Init BLE");
    bleInit();
  #endif

  // 调用硬件特定初始化 (HAL)
  VLF("MSG: Init HAL");
  HAL_INIT();

  #if LED_STATUS != OFF
    pinMode(LED_STATUS_PIN, OUTPUT);
  #endif

  // 尝试连接 OnStep 主控
  int serialSwap = OFF;
  serialSwap = SERIAL_SWAP;
  if (serialSwap == AUTO) serialSwap = AUTO_OFF;

  long serial_baud = SERIAL_BAUD;
  onStep.serialBegin(SERIAL_BAUD_DEFAULT, serialSwap); // 以默认波特率启动串口
  uint8_t tb = 1;
Again:
  VLF("MSG: Clearing serial channel");
  onStep.clearSerialChannel();

  // 寻找 OnStep 信号
  VLF("MSG: Attempting to contact OnStep");
  SERIAL_ONSTEP.print(":GVP#"); delay(100); // 发送查询产品命令
  String s = SERIAL_ONSTEP.readString();
  if (s == "On-Step#") {
    // 如果配置了不止一种波特率 (也就是想要高速通讯)
    if (SERIAL_BAUD != SERIAL_BAUD_DEFAULT) {
      // 获取 OnStep 支持的最快波特率，Mega2560 推荐返回 '4' 代表 19200
      SERIAL_ONSTEP.print(":GB#");
      delay(100);
      if (SERIAL_ONSTEP.available() != 1) { onStep.serialRecvFlush(); goto Again; }
      if (SERIAL_ONSTEP.read() == '4' && serial_baud > 19200) serial_baud = 19200;
      // 设置更快的波特率
      SERIAL_ONSTEP.print(onStep.highSpeedCommsStr(serial_baud)); delay(100);
      if (SERIAL_ONSTEP.available() != 1) { onStep.serialRecvFlush(); goto Again;
      }
      if (SERIAL_ONSTEP.read() != '1') goto Again;
    }

    // 一切就绪，只需更改本地波特率以匹配 OnStep
    onStep.serialBegin(serial_baud, serialSwap);
    #if DEBUG == REMOTE
      debugRemoteConnected = true;
    #endif
    
    VLF("MSG: OnStep Connection established"); // 连接建立成功
  } else {
    if (DEBUG == ON || DEBUG == VERBOSE) { VF("MSG: No valid reply found ("); V(s); VL(")");
    }
    #if LED_STATUS == ON
      digitalWrite(LED_STATUS_PIN, LED_STATUS_OFF_STATE);
    #endif
    // 没有收到有效回复，尝试切换波特率和/或交换串口引脚 (Swap)
    onStep.serialRecvFlush();
    tb++;
    // 循环尝试不同的组合：默认/交换引脚，默认/高速波特率
    if (tb == 16) { tb = 1; if (serialSwap == AUTO_OFF) serialSwap = AUTO_ON;
    else if (serialSwap == AUTO_ON) serialSwap = AUTO_OFF; }
    if (tb == 1) onStep.serialBegin(SERIAL_BAUD_DEFAULT, serialSwap);
    if (tb == 6) onStep.serialBegin(serial_baud, serialSwap);
    if (tb == 11) { if (SERIAL_BAUD_DEFAULT == 9600) { onStep.serialBegin(19200, serialSwap);
    } else tb = 15; }
    goto Again; // 重试
  }
  onStep.clearSerialChannel();

  // ----------------------------------------------------------------------------------------
  // start the NV/EEPROM subsystem for settings storage
  bool initErrorNv = false;
  bool success = nv().init(2);
  if (!success) { DLF("ERR: Setup, NV (EEPROM/FRAM/Flash/etc.) device not found!"); }

  #if defined(NV_WIPE) && NV_WIPE == ON
    if (success) nv().wipe();
  #endif

  NvVolume &nvVolume = nv().volume();

  success = success && (nvVolume.mount("SWS", NV_VOLUME_SIGNATURE) == NvVolume::Status::Ok);
  VF("MSG: Nv, volume ");
  if (success) { VLF("'SWS' mounted"); } else { VLF("invalid/unformatted"); }

  if (!success) {

    // automatic kv partition sizing
    uint32_t vSize = nvVolume.byteCount() - 32;
    uint32_t kvSize = vSize;
    if (vSize > 1039) {

      // start the volume format
      success = nvVolume.formatBegin("SWS", NV_VOLUME_SIGNATURE) == NvVolume::Status::Ok;
      if (success) { VF("MSG: Nv, volume 'SWS' format started ("); V(vSize); VLF(" bytes)"); }

      // add the KV partition
      success = success && nvVolume.formatAddPartition("KV", kvSize);
      if (success) { VF("MSG: Nv, volume format added 'KV' partition ("); V(kvSize); VLF(" bytes)"); }

      // finally commit the volume format
      success = success && (nvVolume.formatCommit() == NvVolume::Status::Ok);
      if (success) { VLF("MSG: Nv, volume format done"); }

      // try to mount the volume again
      success = success && (nvVolume.mount("SWS", NV_VOLUME_SIGNATURE) == NvVolume::Status::Ok);
      if (success) { VLF("MSG: Nv, volume 'SWS' mounted"); } else { DLF("WRN: Nv, volume 'SWS' mount FAILED!"); }

    } else {
      DLF("WRN: Nv, volume compatible storage device NOT FOUND!");
      success = false;
    }
  }

  // Bind global KV instance to the KV partition index
  success = success && (nv().kv().init(nvVolume, "KV") == KvPartition::Status::Ok);
  if (success) { VLF("MSG: Nv, partition 'KV' mounted"); } else { DLF("WRN: Nv, partition 'KV' mount FAILED!"); }

  if (!success) {
    VLF("WRN: Nv, init FAILED!");
    initErrorNv = true;
  }
  nv().kv().resetInitErrorFlag();

  // get the command and web timeouts
  if (!nv().kv().getOrInit("NETWORK_TIMEOUT_CMD", cmdTimeout)) { DLF("WRN: Nv, init failed for NETWORK_TIMEOUT_CMD"); }
  if (!nv().kv().getOrInit("NETWORK_TIMEOUT_WEB", webTimeout)) { DLF("WRN: Nv, init failed for NETWORK_TIMEOUT_WEB"); }

  // ----------------------------------------------------------------------------------------
  // bring network servers up
  #if OPERATIONAL_MODE == WIFI
    VLF("MSG: Init WiFi");
    wifiManager.init();
  #else
    VLF("MSG: Init Ethernet");
    ethernetManager.init();
  #endif

  // 准备编码器
  VLF("MSG: Initialize Encoders");
  encoders.init();

  // ----------------------------------------------------------------------------------------
  // init is done let the user see what's in the KV
  #if DEBUG != OFF
    KvPartition::Stats stats;
    if (success && nv().kv().stats(stats) == KvPartition::Status::Ok) {
      VF("MSG: Nv, partition 'KV' data blocks used = ");
      V(stats.dataBlocksTotal - stats.dataBlocksFree); VF(" (of "); V(stats.dataBlocksTotal); VF(")");
      VF(" key slots used = ");
      V(stats.slotsTotal - stats.slotsFree); VF(" (of "); V(stats.slotsTotal); VLF(")");
    }
  #endif

  // and capture any errors
  if (nv().kv().getInitErrorFlag()) initErrorNv = true;
  UNUSED(initErrorNv);
  // ----------------------------------------------------------------------------------------

  VLF("MSG: Set webpage handlers"); // 设置网页路由处理程序
  registerPageResources();
  registerPageHandler("/index.htm", handleRoot);
  registerPageHandler("/index-ajax-get.txt", indexAjaxGet);
  registerPageHandler("/index.txt", indexAjax);

  registerPageHandler("/mount.htm", handleMount);
  registerPageHandler("/mount-ajax-get.txt", mountAjaxGet);
  registerPageHandler("/mount-ajax.txt", mountAjax);
  registerPageHandler("/libraryHelp.htm", handleLibraryHelp);

  registerPageHandler("/rotator.htm", handleRotator);
  registerPageHandler("/rotator-ajax-get.txt", rotatorAjaxGet);
  registerPageHandler("/rotator-ajax.txt", rotatorAjax);

  registerPageHandler("/focuser.htm", handleFocuser);
  registerPageHandler("/focuser-ajax-get.txt", focuserAjaxGet);
  registerPageHandler("/focuser-ajax.txt", focuserAjax);

  registerPageHandler("/auxiliary.htm", handleAux);
  registerPageHandler("/auxiliary-ajax-get.txt", auxAjaxGet);
  registerPageHandler("/auxiliary-ajax.txt", auxAjax);
  #if ENCODERS == ON
    registerPageHandler("/enc.htm", handleEncoders);
    registerPageHandler("/enc-ajax-get.txt", encAjaxGet);
    registerPageHandler("/enc-ajax.txt", encAjax);
  #endif

  registerPageHandler("/net.htm", handleNetwork);
  #if OPERATIONAL_MODE == WIFI
    // 网络页面通过独立接口读取状态、搜索 Wi-Fi 和提交连接设置
    registerPageHandler("/net-wifi", handleWifiNetwork);
    if (boardIo.activityAvailable() || boardIo.brakeAvailable()) registerPageHandler("/board-io", handleBoardIo);
  #endif

  registerPageHandler("/", handleRoot);
  
  www.onNotFound(handleNotFound);

  #ifdef OTA_PRESENT
    OtaSettings otaSettings = {"", false};
    nv().kv().getOrInit("OTA_SETTINGS", otaSettings);
    if (strlen(otaSettings.password) == 0) otaSettings.enabled = false;

    if (otaSettings.enabled) {
      otaEnabled = true;
      VLF("MSG: Setup, bringing up OTA service for " HOST_NAME "-OTA");
      ArduinoOTA.setHostname(HOST_NAME "-OTA");
      ArduinoOTA.setPassword(otaSettings.password);
      ArduinoOTA.begin();
    }

    // since this disturbs operation (ESP8266) allow one-shot enable only
    if (otaSettings.enabled) {
      otaSettings.enabled = false;
      nv().kv().put("OTA_SETTINGS", otaSettings);
    }
  #endif

  #if COMMAND_SERVER == PERSISTENT || COMMAND_SERVER == BOTH
    if (!otaEnabled) {
      VLF("MSG: Starting port 9996 cmd server");
      persistentCmdSvr1.begin();
      #if OPERATIONAL_MODE != ETHERNET_W5100
        VLF("MSG: Starting port 9997 cmd server");
        persistentCmdSvr2.begin();
      #endif
      VLF("MSG: Starting port 9998 cmd server");
      persistentCmdSvr3.begin();
    } else {
      VLF("MSG: OTA mode active, command ports 9996..9998 disabled");
    }
  #endif

  #if COMMAND_SERVER == STANDARD || COMMAND_SERVER == BOTH
    if (!otaEnabled) {
      VLF("MSG: Starting port 9999 cmd server");
      cmdSvr.begin();
    } else {
      VLF("MSG: OTA mode active, command port 9999 disabled");
    }
  #endif

  VLF("MSG: Starting port 80 web server");
  www.begin();

  // 留出时间让后台服务器启动
  delay(2000);
  // 最后一次清空串口通道
  onStep.clearSerialChannel();

  if (status.onStepFound) {
    status.update();
    delay(100);
  }

  state.init();
  boardIo.begin();

  VF("MSG: Setup, starting cmd channel polling"); // 启动命令通道轮询
  VF(" task (rate 5ms priority 2)... ");
  if (tasks.add(5, 0, true, 2, pollCmdSvr, "cmdPoll")) { VL("success"); } else { VL("FAILED!");
  }

  VF("MSG: Setup, starting web server polling"); // 启动网页服务器轮询
  VF(" task (rate 20ms priority 3)... ");
  if (tasks.add(20, 0, true, 3, pollWebSvr, "webPoll")) { VL("success"); } else { VL("FAILED!");
  }

  // 启动任务管理器调试事件
  #if DEBUG == PROFILER
    tasks.add(142, 0, true, 7, profiler, "Profilr");
  #endif

  #if (BLE_GAMEPAD == ON && ESP32)
    bleSetup();
  #endif

  VLF("MSG: SmartWebServer ready");
}

void loop(void) {
  #if OPERATIONAL_MODE == WIFI
    // 逐步处理扫描和连接，避免等待路由器时阻塞设备的其他任务
    pollWifiNetwork();
  #endif
  #if (BLE_GAMEPAD == ON && ESP32)
    bleTimers(); Y;
    bleConnTest(); Y;
  #endif

  #ifdef OTA_PRESENT
    if (otaEnabled) ArduinoOTA.handle();
  #endif

  tasks.yield();
}
