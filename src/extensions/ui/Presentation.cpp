#include "../../pages/Page.h"
#include "Appearance.h"
#include "../../pages/Pages.common.h"
#include "../web/Request.h"
#include "../boardIo/Activity.h"

// Wi-Fi 直接使用内核的 Flash 发送接口，以太网保留原版字符串接口
void sendPageResource(const char *content, size_t length) {
  for (size_t offset = 0; offset < length; offset += 1024) {
    const size_t count = min(size_t(1024), length - offset);
    #if OPERATIONAL_MODE == WIFI
      www.sendContent_P(content + offset, count);
    #else
      char part[1025];
      memcpy_P(part, content + offset, count);
      part[count] = 0;
      www.sendContent(part);
    #endif
    swsNetworkActivity();
  }
}

// 只缓存静态外观资源，版本随本文件编译更新，刷入新固件后自动换地址
static void servePageStyles() {
  const size_t skip = sizeof("\n<style>") - 1, tail = sizeof("</style>\n") - 1;
  const size_t base = sizeof(html_shared_styles) - 1 - skip - tail;
  const size_t editors = sizeof(html_editor_styles) - 1 - skip - tail;
  const size_t io = sizeof(html_board_io_styles) - 1 - skip - tail;
  www.setContentLength(base + editors + io);
  www.sendHeader("Cache-Control", "public, max-age=31536000, immutable");
  www.send(200, "text/css; charset=utf-8", String());
  sendPageResource(html_shared_styles + skip, base);
  sendPageResource(html_editor_styles + skip, editors);
  sendPageResource(html_board_io_styles + skip, io);
}

static void servePageControls() {
  const size_t skip = sizeof("\n<script>") - 1, tail = sizeof("</script>\n") - 1;
  const size_t controls = sizeof(html_page_controls) - 1 - skip - tail;
  const size_t editors = sizeof(html_settings_editors) - 1 - skip - tail;
  const size_t io = sizeof(html_board_io_script) - 1 - skip - tail;
  const size_t navigation = sizeof(html_page_navigation) - 1 - skip - tail;
  www.setContentLength(controls + editors + io + navigation);
  www.sendHeader("Cache-Control", "public, max-age=31536000, immutable");
  www.send(200, "application/javascript; charset=utf-8", String());
  sendPageResource(html_page_controls + skip, controls);
  sendPageResource(html_settings_editors + skip, editors);
  sendPageResource(html_board_io_script + skip, io);
  sendPageResource(html_page_navigation + skip, navigation);
}

void registerPageResources() {
  registerPageHandler("/sws-ui.css", servePageStyles);
  registerPageHandler("/sws-ui.js", servePageControls);
}

// 帮助和错误页面也复用同一缓存，不缓存设备数据、表单或登录状态
void pageAppearance() {
  // 编译期标签留在 Flash；日期中的空格由浏览器进行 URL 编码
  static const char appearance[] PROGMEM =
    "<link rel='stylesheet' href='/sws-ui.css?v=" __DATE__ "-" __TIME__ "-" STR(DISPLAY_LANGUAGE)
    "'><script src='/sws-ui.js?v=" __DATE__ "-" __TIME__ "-" STR(DISPLAY_LANGUAGE) "'></script>";
  sendPageResource(appearance, sizeof(appearance) - 1);
}
