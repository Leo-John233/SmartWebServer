#pragma once

#include <stddef.h>

// 公共界面入口；页面继续使用自己的数据读取与参数处理函数
void pageHead(bool preventSelection = false);
void sendPageResource(const char *content, size_t length);
