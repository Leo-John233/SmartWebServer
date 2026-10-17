#line 1 "E:\\GitHub\\SmartWebServer\\legacy\\SmartWebServer\\src\\pages\\ui\\Presentation.cpp"
#include "../Page.h"
#include "../Appearance.h"
#include "../Pages.common.h"

// 分段读取 Flash，避免把整份样式或脚本复制到内存。
void sendPageResource(const char *content) {
  char part[513];
  const size_t length = strlen_P(content);
  for (size_t offset = 0; offset < length; offset += sizeof(part) - 1) {
    const size_t count = min(sizeof(part) - 1, length - offset);
    memcpy_P(part, content + offset, count);
    part[count] = 0;
    www.sendContent(part);
  }
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

// 帮助和错误页面也使用统一样式与当前编译语言
void pageAppearance() {
  sendPageResource(html_shared_styles);
  sendPageResource(html_page_controls);
}
