require('./test-output.cjs');
const path = require('node:path');
const assert = require('node:assert/strict');
const {spawn} = require('node:child_process');
const {chromium} = require('playwright');

const root = path.join(__dirname, '..');
const server = spawn(process.execPath, ['demo/server.cjs'], {
  cwd: root, windowsHide: true, env: {...process.env, SWS_DEMO_PORT:'0'},
});

const ready = new Promise((resolve, reject) => {
  let output = '', errors = '';
  const timer = setTimeout(() => reject(Error('模拟服务启动超时\n' + errors)), 30000);
  server.stdout.on('data', chunk => {
    output += chunk;
    const match = output.match(/SWS demo ready (http:\/\/127\.0\.0\.1:\d+)/);
    if (match) { clearTimeout(timer); resolve(match[1]); }
  });
  server.stderr.on('data', chunk => { errors += chunk; });
  server.on('error', error => { clearTimeout(timer); reject(error); });
  server.on('exit', code => {
    clearTimeout(timer);
    reject(Error('模拟服务已退出：' + code + '\n' + errors));
  });
});

(async () => {
  let browser;
  try {
    const base = await ready;
    browser = await chromium.launch({channel:'msedge', headless:true});
    const page = await browser.newPage(), errors = [];
    page.on('pageerror', error => errors.push(error.message));
    const loaded = async target => {
      await page.waitForFunction(target => {
        const frame = document.getElementById('sws-frame');
        return document.documentElement.classList.contains('sws-shell') &&
          document.querySelector('.sws-nav a[aria-current="page"]')?.pathname === target &&
          frame?.contentWindow.location.pathname === target &&
          frame.contentDocument.querySelector('.b') && frame.style.visibility !== 'hidden' &&
          document.getElementById('sws-load').textContent === '';
      }, target);
      assert.equal(await page.locator('.sws-nav').count(), 1);
      assert.equal(await page.locator('#sws-frame').count(), 1);
      assert.equal(new URL(page.url()).searchParams.has('view'), false);
      assert.equal(await page.locator('#sws-frame').evaluate(frame =>
        new URL(frame.contentWindow.location.href).searchParams.get('view')), '1');
    };
    // 首次打开就应加载完整导航，不能等点击标签后才恢复布局
    for (const width of [1440, 390]) {
      await page.setViewportSize({width, height:1000});
      for (const entry of ['/index.htm', '/index.htm?view=1', '/mount.htm?view=1', '/index.htm?view=1&preview=locked']) {
        await page.goto(base + entry);
        await loaded(new URL(entry, base).pathname);
        assert(await page.evaluate(() => document.documentElement.scrollWidth <= innerWidth));
      }
      // 内部内容页保留 view 参数，切换标签和刷新不能产生重复导航或循环跳转
      await page.locator('.sws-nav a[href="/mount.htm"]').click();
      await loaded('/mount.htm');
      await page.reload();
      await loaded('/mount.htm');
    }
    assert.deepEqual(errors, []);
    console.log('PASS: 模拟正常入口、误带 view 参数、首次导航、标签切换、刷新及桌面手机布局');
  } finally {
    if (browser) await browser.close();
    server.kill();
  }
})().catch(error => { console.error(error); process.exitCode = 1; });
