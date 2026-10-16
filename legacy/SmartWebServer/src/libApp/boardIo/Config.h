#pragma once

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
