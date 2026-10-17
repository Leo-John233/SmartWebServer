
#include <string>
#include <cstring>
#include <cstdio>
#include <cassert>
#include <map>
#include <vector>
#include <iostream>
#include <functional>
#include <stdint.h>
#define SWS_ACTIVITY_LED_PIN 2
#define SWS_RA_BRAKE_PIN 5
#define RA_BRAKE_PROTECTION ON
#define AXIS1_ENCODER OFF
#define AXIS2_ENCODER OFF
#define DISPLAY_RESET_CONTROLS OFF
#define FWU -204
#define LOW 0
#define HIGH 1
#define OUTPUT 1
#define OFF -1
#define ON 1
#define SD_TRUE 2
#define LED_STATUS_ON_STATE HIGH
#define F(x) x
#define FPSTR(x) x
#define snprintf_P snprintf
#define HTTP_GET 1
#define HTTP_POST 3
#define OPERATIONAL_MODE 2
#define WIFI 2
#define PROGMEM
uint32_t now=0;
uint32_t millis(){return now;}
std::map<int,int>pins;std::vector<int>pinEvents;
void digitalWrite(int pin,int value){pins[pin]=value;pinEvents.push_back(pin*10+value);}
void pinMode(int pin,int){assert(pins.count(pin));}
struct String:std::string{
  using std::string::string;
  void concat(const char*s){append(s);}void concat(const String&s){append(s);}
  void replace(const char*from,const char*to){size_t p=0;while((p=find(from,p))!=npos){std::string::replace(p,strlen(from),to);p+=strlen(to);}}
};
struct Status{bool onStepFound=false;int mountFound=0;}status;
struct Device{
  int calls=0,stops=0;bool received=true,stopSuccess=true;const char*reply="ST,,,,,,,";std::function<void()>stopHook;
  bool processCommand(const char*command,char*out,int timeout){
    assert(timeout==200);
    if(!strcmp(command,":Q#")){stops++;if(stopHook)stopHook();pinEvents.push_back(900);out[0]=0;return stopSuccess;}
    assert(!strcmp(command,":GXU1#"));calls++;strcpy(out,reply);strcat(out,"#");return received;
  }
}onStep;
struct KvPartition {
  enum class Status{Ok,Error};
  uint8_t stored=3;bool fail=false;
  bool getOrInit(const char*key,uint8_t&out){assert(!strcmp(key,"BOARD_IO_SETTINGS"));out=stored;return !fail;}
  Status put(const char*key,uint8_t value){assert(!strcmp(key,"BOARD_IO_SETTINGS"));if(fail)return Status::Error;stored=value;return Status::Ok;}
};
struct Nv{KvPartition store;KvPartition&kv(){return store;}}storage;
Nv&nv(){return storage;}
struct Tasks{int added=0;bool fail=false;bool add(unsigned long,int,bool,int,void(*)(),const char*){added++;return !fail;}}tasks;
struct Web{
  int methodValue=HTTP_GET,code=0;std::string output;std::map<std::string,String>args;
  std::function<void()> handler;
  int method(){return methodValue;}String arg(const char*k){return args[k];}
  void sendHeader(const char*,const char*){}
  void send(int c,const char*,const String&s){code=c;output=s;}
  void on(const char*,std::function<void()>fn){handler=fn;}
}www;


// 未配置自动保护时保持抱闸，Config.h 中的 ON/OFF 同时决定网页是否显示。
#ifndef RA_BRAKE_PROTECTION
  #define RA_BRAKE_PROTECTION OFF
#endif
#if RA_BRAKE_PROTECTION != ON && RA_BRAKE_PROTECTION != OFF
  #error "RA_BRAKE_PROTECTION must be ON or OFF."
#endif

// 物理引脚独立于功能开关：关闭保护仍需把抱闸引脚保持为 LOW。
#ifndef SWS_ACTIVITY_LED_PIN
  #if OPERATIONAL_MODE == WIFI && defined(ESP8266)
    #define SWS_ACTIVITY_LED_PIN LED_STATUS_PIN
  #else
    #define SWS_ACTIVITY_LED_PIN OFF
  #endif
#endif
#ifndef SWS_RA_BRAKE_PIN
  #if OPERATIONAL_MODE == WIFI && defined(RA_BRAKE_PIN)
    #define SWS_RA_BRAKE_PIN RA_BRAKE_PIN
  #else
    #define SWS_RA_BRAKE_PIN OFF
  #endif
