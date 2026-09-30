#include "SemanticActions.h"

#include "../pages/Pages.common.h"

bool performMountGuideAction(const String& value) {
  if (value == "n1") onStep.commandBlind(":Mn#"); else
  if (value == "s1") onStep.commandBlind(":Ms#"); else
  if (value == "e1") onStep.commandBlind(":Me#"); else
  if (value == "w1") onStep.commandBlind(":Mw#"); else
  if (value == "n0") onStep.commandBlind(":Qn#"); else
  if (value == "s0") onStep.commandBlind(":Qs#"); else
  if (value == "e0") onStep.commandBlind(":Qe#"); else
  if (value == "w0") onStep.commandBlind(":Qw#"); else
  if (value == "q") onStep.commandBlind(":Q#"); else
  if (value == "sy") return onStep.commandBool(":CS#"); else
  if (value.length() == 2 && value.charAt(0) == 'R' && value.charAt(1) >= '0' && value.charAt(1) <= '9') {
    char command[] = ":R0#";
    command[2] = value.charAt(1);
    onStep.commandBlind(command);
  } else return false;
  return true;
}

bool performMountGotoAction(const String& value) {
  if (value == "vs") return onStep.commandBool(":SX93,5#");
  if (value == "s") return onStep.commandBool(":SX93,4#");
  if (value == "n") return onStep.commandBool(":SX93,3#");
  if (value == "f") return onStep.commandBool(":SX93,2#");
  if (value == "vf") return onStep.commandBool(":SX93,1#");
  if (value == "bzr_on") return onStep.commandBool(":SX97,1#");
  if (value == "bzr_off") return onStep.commandBool(":SX97,0#");
  if (value == "af_now") return onStep.commandBool(":MN#");
  if (value == "af_on") return onStep.commandBool(":SX95,1#");
  if (value == "af_off") return onStep.commandBool(":SX95,0#");
  if (value == "mp_on") return onStep.commandBool(":SX98,1#");
  if (value == "mp_off") return onStep.commandBool(":SX98,0#");
  if (value == "mp_cnt") return onStep.commandBool(":SX99,1#");
  if (status.hasMeridianFlipHomeModes() && (status.mountType == MT_GEM || status.mountType == MT_FORK)) {
    if (value == "mh_off") return onStep.commandBool(":SX94,0#");
    if (value == "mh_visit") return onStep.commandBool(":SX94,1#");
    if (value == "mh_pause") return onStep.commandBool(":SX94,2#");
  }
  if (value == "pps_e") return onStep.commandBool(":SX96,E#");
  if (value == "pps_w") return onStep.commandBool(":SX96,W#");
  if (value == "pps_b") return onStep.commandBool(":SX96,B#");
  if (value == "pps_a") return onStep.commandBool(":SX96,A#");
  if (value == "go") {
    char response[80] = "";
    return onStep.command(":MS#", response);
  }
  if (value == "stop") {
    onStep.commandBlind(":Q#");
    return true;
  }
  return false;
}

bool performMountTrackingAction(const String& key, const String& value) {
  if (key == "tk") {
    if (value == "f") onStep.commandBlind(":T+#"); else
    if (value == "-") onStep.commandBlind(":T-#"); else
    if (value == "r") onStep.commandBlind(":TR#"); else return false;
    return true;
  }
  if (key == "rr") {
    if (value == "off") return onStep.commandBool(":Tn#");
    if (value == "on") return onStep.commandBool(":Tr#");
    if (value == "doff") return onStep.commandBool(":T1#");
    if (value == "don") return onStep.commandBool(":T2#");
    if (value == "otk") return onStep.commandBool(":To#");
    return false;
  }
  if (key == "dr") {
    if (value == "T1") return onStep.commandBool(":Te#");
    if (value == "T0") return onStep.commandBool(":Td#");
    if (value == "Ts") onStep.commandBlind(":TQ#"); else
    if (value == "Tl") onStep.commandBlind(":TL#"); else
    if (value == "Th") onStep.commandBlind(":TS#"); else
    if (value == "Tk") onStep.commandBlind(":TK#"); else return false;
    return true;
  }
  return false;
}

bool performMountHomeAction(const String& key, const String& value) {
  if (key == "pk") {
    if (value == "p") return onStep.commandBool(":hP#");
    if (value == "u") return onStep.commandBool(":hR#");
    if (value == "s") return onStep.commandBool(":hQ#");
  }
  if (key == "qb" || key == "dr") {
    if (value == "hf" || value == "qr") onStep.commandBlind(":hF#"); else
    if (value == "hc" || value == "qh") onStep.commandBlind(":hC#"); else
    if (value == "pk") return onStep.commandBool(":hP#"); else
    if (value == "pu") return onStep.commandBool(":hR#"); else return false;
    return true;
  }
  if (key == "ha" && (value == "0" || value == "1")) {
    onStep.commandBlind(value == "1" ? ":hA1#" : ":hA0#");
    return true;
  }
  return false;
}

