// -----------------------------------------------------------------------------------
// Telescope control related functions
#include "Mount.h"
#include "../../extensions/ui/Template.h"

#include "../KeyValue.h"
#include "../Page.h"
#include "../../extensions/ui/Layouts.h"
#include "../Pages.common.h"
#include "../../extensions/mount/CoordinateInput.h"

void processMountGet();

void handleMount()
{
  SERIAL_ONSTEP.setTimeout(webTimeout);
  onStep.serialRecvFlush();

  // 仅处理已提交参数，再读取最新状态供首屏显示
  if (www.args() > 0) processMountGet();
  state.updateMount(true);

  www.setContentLength(CONTENT_LENGTH_UNKNOWN);
  www.sendHeader("Cache-Control", "no-cache");
  www.send(200, "text/html", String());

  pageHead(true);
  #if OPERATIONAL_MODE == WIFI
    if (!www.client().connected()) return;
  #endif
  String data;
  data.reserve(1024);
  data.concat(FPSTR(html_body_begin));
  www.sendContentAndClear(data);
  pageHeader(PAGE_MOUNT);
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
  appendPageFormat(data, html_script_ajax_get, "mount-ajax-get.txt");
  data.concat(FPSTR(html_script_ajax_shortcuts));
  data.concat(FPSTR(html_script_ajax_date_time_return));
  data.concat(F("<script>var ajaxPage='mount-ajax.txt';</script>\n"));
  www.sendContentAndClear(data);
  data.concat(FPSTR(html_script_ajax));
  www.sendContentAndClear(data);

  // 移动控制之后显示位置与限位，再显示坐标 GOTO 和其余设置
  data.concat(FPSTR(html_layout_mount_begin));
  if (!pageClientConnected()) return;
  guideTile(data);
  data.concat(FPSTR(html_layout_mount_tracking));
  if (!pageClientConnected()) return;
  trackingTile(data);
  data.concat(FPSTR(html_layout_mount_location));
  if (!pageClientConnected()) return;
  siteTile(data);
  if (!pageClientConnected()) return;
  limitsTile(data);
  data.concat(FPSTR(html_layout_mount_target));
  if (!pageClientConnected()) return;
  gotoTile(data);
  data.concat(FPSTR(html_layout_mount_home));
  if (!pageClientConnected()) return;
  homeParkTile(data);
  data.concat(FPSTR(html_layout_mount_library));
  if (!pageClientConnected()) return;
  libraryTile(data);
  data.concat(FPSTR(html_layout_mount_advanced));
  if (!pageClientConnected()) return;
  alignTile(data);
  if (!pageClientConnected()) return;
  if (status.pecEnabled) pecTile(data);
  if (!pageClientConnected()) return;
  trackingSettingsTile(data);
  #if DISPLAY_COORDINATE_ORIGIN == ON
    if (!pageClientConnected()) return;
    coordinateTile(data);
  #endif
  data.concat(FPSTR(html_layout_mount_end));

  data.concat(FPSTR(html_page_and_body_end));

  data.concat(FPSTR(html_script_collapsible));

  www.sendContentAndClear(data);
  www.sendContent("");
}

void mountAjaxGet()
{
  www.setContentLength(CONTENT_LENGTH_UNKNOWN);
  www.sendHeader("Cache-Control", "no-cache");
  www.send(200, "text/plain", String());

  const String drive = www.arg("dr");

  // 坐标操作返回本次结果，避免与其他控制参数混合执行
  if (drive.equals("T1") || drive.equals("T0")) {
    // 跟踪开关必须返回主控的实际应答，不能用 HTTP 200 代替设备成功
    char reply[80] = "";
    const bool received = onStep.command(drive.equals("T1") ? ":Te#" : ":Td#", reply);
    www.sendContent(!strcmp(reply, "0") ? "rejected" : !received ? "timeout" : !strcmp(reply, "1") ? "ok" : "invalid");
  }
  else if (!www.arg("coord_action").equals(EmptyStr)) gotoCoordinatesGet();
  else processMountGet();

  www.sendContent("");
}

void mountAjax()
{
  String data = "";
  data.reserve(1024);

  www.setContentLength(CONTENT_LENGTH_UNKNOWN);
  www.sendHeader("Cache-Control", "no-cache");
  www.send(200, "text/plain", String());

  if (status.onStepFound)
  {
    siteTileAjax(data);
    homeParkTileAjax(data);
    alignTileAjax(data);
    gotoTileAjax(data);
    libraryTileAjax(data);
    guideTileAjax(data);
    trackingTileAjax(data);
    if (status.pecEnabled) pecTileAjax(data);
    limitsTileAjax(data);
  }
  else
  {
    // Disable everything
    static const char keys_en[27][8] = {
      "c_goto", "bzr_off", "bzr_on", "mfa_off", "mfa_on", "mfp_off", "mfp_on",
      "sr_vf", "sr_f", "sr_n", "sr_s", "sr_vs", "trk_off", "trk_on", "alg1",
      "alg2", "alg3", "alga", "rpa", "trk_sid", "trk_sol", "trk_lun",
      "ot_on", "ot_ref", "ot_off", "ot_dul", "ot_sgl",
    };
    for (int i = 0; i < 27; i++) data.concat(keyValueBoolEnabled(keys_en[i], false));

    static const char keys_str[14][18] = {
      "date_ut", "time_ut", "time_lst", "site_long", "site_lat", "pier_side", "idx_a1", 
      "idx_a2", "tgt_a1", "tgt_a2", "track", "align_progress", "align_lr", "align_ud",
    };
    for (int i = 0; i < 5; i++) data.concat(keyValueString(keys_str[i], "?"));
  }

  www.sendContentAndClear(data);
  www.sendContent("");

  state.lastMountPageLoadTime = millis();
}

void processMountGet()
{
  if (www.args() == 0) return;
  siteTileGet();
  homeParkTileGet();
  #if DISPLAY_COORDINATE_ORIGIN == ON
    coordinateTileGet();
  #endif
  alignTileGet();
  gotoTileGet();
  libraryTileGet();
  guideTileGet();
  trackingTileGet();
  if (status.pecEnabled) pecTileGet();
  limitsTileGet();

  state.lastMountPageLoadTime = millis();
}