#endif

// IO2 灌电流点亮 LED；IO5 驱动 MOS 管，高电平给抱闸线圈通电、释放抱闸。

#if SWS_RA_BRAKE_PIN != OFF
  #if AXIS2_ENCODER != OFF && AXIS2_ENCODER_A_PIN == OFF
    #error "Axis 2 encoder A is disabled: GPIO5 is mapped to RA_BRAKE_PIN. Remap the encoder before enabling it."
  #endif
  #if DISPLAY_RESET_CONTROLS == FWU && SWS_RA_BRAKE_PIN == BOOT0_PIN
    #error "IO5 is reserved for the RA brake; move BOOT0 before enabling firmware-upload controls."
  #endif
  #if AXIS1_ENCODER != OFF && (SWS_RA_BRAKE_PIN == AXIS1_ENCODER_A_PIN || SWS_RA_BRAKE_PIN == AXIS1_ENCODER_B_PIN)
    #error "RA brake pin conflicts with the axis 1 encoder."
  #endif
  #if AXIS2_ENCODER != OFF && (SWS_RA_BRAKE_PIN == AXIS2_ENCODER_A_PIN || SWS_RA_BRAKE_PIN == AXIS2_ENCODER_B_PIN)
    #error "RA brake pin conflicts with the axis 2 encoder."
  #endif
  #if defined(ESP32) && (SWS_RA_BRAKE_PIN == SERIAL_RX || SWS_RA_BRAKE_PIN == SERIAL_TX)
    #error "RA brake pin conflicts with the configured primary OnStep serial port."
  #endif
  #if defined(ESP32) && SERIAL_SWAP != OFF && (SWS_RA_BRAKE_PIN == SERIAL_SWAPPED_RX || SWS_RA_BRAKE_PIN == SERIAL_SWAPPED_TX)
    #error "RA brake pin conflicts with swapped OnStep serial. Set SERIAL_SWAP OFF or remap the serial pins."
  #endif
#endif




enum class BrakeState { Starting, Disabled, Waiting, Fault, Stale, Released };

// 无串口、网页、存储或 GPIO 依赖，可独立验证输出判据。
class BoardIoPolicy {
public:
  enum : uint32_t { PollIntervalMs = 250, FeedbackStaleMs = 1000, ReadyMs = 500 };
  void configure(bool activity, bool protection, uint32_t now);
  void traffic(uint32_t now);
  void driverFeedback(bool received, const char *reply, uint32_t now);
  bool ledLit(bool connected, bool originalIdleLit, uint32_t now) const;
  BrakeState brakeState(bool connected, uint32_t now) const;
  bool releaseBrake(bool connected, uint32_t now) const;
  bool activityEnabled() const { return activity_; }
  bool protectionEnabled() const { return protection_; }
  bool driverFault() const { return fault_; }
  bool feedbackDue(uint32_t now) const;

private:
  bool configured_ = false, activity_ = true, protection_ = true;
  bool trafficSeen_ = false, feedbackSeen_ = false, healthy_ = false, ready_ = false, fault_ = false;
  uint32_t trafficAt_ = 0, feedbackAt_ = 0, healthyAt_ = 0;
};







class BoardIo {
public:
  void initPins();
  void begin();
  void tick();
  void networkActivity();
  void pollDriver();
  void driverFeedback(bool received, const char *reply);
  bool save(bool activity, bool protection);
  bool ownsPin(int pin) const;
  bool activityAvailable() const { return SWS_ACTIVITY_LED_PIN != OFF; }
  bool brakeAvailable() const { return RA_BRAKE_PROTECTION == ON && SWS_RA_BRAKE_PIN != OFF; }
  const BoardIoPolicy &policy() const { return policy_; }
  BrakeState brakeState() const;

private:
  BoardIoPolicy policy_;
  bool started_ = false;
  bool stopping_ = false, stopIssued_ = false;
  int ledLevel_ = -1, brakeLevel_ = -1;
};

extern BoardIo boardIo;




