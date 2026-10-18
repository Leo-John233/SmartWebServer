#pragma once

#include <stddef.h>

// 公共界面入口；页面继续使用自己的数据读取与参数处理函数
void pageHead(bool preventSelection = false);
void pageScripts();
void sendPageResource(const char *content, size_t length);
// 只读页面生成检查原生客户端，取消旧页面时不再启动下一项查询
bool pageClientConnected();
