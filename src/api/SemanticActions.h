#pragma once

#include "../Common.h"

bool performMountGuideAction(const String& value);
bool performMountGotoAction(const String& value);
bool performMountTrackingAction(const String& key, const String& value);
bool performMountHomeAction(const String& key, const String& value);
bool performMountAlignAction(const String& key, const String& value);
bool performMountPecAction(const String& key, const String& value);
bool performRotatorAction(const String& key, const String& value);
bool performFocuserAction(const String& key, const String& value);
bool performEncoderAction(const String& value);
