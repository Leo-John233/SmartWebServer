// -----------------------------------------------------------------------------------
// Telescope auxiliary feature related functions 
#include "Auxiliary.h"

#include "../Page.h"
#include "../Pages.common.h"

#include "../../lib/convert/Convert.h"

extern void handleNotFound();
void processAuxGet();

// 通道名称来自主控配置，作为文本输出并保留完整名称
void auxiliaryName(String &data, const char *name) {
  // 连续文本批量追加，兼容旧内核没有公开定长 concat 的情况
  char part[65];
  size_t used = 0;
  for (const char *c = name;; ++c) {
    const bool escaped = *c == '&' || *c == '<' || *c == '>';
    if (!*c || escaped || used == sizeof(part) - 1) {
      if (used) { part[used] = 0; data.concat(part); used = 0; }
    }
    if (!*c) break;
    if (*c == '&') data.concat(F("&amp;"));
    else if (*c == '<') data.concat(F("&lt;"));
    else if (*c == '>') data.concat(F("&gt;"));
    else part[used++] = *c;
  }
}

// 每行固定为标签、操作和读数，沿用原有状态节点与请求参数
void auxiliaryRowBegin(String &data, int channel, int field, const char *label) {
  char temp[80];
  snprintf(temp, sizeof(temp), "<div class='sws-aux-row'><span class='sws-aux-label' id='aux%d-label%d'>", channel, field);
  data.concat(temp);
  data.concat(label);
  data.concat(F("</span><div class='sws-aux-controls'>"));
}

void auxiliarySliderRow(String &data, int channel, int field, const char *label, const char *control, int value, const char *reading, const char *unit) {
  char temp[160];
  auxiliaryRowBegin(data, channel, field, label);
  data.concat(FPSTR(control));
  snprintf(temp, sizeof(temp), "%d' aria-labelledby='aux%d-title aux%d-label%d' onchange=\"sz('x%dv%d',this.value)\">", value, channel, channel, field, channel, field);
  data.concat(temp);
  data.concat(F("</div><span class='sws-aux-value'>"));
  snprintf(temp, sizeof(temp), "<span id='x%dv%d'>", channel, field);
  data.concat(temp);
  data.concat(reading);
  data.concat(F("</span>"));
  data.concat(unit);
  data.concat(F("</span></div>"));
  www.sendContentAndClear(data);
}

