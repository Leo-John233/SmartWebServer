#include "../Page.h"
#include "../Appearance.h"
#include "../Pages.common.h"
#include "../Request.h"
#include "../../libApp/boardIo/Activity.h"

// 固定大小分段读取 Flash；也可只发送 style/script 标签内的内容。
void sendPageResource(const char *content, size_t length) {
  char part[1025];
  for (size_t offset = 0; offset < length; offset += sizeof(part) - 1) {
    const size_t count = min(sizeof(part) - 1, length - offset);
    memcpy_P(part, content + offset, count);
    part[count] = 0;
    www.sendContent(part);
    swsNetworkActivity();
  }
}

void sendPageResource(const char *content) {
  sendPageResource(content, strlen_P(content));
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
  www.sendContent("<p id='sws-load' role='status'></p><iframe id='sws-frame' title=\"" L_UI_CONTENT "\"></iframe>");
  sendPageResource(html_shell_script);
  www.sendContent("");
  return true;
}

// 只缓存静态外观资源。版本随本文件编译更新，刷入新固件后自动换地址。
static void servePageStyles() {
  const size_t skip = sizeof("\n<style>") - 1, tail = sizeof("</style>\n") - 1;
  const size_t base = strlen_P(html_shared_styles) - skip - tail;
  const size_t editors = strlen_P(html_editor_styles) - skip - tail;
  const size_t io = strlen_P(html_board_io_styles) - skip - tail;
  www.setContentLength(base + editors + io);
  www.sendHeader("Cache-Control", "public, max-age=31536000, immutable");
  www.send(200, "text/css; charset=utf-8", String());
  sendPageResource(html_shared_styles + skip, base);
  sendPageResource(html_editor_styles + skip, editors);
  sendPageResource(html_board_io_styles + skip, io);
}

static void servePageControls() {
  const size_t skip = sizeof("\n<script>") - 1, tail = sizeof("</script>\n") - 1;
  const size_t controls = strlen_P(html_page_controls) - skip - tail;
  const size_t editors = strlen_P(html_settings_editors) - skip - tail;
  const size_t io = strlen_P(html_board_io_script) - skip - tail;
  www.setContentLength(controls + editors + io);
  www.sendHeader("Cache-Control", "public, max-age=31536000, immutable");
  www.send(200, "application/javascript; charset=utf-8", String());
  sendPageResource(html_page_controls + skip, controls);
  sendPageResource(html_settings_editors + skip, editors);
  sendPageResource(html_board_io_script + skip, io);
}

void registerPageResources() {
  registerPageHandler("/sws-ui.css", servePageStyles);
  registerPageHandler("/sws-ui.js", servePageControls);
}

// 帮助和错误页面也复用同一缓存，不缓存设备数据、表单或登录状态。
void pageAppearance() {
  String version = __DATE__ "-" __TIME__ "-" STR(DISPLAY_LANGUAGE);
  version.replace(" ", "");
  version.replace(":", "");
  String data = "<link rel='stylesheet' href='/sws-ui.css?v=";
  data.concat(version);
  data.concat("'><script src='/sws-ui.js?v=");
  data.concat(version);
  data.concat("'></script>");
  www.sendContentAndClear(data);
}
