#pragma once
#include <cstring>
#include <stdint.h>

// 命令通道只依赖此接口，不依赖保护策略或 GPIO
class SwsCommandGate {
public:
  typedef bool (*Permit)();
  static Permit &permit() { static Permit callback = nullptr; return callback; }
  static uint32_t &epoch() { static uint32_t value = 0; return value; }
  static void setLocked(bool locked) {
    static bool previous = true;
    if (locked != previous) { previous = locked; epoch()++; }
  }
  static bool allows(const char *command) {
    if (permit() && permit()()) return true;
    if (!command) return false;
    if (command[0] == char(6) && command[1] == 0) return true;
    if (!strcmp(command, ":Q#") || !strcmp(command, ":Qe#") || !strcmp(command, ":Qw#") ||
        !strcmp(command, ":Qn#") || !strcmp(command, ":Qs#") || !strcmp(command, ":Td#")) return true;
    // 锁定时只放行诊断查询、停止和通信速率设置；未知扩展及校验帧拒绝转发
    const char *baudCommands[] = {":SB0#", ":SB1#", ":SB2#", ":SB3#", ":SB4#", ":SB5#", ":SBA#", ":SBB#"};
    for (const char *baud : baudCommands) if (!strcmp(command, baud)) return true;
    // 状态发现依赖校准和附件查询，锁定时也必须可读，避免被误判为串口失联
    // 使用完整命令匹配，不能放行同前缀的校准、选设备或运动设置
    const char *readOnlyCommands[] = {
      ":A?#", ":FA#", ":Fa#", ":fA#",
      ":F1a#", ":F2a#", ":F3a#", ":F4a#", ":F5a#", ":F6a#",
      ":Ft#", ":Fb#", ":Fd#", ":Fc#", ":FC#", ":FW#", ":FT#",
      ":rT#", ":rG#", ":rW#", ":rb#"
    };
    for (const char *query : readOnlyCommands) if (!strcmp(command, query)) return true;
    if (command[0] != ':' || command[1] != 'G') return false;
    const size_t length = std::strlen(command);
    if (length < 4 || command[length - 1] != '#') return false;
    for (size_t i = 2; i < length - 1; i++) if (command[i] == ':' || command[i] == ';' || command[i] == '#') return false;
    return true;
  }
};
