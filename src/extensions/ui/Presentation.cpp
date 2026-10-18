#include "../../pages/Page.h"
#include "Appearance.h"
#include "../../pages/Pages.common.h"
#include "../../lib/tasks/OnTask.h"

// 沿用原版 String 与 sendContentAndClear 输出，不另建静态资源请求或发送接管层
// 新界面模板较大，分段复制避免一次占满 ESP8266 堆内存
void sendPageResource(const char *content, size_t length) {
  char part[1025];
  String data;
  for (size_t offset = 0; offset < length; offset += 1024) {
    #if OPERATIONAL_MODE == WIFI
      // 快速切页会取消旧请求；停止为已经断开的客户端复制和发送模板。
      if (!www.client().connected()) return;
    #endif
    const size_t count = min(size_t(1024), length - offset);
    memcpy_P(part, content + offset, count);
    part[count] = 0;
    data = part;
    www.sendContentAndClear(data);
    // 完整发送调用结束后再调度，保持保护采样与串口事务的边界
    Y;
  }
}

void pageAppearance() {
  // 与原页面自己的脚本一样，只内联当前页面需要的增强内容。
  const String page = www.uri();
  const bool mount = page == "/mount.htm";
  const bool controller = page == "/" || page == "/index.htm";
  if (page == "/net.htm") sendPageResource(html_network_css, sizeof(html_network_css) - 1);
  sendPageResource(html_shared_styles, sizeof(html_shared_styles) - 1);
  if (mount) sendPageResource(html_editor_styles, sizeof(html_editor_styles) - 1);
  if (controller) sendPageResource(html_board_io_styles, sizeof(html_board_io_styles) - 1);
  sendPageResource(html_page_controls, sizeof(html_page_controls) - 1);
  if (mount) sendPageResource(html_settings_editors, sizeof(html_settings_editors) - 1);
  if (controller) sendPageResource(html_board_io_script, sizeof(html_board_io_script) - 1);
  sendPageResource(html_page_navigation, sizeof(html_page_navigation) - 1);
}

// 与原版一样在同一 HTML 响应中完成 head、样式和脚本，再输出页面正文
void pageHead(bool preventSelection) {
  sendPageResource(html_head_begin, sizeof(html_head_begin) - 1);
  sendPageResource(html_main_css_begin, sizeof(html_main_css_begin) - 1);
  sendPageResource(html_main_css_core, sizeof(html_main_css_core) - 1);
  sendPageResource(html_main_css_control, sizeof(html_main_css_control) - 1);
  sendPageResource(html_main_css_buttons, sizeof(html_main_css_buttons) - 1);
  sendPageResource(html_main_css_collapse, sizeof(html_main_css_collapse) - 1);
  if (preventSelection) sendPageResource(html_main_css_no_select, sizeof(html_main_css_no_select) - 1);
  sendPageResource(html_main_css_end, sizeof(html_main_css_end) - 1);
  pageAppearance();
  sendPageResource(html_head_end, sizeof(html_head_end) - 1);
}
