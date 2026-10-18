// SWS standard page
#include <Arduino.h>
#include "Page.h"

#include "Pages.common.h"

void pageHeader(int selected) {
  char temp[80];
  String data;
  data.reserve(1024);
  // 沿用原版在固件端生成页头，浏览器收到的就是最终导航结构
  data.concat(F("<div class='t'><div class='sws-brand'>"));
  if (strlen(status.configName) > 0) data.concat(status.configName); else data.concat(F("OnStep"));
  data.concat(F("</div><nav class='sws-nav' aria-label='" L_UI_NAVIGATION "'>"));

  data.concat(FPSTR(html_links_idx_begin));
  if (selected == PAGE_CONTROLLER) data.concat(FPSTR(html_links_selected));
  data.concat(FPSTR(html_links_idx_end));

  www.sendContentAndClear(data);
  if (status.mountFound == SD_TRUE) {
    data.concat(FPSTR(html_links_mnt_begin));
    if (selected == PAGE_MOUNT) data.concat(FPSTR(html_links_selected));
    data.concat(FPSTR(html_links_mnt_end));
    www.sendContentAndClear(data);
  }

  if (status.onStepFound) {
    data.concat(FPSTR(html_links_net_begin));
    if (selected == PAGE_NETWORK) data.concat(FPSTR(html_links_selected));
    data.concat(FPSTR(html_links_net_end));
    www.sendContentAndClear(data);
  }
  bool extras = status.rotatorFound == SD_TRUE || status.focuserFound == SD_TRUE || status.auxiliaryFound == SD_TRUE;
  #if ENCODERS == ON
    extras = extras || status.mountFound == SD_TRUE;
  #endif
  if (extras) {
    data.concat(F("<input type='checkbox' id='sws-nav-toggle' class='sws-nav-toggle' aria-label='" L_UI_MORE "'><label class='sws-nav-more' for='sws-nav-toggle' data-active='"));
    data.concat(selected >= PAGE_ROTATOR && selected <= PAGE_ENCODERS ? F("true") : F("false"));
    data.concat(F("'>" SWS_NAV_ICON(more) "<span>" L_UI_MORE "</span></label><div class='sws-extra-nav'>"));
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
    www.sendContentAndClear(data);
  }

  if (status.auxiliaryFound == SD_TRUE) {
    data.concat(FPSTR(html_links_aux_begin));
    if (selected == PAGE_AUXILIARY) data.concat(FPSTR(html_links_selected));
    data.concat(FPSTR(html_links_aux_end));
    www.sendContentAndClear(data);
  }

  #if ENCODERS == ON
    if (status.mountFound == SD_TRUE) {
      data.concat(FPSTR(html_links_enc_begin));
      if (selected == PAGE_ENCODERS) data.concat(FPSTR(html_links_selected));
      data.concat(FPSTR(html_links_enc_end));
      www.sendContentAndClear(data);
    }
  #endif
  if (extras) data.concat(F("</div>"));
  data.concat(F("</nav><div class='sws-sidebar-note'>" L_SIDEBAR_NOTE "</div></div><header class='sws-topbar'><h1 id='sws-page-title'>"));
  switch (selected) {
    case PAGE_MOUNT: data.concat(F(L_PAGE_MOUNT)); break;
    case PAGE_ROTATOR: data.concat(F(L_ROTATOR)); break;
    case PAGE_FOCUSER: data.concat(F(L_FOCUSER)); break;
    case PAGE_AUXILIARY: data.concat(F(L_PAGE_AUXILIARY)); break;
    case PAGE_ENCODERS: data.concat(F(L_PAGE_ENCODERS)); break;
    case PAGE_NETWORK: data.concat(F(L_PAGE_NETWORK)); break;
    default: data.concat(F(L_CONTROLLER)); break;
  }
  data.concat(F("</h1><span class='sws-version'>Smart Web Server "));
  data.concat(firmwareVersion.str);
  data.concat(F(" (OnStep"));
  if (status.getVersionStr(temp)) data.concat(temp); else data.concat(F("?"));
  data.concat(F(")</span></header>"));
  www.sendContentAndClear(data);
}
