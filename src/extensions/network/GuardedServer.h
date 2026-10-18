#pragma once

#include <ESP8266WebServer.h>
#include "NetworkBudget.h"

// 兼容同步 WebServer 的独立传输适配层，不改变页面与设备命令的处理逻辑
// 防护关闭时全部沿用内核，包括发送等待和短写处理
class SwsWebServer : public ESP8266WebServer {
  public:
    explicit SwsWebServer(int port) : ESP8266WebServer(port) {}

    #if NETWORK_LOAD_GUARD == ON
      void handleClient() {
        if (handling) return;
        handling = true;
        // 响应已完整进入 TCP 发送队列后主动发送 FIN，无需等浏览器关闭
        // stop() 保留正常的 TCP 发送队列；不能用 abort() 丢弃尚未确认的数据
        if (_currentStatus == HC_WAIT_CLOSE ||
            (_currentStatus == HC_WAIT_READ &&
             swsNetwork::elapsed(millis(), _statusChange, swsNetwork::readWaitMs))) releaseClient();

        if (_currentStatus == HC_NONE) {
          _currentClient = _server.available();
          if (!_currentClient) { handling = false; return; }
          _currentStatus = HC_WAIT_READ;
          _statusChange = millis();
          responseStarted = false;
          // 网页有大量短分段，合并小包以减少快速切页时的 TCP 队列占用
          _currentClient.setNoDelay(false);
        }

        // 在解析请求及生成页面前退让，避免并发连接把剩余堆耗尽
        if (_currentStatus == HC_WAIT_READ && ESP.getFreeHeap() < swsNetwork::minimumHeap) {
          static const char busy[] PROGMEM =
            "HTTP/1.1 503 Service Unavailable\r\nConnection: close\r\nRetry-After: 2\r\n"
            "Content-Type: text/plain\r\nContent-Length: 26\r\n\r\nDevice busy, retry shortly";
          _currentClient.setTimeout(100);
          _currentClient.write_P(busy, sizeof(busy) - 1);
          releaseClient();
          handling = false;
          return;
        }
        // 旧版内核在解析请求行及头部前未设置读取超时
        _currentClient.setTimeout(swsNetwork::readWaitMs);
        ESP8266WebServer::handleClient();
        if (_currentStatus == HC_WAIT_CLOSE) releaseClient();
        handling = false;
      }
  protected:
    size_t _currentClientWrite(const char* data, size_t length) override {
      if (!prepareWrite()) return 0;
      const size_t sent = _currentClient.write(data, length);
      if (sent != length) _currentClient.stop();
      return sent;
    }
    size_t _currentClientWrite_P(PGM_P data, size_t length) override {
      if (!prepareWrite()) return 0;
      const size_t sent = _currentClient.write_P(data, length);
      if (sent != length) _currentClient.stop();
      return sent;
    }

  private:
    bool prepareWrite() {
      if (!_currentClient.connected()) return false;
      // 生成页面期间新连接也会占用堆，每次写入前继续为 Wi-Fi 保留内存
      if (ESP.getFreeHeap() < swsNetwork::minimumHeap) {
        _currentClient.stop();
        return false;
      }
      if (!responseStarted) { responseStarted = true; responseStart = millis(); }
      if (swsNetwork::elapsed(millis(), responseStart, swsNetwork::responseLimitMs)) {
        _currentClient.stop();
        return false;
      }
      _currentClient.setTimeout(swsNetwork::writeWaitMs);
      return true;
    }
    bool handling = false;
    bool responseStarted = false;
    uint32_t responseStart = 0;

    void releaseClient() {
      _currentClient.stop();
      _currentClient = WiFiClient();
      _currentStatus = HC_NONE;
      _currentUpload.reset();
    }
    #endif
};