// GXU1 的七个逗号分隔字段及末尾故障码；不把超时、0 或任意非空文本当作正常。
static bool readDriverReply(const char *reply, bool &fault) {
  const char *tokens[] = {"ST", "OA", "OB", "GA", "GB", "OT", "PW", "GF"};
  bool flags[8] = {};
  const char *field = reply;
  for (int i = 0; i < 8; i++) {
    const char *end = std::strchr(field, ',');
    if ((i < 7 && !end) || (i == 7 && end)) return false;
    const size_t length = end ? size_t(end - field) : std::strlen(field);
    if (length && (length != 2 || std::strncmp(field, tokens[i], 2))) return false;
    flags[i] = length != 0;
    if (end) field = end + 1;
  }
  // ST 只表示静止；其余任一故障位均触发停止与抱闸，包括静止时的开路。
  fault = false;
  for (int i = 1; i < 8; i++) fault = fault || flags[i];
  return true;
}

void BoardIoPolicy::configure(bool activity, bool protection, uint32_t now) {
  if (!configured_ || (!protection_ && protection)) {
    feedbackSeen_ = false;
    healthy_ = false;
    ready_ = false;
    fault_ = false;
    healthyAt_ = now;
  }
  configured_ = true;
  activity_ = activity;
  protection_ = protection;
}

void BoardIoPolicy::traffic(uint32_t now) {
  trafficSeen_ = true;
  trafficAt_ = now;
}

void BoardIoPolicy::driverFeedback(bool received, const char *reply, uint32_t now) {
  bool fault = false;
  const bool valid = received && reply && readDriverReply(reply, fault);
  fault_ = valid && fault;
  const bool healthy = valid && !fault_;
  if (!healthy_ || !feedbackSeen_ || uint32_t(now - feedbackAt_) > FeedbackStaleMs) {
    healthyAt_ = now;
    ready_ = false;
  }
  feedbackAt_ = now;
  feedbackSeen_ = true;
  healthy_ = healthy;
  // 必须由后续正常反馈确认稳定，不能收到一帧后仅靠等待就释放。
  if (!healthy) ready_ = false;
  else if (uint32_t(now - healthyAt_) >= ReadyMs) ready_ = true;
}

bool BoardIoPolicy::ledLit(bool connected, bool originalIdleLit, uint32_t now) const {
  if (!connected) return (now / 400) % 2 == 0;
  if (!activity_) return originalIdleLit;
  // 连续数据也保留熄灭间隙，避免频繁刷新时变成常亮。
  return trafficSeen_ && uint32_t(now - trafficAt_) < 180 && (now / 120) % 2 == 0;
}

BrakeState BoardIoPolicy::brakeState(bool connected, uint32_t now) const {
  if (!configured_) return BrakeState::Starting;
  if (!protection_) return BrakeState::Disabled;
  if (!connected || !feedbackSeen_) return BrakeState::Waiting;
  if (uint32_t(now - feedbackAt_) > FeedbackStaleMs) return BrakeState::Stale;
  if (!healthy_) return BrakeState::Fault;
  if (!ready_) return BrakeState::Waiting;
  return BrakeState::Released;
}

bool BoardIoPolicy::releaseBrake(bool connected, uint32_t now) const {
  const BrakeState state = brakeState(connected, now);
  // 关闭开关也保持抱闸，只有开启后的持续正常反馈才允许松闸。
  return state == BrakeState::Released;
}

bool BoardIoPolicy::feedbackDue(uint32_t now) const {
  return protection_ && (!feedbackSeen_ || uint32_t(now - feedbackAt_) >= PollIntervalMs);
}







BoardIo boardIo;
static void pollBoardOutputs() { boardIo.tick(); }
static void pollBoardDriver() { boardIo.pollDriver(); }

void BoardIo::initPins() {
  #if SWS_RA_BRAKE_PIN != OFF
    // 先设置输出锁存值，再切换方向；启动及等待主控期间保持线圈断电。
    digitalWrite(SWS_RA_BRAKE_PIN, LOW);
    pinMode(SWS_RA_BRAKE_PIN, OUTPUT);
    brakeLevel_ = LOW;
  #endif
}

void BoardIo::begin() {
  if (!activityAvailable() && !brakeAvailable()) return;
  uint8_t settings = 3; // 默认开启活动指示和自动保护，不持久保存“已就绪”。
  if (!nv().kv().getOrInit("BOARD_IO_SETTINGS", settings) || settings > 3) settings = 3;
  policy_.configure(settings & 1, brakeAvailable() && (settings & 2), millis());
  #if SWS_ACTIVITY_LED_PIN != OFF
    digitalWrite(SWS_ACTIVITY_LED_PIN, HIGH);
    pinMode(SWS_ACTIVITY_LED_PIN, OUTPUT);
    ledLevel_ = HIGH;
  #endif
  // 无法建立输出监控任务时维持启动锁定，不允许网页声称已经释放。
  if (!tasks.add(20, 0, true, 1, pollBoardOutputs, "BoardIO")) return;
  started_ = true;
  // 与命令通道同优先级，允许在网页让出执行权时采样，避免长页面使反馈过期。
  if (brakeAvailable()) tasks.add(BoardIoPolicy::PollIntervalMs, 0, true, 2, pollBoardDriver, "RaBrake");
  tick();
}

