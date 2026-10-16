// -----------------------------------------------------------------------------------
// Locale
#pragma once

#include "../Constants.h"
#include "../../Config.h"

#include "Locales.h"

// see Strings_xx.h for individual locale translations

// ISO639-1 language codes (these control the inclusion of individual Strings_xx.h files as specified in Config.h)
#if DISPLAY_LANGUAGE == L_ca
#include "Strings_ca.h"
#define UNITS METRIC
#endif
#if DISPLAY_LANGUAGE == L_cn
  #include "Strings_cn.h"
  #define UNITS METRIC
#endif
#if DISPLAY_LANGUAGE == L_de
  #include "Strings_de.h"
  #define UNITS METRIC
#endif
#if DISPLAY_LANGUAGE == L_en
  #include "Strings_en.h"
  #define UNITS METRIC
#endif
#if DISPLAY_LANGUAGE == L_es
  #include "Strings_es.h"
  #define UNITS METRIC
#endif
#if DISPLAY_LANGUAGE == L_fr
  #include "Strings_fr.h"
  #define UNITS METRIC
#endif
#if DISPLAY_LANGUAGE == L_it
  #include "Strings_it.h"
  #define UNITS METRIC
#endif
#if DISPLAY_LANGUAGE == L_jp
  #include "Strings_jp.h"
  #define UNITS METRIC
#endif
#if DISPLAY_LANGUAGE == L_us // not ISO639-1 but might be useful
  #include "Strings_en.h"
  #define UNITS IMPERIAL
#endif
#if DISPLAY_LANGUAGE == L_ro
  #include "Strings_ro.h"
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
