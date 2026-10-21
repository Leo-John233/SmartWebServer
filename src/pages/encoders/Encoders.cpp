// -----------------------------------------------------------------------------------
// Encoders
#include "Encoders.h"

#include "../Page.h"
#include "../../extensions/ui/Layouts.h"
#include "../Pages.common.h"
#include "../../extensions/ui/Template.h"

#if ENCODERS == ON

void processEncodersGet();

void handleEncoders()
{

  SERIAL_ONSTEP.setTimeout(webTimeout);
  onStep.serialRecvFlush();

  // 仅处理已提交参数，再读取最新状态供首屏显示
  if (www.args() > 0) processEncodersGet();
  state.updateMount(true);

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
  pageHeader(PAGE_ENCODERS);
  data.concat(FPSTR(html_onstep_page_begin));

  // OnStep wasn't found, show warning and info.
  if (!status.onStepFound)
  {
    data.concat(FPSTR(html_bad_comms_message));
    data.concat(FPSTR(html_page_and_body_end));
    www.sendContentAndClear(data);
    www.sendContent("");
    return;
  }

  // scripts
  appendPageFormat(data, html_script_ajax_get, "enc-ajax-get.txt");

  data.concat(F("<script>var ajaxPage='enc-ajax.txt';</script>\n"));
  www.sendContentAndClear(data);
  data.concat(FPSTR(html_script_ajax));
  www.sendContentAndClear(data);

  // 同步状态与两轴参数分区排列，避免窄屏并排挤压
  data.concat(FPSTR(html_layout_encoders_begin));
  if (!pageClientConnected()) return;
  syncTile(data);
  data.concat(FPSTR(html_layout_encoders_settings));
  if (!pageClientConnected()) return;
  encAxisTile(0, data);
  if (!pageClientConnected()) return;
  encAxisTile(1, data);
  data.concat(FPSTR(html_layout_encoders_end));

  data.concat(FPSTR(html_page_and_body_end));

  data.concat(FPSTR(html_script_collapsible));

  www.sendContentAndClear(data);
  www.sendContent("");
}

void encAjaxGet()
{
  www.setContentLength(CONTENT_LENGTH_UNKNOWN);
  www.sendHeader("Cache-Control", "no-cache");
  www.send(200, "text/plain", String());

  processEncodersGet();

  www.sendContent("");
}

void encAjax()
{
  String data = "";
  data.reserve(1024);

  www.setContentLength(CONTENT_LENGTH_UNKNOWN);
  www.sendHeader("Cache-Control", "no-cache");
  www.send(200, "text/plain", String());

  syncTileAjax(data);
  encAxisTileAjax(data);
  
  www.sendContentAndClear(data);
  www.sendContent("");

  state.lastMountPageLoadTime = millis();
}

void processEncodersGet()
{
  String v;

  syncTileGet();
  encAxisTileGet();

  state.lastMountPageLoadTime = millis();
}
#endif
