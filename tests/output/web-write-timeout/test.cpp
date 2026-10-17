#include <cassert>
#include <cstddef>
using PGM_P=const char*;
#define NETWORK_LOAD_GUARD 0
#define ON 1


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

struct Client {bool closed=false,partial=false;int waits=0,timeout=0;void setTimeout(int ms){timeout=ms;}size_t write(const char*,size_t n){if(closed)return 0;++waits;return partial?n/2:n;}size_t write_P(const char*p,size_t n){return write(p,n);}void stop(){closed=true;}};
class ESP8266WebServer {public:ESP8266WebServer(int){};protected:Client _currentClient;virtual size_t _currentClientWrite(const char*,size_t)=0;virtual size_t _currentClientWrite_P(PGM_P,size_t)=0;};





// 兼容同步 WebServer 的独立传输适配层，不改变页面与设备命令的处理逻辑。
class SwsWebServer : public ESP8266WebServer {
  public:
    explicit SwsWebServer(int port) : ESP8266WebServer(port) {}

    #if NETWORK_LOAD_GUARD == ON
      void handleClient() {
        if (handling) return;
        handling = true;
        // 响应已完整进入 TCP 发送队列后主动发送 FIN，无需等浏览器关闭。
        // stop() 保留正常的 TCP 发送队列；不能用 abort() 丢弃尚未确认的数据。
        if (_currentStatus == HC_WAIT_CLOSE ||
            (_currentStatus == HC_WAIT_READ &&
             swsNetwork::elapsed(millis(), _statusChange, swsNetwork::readWaitMs))) releaseClient();

        if (_currentStatus == HC_NONE) {
          _currentClient = _server.available();
          if (!_currentClient) { handling = false; return; }
          _currentStatus = HC_WAIT_READ;
          _statusChange = millis();
          responseStarted = false;
          _currentClient.setNoDelay(true);
        }

        // 在解析请求及生成页面前退让，避免并发连接把剩余堆耗尽。
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
        // 旧版内核在解析请求行及头部前未设置读取超时。
        _currentClient.setTimeout(swsNetwork::readWaitMs);
        ESP8266WebServer::handleClient();
        if (_currentStatus == HC_WAIT_CLOSE) releaseClient();
        handling = false;
      }
    #endif

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
      #if NETWORK_LOAD_GUARD == ON
        if (!responseStarted) { responseStarted = true; responseStart = millis(); }
        if (swsNetwork::elapsed(millis(), responseStart, swsNetwork::responseLimitMs)) {
          _currentClient.stop();
          return false;
        }
      #endif
      _currentClient.setTimeout(swsNetwork::writeWaitMs);
      return true;
    }
    #if NETWORK_LOAD_GUARD == ON
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

class TestServer:public SwsWebServer {public:TestServer():SwsWebServer(80){}using SwsWebServer::_currentClientWrite;using SwsWebServer::_currentClientWrite_P;Client& client(){return _currentClient;}};
int main(){for(int flash=0;flash<2;++flash){TestServer server;auto& c=server.client();auto send=[&](){return flash?server._currentClientWrite_P("data",4):server._currentClientWrite("data",4);};assert(send()==4&&!c.closed&&c.timeout==1500);c.partial=true;assert(send()==2&&c.closed);int waits=c.waits;for(int i=0;i<100;++i)assert(send()==0);assert(c.waits==waits);}}