void BoardIo::tick() {
  if (!started_) return;
  const uint32_t now = millis();
  const bool connected = status.onStepFound && status.mountFound == SD_TRUE;
  const bool released = policy_.releaseBrake(connected, now);
  // 故障持续期间不重复停止，重新开启或稳定恢复后允许处理下一次故障。
  if (!stopping_ && (!policy_.protectionEnabled() || released)) stopIssued_ = false;
  #if SWS_ACTIVITY_LED_PIN != OFF
    const bool lit = policy_.ledLit(status.onStepFound, LED_STATUS_ON_STATE == LOW, now);
    const int level = lit ? LOW : HIGH;
    if (level != ledLevel_) { digitalWrite(SWS_ACTIVITY_LED_PIN, level); ledLevel_ = level; }
  #endif
  #if SWS_RA_BRAKE_PIN != OFF
    // 发送 STOP 期间即使其他任务调用 tick，也不能抢先改变抱闸输出。
    if (!stopping_) {
      const int brake = brakeAvailable() && released ? HIGH : LOW;
      if (brake != brakeLevel_) { digitalWrite(SWS_RA_BRAKE_PIN, brake); brakeLevel_ = brake; }
    }
  #endif
}

void BoardIo::networkActivity() {
  if (!started_ || !activityAvailable()) return;
  // 原始上电连接流程自行闪灯；网络启动后交由此模块合并活动脉冲。
  policy_.traffic(millis());
  tick();
}

void swsNetworkActivity() { boardIo.networkActivity(); }

void BoardIo::pollDriver() {
  if (!started_ || !brakeAvailable() || !policy_.feedbackDue(millis())) return;
  if (!status.onStepFound || status.mountFound != SD_TRUE) {
    driverFeedback(false, "");
    return;
  }
  char reply[80] = "";
  // 同一原始查询，不新增 OnStep 命令；不依赖浏览器是否打开控制器页面。
  const bool received = onStep.processCommand(":GXU1#", reply, 200);
  const size_t length = strlen(reply);
  if (length && reply[length - 1] == '#') reply[length - 1] = 0;
  driverFeedback(received, reply);
}

void BoardIo::driverFeedback(bool received, const char *reply) {
  if (!started_ || !brakeAvailable()) return;
  policy_.driverFeedback(received, reply, millis());
  if (policy_.protectionEnabled() && policy_.driverFault() && !stopIssued_) {
    stopIssued_ = true;
    stopping_ = true;
    char response[80] = "";
    // 与网页 STOP 相同的命令，不等待停稳；发送失败也继续抱闸。
    onStep.processCommand(":Q#", response, 200);
    stopping_ = false;
  }
  tick();
}

bool BoardIo::save(bool activity, bool protection) {
  if (!started_) return false;
  protection = brakeAvailable() && protection;
  const uint8_t settings = (activity ? 1 : 0) | (protection ? 2 : 0);
  if (nv().kv().put("BOARD_IO_SETTINGS", settings) != KvPartition::Status::Ok) return false;
  policy_.configure(activity, protection, millis());
  tick();
  return true;
}

bool BoardIo::ownsPin(int pin) const {
  return pin >= 0 && (pin == SWS_ACTIVITY_LED_PIN || pin == SWS_RA_BRAKE_PIN);
}

BrakeState BoardIo::brakeState() const {
  if (!started_) return BrakeState::Starting;
  return policy_.brakeState(status.onStepFound && status.mountFound == SD_TRUE, millis());
}

#define DISPLAY_LANGUAGE L_us
// -----------------------------------------------------------------------------------
// Constants
#pragma once

// misc.
#define PROD_ABV                     "SWS"

#define METRIC                       ON
#define IMPERIAL                     -200
#define LOCALE_DEFAULT               -201
#define AUTO_ON                      -202
#define AUTO_OFF                     -203
#define FWU                          -204

