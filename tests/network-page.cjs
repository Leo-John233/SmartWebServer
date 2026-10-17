const testOutput = require('./test-output.cjs');
// 执行真实网络页及分段发送代码，模拟空 HTTP 分块会结束响应的行为。
const fs = require('node:fs');
const path = require('node:path');
const cp = require('node:child_process');
const assert = require('node:assert/strict');

module.exports = ({ ota = true, reset = false, loggedIn = true, fragment = false } = {}) => {
  const root = path.join(__dirname, '../src/pages');
  const read = (file) => fs.readFileSync(path.join(root, file), 'utf8');
  const clean = (source) => source.replace(/^\s*#include.*$/gm, '').replace(/^#pragma once.*$/gm, '');
  let code = `
#include <string>
#include <cstring>
#include <cstdio>
#include <algorithm>
#include <iostream>
#define PROGMEM
#define F(x) x
#define FPSTR(x) x
#define snprintf_P snprintf
#define sstrcpy strcpy
#define strlen_P strlen
#define memcpy_P memcpy
#define ON 1
#define OFF 0
#define WIFI 2
#define OPERATIONAL_MODE WIFI
#define DISPLAY_SPECIAL_CHARS ON
#define DISPLAY_RESET_CONTROLS ${reset ? 'ON' : 'OFF'}
${ota ? '#define OTA_PRESENT' : ''}
#define CONTENT_LENGTH_UNKNOWN -1
#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)
using std::min;
struct String:std::string {using std::string::string;void concat(const char*s){append(s);}};
struct Web {
  std::string output;bool ended=false;
  void setContentLength(int){}
  void sendHeader(const char*,const char*){}
  void send(int,const char*,const String&){}
  void sendContent(const char*s){if(ended)return;if(!*s)ended=true;else output+=s;}
  void sendContent(const String&s){sendContent(s.c_str());}
}www;
// 和 Wi-Fi WebServer.h 相同；空字符串在分块响应中表示结束。
#define sendContentAndClear(x) sendContent(x); x = "";
struct Serial {void setTimeout(int){}} serial;
#define SERIAL_ONSTEP serial
struct Device {void serialRecvFlush(){}} onStep;
struct Status {bool onStepFound=true;} status;
struct Kv {template<class T>void get(const char*,T&){}};
struct Nv {Kv store;Kv&kv(){return store;}} storage;
Nv&nv(){return storage;}
int webTimeout=200,cmdTimeout=200;
bool servePageShell(int){return false;}
void pageHeader(int){}
void processNetworkGet(){}
void renderWifiNetwork(String&);
void sendPageResource(const char*);
void swsNetworkActivity(){}
`;
  code += require('./locale-source.cjs')() + '\n' + require('./page-support.cjs')() + '\n';
  for (const file of ['htmlHeaders.h', 'htmlMessages.h', 'Page.h', 'network/Network.h', 'network/ui/Styles.h', 'network/ui/View.h', 'network/ui/Script.h'])
    code += clean(read(file)) + '\n';
  const presentation = clean(read('ui/Presentation.cpp'));
  code += presentation.slice(presentation.indexOf('void sendPageResource('), presentation.indexOf('// 外壳'));
  code += clean(read('network/ui/View.cpp')) + '\n';
  const network = clean(read('network/Network.cpp'));
  code += network.slice(0, network.indexOf('\nvoid processNetworkGet() {'));
  code += `
int main(){loginRequired=${!loggedIn};handleNetwork();
  if(!www.ended){std::cerr<<"missing response terminator";return 1;}
  std::cout<<www.output;
}`;
  const dir = fs.mkdtempSync(path.join(testOutput, 'sws-network-'));
  const input = path.join(dir, 'page.cpp'), exe = path.join(dir, 'page.exe');
  fs.writeFileSync(input, code);
  const build = cp.spawnSync(process.env.CXX || 'g++', ['-std=c++11', input, '-o', exe], { encoding: 'utf8' });
  if (build.status !== 0) throw Error(build.stderr);
  const run = cp.spawnSync(exe, [], { encoding: 'utf8', maxBuffer: 2 * 1024 * 1024 });
  if (run.status !== 0) throw Error(run.stderr);
  if (!fragment) return run.stdout;
  const begin = "<div class='b'>", end = '</div></body></html>';
  const start = run.stdout.indexOf(begin), finish = run.stdout.lastIndexOf(end);
  assert(start >= 0 && finish > start, '真实网络页面必须完整发送，才能提取预览内容');
  return run.stdout.slice(start + begin.length, finish);
};

if (require.main === module) {
  for (const options of [{}, { ota: false }, { reset: true }]) {
    const html = module.exports(options);
    assert(html.includes('id="nw-sta"'), '保留 Wi-Fi 设置');
    assert(html.includes("name='ccto'") && html.includes("name='wcto'"), 'Wi-Fi 输出后仍发送通信超时设置');
    assert(html.includes("name='webpwd'") && html.includes("name='logout'"), '保留账号和注销表单');
    assert.equal(html.includes("name='otapwd'"), options.ota !== false, 'OTA 设置按编译功能显示');
    if (options.reset) assert(html.includes("name='otareboot'"));
    assert(html.trim().endsWith('</html>'), '完整发送页尾，未被空分块截断');
  }
  const login = module.exports({ loggedIn: false });
  assert(login.includes("name='login'"));
  assert(!login.includes("name='ccto'") && !login.includes('id="nw-sta"'), '登录前不输出受保护设置');
  console.log('PASS: actual network handler, chunk termination, OTA capability, reset mode and login gate');
}
