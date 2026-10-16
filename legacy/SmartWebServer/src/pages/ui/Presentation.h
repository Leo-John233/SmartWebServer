#pragma once

// 公共界面入口；页面继续使用自己的数据读取与参数处理函数。
void pageAppearance();
bool servePageShell(int selected);
void sendPageResource(const char *content);