void handleAux() {
  char temp[240] = "";
  char temp1[80] = "";

  SERIAL_ONSTEP.setTimeout(webTimeout);
  onStep.serialRecvFlush();

  // 仅处理已提交参数，再读取最新状态供首屏显示
  if (www.args() > 0) processAuxGet();
  state.updateAuxiliary(false, true);
  if (status.auxiliaryFound != SD_TRUE) { handleNotFound(); return; }

  www.setContentLength(CONTENT_LENGTH_UNKNOWN);
  www.sendHeader("Cache-Control", "no-cache");
  www.send(200, "text/html", String());

  pageHead();
  #if OPERATIONAL_MODE == WIFI
    if (!www.client().connected()) return;
  #endif
  String data;
  data.reserve(1024);
  data.concat(FPSTR(html_body_begin));
  www.sendContentAndClear(data);
  pageHeader(PAGE_AUXILIARY);
  data.concat(FPSTR(html_onstep_page_begin));

  // OnStep wasn't found, show warning and info.
  if (!status.onStepFound) {
    data.concat(FPSTR(html_bad_comms_message));
    data.concat(FPSTR(html_page_and_body_end));
    www.sendContentAndClear(data);
    www.sendContent("");
    return;
  }

  // scripts
  snprintf_P(temp, sizeof(temp), html_script_ajax_get, "auxiliary-ajax-get.txt");
  data.concat(temp);
  data.concat(FPSTR(html_script_ajax_shortcuts));

  // active ajax page is: auxAjax();
  data.concat(F("<script>var ajaxPage='auxiliary-ajax.txt';</script>\n"));
  www.sendContentAndClear(data);
  data.concat(FPSTR(html_script_ajax));
  www.sendContentAndClear(data);

  // 原始 SWS 的功能类型、文字来源与参数保持不变，只调整布局
  data.concat(F("<div class='sws-device-layout sws-auxiliary'><div class='sws-aux-grid'>"));
  if (status.auxiliaryFound == SD_TRUE) {
    for (int i = 0; i < 8; i++) {
      if (!pageClientConnected()) return;
      state.selectFeature(i);
      int purpose = state.featurePurpose(), channel = i + 1;
      if (purpose <= 0) continue;
      const char *type = "Unknown";
      switch (purpose) {
        case SWITCH: type = "Switch"; break;
        case ANALOG_OUTPUT: type = "Analog Out"; break;
        case DEW_HEATER: type = "Dew Heater"; break;
        case INTERVALOMETER: type = "Intervalometer"; break;
      }
      data.concat(F("<div class='b1 sws-aux-card'><header class='sws-aux-head'><div class='sws-aux-title'><span>"));
      auxiliaryName(data, state.featureName());
      snprintf(temp, sizeof(temp), "</span><h2 id='aux%d-title'>", channel);
      data.concat(temp);
      data.concat(type);
      data.concat(F("</h2></div><div class='sws-aux-power c'>"));
      float voltageV = state.featureVoltage(), currentI = state.featureCurrent();
      if (!isnan(voltageV)) {
        snprintf(temp, sizeof(temp), "<span><span id='vout%d'>?</span> V</span>", channel);
        data.concat(temp);
      }
      if (!isnan(currentI)) {
        snprintf(temp, sizeof(temp), "<span><span id='iout%d'>?</span> A</span>", channel);
        data.concat(temp);
      }
      data.concat(F("</div></header><div class='sws-aux-body'>"));
      www.sendContentAndClear(data);

      if (purpose == SWITCH || purpose == DEW_HEATER) {
        #if UNITS == METRIC
          const char *dewLabel = "DP " L_DP_MSG " (&deg;C)";
        #else
          const char *dewLabel = "DP " L_DP_MSG " (&deg;F)";
        #endif
        auxiliaryRowBegin(data, channel, 1, purpose == SWITCH ? "" : dewLabel);
        snprintf_P(temp, sizeof(temp), html_auxOnSwitch, channel, channel); data.concat(temp);
        snprintf_P(temp, sizeof(temp), html_auxOffSwitch, channel, channel); data.concat(temp);
        data.concat(F("</div><span class='sws-aux-value'>"));
        if (purpose == DEW_HEATER) {
          dtostrf(celsiusToNativeRelative(state.featureValue4()), 3, 1, temp1);
          snprintf(temp, sizeof(temp), "&Delta;<span id='x%dv4'>%s</span>&deg;" TEMPERATURE_UNITS_ABV, channel, temp1);
          data.concat(temp);
        }
        data.concat(F("</span></div>"));
        www.sendContentAndClear(data);
      }
      if (purpose == ANALOG_OUTPUT) {
        snprintf(temp1, sizeof(temp1), "%d", (int)lround((state.featureValue1()/255.0)*100.0));
        auxiliarySliderRow(data, channel, 1, "", html_auxAnalog, state.featureValue1(), temp1, "%");
      } else if (purpose == DEW_HEATER) {
        dtostrf(celsiusToNativeRelative(state.featureValue2()), 3, 1, temp1);
        auxiliarySliderRow(data, channel, 2, L_DP_ZERO, html_auxHeater, (int)lround(celsiusToNativeRelative(state.featureValue2())*DEW_HEATER_CONTROL_SCALE), temp1, "&deg;" TEMPERATURE_UNITS_ABV);
        dtostrf(celsiusToNativeRelative(state.featureValue3()), 3, 1, temp1);
        auxiliarySliderRow(data, channel, 3, L_DP_SPAN, html_auxHeater, (int)lround(celsiusToNativeRelative(state.featureValue3())*DEW_HEATER_CONTROL_SCALE), temp1, "&deg;" TEMPERATURE_UNITS_ABV);
      } else if (purpose == INTERVALOMETER) {
        auxiliaryRowBegin(data, channel, 1, "");
        data.concat(FPSTR(html_auxStartStop1));
        snprintf(temp, sizeof(temp), "x%dv1", channel); data.concat(temp);
        data.concat(FPSTR(html_auxStartStop2)); data.concat(temp);
        data.concat(FPSTR(html_auxStartStop3));
        snprintf(temp, sizeof(temp), "</div><span class='sws-aux-value' id='x%dv1'>-</span></div>", channel); data.concat(temp);
        dtostrf(state.featureValue4(), 0, 0, temp1);
        auxiliarySliderRow(data, channel, 4, L_CAMERA_COUNT, html_auxCount, (int)state.featureValue4(), temp1, " x");
        float value = state.featureValue2();
        int digits = value < 1.0 ? 3 : value < 10.0 ? 2 : value < 30.0 ? 1 : 0;
        dtostrf(value, 0, digits, temp1);
        auxiliarySliderRow(data, channel, 2, L_CAMERA_EXPOSURE, html_auxExposure, (int)timeToByte(value), temp1, " sec");
        value = state.featureValue3();
        digits = value < 10.0 ? 2 : value < 30.0 ? 1 : 0;
        dtostrf(value, 0, digits, temp1);
        auxiliarySliderRow(data, channel, 3, L_CAMERA_DELAY, html_auxDelay, (int)timeToByte(value), temp1, " sec");
      }
      data.concat(F("</div></div>"));
      www.sendContentAndClear(data);
    }
  }
  data.concat(F("</div></div>"));
  data.concat(FPSTR(html_page_and_body_end));
  www.sendContentAndClear(data);
  www.sendContent("");
}