// ajax page update feature
#define AJAX_PAGE_UPDATE_RATE_FAST_MS 500     // fast update rate
#define AJAX_PAGE_UPDATE_FAST_SHED_MS 5000    // time before return to normal update rate
#define AJAX_PAGE_LAZY_GET_MS         1000    // wait time for lazy get

// various auxillary features
#define SWITCH                       1
#define ANALOG_OUTPUT                2
#define DEW_HEATER                   3
#define INTERVALOMETER               4
#define COVER_SWITCH                 7

// The settings below are for initialization only, afterward they are stored and recalled from EEPROM and must
// be changed in the web interface OR with a reset (for initialization again) as described in the Config.h comments
#define TIMEOUT_WEB                  200
#define TIMEOUT_COMMAND              200

// task manager
#define TASKS_MAX                    40      // up to 40 tasks
#define TASKS_SKIP_MISSED

// NV -------------------------------------------------------------------------------------------------------------------

// unique volume signature for this volume/partition layout
#define NV_VOLUME_SIGNATURE         0x0001u

// -----------------------------------------------------------------------------------
// Locales
#pragma once

// see Strings_xx.h for individual locale translations

// languages (one unique numeric code required for each)
#define L_ca 1 // Catalan
#define L_cn 2 // Chinese
#define L_de 3 // German
#define L_en 4 // English
#define L_es 5 // Spanish
#define L_fr 6 // French
#define L_it 7 // Italian
#define L_jp 8 // Japanese
#define L_us 9 // English (US, imperial units)
#define L_ro 10 // Romanian

// -----------------------------------------------------------------------------------
// Locale
#pragma once




// see Strings_xx.h for individual locale translations

// ISO639-1 language codes (these control the inclusion of individual Strings_xx.h files as specified in Config.h)
#if DISPLAY_LANGUAGE == L_ca
#include "E:/GitHub/SmartWebServer/legacy/SmartWebServer/src/locales/Strings_ca.h"
#define UNITS METRIC
#endif
#if DISPLAY_LANGUAGE == L_cn
  #include "E:/GitHub/SmartWebServer/legacy/SmartWebServer/src/locales/Strings_cn.h"
  #define UNITS METRIC
#endif
#if DISPLAY_LANGUAGE == L_de
  #include "E:/GitHub/SmartWebServer/legacy/SmartWebServer/src/locales/Strings_de.h"
  #define UNITS METRIC
#endif
#if DISPLAY_LANGUAGE == L_en
  #include "E:/GitHub/SmartWebServer/legacy/SmartWebServer/src/locales/Strings_en.h"
  #define UNITS METRIC
#endif
#if DISPLAY_LANGUAGE == L_es
  #include "E:/GitHub/SmartWebServer/legacy/SmartWebServer/src/locales/Strings_es.h"
  #define UNITS METRIC
#endif
#if DISPLAY_LANGUAGE == L_fr
  #include "E:/GitHub/SmartWebServer/legacy/SmartWebServer/src/locales/Strings_fr.h"
  #define UNITS METRIC
#endif
#if DISPLAY_LANGUAGE == L_it
  #include "E:/GitHub/SmartWebServer/legacy/SmartWebServer/src/locales/Strings_it.h"
  #define UNITS METRIC
#endif
#if DISPLAY_LANGUAGE == L_jp
  #include "E:/GitHub/SmartWebServer/legacy/SmartWebServer/src/locales/Strings_jp.h"
  #define UNITS METRIC
#endif
#if DISPLAY_LANGUAGE == L_us // not ISO639-1 but might be useful
  #include "E:/GitHub/SmartWebServer/legacy/SmartWebServer/src/locales/Strings_en.h"
  #define UNITS IMPERIAL
#endif
#if DISPLAY_LANGUAGE == L_ro
  #include "E:/GitHub/SmartWebServer/legacy/SmartWebServer/src/locales/Strings_ro.h"
  #define UNITS METRIC
#endif

// 未提供专用停止文字时沿用当前语言的停止描述
#ifndef BUTTON_STOP
  #define BUTTON_STOP L_STOP
#endif

// 未提供本地翻译时保留通用同步文字
#ifndef L_COORD_SYNC
  #define L_COORD_SYNC "Sync"
#endif

// misc. locale support functions
#ifndef DISPLAY_UNITS
  #define DISPLAY_UNITS UNITS
#else
  #if DISPLAY_UNITS == LOCALE_DEFAULT
    #undef DISPLAY_UNITS
    #define DISPLAY_UNITS UNITS
  #endif
#endif

