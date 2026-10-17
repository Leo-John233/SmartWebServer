void request(const char *action){www.args["action"]=action;www.code=0;handleWifiNetwork();}
void reset(){pending=0;connecting=false;scanning=false;failed=false;scanAddedSta=false;stationRetryBlocked=false;stationCandidate=false;notice="";noticeTarget="sta";lastRetry=clockMs=0;wifiManager.settings=WifiSettings();wifiManager.station[0]=StationSettings();wifiManager.stationPassword[0]=StationPassword();wifiManager.setStation(1);wifiManager.writes=0;wifiManager.networkBusy=wifiManager.webManaged=false;WiFi=WifiFake();www=WebFake();loginRequired=false;}
void station(){www.args={{"ssid","New Home"},{"password","new-pass-123"},{"enabled","1"},{"dhcp","1"}};}
void apRequest(){www.args={{"ssid","ONSTEPX"},{"password",""},{"enabled","1"},{"channel","7"},{"ip","192.168.0.1"},{"gw","192.168.0.1"},{"sn","255.255.255.0"}};}
int main(){
reset();loginRequired=true;request("scan");assert(www.code==403&&WiFi.mode==2);loginRequired=false;www.verb=0;request("connect");assert(www.code==405);
reset();request("status");assert(www.code==200&&www.body.find("password")==std::string::npos&&www.body.find("\"messageTarget\":\"sta\"")!=std::string::npos);
reset();request("scan");assert(scanning&&WiFi.mode==3&&wifiManager.networkBusy&&WiFi.disconnects==0);WiFi.scan=2;request("scan");assert(www.code==200&&!scanning&&WiFi.mode==2&&!wifiManager.networkBusy);assert(www.body.find("\\\"")!=std::string::npos);
reset();WiFi.mode=3;WiFi.connection=3;request("scan");WiFi.scan=0;request("scan");assert(WiFi.mode==3&&WiFi.connection==3&&WiFi.disconnects==0);
reset();request("scan");clockMs=30000;pollWifiNetwork();assert(!scanning&&WiFi.mode==2&&!wifiManager.networkBusy);
reset();station();www.args["password"]="short";request("connect");assert(www.code==400&&pending==0&&wifiManager.writes==0);
reset();station();www.args["ssid"]=std::string(32,'a');request("connect");assert(www.code==400);
reset();station();www.args["password"]="";request("connect");assert(www.code==400);www.args["ssid"]="Home";request("connect");assert(www.code==202);
reset();station();www.args["password"]="";www.args["open"]="1";request("connect");assert(www.code==202&&proposedPassword.password[0]==0);
reset();station();www.args["dhcp"]="0";www.args["ip"]="192.168.0.55";www.args["gw"]="192.168.0.1";www.args["sn"]="255.255.255.0";request("connect");assert(www.code==400);www.args["ip"]="192.168.1.55";www.args["gw"]="192.168.1.1";www.args["sn"]="255.0.255.0";request("connect");assert(www.code==400);
reset();station();request("connect");assert(www.code==202&&WiFi.begins==0);clockMs=500;pollWifiNetwork();assert(WiFi.begins==0);clockMs=1001;pollWifiNetwork();assert(connecting&&WiFi.begins==1&&apActive()&&wifiManager.writes==0&&!WiFi.autoReconnect);
// 确认新版独立保存的路由器参数和密码都在延迟应用后更新
assert(std::string(wifiManager.sta->ssid)=="New Home"&&std::string(wifiManager.staPwd->password)=="new-pass-123");
WiFi.connection=3;pollWifiNetwork();assert(!connecting&&!failed&&!wifiManager.networkBusy&&wifiManager.writes==1);
reset();station();request("connect");clockMs=1001;pollWifiNetwork();clockMs=27002;pollWifiNetwork();assert(!connecting&&failed&&apActive()&&WiFi.mode==WIFI_AP&&wifiManager.writes==0);clockMs=58003;pollWifiNetwork();assert(!connecting&&WiFi.begins==1);
// 错误密码或找不到网络立即结束尝试，长时间等待也不会自动重试错误凭据。
for(int status:{WL_CONNECT_FAILED,WL_NO_SSID_AVAIL}){
 reset();station();request("connect");clockMs=1001;pollWifiNetwork();WiFi.connection=status;clockMs=2000;pollWifiNetwork();
 assert(failed&&!connecting&&!wifiManager.networkBusy&&stationRetryBlocked&&WiFi.mode==WIFI_AP&&!WiFi.autoReconnect&&wifiManager.writes==0);
 assert(std::string(wifiManager.sta->ssid)=="Home"&&std::string(wifiManager.staPwd->password)=="password");
 clockMs=600000;pollWifiNetwork();assert(WiFi.begins==1&&!connecting);
 request("status");assert(www.code==200&&www.body.find("\"failed\":true")!=std::string::npos);
 // 失败后仍可扫描，并提交正确密码连接成功。
 request("scan");WiFi.scan=2;request("scan");assert(www.code==200&&WiFi.mode==WIFI_AP);
 station();www.args["password"]="correct-pass";request("connect");assert(www.code==202);clockMs+=1001;pollWifiNetwork();assert(connecting&&WiFi.begins==2&&!stationRetryBlocked);
 WiFi.connection=WL_CONNECTED;pollWifiNetwork();assert(!failed&&!connecting&&wifiManager.writes==1);
 // 已成功连接后意外掉线仍保留一次自动恢复机会。
 WiFi.connection=0;clockMs+=31000;pollWifiNetwork();assert(connecting&&WiFi.begins==3);
 WiFi.connection=WL_CONNECTED;pollWifiNetwork();assert(!connecting&&wifiManager.writes==1);
}
reset();station();request("connect");clockMs=1001;pollWifiNetwork();WiFi.local[2]=0;WiFi.connection=3;pollWifiNetwork();assert(failed&&WiFi.connection==0&&apActive());
reset();apRequest();www.args["enabled"]="0";request("ap");assert(www.code==400);WiFi.connection=3;wifiManager.settings.stationEnabled=true;request("ap");assert(www.code==202);clockMs=1001;pollWifiNetwork();assert(!apActive()&&wifiManager.writes==1&&std::string(noticeTarget)=="ap");request("status");assert(www.body.find("\"messageTarget\":\"ap\"")!=std::string::npos);
reset();wifiManager.settings.accessPointEnabled=false;station();www.args["enabled"]="0";request("connect");assert(www.code==400);
reset();apRequest();WiFi.connection=3;WiFi.local[2]=0;request("ap");assert(www.code==400);
// 提交关闭热点后突然掉线时，取消操作并保留原配置
reset();apRequest();www.args["enabled"]="0";WiFi.connection=3;wifiManager.settings.stationEnabled=true;request("ap");assert(www.code==202);WiFi.connection=0;clockMs=1001;pollWifiNetwork();assert(apActive()&&wifiManager.writes==0&&failed);
std::cout<<"PASS: authentication, method checks, password privacy, scan lifecycle/AP preservation, validation, delayed connect, success, failure blocks retry, manual correction, dropped-link recovery, DHCP overlap, lockout prevention.\n";
}
