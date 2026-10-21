// -----------------------------------------------------------------------------------
// Focuser
#include "Focuser.h"
#include "../../extensions/ui/Template.h"

#include "../KeyValue.h"
#include "../Page.h"
#include "../../extensions/ui/Layouts.h"
#include "../Pages.common.h"

extern void handleNotFound();
void processFocuserGet();

void handleFocuser() {
  SERIAL_ONSTEP.setTimeout(webTimeout);
  onStep.serialRecvFlush();

  // 仅处理已提交参数，再读取最新状态供首屏显示
  if (www.args() > 0) processFocuserGet();
  state.updateFocuser(true);
  if (status.focuserFound != SD_TRUE) { handleNotFound(); return; }

  www.setContentLength(CONTENT_LENGTH_UNKNOWN);
  www.sendHeader("Cache-Control", "no-cache");
  www.send(200, "text/html", String());

  // send a standard http response header
  pageHead();
  #if OPERATIONAL_MODE == WIFI
    if (!www.client().connected()) return;
  #endif
  String data;
  data.reserve(1024);
  data.concat(FPSTR(html_body_begin));
  www.sendContentAndClear(data);
  pageHeader(PAGE_FOCUSER);
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
  appendPageFormat(data, html_script_ajax_get, "focuser-ajax-get.txt");
  data.concat(FPSTR(html_script_ajax_shortcuts));
  data.concat(F("<script>var ajaxPage='focuser-ajax.txt';</script>\n"));
  www.sendContentAndClear(data);
  data.concat(FPSTR(html_script_ajax));
  www.sendContentAndClear(data);

  // 主操作与参数区并排，手机端按操作顺序堆叠
  data.concat(FPSTR(html_layout_focuser_begin));
  if (!pageClientConnected()) return;
  focuserSelectTile(data);
  data.concat(FPSTR(html_layout_focuser_motion));
  if (!pageClientConnected()) return;
  focuserSlewingTile(data);
  data.concat(FPSTR(html_layout_accessory_home));
  if (!pageClientConnected()) return;
  focuserHomeTile(data);
  data.concat(FPSTR(html_layout_accessory_settings));
  if (!pageClientConnected()) return;
  focuserBacklashTcfTile(data);
  data.concat(FPSTR(html_layout_accessory_end));

  data.concat(FPSTR(html_page_and_body_end));

  data.concat(FPSTR(html_script_collapsible));

  www.sendContentAndClear(data);
  www.sendContent("");
}

void focuserAjaxGet()
{
  www.setContentLength(CONTENT_LENGTH_UNKNOWN);
  www.sendHeader("Cache-Control", "no-cache");
  www.send(200, "text/plain", String());

  processFocuserGet();

  www.sendContent("");
}

void focuserAjax()
{
  String data = "";
  data.reserve(1024);

  www.setContentLength(CONTENT_LENGTH_UNKNOWN);
  www.sendHeader("Cache-Control", "no-cache");
  www.send(200, "text/plain", String());

  focuserSelectTileAjax(data);
  focuserHomeTileAjax(data);
  focuserSlewingTileAjax(data);
  focuserBacklashTcfTileAjax(data);

  www.sendContentAndClear(data);
  www.sendContent("");

  state.lastFocuserPageLoadTime = millis();
}

void processFocuserGet()
{
  if (www.args() == 0) return;
  focuserSelectTileGet();
  focuserHomeTileGet();
  focuserSlewingTileGet();
  focuserBacklashTcfTileGet();

  state.lastFocuserPageLoadTime = millis();
}