bool performMountAlignAction(const String& key, const String& value) {
  if (key == "rp" && value == "a") return onStep.commandBool(":MP#");
  if (key != "al") return false;
  if (value == "n") return onStep.commandBool(":A+#");
  if (value == "q") {
    onStep.commandBlind(":Q#");
    return true;
  }
  if (value.length() == 1 && value.charAt(0) >= '1' && value.charAt(0) <= '9') {
    char command[] = ":A1#";
    command[2] = value.charAt(0);
    return onStep.commandBool(command);
  }
  return false;
}

bool performMountPecAction(const String& key, const String& value) {
  if (key == "ob" && value == "set") return onStep.commandBool(":SEO#");
  if (key != "pec") return false;
  if (value == "play") onStep.commandBlind(":$QZ+#"); else
  if (value == "stop") onStep.commandBlind(":$QZ-#"); else
  if (value == "clr") onStep.commandBlind(":$QZZ#"); else
  if (value == "rec") onStep.commandBlind(":$QZ/#"); else
  if (value == "wrt") onStep.commandBlind(":$QZ!#"); else return false;
  return true;
}

bool performRotatorAction(const String& key, const String& value) {
  if (key == "rot") {
    if (value == "stop") onStep.commandBlind(":rQ#"); else
    if (value == "ccwf") onStep.commandBlind(":r3#:rc#:r<#"); else
    if (value == "ccw") onStep.commandBlind(":r1#:rc#:r<#"); else
    if (value == "cw") onStep.commandBlind(":r1#:rc#:r>#"); else
    if (value == "cwf") onStep.commandBlind(":r3#:rc#:r>#"); else
    if (value == "vs") onStep.commandBlind(":r5#"); else
    if (value == "s") onStep.commandBlind(":r6#"); else
    if (value == "n") onStep.commandBlind(":r7#"); else
    if (value == "f") onStep.commandBlind(":r8#"); else
    if (value == "vf") onStep.commandBlind(":r9#"); else
    if (value == "reset") onStep.commandBlind(":rF#"); else
    if (value == "home") onStep.commandBlind(":rC#"); else
    if (value == "off") onStep.commandBlind(":r-#"); else
    if (value == "on") onStep.commandBlind(":r+#"); else
    if (value == "rev") onStep.commandBlind(":rR#"); else
    if (value == "par") onStep.commandBlind(":rP#"); else return false;
    return true;
  }
  if (key == "rs") {
    double position = value.toFloat();
    if (position < -360.0 || position > 360.0) return false;
    char angle[40], command[80];
    convert.doubleToDms(angle, position, true, true, PM_HIGH);
    snprintf(command, sizeof(command), ":rS%s#", angle);
    return onStep.commandBool(command);
  }
  if (key == "b3" && value.toInt() >= 0 && value.toInt() <= 32767) {
    char command[32];
    snprintf(command, sizeof(command), ":rb%d#", (int16_t)value.toInt());
    return onStep.commandBool(command);
  }
  return false;
}

bool performFocuserAction(const String& key, const String& value) {
  if (key == "foc") {
    if (value == "stop") onStep.commandBlind(":FQ#"); else
    if (value == "inf") onStep.commandBlind(":F4#:F-#"); else
    if (value == "in") onStep.commandBlind(":F2#:F-#"); else
    if (value == "out") onStep.commandBlind(":F2#:F+#"); else
    if (value == "outf") onStep.commandBlind(":F4#:F+#"); else
    if (value == "vs") onStep.commandBlind(":F5#"); else
    if (value == "s") onStep.commandBlind(":F6#"); else
    if (value == "n") onStep.commandBlind(":F7#"); else
    if (value == "f") onStep.commandBlind(":F8#"); else
    if (value == "vf") onStep.commandBlind(":F9#"); else return false;
    return true;
  }
  if (key == "dr") {
    if (value.length() == 2 && value.charAt(0) == 'F' && value.charAt(1) >= '1' && value.charAt(1) <= '6') {
      char command[] = ":FA1#";
      command[3] = value.charAt(1);
      if (onStep.commandBool(command)) state.focuserSelected = value.charAt(1) - '0';
      return true;
    }
    if (value == "FH") onStep.commandBlind(":FH#"); else
    if (value == "Fh") onStep.commandBlind(":Fh#"); else return false;
    return true;
  }
  if (key == "fs") {
    int position = value.toInt();
    if (position < -500000 || position > 500000) return false;
    char command[32];
    snprintf(command, sizeof(command), ":FS%d#", position);
    return onStep.commandBool(command);
  }
  return false;
}

bool performEncoderAction(const String& value) {
  #if ENCODERS == ON
    if (value == "o2e") encoders.syncFromOnStep(); else
    if (value == "e2o") encoders.syncToOnStep(); else
    #ifdef ENC_ABSOLUTE
      if (value == "zro") encoders.originFromOnStep(); else
    #endif
    if (value == "on") encoders.settings.autoSync = true; else
    if (value == "off") encoders.settings.autoSync = false; else return false;
    nv().kv().put("ENCODER_SETTINGS", encoders.settings);
    return true;
  #else
    UNUSED(value);
    return false;
  #endif
}