#if DISPLAY_UNITS == IMPERIAL
  #define nativeToCelsius(t) ((t - 32.0F)*(5.0F/9.0F))
  #define celsiusToNative(t) (t*(9.0F/5.0F) + 32.0F)
  #define nativeToCelsiusRelative(t) (t*(5.0F/9.0F))
  #define celsiusToNativeRelative(t) (t*(9.0F/5.0F))
  #define TEMPERATURE_UNITS_ABV "F"
#else
  #define nativeToCelsius(t) (t)
  #define celsiusToNative(t) (t)
  #define nativeToCelsiusRelative(t) (t)
  #define celsiusToNativeRelative(t) (t)
  #define TEMPERATURE_UNITS_ABV "C"
#endif

// 其他语言未提供新标签时使用默认文字
#ifndef L_HOME_ACTIONS
#define L_HOME_ACTIONS "Home"
#endif
#ifndef L_PARK_ACTIONS
#define L_PARK_ACTIONS "Parking"
#endif
#ifndef L_COORD_GOTO
#define L_COORD_GOTO "Coordinate GOTO"
#endif
#ifndef L_COORD_SYSTEM
#define L_COORD_SYSTEM "Coordinate system"
#endif
#ifndef L_COORD_JNOW
#define L_COORD_JNOW "RA / Dec (JNOW)"
#endif
#ifndef L_COORD_RA
#define L_COORD_RA "RA"
#endif
#ifndef L_COORD_DEC
#define L_COORD_DEC "DEC"
#endif
#ifndef L_COORD_SIGN
#define L_COORD_SIGN "Sign"
#endif
#ifndef L_COORD_H
#define L_COORD_H "h"
#endif
#ifndef L_COORD_D
#define L_COORD_D "deg"
#endif
#ifndef L_COORD_M
#define L_COORD_M "min"
#endif
#ifndef L_COORD_S
#define L_COORD_S "sec"
#endif
#ifndef L_COORD_SEND
#define L_COORD_SEND "Sending coordinates"
#endif
#ifndef L_COORD_OK
#define L_COORD_OK "Command accepted"
#endif
#ifndef L_COORD_REJECT
#define L_COORD_REJECT "Command rejected, check device status"
#endif
#ifndef L_COORD_INVALID
#define L_COORD_INVALID "Enter valid coordinates; minutes and seconds must be zero at 90 degrees"
#endif
#ifndef L_COORD_NETWORK
#define L_COORD_NETWORK "Connection lost, check device before retrying"
#endif
#ifndef L_CAT_PREV
#define L_CAT_PREV "Previous"
#endif
#ifndef L_CAT_NEXT
#define L_CAT_NEXT "Next"
#endif
#ifndef L_CAT_TARGET
#define L_CAT_TARGET "Set target"
#endif
#ifndef L_CAT_HELP
#define L_CAT_HELP "Catalog help"
#endif

#ifndef L_COORD_AZ
#define L_COORD_AZ "Azimuth"
#endif
#ifndef L_COORD_ALT
#define L_COORD_ALT "Altitude"
#endif
#ifndef L_GOTO_ACTION
#define L_GOTO_ACTION "GOTO"
#endif

#ifndef L_GOTO_TARGET
#define L_GOTO_TARGET "Go to current target"
#endif
#ifndef L_SYNC_TARGET
#define L_SYNC_TARGET "Sync current target"
#endif

#ifndef L_TRK_AXES
#define L_TRK_AXES "Compensation axes"
#endif

#ifndef L_DEVICE_TRACKING
#define L_DEVICE_TRACKING "Device tracking"
#endif

#ifndef L_SIDEBAR_NOTE
#define L_SIDEBAR_NOTE "OnStep smart control"
#endif


