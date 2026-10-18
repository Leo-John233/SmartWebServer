#include "../../pages/Page.h"
#include "Appearance.h"
#include "../../pages/Pages.common.h"
#include "../web/Request.h"
#include "../boardIo/Activity.h"
#include "../../lib/tasks/OnTask.h"

// Wi-Fi 交由内核分段发送，避免每个小块重新开始等待超时
void sendPageResource(const char *content, size_t length) {
  if (!length) return;
  #if OPERATIONAL_MODE == WIFI
    www.sendContent_P(content, length);
  #else
    for (size_t offset = 0; offset < length; offset += 1024) {
      const size_t count = min(size_t(1024), length - offset);
      char part[1025];
      memcpy_P(part, content + offset, count);
      part[count] = 0;
      www.sendContent(part);
    }
  #endif
  swsNetworkActivity();
  Y;
}

// 只缓存静态外观资源，版本随本文件编译更新，刷入新固件后自动换地址
static void servePageStyles() {
  const size_t skip = sizeof("\n<style>") - 1, tail = sizeof("</style>\n") - 1;
  const size_t base = sizeof(html_shared_styles) - 1 - skip - tail;
  const size_t editors = sizeof(html_editor_styles) - 1 - skip - tail;
  const size_t io = sizeof(html_board_io_styles) - 1 - skip - tail;
  const size_t network = sizeof(html_network_css) - 1 - skip - tail;
  const size_t native = sizeof(html_main_css_core) + sizeof(html_main_css_control) +
    sizeof(html_main_css_buttons) + sizeof(html_main_css_collapse) - 4;
  www.setContentLength(native + network + base + editors + io);
  www.sendHeader("Cache-Control", "public, max-age=31536000, immutable");
  www.send(200, "text/css; charset=utf-8", String());
  sendPageResource(html_main_css_core, sizeof(html_main_css_core) - 1);
  sendPageResource(html_main_css_control, sizeof(html_main_css_control) - 1);
  sendPageResource(html_main_css_buttons, sizeof(html_main_css_buttons) - 1);
  sendPageResource(html_main_css_collapse, sizeof(html_main_css_collapse) - 1);
  sendPageResource(html_network_css + skip, network);
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
    "'><script defer src='/sws-ui.js?v=" __DATE__ "-" __TIME__ "-" STR(DISPLAY_LANGUAGE) "'></script>";
  sendPageResource(appearance, sizeof(appearance) - 1);
}

// 按原版顺序先完成 head，样式就绪后再绘制正文，脚本不阻塞 HTML 解析
void pageHead(bool preventSelection) {
  sendPageResource(html_head_begin, sizeof(html_head_begin) - 1);
  if (preventSelection) {
    sendPageResource(html_main_css_begin, sizeof(html_main_css_begin) - 1);
    sendPageResource(html_main_css_no_select, sizeof(html_main_css_no_select) - 1);
    sendPageResource(html_main_css_end, sizeof(html_main_css_end) - 1);
  }
  pageAppearance();
  sendPageResource(html_head_end, sizeof(html_head_end) - 1);
}
