#pragma once

#include <stdint.h>

// 与网页、串口命令及 AP/STA 配置分离，仅约束网络传输占用的资源。
namespace swsNetwork {
  static const uint32_t minimumHeap = 10000;
  static const uint32_t readWaitMs = 1000;
  static const uint32_t writeWaitMs = 1500;
  static const uint32_t responseLimitMs = 15000;
  static const unsigned commandBytesPerTurn = 64;
  static const unsigned commandsPerTurn = 1;

  inline bool elapsed(uint32_t now, uint32_t start, uint32_t limit) {
    return uint32_t(now - start) >= limit;
  }
}