const char html_board_io_styles[]="\n<style>\n.sws-overview-grid .content { padding:12px 0 0; background:transparent; }\n.sws-overview-grid .content > br { display:none; }\n.sws-controller-settings { display:grid; grid-template-columns:repeat(var(--settings-columns,3),minmax(0,1fr)); gap:14px; margin:0 0 18px; }\n.sws-io-controls { display:contents; }\n.sws-io-control, .sws-mount-type { min-width:0; margin:0; padding:18px; border:1px solid var(--border); border-radius:12px; background:var(--field); }\n.sws-io-control { display:grid; grid-template-columns:minmax(0,1fr) auto; gap:12px 16px; align-content:start; align-items:center; }\n.sws-io-control h3, .sws-mount-type > label { margin:0; font-size:17px; font-weight:600; line-height:1.5; overflow-wrap:anywhere; }\n.sws-io-control p { grid-column:1 / -1; margin:0; color:var(--muted); font-size:14px; font-weight:400; line-height:1.7; }\n.sws-io-control button.sws-switch { margin:0; width:44px; min-width:44px; height:26px; min-height:26px; padding:3px; background:#494949; flex-shrink:0; }\n.sws-io-control button.sws-switch::after { width:18px; height:18px; background:#fff; }\n.sws-io-control button.sws-switch[aria-checked='true'] { background:var(--accent); }\n.sws-io-control button.sws-switch[aria-checked='true']::after { transform:translateX(18px); }\n.sws-io-status { grid-column:1 / -1; padding-top:10px; border-top:1px solid var(--border); color:var(--text); font-size:14px; font-weight:400; line-height:1.6; }\n.sws-io-error { grid-column:1 / -1; order:1; margin:0; color:var(--alert,#d6b786); font-size:14px; }\n.sws-io-error:empty { display:none; }\n.sws-mount-type { display:flex; flex-direction:column; gap:12px; }\n.sws-controller .sws-mount-type select { width:100%; min-width:0; min-height:42px; margin:0; font-size:15px; }\n.sws-setting-actions { display:flex; flex-wrap:wrap; gap:8px; margin-top:auto; }\n.sws-controller .sws-setting-actions button { min-height:36px; margin:0; padding:6px 14px; font-size:14px; }\n.sws-setting-actions button[name='revert'] { background:#333; }\n@media(max-width:800px) { .sws-controller-settings { grid-template-columns:minmax(0,1fr); gap:10px; } .sws-io-control, .sws-mount-type { padding:16px; } }\n</style>\n";
const char html_board_io_script[]="\n<script>\ndocument.addEventListener('DOMContentLoaded', () => {\n  const controls = document.querySelector('.sws-io-controls');\n  if (!controls) return;\n  const buttons = [...controls.querySelectorAll('[role=switch]')];\n  let pending = false;\n  buttons.forEach((button) => {\n    const sync = () => button.setAttribute('aria-checked', String(button.dataset.selected === 'true'));\n    new MutationObserver(sync).observe(button, {attributes:true,attributeFilter:['data-selected']});\n    sync();\n    button.addEventListener('click', async () => {\n      if (pending) return;\n      const enabled = button.dataset.selected !== 'true';\n      if (button.dataset.setting === 'protection' && !enabled && !confirm(controls.dataset.confirm)) return;\n      pending = true;\n      window.swsPendingCommands = (window.swsPendingCommands || 0) + 1;\n      buttons.forEach((b) => b.disabled = true);\n      const error = controls.querySelector('[role=alert]');\n      error.textContent = '';\n      const abort = new AbortController(), timer = setTimeout(() => abort.abort(), 8000);\n      try {\n        const response = await fetch('/board-io', {\n          method:'POST', cache:'no-store', signal:abort.signal,\n          headers:{'Content-Type':'application/x-www-form-urlencoded'},\n          body:new URLSearchParams({setting:button.dataset.setting, enabled:enabled ? '1' : '0'}),\n        });\n        if (!response.ok) throw Error();\n        const state = await response.json();\n        buttons.forEach((b) => b.dataset.selected = String(state[b.dataset.setting]));\n        const status = document.getElementById('sws-io-brake-state');\n        if (status) status.textContent = state.message;\n      } catch {\n        error.textContent = controls.dataset.error;\n      } finally {\n        clearTimeout(timer);\n        pending = false;\n        window.swsPendingCommands--;\n        buttons.forEach((b) => b.disabled = false);\n      }\n    });\n  });\n});\n</script>\n";
const char html_board_io_begin[]="<section class='sws-io-controls' data-confirm=\"Turning protection off keeps the brake locked; continue?\" data-error=\"Save failed, please retry\">";
const char html_board_io_activity[]="<div class='sws-io-control'><h3>Network activity LED</h3><button type='button' id='sws-io-activity' class='sws-switch' role='switch' aria-label=\"Network activity LED\" aria-checked='%s' data-selected='%s' data-setting='activity'></button><p>Blinks on data traffic and while the controller is disconnected</p></div>";
const char html_board_io_brake[]="<div class='sws-io-control'><h3>RA stop and brake</h3><button type='button' id='sws-io-protection' class='sws-switch' role='switch' aria-label=\"RA stop and brake\" aria-checked='%s' data-selected='%s' data-setting='protection'></button><p>Driver faults send STOP before locking the brake; stays locked when off</p><div id='sws-io-brake-state' class='sws-io-status' role='status'>%s</div></div>";




