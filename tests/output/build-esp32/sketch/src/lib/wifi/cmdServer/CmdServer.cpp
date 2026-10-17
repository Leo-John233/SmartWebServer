#line 1 "E:\\GitHub\\SmartWebServer\\legacy\\SmartWebServer\\src\\lib\\wifi\\cmdServer\\CmdServer.cpp"
// wifi IP command server
#include "CmdServer.h"

#if OPERATIONAL_MODE == WIFI && COMMAND_SERVER != OFF

  #include "../../tasks/OnTask.h"
  #include "../../../libApp/cmd/Cmd.h"
  #include "../../../libApp/boardIo/Activity.h"
  #include "../NetworkBudget.h"

  CmdServer::CmdServer(uint32_t port, long clientTimeoutMs, bool persist) {
    cmdSvr = nullptr;
    cmdBuffer[0] = 0;
    cmdBufferPos = 0;
    clientEndTimeMs = 0;
    this->clientTimeoutMs = clientTimeoutMs;
    this->persist = persist;
    this->port = port;
  }

  void CmdServer::begin() {
    cmdSvr = new WiFiServer(port);
    cmdSvr->begin();
    cmdSvr->setNoDelay(true);
  }

  void CmdServer::handleClient() {
    const unsigned long now = millis();

    // drop stale client socket (disconnected or timed out/hung)
    bool timedOut = (long)(clientEndTimeMs - now) < 0;
    #if NETWORK_LOAD_GUARD == ON
      // 分轮处理不能把标准端口已收到的批量命令按空闲超时丢弃。
      if (cmdSvrClient.available() > 0) timedOut = false;
    #endif
    if (cmdSvrClient &&
       (!cmdSvrClient.connected() || timedOut)) {
      cmdSvrClient.stop();
      cmdBuffer[0] = 0;
      cmdBufferPos = 0;
    }

    // new client
    if (!cmdSvrClient && cmdSvr->hasClient()) {
      cmdSvrClient = cmdSvr->available();
      if (cmdSvrClient) {
        #if NETWORK_LOAD_GUARD == ON
          if (ESP.getFreeHeap() < swsNetwork::minimumHeap) { cmdSvrClient.stop(); return; }
          cmdSvrClient.setTimeout(swsNetwork::writeWaitMs);
        #endif
        clientEndTimeMs = now + clientTimeoutMs;
        cmdBuffer[0] = 0;
        cmdBufferPos = 0;
      }
    }

    // check clients for data, if found get the command, pass to OnStep and pickup the response, then return the response to client
    #if NETWORK_LOAD_GUARD == ON
      unsigned bytes = 0, commands = 0;
    #endif
    while (cmdSvrClient && cmdSvrClient.connected() && cmdSvrClient.available() > 0) {
      #if NETWORK_LOAD_GUARD == ON
        // 保留半帧和未读数据至下一轮，连续指令不能独占调度器。
        if (bytes >= swsNetwork::commandBytesPerTurn || commands >= swsNetwork::commandsPerTurn) break;
      #endif
      // still active? push back disconnect
      if (persist) clientEndTimeMs = millis() + clientTimeoutMs;

      // get the data
      int c = cmdSvrClient.read();
      if (c < 0 || c > 255) break;
      #if NETWORK_LOAD_GUARD == ON
        bytes++;
      #endif
      swsNetworkActivity();
      
      // insert into the command buffer
      cmdBuffer[cmdBufferPos] = (char)c;
      cmdBufferPos++;
      if (cmdBufferPos > 39) cmdBufferPos = 39;
      cmdBuffer[cmdBufferPos] = 0;

      // send cmd and pickup the response
      if (c == '#' || (strlen(cmdBuffer) == 1 && c == (char)6)) {
        char result[80] = "";

        onStep.processCommand(cmdBuffer, result, cmdTimeout); 

        // pickup response
        if (strlen(result) > 0) {
          if (cmdSvrClient && cmdSvrClient.connected()) {
            #if NETWORK_LOAD_GUARD == ON
              const size_t length = strlen(result);
              if (cmdSvrClient.write((const uint8_t*)result, length) != length) cmdSvrClient.stop();
            #else
              cmdSvrClient.print(result);
            #endif
            swsNetworkActivity();
          }
        }

        // reset command buffer
        cmdBuffer[0] = 0;
        cmdBufferPos = 0;
        #if NETWORK_LOAD_GUARD == ON
          commands++;
        #endif
      } else tasks.yield();
    }
  }

#endif