void auxAjaxGet() {
  www.setContentLength(CONTENT_LENGTH_UNKNOWN);
  www.sendHeader("Cache-Control", "no-cache");
  www.send(200, "text/plain", String());

  processAuxGet();

  www.sendContent("");
}

void auxAjax() {
  String data="";
  data.reserve(1024);
  char temp[120]="";

  www.setContentLength(CONTENT_LENGTH_UNKNOWN);
  www.sendHeader("Cache-Control", "no-cache");
  www.send(200, "text/plain", String());

  // update auxiliary feature values
  if (status.auxiliaryFound == SD_TRUE) {

    for (int i = 0; i < 8; i++) {
      state.selectFeature(i);

      if (state.featurePurpose() && state.featurePurpose() != INTERVALOMETER && state.featurePurpose() != COVER_SWITCH) {

        const float voltageV = state.featureVoltage();
        const float currentI = state.featureCurrent();
        if (!isnan(voltageV)) {
          if (state.featureValue1()) {
            snprintf(temp, sizeof(temp), "vout%d|", i + 1);
            data.concat(temp);
            sprintF(temp, "%3.1f\n", voltageV);
            data.concat(temp);
          } else {
            snprintf(temp, sizeof(temp), "vout%d|0.0\n", i + 1);
            data.concat(temp);
          }
        }

        if (!isnan(currentI)) {
          if (fabs(currentI) > 0.2499F) {
            snprintf(temp, sizeof(temp), "iout%d|", i + 1);
            data.concat(temp);
            sprintF(temp, "%3.1f\n", currentI);
            data.concat(temp);
          } else {
            snprintf(temp, sizeof(temp), "iout%d|0.0\n", i + 1);
            data.concat(temp);
          }
        }
      }

      if (state.featurePurpose() == SWITCH) {
        if (state.featureValue1() != 0) {
          snprintf(temp, sizeof(temp), "sw%d_on|%s\n",i+1,"selected"); data.concat(temp);
          snprintf(temp, sizeof(temp), "sw%d_off|%s\n",i+1,"unselected"); data.concat(temp);
        } else {
          snprintf(temp, sizeof(temp), "sw%d_on|%s\n",i+1,"unselected"); data.concat(temp);
          snprintf(temp, sizeof(temp), "sw%d_off|%s\n",i+1,"selected"); data.concat(temp);
        }
      } else

      if (state.featurePurpose() == ANALOG_OUTPUT) {
        snprintf(temp, sizeof(temp), "x%dv1|%d\n",i+1,(int)lround((state.featureValue1()/255.0)*100.0)); data.concat(temp);
      } else

      if (state.featurePurpose() == DEW_HEATER) {
        char s[40];
        if (state.featureValue1() != 0) {
          snprintf(temp, sizeof(temp), "sw%d_on|%s\n",i+1,"selected"); data.concat(temp);
          snprintf(temp, sizeof(temp), "sw%d_off|%s\n",i+1,"unselected"); data.concat(temp);
        } else {
          snprintf(temp, sizeof(temp), "sw%d_on|%s\n",i+1,"unselected"); data.concat(temp);
          snprintf(temp, sizeof(temp), "sw%d_off|%s\n",i+1,"selected"); data.concat(temp);
        }
        dtostrf(celsiusToNativeRelative(state.featureValue2()),3,1,s); snprintf(temp, sizeof(temp), "x%dv2|%s\n",i+1,s); data.concat(temp);
        dtostrf(celsiusToNativeRelative(state.featureValue3()),3,1,s); snprintf(temp, sizeof(temp), "x%dv3|%s\n",i+1,s); data.concat(temp);
        dtostrf(celsiusToNativeRelative(state.featureValue4()),3,1,s); snprintf(temp, sizeof(temp), "x%dv4|%s\n",i+1,s); data.concat(temp);
      } else

      if (state.featurePurpose() == INTERVALOMETER) {
        char s[40];
        float v; int d;
        
        v=state.featureValue1();
        if (fabs(v) < 0.001) snprintf(temp, sizeof(temp), "x%dv1|-\n",i+1); else snprintf(temp, sizeof(temp), "x%dv1|%d\n",i+1,(int)v); data.concat(temp);
        v=state.featureValue2(); if (v < 1.0) d=3; else if (v < 10.0) d=2; else if (v < 30.0) d=1; else d=0;
        dtostrf(v,0,d,s); snprintf(temp, sizeof(temp), "x%dv2|%s\n",i+1,s); data.concat(temp);
        v=state.featureValue3(); if (v < 10.0) d=2; else if (v < 30.0) d=1; else d=0;
        dtostrf(v,0,d,s); snprintf(temp, sizeof(temp), "x%dv3|%s\n",i+1,s); data.concat(temp);
        snprintf(temp, sizeof(temp), "x%dv4|%d\n",i+1,(int)state.featureValue4()); data.concat(temp);
      }
    }
  }

  www.sendContentAndClear(data);
  www.sendContent("");

  state.lastAuxPageLoadTime = millis();
}