static const char *brakeMessage() {
  switch (boardIo.brakeState()) {
    case BrakeState::Starting: return L_IO_BRAKE_STARTING;
    case BrakeState::Disabled: return L_IO_BRAKE_DISABLED;
    case BrakeState::Waiting: return L_IO_BRAKE_WAITING;
    case BrakeState::Fault: return L_IO_BRAKE_FAULT;
    case BrakeState::Stale: return L_IO_BRAKE_STALE;
    case BrakeState::Released: return L_IO_BRAKE_RELEASED;
  }
  return L_UNKNOWN;
}

void boardIoTile(String &data) {
  if (!boardIo.activityAvailable() && !boardIo.brakeAvailable()) return;
  char buffer[1200];
  data.concat(FPSTR(html_board_io_begin));
  if (boardIo.activityAvailable()) {
    const char *enabled = boardIo.policy().activityEnabled() ? "true" : "false";
    snprintf_P(buffer, sizeof(buffer), html_board_io_activity, enabled, enabled);
    data.concat(buffer);
  }
  if (boardIo.brakeAvailable()) {
    const char *enabled = boardIo.policy().protectionEnabled() ? "true" : "false";
    snprintf_P(buffer, sizeof(buffer), html_board_io_brake, enabled, enabled, brakeMessage());
    data.concat(buffer);
  }
  data.concat(F("<p class='sws-io-error' role='alert'></p></section>"));
}

void boardIoTileAjax(String &data) {
  if (boardIo.activityAvailable()) {
    data.concat(F("sws-io-activity|"));
    data.concat(boardIo.policy().activityEnabled() ? "selected\n" : "unselected\n");
  }
  if (boardIo.brakeAvailable()) {
    data.concat(F("sws-io-protection|"));
    data.concat(boardIo.policy().protectionEnabled() ? "selected\n" : "unselected\n");
    data.concat(F("sws-io-brake-state|"));
    data.concat(brakeMessage());
    data.concat("\n");
  }
}

// 独立的 SWS 设置接口，不把本机 IO 设置传给 OnStep，也不复用运动命令。
void handleBoardIo() {
  www.sendHeader("Cache-Control", "no-store");
  if (www.method() == HTTP_POST) {
    const String setting = www.arg("setting"), value = www.arg("enabled");
    if ((setting != "activity" && setting != "protection") || (value != "0" && value != "1")) {
      www.send(400, "application/json", "{}"); return;
    }
    if ((setting == "activity" && !boardIo.activityAvailable()) ||
        (setting == "protection" && !boardIo.brakeAvailable())) {
      www.send(409, "application/json", "{}"); return;
    }
    const bool activity = setting == "activity" ? value == "1" : boardIo.policy().activityEnabled();
    const bool protection = setting == "protection" ? value == "1" : boardIo.policy().protectionEnabled();
    if (!boardIo.save(activity, protection)) { www.send(500, "application/json", "{}"); return; }
  } else if (www.method() != HTTP_GET) {
    www.send(405, "application/json", "{}"); return;
  }
  // 消息先作为 JSON 字符串转义，避免译文中的引号破坏响应。
  String message = brakeMessage();
  message.replace("\\", "\\\\"); message.replace("\"", "\\\"");
  String result = "{\"activity\":";
  result.concat(boardIo.policy().activityEnabled() ? "true" : "false");
  result.concat(",\"protection\":");
  result.concat(boardIo.policy().protectionEnabled() ? "true" : "false");
  result.concat(",\"message\":\""); result.concat(message); result.concat("\"}");
  www.send(200, "application/json; charset=utf-8", result);
}





void registerPageHandler(const char *path, void (*handler)()) {
  #if OPERATIONAL_MODE == WIFI
    www.on(path, [handler]() {
      swsNetworkActivity();
      handler();
      swsNetworkActivity();
    });
  #else
    www.on(path, handler);
  #endif
}

int main(){boardIo.initPins();boardIo.begin();status.onStepFound=true;status.mountFound=SD_TRUE;now=10;boardIo.driverFeedback(true,"ST,,,,,,,");now=600;boardIo.driverFeedback(true,"ST,,,,,,,");String data;boardIoTile(data);std::cout<<data;}