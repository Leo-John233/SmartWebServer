void request(const char *action){www.args["action"]=action;www.code=0;handleWifiNetwork();}
void reset(){pending=0;connecting=false;scanning=false;failed=false;scanAddedSta=false;networkAdopted=backgroundConnecting=wasConnected=stationStarted=authenticationRejected=false;stationReconnect=swsWifi::StationReconnect();previousReconnect=swsWifi::StationReconnect();stationCandidate=false;notice="";noticeTarget="sta";clockMs=0;wifiManager.settings=WifiSettings();wifiManager.station[0]=StationSettings();wifiManager.stationPassword[0]=StationPassword();wifiManager.setStation(1);wifiManager.writes=0;wifiManager.storedPassword.clear();wifiManager.networkBusy=wifiManager.webManaged=wifiManager.stationEnabledAtBoot=false;WiFi=WifiFake();www=WebFake();loginRequired=false;}
void station(){www.args={{"ssid","New Home"},{"password","new-pass-123"},{"enabled","1"},{"dhcp","1"}};}
void apRequest(){www.args={{"ssid","ONSTEPX"},{"password",""},{"enabled","1"},{"channel","7"},{"ip","192.168.0.1"},{"gw","192.168.0.1"},{"sn","255.255.255.0"}};}
void advance(unsigned long ms){clockMs+=ms;pollWifiNetwork();}
void startConnection(){advance(1001);advance(350);}
int main(){
reset();loginRequired=true;request("scan");assert(www.code==403&&WiFi.mode==2);loginRequired=false;www.verb=0;request("connect");assert(www.code==405);
reset();request("status");assert(www.code==200&&www.body.find("password")==std::string::npos&&www.body.find("\"messageTarget\":\"sta\"")!=std::string::npos);
reset();request("scan");assert(scanning&&WiFi.mode==3&&wifiManager.networkBusy&&WiFi.disconnects==0);WiFi.scan=2;request("scan");assert(www.code==200&&!scanning&&WiFi.mode==2&&!wifiManager.networkBusy);assert(www.body.find("\\\"")!=std::string::npos);
reset();WiFi.mode=3;WiFi.connection=3;request("scan");WiFi.scan=0;request("scan");assert(WiFi.mode==3&&WiFi.connection==3&&WiFi.disconnects==0);
reset();request("scan");clockMs=30000;pollWifiNetwork();assert(!scanning&&WiFi.mode==2&&!wifiManager.networkBusy);
reset();station();www.args["password"]="short";request("connect");assert(www.code==400&&pending==0&&wifiManager.writes==0);
reset();station();www.args["ssid"]=std::string(32,'a');request("connect");assert(www.code==400);
reset();station();www.args["password"]="";request("connect");assert(www.code==400);www.args["ssid"]="Home";request("connect");assert(www.code==202);startConnection();assert(std::string(wifiManager.staPwd->password)=="password"&&WiFi.begins==1);
reset();station();www.args["password"]="";www.args["open"]="1";request("connect");assert(www.code==202&&proposedPassword.password[0]==0);
reset();station();www.args["dhcp"]="0";www.args["ip"]="192.168.0.55";www.args["gw"]="192.168.0.1";www.args["sn"]="255.255.255.0";request("connect");assert(www.code==400);www.args["ip"]="192.168.1.55";www.args["gw"]="192.168.1.1";www.args["sn"]="255.0.255.0";request("connect");assert(www.code==400);
reset();station();request("connect");assert(www.code==202&&WiFi.begins==0);clockMs=500;pollWifiNetwork();assert(WiFi.begins==0);clockMs=1001;pollWifiNetwork();assert(connecting&&WiFi.begins==0&&apActive());advance(350);assert(WiFi.begins==1&&wifiManager.writes==0&&!WiFi.autoReconnect);
// 新参数只用于验证中的连接，成功前不能改变已保存的参数和密码
assert(std::string(connectionStation().ssid)=="New Home"&&std::string(connectionPassword().password)=="new-pass-123");assert(std::string(wifiManager.sta->ssid)=="Home"&&std::string(wifiManager.staPwd->password)=="password");
assert(WiFi.startedSsid=="New Home"&&WiFi.startedPassword=="new-pass-123");
WiFi.connection=3;pollWifiNetwork();assert(!connecting&&!failed&&!wifiManager.networkBusy&&wifiManager.writes==1);
reset();station();request("connect");startConnection();advance(15000);assert(connecting&&WiFi.begins==1);advance(350);assert(WiFi.begins==2);advance(14650);assert(!connecting&&failed&&apActive()&&WiFi.mode==WIFI_AP&&wifiManager.writes==0);clockMs=60000;pollWifiNetwork();assert(!connecting&&WiFi.begins==2);
// 临时失败状态不终止正确连接；持续失败至多重试一次，之后等待用户修改。
for(int status:{WL_CONNECT_FAILED,WL_NO_SSID_AVAIL}){
 reset();station();request("connect");startConnection();WiFi.connection=status;advance(100);assert(connecting&&!failed);advance(1000);assert(connecting);WiFi.connection=WL_CONNECTED;advance(100);assert(!connecting&&!failed&&WiFi.begins==1&&wifiManager.writes==1);
}
reset();station();request("connect");startConnection();WiFi.connection=WL_NO_SSID_AVAIL;advance(3500);advance(1500);advance(350);assert(WiFi.begins==2&&connecting);WiFi.connection=WL_CONNECTED;advance(100);assert(!failed&&!connecting&&wifiManager.writes==1);
for(int status:{WL_CONNECT_FAILED,WL_NO_SSID_AVAIL}){
 reset();station();request("connect");startConnection();WiFi.connection=status;advance(3500);advance(1500);assert(connecting&&!failed&&WiFi.begins==1);advance(350);assert(WiFi.begins==2);WiFi.connection=status;advance(3500);advance(1500);
 assert(failed&&!connecting&&!wifiManager.networkBusy&&!stationReconnect.known()&&WiFi.mode==WIFI_AP&&!WiFi.autoReconnect&&wifiManager.writes==0);
 assert(std::string(wifiManager.sta->ssid)=="Home"&&std::string(wifiManager.staPwd->password)=="password");
 clockMs=600000;pollWifiNetwork();assert(WiFi.begins==2&&!connecting);
 request("status");assert(www.code==200&&www.body.find("\"failed\":true")!=std::string::npos);
 // 失败后仍可扫描，并提交正确密码连接成功。
 request("scan");WiFi.scan=2;request("scan");assert(www.code==200&&WiFi.mode==WIFI_AP);
 station();www.args["password"]="correct-pass";request("connect");assert(www.code==202);startConnection();assert(connecting&&WiFi.begins==3);
 WiFi.connection=WL_CONNECTED;pollWifiNetwork();assert(!failed&&!connecting&&wifiManager.writes==1);
 // 已成功连接后意外掉线仍保留一次自动恢复机会。
 WiFi.connection=0;advance(1);assert(!connecting);advance(1999);assert(!connecting);advance(1);assert(connecting&&WiFi.begins==3);advance(350);assert(WiFi.begins==4);
 WiFi.connection=WL_CONNECTED;pollWifiNetwork();assert(!connecting&&wifiManager.writes==1);
}
reset();station();request("connect");startConnection();WiFi.local[2]=0;WiFi.connection=3;pollWifiNetwork();assert(failed&&WiFi.connection==0&&apActive());
// STA 密码使用实际的 63 字节上限，AP 的旧存储边界仍是 31 字节。
reset();station();www.args["password"]=std::string(63,'x');request("connect");assert(www.code==202);startConnection();assert(std::string(connectionPassword().password)==std::string(63,'x'));WiFi.connection=WL_CONNECTED;pollWifiNetwork();assert(wifiManager.writes==1);request("status");assert(www.body.find(std::string(63,'x'))==std::string::npos&&www.body.find("\"remembered\":true")!=std::string::npos);
reset();station();www.args["password"]=std::string(64,'x');request("connect");assert(www.code==400&&pending==0);apRequest();www.args["password"]=std::string(32,'x');request("ap");assert(www.code==400);
// 已连接网络的原样保存不掉线、不擦除密码、不写 Flash。
// 另一个页面在连接验证中保存管理设置，也不能提前持久化未经验证的新密码。
reset();station();request("connect");startConnection();wifiManager.writeSettings();assert(wifiManager.storedPassword=="password");request("status");assert(www.body.find("New Home")!=std::string::npos&&www.body.find("\"remembered\":false")!=std::string::npos);advance(30000);assert(failed&&wifiManager.storedPassword=="password"&&std::string(wifiManager.sta->ssid)=="Home");
reset();station();request("connect");startConnection();wifiManager.writeSettings();WiFi.connection=WL_CONNECTED;advance(1);assert(wifiManager.storedPassword=="new-pass-123"&&std::string(wifiManager.sta->ssid)=="New Home");
reset();WiFi.mode=3;WiFi.connection=WL_CONNECTED;wifiManager.settings.stationEnabled=true;station();www.args["ssid"]="Home";www.args["password"]="";request("connect");assert(www.code==200&&pending==0&&WiFi.disconnects==0&&wifiManager.writes==0);assert(std::string(wifiManager.staPwd->password)=="password");
// 还未拿到 IP 时不能报告成功，也不会把错误网络地址写入已保存配置。
reset();station();request("connect");startConnection();memset(WiFi.local,0,4);WiFi.connection=WL_CONNECTED;advance(1000);assert(connecting&&wifiManager.writes==0);advance(29000);assert(failed&&!connecting&&wifiManager.writes==0);
// 配置失败同样恢复热点和旧凭据。
reset();station();request("connect");WiFi.configOk=false;startConnection();assert(failed&&!connecting&&apActive()&&wifiManager.writes==0);
// 无线时序在 millis 回绕后仍保持相同的等待与总超时限制。
reset();station();request("connect");WiFi.operationDelayMs=3;advance(1001);assert(connecting&&!failed&&WiFi.begins==0);advance(350);assert(connecting&&WiFi.begins==1);WiFi.connection=WL_CONNECTED;advance(1);assert(!failed&&!connecting&&wifiManager.writes==1);
{swsWifi::StationConnection flow;uint32_t start=0xffffff00;flow.begin(start);assert(flow.poll(uint32_t(start+349),false,false,false)==swsWifi::StationConnection::Wait);assert(flow.poll(uint32_t(start+350),false,false,false)==swsWifi::StationConnection::Start);assert(flow.poll(uint32_t(start+30000),false,false,false)==swsWifi::StationConnection::Failed);}
// 启动时回退到 AP 不应丢失原来已开启的 STA 意图，恢复流程不依赖浏览器。
reset();wifiManager.stationEnabledAtBoot=true;pollWifiNetwork();assert(wifiManager.webManaged&&!WiFi.autoReconnect&&wifiManager.settings.stationEnabled);advance(1999);assert(!connecting);advance(1);assert(connecting&&backgroundConnecting&&apActive());advance(350);assert(WiFi.begins==1);WiFi.connection=WL_CONNECTED;advance(1);assert(!connecting&&!backgroundConnecting&&wifiManager.writes==0);
// 断线后的每次后台尝试有截止时间，等待间隔递增至 60 秒；恢复后不重复写 Flash。
WiFi.connection=0;advance(1);advance(2000);assert(backgroundConnecting);
for(unsigned long wait:{5000UL,10000UL,20000UL,40000UL,60000UL,60000UL}){
 advance(30000);assert(failed&&!connecting&&apActive());int begins=WiFi.begins;
 advance(wait-1);assert(!connecting&&WiFi.begins==begins);advance(1);assert(connecting&&backgroundConnecting);advance(350);assert(WiFi.begins==begins+1);
}
WiFi.connection=WL_CONNECTED;advance(1);assert(!failed&&!connecting&&wifiManager.writes==0);
// 已知网络后台连接时用户刷新、换网和关闭开关都优先，无需等 30 秒。
WiFi.connection=0;advance(1);advance(2000);advance(350);assert(backgroundConnecting);
request("scan");assert(www.code==200&&scanning&&!connecting&&!backgroundConnecting);WiFi.scan=0;request("scan");assert(!scanning);advance(5000);assert(backgroundConnecting);advance(350);
station();request("connect");assert(www.code==202&&!connecting&&pending==1);startConnection();assert(connecting&&!backgroundConnecting);WiFi.connection=WL_CONNECTED;advance(1);assert(wifiManager.writes==1);
WiFi.connection=0;advance(1);advance(2000);assert(backgroundConnecting);station();www.args["enabled"]="0";request("connect");assert(www.code==202&&!connecting);advance(1001);assert(!wifiManager.settings.stationEnabled);int disabledBegins=WiFi.begins;advance(600000);assert(!connecting&&WiFi.begins==disabledBegins);
// SDK 明确返回密码错误时停止自动尝试，通用连接失败仍可后台恢复。
reset();wifiManager.stationEnabledAtBoot=true;pollWifiNetwork();advance(2000);advance(350);WiFi.wrongPassword=true;WiFi.connection=WL_CONNECT_FAILED;advance(3000);advance(1500);assert(failed&&!connecting&&WiFi.begins==1);advance(600000);assert(!connecting&&WiFi.begins==1);assert(notice.find(L_UI_WIFI_AUTH_FAILED)!=std::string::npos);
WiFi.wrongPassword=false;station();www.args["ssid"]="Home";www.args["password"]="corrected-pass";request("connect");startConnection();WiFi.connection=WL_CONNECTED;advance(1);assert(!failed&&wifiManager.writes==1);WiFi.connection=0;advance(1);advance(2000);assert(backgroundConnecting);
reset();wifiManager.stationEnabledAtBoot=true;pollWifiNetwork();advance(2000);advance(350);WiFi.connection=WL_CONNECT_FAILED;advance(3000);advance(1500);advance(350);WiFi.connection=WL_CONNECT_FAILED;advance(3000);advance(1500);assert(failed&&!connecting&&WiFi.begins==2);advance(5000);assert(backgroundConnecting);
// 新网络密码错误不覆盖原网络，仍自动恢复原先可用的网络。
reset();wifiManager.settings.stationEnabled=true;WiFi.connection=WL_CONNECTED;pollWifiNetwork();station();request("connect");startConnection();WiFi.wrongPassword=true;WiFi.connection=WL_CONNECT_FAILED;advance(3000);advance(1500);assert(!connecting&&std::string(wifiManager.sta->ssid)=="Home"&&std::string(wifiManager.staPwd->password)=="password"&&wifiManager.writes==0);WiFi.wrongPassword=false;advance(5000);assert(backgroundConnecting);
// 地址冲突属于需修改设置的问题，不能在后台反复尝试同一配置。
reset();wifiManager.stationEnabledAtBoot=true;pollWifiNetwork();advance(2000);advance(350);WiFi.local[2]=0;WiFi.connection=WL_CONNECTED;advance(1);assert(failed&&!connecting);advance(600000);assert(!connecting&&WiFi.begins==1);
// 修改地址后遇到冲突时，应回到原先有效的地址设置，允许恢复旧连接。
reset();wifiManager.settings.stationEnabled=true;wifiManager.sta->dhcpEnabled=false;WiFi.connection=WL_CONNECTED;pollWifiNetwork();station();www.args["ssid"]="Home";www.args["password"]="";request("connect");startConnection();WiFi.local[2]=0;WiFi.connection=WL_CONNECTED;advance(1);assert(failed&&!wifiManager.sta->dhcpEnabled);advance(5000);assert(backgroundConnecting);
// 退避调度同样处理毫秒计数回绕，手动操作可短暂让出后台尝试。
{swsWifi::StationReconnect flow;uint32_t start=0xffffff00;flow.init(true,start);assert(!flow.due(uint32_t(start+1999)));assert(flow.due(uint32_t(start+2000)));flow.failed(start,false);assert(!flow.due(uint32_t(start+4999)));assert(flow.due(uint32_t(start+5000)));flow.failed(start,true);assert(!flow.due(uint32_t(start+600000)));flow.online(start);flow.defer(start);assert(!flow.due(uint32_t(start+4999)));assert(flow.due(uint32_t(start+5000)));}
reset();apRequest();www.args["enabled"]="0";request("ap");assert(www.code==400);WiFi.connection=3;wifiManager.settings.stationEnabled=true;request("ap");assert(www.code==202);clockMs=1001;pollWifiNetwork();assert(!apActive()&&wifiManager.writes==1&&std::string(noticeTarget)=="ap");request("status");assert(www.body.find("\"messageTarget\":\"ap\"")!=std::string::npos);
reset();wifiManager.settings.accessPointEnabled=false;station();www.args["enabled"]="0";request("connect");assert(www.code==400);
reset();apRequest();WiFi.connection=3;WiFi.local[2]=0;request("ap");assert(www.code==400);
// 提交关闭热点后突然掉线时，取消操作并保留原配置
reset();apRequest();www.args["ssid"]="Rejected AP";request("ap");WiFi.apFailures=1;advance(1001);assert(failed&&wifiManager.writes==0&&std::string(wifiManager.settings.ap.ssid)=="ONSTEPX"&&apActive()&&!pending);
reset();WiFi.connection=WL_CONNECTED;wifiManager.settings.stationEnabled=true;apRequest();www.args["enabled"]="0";request("ap");WiFi.apCloseOk=false;advance(1001);assert(failed&&wifiManager.writes==0&&wifiManager.settings.accessPointEnabled&&apActive());
reset();apRequest();www.args["enabled"]="0";WiFi.connection=3;wifiManager.settings.stationEnabled=true;request("ap");assert(www.code==202);WiFi.connection=0;clockMs=1001;pollWifiNetwork();assert(apActive()&&wifiManager.writes==0&&failed);
std::cout<<"PASS: authentication, password privacy and reuse, 63-byte STA passwords, non-disruptive save, staged connection, transient failure recovery, one retry/30s deadline, no-IP wait, scan/AP preservation, rollback, manual correction and dropped-link recovery.\n";
}
