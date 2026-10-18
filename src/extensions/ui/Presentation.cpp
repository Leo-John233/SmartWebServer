#include "../../pages/Page.h"
#include "Appearance.h"
#include "../../pages/Pages.common.h"
#include "../../lib/tasks/OnTask.h"

bool pageClientConnected() {
  #if OPERATIONAL_MODE == WIFI
    return www.client().connected();
  #else
    return true;
  #endif
}

// 保留原版发送方式，同一次输出共用 1 KB 缓冲
// 不增加请求，完整发送结束后才让出任务
class PageResourceWriter {
  public:
    PageResourceWriter() { data_.reserve(1024); }
    void append(const char *content, size_t length) {
      char part[257];
      for (size_t offset = 0; offset < length;) {
        if (!connected()) { data_ = ""; return; }
        const size_t count = min(size_t(256), min(length - offset, size_t(1024) - data_.length()));
        memcpy_P(part, content + offset, count);
        part[count] = 0;
        data_ += part;
        offset += count;
        if (data_.length() == 1024) finish();
      }
    }
    void finish() {
      if (!data_.length()) return; // 空分块会结束响应，不能发送空缓冲
      if (!connected()) { data_ = ""; return; }
      www.sendContentAndClear(data_);
      Y;
    }
  private:
    bool connected() const {
      return pageClientConnected();
    }
    String data_;
};

void sendPageResource(const char *content, size_t length) {
  PageResourceWriter writer;
  writer.append(content, length);
  writer.finish();
}

void pageAppearance(PageResourceWriter &writer) {
  // 首屏只发送完整样式和极简导航停止拦截
  const String page = www.uri();
  const bool mount = page == "/mount.htm";
  const bool controller = page == "/" || page == "/index.htm";
  if (page == "/net.htm") writer.append(html_network_css, sizeof(html_network_css) - 1);
  writer.append(html_shared_styles, sizeof(html_shared_styles) - 1);
  if (page == "/focuser.htm" || page == "/rotator.htm") writer.append(html_accessory_styles, sizeof(html_accessory_styles) - 1);
  if (page == "/enc.htm") writer.append(html_encoder_styles, sizeof(html_encoder_styles) - 1);
  if (page == "/auxiliary.htm") writer.append(html_auxiliary_styles, sizeof(html_auxiliary_styles) - 1);
  if (mount) writer.append(html_mount_detail_styles, sizeof(html_mount_detail_styles) - 1);
  if (mount) writer.append(html_editor_styles, sizeof(html_editor_styles) - 1);
  if (controller) writer.append(html_board_io_styles, sizeof(html_board_io_styles) - 1);
  writer.append(html_page_navigation, sizeof(html_page_navigation) - 1);
}

void pageScripts() {
  PageResourceWriter writer;
  const String page = www.uri();
  writer.append(html_page_controls, sizeof(html_page_controls) - 1);
  if (page == "/mount.htm") writer.append(html_settings_editors, sizeof(html_settings_editors) - 1);
  if (page == "/" || page == "/index.htm") writer.append(html_board_io_script, sizeof(html_board_io_script) - 1);
  writer.finish();
}

// 与原版一样在同一 HTML 响应中完成 head、样式和脚本，再输出页面正文
void pageHead(bool preventSelection) {
  PageResourceWriter writer;
  writer.append(html_head_begin, sizeof(html_head_begin) - 1);
  writer.append(html_main_css_begin, sizeof(html_main_css_begin) - 1);
  writer.append(html_main_css_core, sizeof(html_main_css_core) - 1);
  writer.append(html_main_css_control, sizeof(html_main_css_control) - 1);
  writer.append(html_main_css_buttons, sizeof(html_main_css_buttons) - 1);
  writer.append(html_main_css_collapse, sizeof(html_main_css_collapse) - 1);
  if (preventSelection) writer.append(html_main_css_no_select, sizeof(html_main_css_no_select) - 1);
  writer.append(html_main_css_end, sizeof(html_main_css_end) - 1);
  pageAppearance(writer);
  writer.append(html_head_end, sizeof(html_head_end) - 1);
  writer.finish();
}
