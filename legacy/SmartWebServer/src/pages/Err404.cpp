// 错误页面仅显示可操作的提示，不回显请求中的账号或设备参数
#include "Page.h"
#include "Pages.common.h"

void handleNotFound() {
  www.setContentLength(CONTENT_LENGTH_UNKNOWN);
  www.sendHeader("Cache-Control", "no-store");
  www.send(404, "text/html", String());
  String data = FPSTR(html_head_begin);
  data.concat(FPSTR(html_head_end));
  data.concat(FPSTR(html_body_begin));
  www.sendContentAndClear(data);
  pageAppearance();
  www.sendContent("<main class='sws-document sws-empty'><span class='sws-brand-small'>404</span><h1>找不到这个页面</h1><p>页面地址可能有误，或设备未启用这项功能</p><a href='/index.htm' target='_top'>返回控制器</a></main></body></html>");
  www.sendContent("");
}
