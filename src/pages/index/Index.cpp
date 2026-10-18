// -----------------------------------------------------------------------------------
// The home page, status information
#include "Index.h"

#include "../Page.h"
#include "../../extensions/ui/Layouts.h"
#include "../Pages.common.h"
#include "../../extensions/boardIo/Page.h"
#include "../../extensions/boardIo/BoardIo.h"

void processIndexGet();

void handleRoot()
{
  char temp[240] = "";

  SERIAL_ONSTEP.setTimeout(webTimeout);
  onStep.serialRecvFlush();

  // 仅处理已提交参数，再读取最新状态供首屏显示
  if (www.args() > 0) processIndexGet();
  state.updateController(true);

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
  pageHeader(PAGE_CONTROLLER);
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
  data.concat(F("<script>var ajaxPage='index.txt';</script>\n"));
  www.sendContentAndClear(data);
  data.concat(FPSTR(html_script_ajax));
  www.sendContentAndClear(data);
  snprintf_P(temp, sizeof(temp), html_script_ajax_get, "index-ajax-get.txt");
  data.concat(temp);
  www.sendContentAndClear(data);

  // 概况与驱动分区展示，详细参数在卡片内展开
  data.concat(FPSTR(html_layout_controller_begin));


  if (!pageClientConnected()) return;
  statusTile(data);
  data.concat(FPSTR(html_layout_controller_axes));

  int numShown = 0;
  int numAxes = 5;
  if (status.getVersionMajor() >= 10) numAxes = 9;
  for (int axis = 0; axis < numAxes; axis++)
  {
    #if OPERATIONAL_MODE == WIFI
      // 只取消已断开页面的只读查询，不中断已经开始的串口事务。
      if (!www.client().connected()) return;
    #endif
    if (axisTile(axis, data)) numShown++;
  }

  data.concat(FPSTR(html_layout_controller_axes_end));

  #if DISPLAY_SERVO_MONITOR == ON
    if (!pageClientConnected()) return;
    data.concat(F("<hr>"));
    servoTile(data);
    data.concat(F("<br class=\"clear\" />\n"));
  #endif

  #if DISPLAY_STALLGUARD_MONITOR == ON
    if (!pageClientConnected()) return;
    data.concat(F("<hr>"));
    stallGuardTile(data);
    data.concat(F("<br class=\"clear\" />\n"));
  #endif

  #if DRIVE_CONFIGURATION == ON
    data.concat(FPSTR(html_layout_controller_advanced));
    if (numShown == 0) data.concat(F("<br />" L_ADV_SET_NO_EDIT "<br />"));

    snprintf_P(temp, sizeof(temp), html_form_begin, "index.htm");
    data.concat(temp);

    data.concat(F("<br /><button name='advanced' type='submit' "));
    if (numShown == 0) data.concat(F("value='enable'>" L_ADV_ENABLE "</button>")); else data.concat(F("value='disable'>" L_ADV_DISABLE "</button>"));

    data.concat(FPSTR(html_form_end));
    www.sendContentAndClear(data);

    if (numShown > 0) {
      data.concat(FPSTR(html_configAxesNotes));
      if (status.getVersionMajor() < 10) data.concat(FPSTR(html_configAxesNotesOnStep));
    } else
      data.concat(F("<br /><hr>"));
    www.sendContentAndClear(data);
  #endif

  #if DRIVE_CONFIGURATION == ON
    data.concat(FPSTR(html_layout_controller_advanced_end));
  #endif
  data.concat(FPSTR(html_layout_controller_end));
  data.concat(FPSTR(html_script_collapsible));

  data.concat(FPSTR(html_page_and_body_end));

  www.sendContentAndClear(data);
  www.sendContent("");
}

void indexAjaxGet()
{
  www.setContentLength(CONTENT_LENGTH_UNKNOWN);
  www.sendHeader("Cache-Control", "no-cache");
  www.send(200, "text/plain", String());

  processIndexGet();

  www.sendContent("");
}

void indexAjax() {
  String data = "";
  data.reserve(1024);

  www.setContentLength(CONTENT_LENGTH_UNKNOWN);
  www.sendHeader("Cache-Control", "no-cache");
  www.send(200, "text/plain", String());
  // 本机设置状态在主控掉线时也继续返回。
  boardIoTileAjax(data);
  if (!status.onStepFound && boardIo.brakeAvailable()) axisTileAjax(0, data);

  if (status.onStepFound)
  {
    #if DISPLAY_WEATHER == ON
      ambientTileAjax(data);
    #endif
    statusTileAjax(data);
    int numAxes = 2;
    if (status.getVersionMajor() >= 10) numAxes = 9;
    for (int axis = 0; axis < numAxes; axis++) axisTileAjax(axis, data);
    #if DISPLAY_SERVO_MONITOR == ON
      servoTileAjax(data);
    #endif
    #if DISPLAY_STALLGUARD_MONITOR == ON
      stallGuardTileAjax(data);
    #endif
  }

  www.sendContentAndClear(data);
  www.sendContent("");

  state.lastControllerPageLoadTime = millis();
}

void processIndexGet()
{
  String ssa = www.arg("advanced");
  if (ssa.equals("enable"))
  {
    onStep.commandBool(":SXAC,0#");
  } else
  if (ssa.equals("disable"))
  {
    onStep.commandBool(":SXAC,1#");
  }

  #if DISPLAY_WEATHER == ON
    ambientTileGet();
  #endif
  statusTileGet();
  axisTileGet();
  #if DISPLAY_SERVO_MONITOR == ON
    servoTileGet();
  #endif
  #if DISPLAY_STALLGUARD_MONITOR == ON
    stallGuardTileGet();
  #endif

  state.lastControllerPageLoadTime = millis();
}
