// SWS standard page
#include <Arduino.h>
#include "Page.h"
#include "Appearance.h"

#include "Pages.common.h"

void pageHeader(int selected) {
  char temp[80];
  String data = "";

  pageAppearance();
  data.concat(FPSTR(html_onstep_header_begin));
  
  if (strlen(status.configName) > 0) data.concat(status.configName); else data.concat(F("OnStep"));
  data.concat(FPSTR(html_onstep_header_title));
  data.concat(firmwareVersion.str);
  data.concat(F(" (OnStep"));
  if (status.getVersionStr(temp)) data.concat(temp); else data.concat("?");
  www.sendContentAndClear(data);

  data.concat(FPSTR(html_onstep_header_links));

  data.concat(FPSTR(html_links_idx_begin));
  if (selected == PAGE_CONTROLLER) data.concat(FPSTR(html_links_selected));
  data.concat(FPSTR(html_links_idx_end));

  if (status.mountFound == SD_TRUE) {
    data.concat(FPSTR(html_links_mnt_begin));
    if (selected == PAGE_MOUNT) data.concat(FPSTR(html_links_selected));
    data.concat(FPSTR(html_links_mnt_end));
  }

  if (status.rotatorFound == SD_TRUE) {
    data.concat(FPSTR(html_links_rot_begin));
    if (selected == PAGE_ROTATOR) data.concat(FPSTR(html_links_selected));
    data.concat(FPSTR(html_links_rot_end));
    www.sendContentAndClear(data);
  }

  if (status.focuserFound == SD_TRUE) {
    data.concat(FPSTR(html_links_foc_begin));
    if (selected == PAGE_FOCUSER) data.concat(FPSTR(html_links_selected));
    data.concat(FPSTR(html_links_foc_end));
  }

  if (status.auxiliaryFound == SD_TRUE) {
    data.concat(FPSTR(html_links_aux_begin));
    if (selected == PAGE_AUXILIARY) data.concat(FPSTR(html_links_selected));
    data.concat(FPSTR(html_links_aux_end));
  }

  #if ENCODERS == ON
    if (status.mountFound == SD_TRUE) {
      data.concat(FPSTR(html_links_enc_begin));
      if (selected == PAGE_ENCODERS) data.concat(FPSTR(html_links_selected));
      data.concat(FPSTR(html_links_enc_end));
    }
  #endif

  if (status.onStepFound) {
    data.concat(FPSTR(html_links_net_begin));
    if (selected == PAGE_NETWORK) data.concat(FPSTR(html_links_selected));
    data.concat(FPSTR(html_links_net_end));
  }

  data.concat(FPSTR(html_onstep_header_end));
  www.sendContentAndClear(data);
}

// 外壳不处理表单或带参数的命令，保持原有登录和设备控制接口语义
bool servePageShell(int selected) {
  if (www.method() != HTTP_GET || www.args() != 0 || !status.onStepFound) return false;
  www.setContentLength(CONTENT_LENGTH_UNKNOWN);
  www.sendHeader("Cache-Control", "no-store");
  www.send(200, "text/html", String());
  String data = FPSTR(html_head_begin);
  data.concat(FPSTR(html_head_end));
  data.concat(FPSTR(html_body_begin));
  www.sendContentAndClear(data);
  pageHeader(selected);
  www.sendContent("<p id='sws-load' role='status'></p><iframe id='sws-frame' title='标签内容'></iframe>");
  char part[513];
  for (size_t offset = 0; offset < sizeof(html_shell_script) - 1; offset += 512) {
    size_t count = min((size_t)512, sizeof(html_shell_script) - 1 - offset);
    memcpy_P(part, html_shell_script + offset, count); part[count] = 0;
    www.sendContent(part);
  }
  www.sendContent("");
  return true;
}

// 帮助和错误页面也使用统一样式
void pageAppearance() {
  // 分段输出共用主题，避免在小内存设备上复制整段样式
  const char *appearance = html_shared_appearance;
  char part[513];
  for (size_t offset = 0; offset < sizeof(html_shared_appearance) - 1; offset += 512) {
    size_t count = min((size_t)512, sizeof(html_shared_appearance) - 1 - offset);
    memcpy_P(part, appearance + offset, count); part[count] = 0;
    www.sendContent(part);
  }
}
