#pragma once

// 统一登记网页入口；仅报告业务活动，仍调用原来的页面及命令处理函数
void registerPageHandler(const char *path, void (*handler)());