void processAuxGet() {
  String v;
  char temp[80] = "";
  char temp1[40] = "";

  // Auxiliary Feature set Value1 to Value4
  for (char c = '1'; c <= '8'; c++) {
    state.selectFeature(c - '1');

    snprintf(temp, sizeof(temp), "x%cv1", c);
    v = www.arg(temp);
    if (!v.equals(EmptyStr)) {
      snprintf(temp, sizeof(temp), ":SXX%c,V%s#", c, v.c_str());
      onStep.commandBool(temp);
    }

    if (state.featurePurpose() == DEW_HEATER) {
      snprintf(temp, sizeof(temp), "x%cv2", c);
      v = www.arg(temp);
      if (!v.equals(EmptyStr)) {
        dtostrf(nativeToCelsiusRelative(v.toFloat()/DEW_HEATER_CONTROL_SCALE), 0, 1, temp1);
        snprintf(temp, sizeof(temp), ":SXX%c,Z%s#", c, temp1);
        onStep.commandBool(temp);
      }
      snprintf(temp, sizeof(temp), "x%cv3", c);
      v = www.arg(temp);
      if (!v.equals(EmptyStr)) {
        dtostrf(nativeToCelsiusRelative(v.toFloat()/DEW_HEATER_CONTROL_SCALE), 0, 1, temp1);
        snprintf(temp, sizeof(temp), ":SXX%c,S%s#", c, temp1);
        onStep.commandBool(temp);
      }
    } else

    if (state.featurePurpose() == INTERVALOMETER) {
      snprintf(temp, sizeof(temp), "x%cv2", c);
      v = www.arg(temp);
      if (!v.equals(EmptyStr)) {
        dtostrf(byteToTime(v.toInt()), 0, 3, temp1);
        snprintf(temp, sizeof(temp), ":SXX%c,E%s#", c, temp1);
        onStep.commandBool(temp);
      }
      snprintf(temp, sizeof(temp), "x%cv3", c); v = www.arg(temp);
      if (!v.equals(EmptyStr)) {
        dtostrf(byteToTime(v.toInt()), 0, 2, temp1);
        snprintf(temp, sizeof(temp), ":SXX%c,D%s#", c, temp1);
        onStep.commandBool(temp);
      }
      snprintf(temp, sizeof(temp), "x%cv4", c); v = www.arg(temp);
      if (!v.equals(EmptyStr)) {
        dtostrf(v.toFloat(), 0, 0, temp1);
        snprintf(temp, sizeof(temp), ":SXX%c,C%s#", c, temp1);
        onStep.commandBool(temp);
      }
    }
  }

  state.lastAuxPageLoadTime = millis();
}
