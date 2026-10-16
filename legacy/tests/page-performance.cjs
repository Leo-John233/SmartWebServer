const output = require('./test-output.cjs');
const fs = require('node:fs');
const path = require('node:path');
const cp = require('node:child_process');
const assert = require('node:assert/strict');
const http = require('node:http');
const { chromium } = require('../../webui/node_modules/playwright');
const root = path.join(__dirname, '../SmartWebServer');
const templates = require('./page-templates.cjs')(root);
const read = (file) => fs.readFileSync(path.join(root, 'src/pages', file), 'utf8');
const clean = (source) => source.replace(/^\s*#include.*$/gm, '');
const work = fs.mkdtempSync(path.join(output, 'page-performance-'));

// 编译真实发送代码，检查长度、缓存头以及导航是否还会读取串口。
function nativeResources() {
  const values = {
    ...templates['htmlHeaders.h'],
    ...templates['ui/Styles.h'],
    ...templates['ui/Controls.h'],
    ...templates['ui/Editors.h'],
    ...templates['ui/Navigation.h'],
  };
  let code = `
#include <string>
#include <cstring>
#include <algorithm>
#include <fstream>
#include <cassert>
using std::min;
#define strlen_P strlen
#define memcpy_P memcpy
#define FPSTR(x) x
#define STR_INNER(x) #x
#define STR(x) STR_INNER(x)
#define DISPLAY_LANGUAGE 1
#define HTTP_GET 1
#define CONTENT_LENGTH_UNKNOWN -1
#define L_UI_CONTENT "content"
#define SD_TRUE 1
#define PAGE_CONTROLLER 0
#define PAGE_MOUNT 1
#define PAGE_ROTATOR 2
#define PAGE_FOCUSER 3
#define PAGE_AUXILIARY 4
#define PAGE_ENCODERS 5
#define PAGE_NETWORK 6
struct String:std::string {
  using std::string::string;
  void concat(const char*s){append(s);}
  void concat(const String&s){append(s);}
  void replace(const char*from,const char*to){size_t p=0;while((p=find(from,p))!=npos){std::string::replace(p,strlen(from),to);p+=strlen(to);}}
};
struct Web {
  int methodValue=1,argsValue=0; long length=0; size_t largest=0;
  std::string body,cache,mime;
  int method(){return methodValue;} int args(){return argsValue;}
  void setContentLength(long n){length=n;}
  void sendHeader(const char*k,const char*v){if(!strcmp(k,"Cache-Control"))cache=v;}
  void send(int,const char*m,const String&){body.clear();mime=m;largest=0;}
  void sendContent(const char*s){body+=s;largest=std::max(largest,strlen(s));}
  void sendContent(const String&s){sendContent(s.c_str());}
  void on(const char*,void(*)()){}
}www;
#define sendContentAndClear(x) sendContent(x); x="";
struct Status {bool onStepFound=true;int focuserFound=1,rotatorFound=1,auxiliaryFound=1;}status;
void pageHeader(int){}
`;
  for (const [key, value] of Object.entries(values))
    code += 'const char ' + key + '[]=' + JSON.stringify(value) + ';\n';
  code += clean(read('ui/Presentation.cpp'));
  code += `
int stateReads=0,commands=0,flushes=0,notFound=0,webTimeout=200;
struct State {
  void updateController(bool){stateReads++;} void updateMount(bool){stateReads++;}
  void updateRotator(bool){stateReads++;} void updateFocuser(bool){stateReads++;}
  void updateAuxiliary(bool,bool){stateReads++;}
}state;
struct Serial {void setTimeout(int){}}serial;
#define SERIAL_ONSTEP serial
struct Device {void serialRecvFlush(){flushes++;}}onStep;
void handleNotFound(){notFound++;}
void processIndexGet(){commands++;} void processMountGet(){commands++;}
void processRotatorGet(){commands++;} void processFocuserGet(){commands++;}
void processAuxGet(){commands++;} void processEncodersGet(){commands++;}
void processNetworkGet(){commands++;}
`;
  const handlers = [
    ['index/Index.cpp', 'handleRoot'], ['mount/Mount.cpp', 'handleMount'],
    ['rotator/Rotator.cpp', 'handleRotator'], ['focuser/Focuser.cpp', 'handleFocuser'],
    ['auxiliary/Auxiliary.cpp', 'handleAux'], ['encoders/Encoders.cpp', 'handleEncoders'],
    ['network/Network.cpp', 'handleNetwork'],
  ];
  for (const [file, name] of handlers) {
    const source = read(file), start = source.indexOf('void ' + name + '(');
    // 保留实际请求分派至输出正文之前的全部代码。
    code += source.slice(start, source.indexOf('www.setContentLength', start)) + '}\n';
  }
  code += `
void save(const char*file){std::ofstream f(file,std::ios::binary);f<<www.body;}
int main(){
  pageAppearance();save("appearance.html");
  servePageStyles();assert(www.length==(long)www.body.size());assert(www.largest<=1024);
  assert(www.cache=="public, max-age=31536000, immutable");
  assert(www.mime=="text/css; charset=utf-8");save("styles.css");
  servePageControls();assert(www.length==(long)www.body.size());assert(www.largest<=1024);
  assert(www.cache=="public, max-age=31536000, immutable");
  assert(www.mime=="application/javascript; charset=utf-8");save("controls.js");
  void(*handlers[])()={${handlers.map(([, name]) => name).join(',')}};
  for(auto handler:handlers){
    www.methodValue=HTTP_GET;www.argsValue=0;stateReads=commands=flushes=0;
    handler();assert(stateReads==0&&commands==0&&flushes==0);
    for(int mode=0;mode<2;mode++){
      www.methodValue=mode?3:HTTP_GET;www.argsValue=mode?0:1;
      stateReads=commands=flushes=0;handler();
      assert(commands==1&&flushes==1);
    }
  }
  status.onStepFound=false;www.methodValue=HTTP_GET;www.argsValue=0;
  assert(!servePageShell(0));
}`;
  const source = path.join(work, 'resources.cpp'), exe = path.join(work, 'resources.exe');
  const ino=fs.readFileSync(path.join(root,'SmartWebServer.ino'),'utf8');
  const routes=[...ino.matchAll(/www\.on\(/g)].length + 2;
  const ethernet=fs.readFileSync(path.join(root,'src/lib/ethernet/webServer/WebServer.h'),'utf8');
  assert(routes<=Number(ethernet.match(/#define WEB_HANDLER_COUNT_MAX\s+(\d+)/)[1]), '新增资源不超过以太网路由容量');
  assert(Buffer.byteLength(templates['htmlScripts.h'].html_script_ajax_get.replace('%s','auxiliary-ajax-get.txt')) < 240, '普通命令脚本不超出原页面缓冲区');
  fs.writeFileSync(source, code);
  cp.execFileSync(process.env.CXX || 'g++', ['-std=c++11', source, '-o', exe], { cwd: work });
  cp.execFileSync(exe, [], { cwd: work });
  const css = fs.readFileSync(path.join(work, 'styles.css'), 'utf8');
  const js = fs.readFileSync(path.join(work, 'controls.js'), 'utf8');
  assert(!/<\/?(?:style|script)>/.test(css + js), '静态资源不包含 HTML 包装标签');
  assert.equal(css, templates['ui/Styles.h'].html_shared_styles.slice(8, -9) + templates['ui/Editors.h'].html_editor_styles.slice(8, -9));
  assert.equal(js, templates['ui/Controls.h'].html_page_controls.slice(9, -10) + templates['ui/Editors.h'].html_settings_editors.slice(9, -10));
  return { css, js, appearance: fs.readFileSync(path.join(work, 'appearance.html'), 'utf8') };
}

async function polling(page) {
  await page.goto('about:blank');
  await page.clock.install();
  await page.setContent('<div id="readout"></div><button id="selected"></button><input id="input"><div id="datetime"></div>');
  await page.evaluate(() => {
    window.requests = []; window.delay = 2000; window.replyStatus = 200;
    window.ajaxPage = 'mount-ajax.txt'; window.mutations = 0;
    new MutationObserver((entries) => window.mutations += entries.length)
      .observe(document.body, { subtree: true, childList: true, attributes: true, characterData: true });
    window.XMLHttpRequest = class {
      open(method, url) { this.url = url; }
      send() {
        this.readyState = 1; window.requests.push(this);
        this.timer = setTimeout(() => {
          this.status = window.replyStatus; this.readyState = 4;
          this.responseText = 'readout|45&deg;30\'\nselected|selected\ninput&12\ncall|update_date_time\n';
          this.onreadystatechange();
        }, window.delay);
      }
      abort() { clearTimeout(this.timer); this.readyState = 4; this.aborted = true; this.status = 0; this.onreadystatechange(); }
    };
  });
  await page.addScriptTag({ content: templates['htmlScripts.h'].html_script_ajax.replace(/^<script>\n|<\/script>\n$/g, '') });
  await page.evaluate(() => autoRun());
  await page.clock.runFor(1500);
  assert.equal(await page.evaluate(() => requests.length), 1, '慢响应不叠加轮询请求');
  await page.clock.runFor(500);
  assert.equal(await page.locator('#readout').innerText(), "45°30'");
  assert.equal(await page.locator('#input').inputValue(), '12');
  assert.equal(await page.locator('#selected').getAttribute('data-selected'), 'true');
  await page.evaluate(() => { window.before = mutations; pageReady(ajaxPage).call(requests[0]); });
  await page.clock.runFor(0);
  assert.equal(await page.evaluate(() => mutations), await page.evaluate(() => before), '相同数据不改写 DOM');
  await page.evaluate(() => window.swsPendingCommands = 1);
  await page.clock.runFor(2000);
  assert.equal(await page.evaluate(() => requests.length), 1, '命令未结束时不增加状态请求');
  await page.evaluate(() => { window.swsPendingCommands = 0; window.delay = 20; });
  await page.clock.runFor(600);
  assert(await page.evaluate(() => requests.length > 1));
  await page.evaluate(() => {
    window.testHidden=true;
    Object.defineProperty(document,'hidden',{configurable:true,get:()=>window.testHidden});
    document.dispatchEvent(new Event('visibilitychange'));
  });
  const hidden = await page.evaluate(() => requests.length);
  await page.clock.runFor(5000);
  assert.equal(await page.evaluate(() => requests.length),hidden,'后台标签页不发状态请求');
  await page.evaluate(() => {window.testHidden=false;document.dispatchEvent(new Event('visibilitychange'));});
  assert.equal(await page.evaluate(() => requests.length),hidden+1,'回到前台立即刷新');
  await page.evaluate(() => swsPausePolling());
  const stopped = await page.evaluate(() => requests.length);
  await page.clock.runFor(6000);
  assert.equal(await page.evaluate(() => requests.length), stopped, '切换页面后停止轮询');
  await page.evaluate(() => {
    autoStopped = false; window.replyStatus = 503;
    window.requests = []; autoFailures = 0; autoRun();
  });
  await page.clock.runFor(7000);
  assert((await page.evaluate(() => requests.length)) <= 3, '失败后逐步退避');
  await page.evaluate(() => swsPausePolling());
  // 加快刷新仍采用原有配置，结束后回到普通间隔。
  await page.evaluate(() => { autoFastRun(); window.fastRate = autoRate(); });
  assert.equal(await page.evaluate(() => fastRate), 500);
  await page.clock.runFor(5100);
  assert.equal(await page.evaluate(() => autoRate()), 500);
}

(async () => {
  const resources = nativeResources(), counts = { css: 0, js: 0 };
  const server = http.createServer((req, res) => {
    const url = new URL(req.url, 'http://localhost');
    if (url.pathname === '/sws-ui.css' || url.pathname === '/sws-ui.js') {
      const key = url.pathname.endsWith('.css') ? 'css' : 'js'; counts[key]++;
      res.setHeader('Cache-Control', 'public, max-age=31536000, immutable');
      res.setHeader('Content-Type', key === 'css' ? 'text/css; charset=utf-8' : 'application/javascript; charset=utf-8');
      return res.end(resources[key]);
    }
    res.setHeader('Cache-Control', 'no-store');
    res.setHeader('Content-Type', 'text/html; charset=utf-8');
    res.end('<!doctype html><html><head><meta charset="utf-8"></head><body>' + resources.appearance + '<main>test</main></body></html>');
  });
  await new Promise((resolve) => server.listen(0, '127.0.0.1', resolve));
  const browser = await chromium.launch({ channel: 'msedge', headless: true });
  try {
    const page = await browser.newPage(), errors = [];
    page.on('pageerror', (e) => errors.push(e.message));
    const base = 'http://127.0.0.1:' + server.address().port;
    await page.goto(base + '/first');
    assert(await page.evaluate(() => getComputedStyle(document.documentElement).getPropertyValue('--bg').trim()));
    await page.goto(base + '/second');
    assert.deepEqual(counts, { css: 1, js: 1 }, '切换页面复用浏览器缓存');
    await page.evaluate(() => {
      for (const url of ['/sws-ui.css?v=new-build', '/sws-ui.js?v=new-build']) fetch(url);
    });
    await page.waitForFunction(() => performance.getEntriesByType('resource').filter((r) => r.name.includes('new-build')).length === 2);
    assert.deepEqual(counts, { css: 2, js: 2 }, '新固件资源地址触发重新下载');
    assert.deepEqual(errors, []);
    await polling(page);
    const bytes = Buffer.byteLength(templates['Appearance.h'].html_shared_appearance);
    console.log('PASS: native resources and lengths, cached CSS/JS, seven shell handlers skip duplicate reads, original command dispatch, single in-flight poll, unchanged DOM, command priority, navigation stop and failure backoff');
    console.log('公共资源复用：每次后续页面加载少传约 ' + (bytes / 1024).toFixed(1) + ' KiB；轮询不再使用 10 ms 定时器。');
  } finally {
    await browser.close(); await new Promise((resolve) => server.close(resolve));
  }
})().catch((e) => { console.error(e); process.exitCode = 1; });
