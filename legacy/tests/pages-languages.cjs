const testOutput = require('./test-output.cjs');
// 各语言执行实际页面生成函数，并检查控件、脚本和手机布局
const fs = require('node:fs'),
  path = require('node:path'),
  cp = require('node:child_process'),
  assert = require('node:assert/strict');
const { chromium } = require('../../webui/node_modules/playwright');
const root = path.join(__dirname, '../SmartWebServer'),
  read = (p) => fs.readFileSync(path.join(root, p), 'utf8'),
  out = process.argv[2] || testOutput;
fs.mkdirSync(out, { recursive: true });
const statuses = {
  sta: { enabled: false, ssid: 'Test Wi-Fi', open: false, dhcp: true },
  ap: {
    enabled: true,
    ssid: 'SWS',
    channel: 7,
    ip: '192.168.0.1',
    gw: '192.168.0.1',
    sn: '255.255.255.0',
  },
  connected: false,
  connecting: false,
  pending: false,
  apActive: true,
  apSsid: 'SWS',
  apIp: '192.168.0.1',
  message: '',
};
(async () => {
  const browser = await chromium.launch({ channel: 'msedge', headless: true });
  try {
    const page = await browser.newPage({ viewport: { width: 1440, height: 1100 } });
    let errors = [];
    page.on('pageerror', (e) => errors.push(e.message));
    await page.route('**/*', (route) =>
      route.fulfill({
        contentType: 'application/json',
        body: route.request().url().includes('/net-wifi') ? JSON.stringify(statuses) : '',
      }),
    );
    const shapes = {},
      report = [];
    for (const language of ['cn', 'en', 'us', 'ca', 'de', 'es', 'fr', 'it', 'jp', 'ro']) {
      process.env.SWS_TEST_LANGUAGE = language;
      const templates = require('./page-templates.cjs')(root, language),
        h = (name) => templates['htmlHeaders.h'][name],
        defs = {};
      for (const m of read(
        'src/locales/Strings_' + (language === 'us' ? 'en' : language) + '.h',
      ).matchAll(/^#define\s+(\w+)\s+("(?:\\.|[^"\\])*")/gm))
        defs[m[1]] = JSON.parse(m[2]);
      const tile = (title, body) =>
        h('html_tile_text_beg').replace('%s', '22em').replace('%s', '18em').replace('%s', title) +
        '<br><hr>' +
        body +
        h('html_tile_end');
      const motion =
        templates['htmlScripts.h'].html_script_motion_controls_1 +
        templates['htmlScripts.h'].html_script_motion_controls_2;
      const axisPath = path.join(out, 'language-axis-' + language + '.html');
      cp.execFileSync(process.execPath, [path.join(__dirname, 'axis-page.cjs'), axisPath], {
        stdio: 'pipe',
      });
      const pages = require('./full-page-preview.cjs')({
        templates,
        defs,
        tile,
        h,
        motion,
        axis: fs.readFileSync(axisPath, 'utf8'),
      });
      const mountForm=templates['index/StatusTile.h'].html_indexMountType.replace('%s',' selected').replace('%s','').replace('%s','');
      assert(Buffer.byteLength(mountForm)<1024,language+' 赤道仪类型表单适配固件缓冲区');
      const bodies = {
        index: pages.controller,
        mount: pages.mount,
        focuser: pages.optional['/focuser.htm'],
        rotator: pages.optional['/rotator.htm'],
        auxiliary: pages.optional['/auxiliary.htm'],
        enc: pages.optional['/enc.htm'],
        net: require('./network-page.cjs')({fragment:true}),
      };
      const appearance = templates['Appearance.h'].html_shared_appearance,
        counts = {};
      for (const [name, body] of Object.entries(bodies)) {
        await page.goto('http://languages.test/' + name + '.htm');
        errors = [];
        const stubs =
          '<script>function update_date_time(){}function SetDateTime(){}function autoFastRun(){}function s(){}function g(){}function gf(){}function sf(){}function sz(){}</script>';
        await page.setContent(
          '<!doctype html><html><head><meta charset="utf-8">' +
            appearance +
            (name === 'net' ? templates['network/NetworkWifi.h'].html_network_css : '') +
            '</head><body>' +
            stubs +
            body +
            '</body></html>',
        );
        await page.evaluate(() =>
          document.querySelectorAll('.collapsible').forEach((e) => e.click()),
        );
        const data = await page.evaluate(() => {
          const failures = [];
          for (const node of document.querySelectorAll('*'))
            for (const attr of node.attributes)
              if (/^on/.test(attr.name))
                try {
                  new Function(attr.value);
                  if (attr.value.includes('confirm(this.dataset.confirm)') && !node.dataset.confirm)
                    failures.push('确认按钮缺少当前语言文案');
                } catch (e) {
                  failures.push(attr.name + ': ' + attr.value);
                }
          return {
            failures,
            controls: [...document.querySelectorAll('button,input,select,textarea')].map((e) => ({
              name: e.getAttribute('name'),
              disabled: e.disabled,
              readonly: e.readOnly,
              inForm: !!e.form,
              handlers: [...e.attributes].filter((a) => /^on/.test(a.name)).map((a) => a.value),
            })),
            shape: [...document.querySelectorAll('button,input,select,textarea')].map((e) => [
              e.tagName,
              e.getAttribute('name'),
              e.id,
              e.getAttribute('type'),
            ]),
          };
        });
        require('./page-interfaces.cjs')(name, data.controls);
        assert.deepEqual(data.failures, [], language + '/' + name + ' 事件脚本完整');
        assert.deepEqual(errors, [], language + '/' + name + ' 页面脚本正常');
        if (language === 'cn') shapes[name] = data.shape;
        else
          assert.deepEqual(
            data.shape,
            shapes[name],
            language + '/' + name + ' 控件和参数未被翻译截断',
          );
        counts[name] = data.shape.length;
        for (const width of [1440, 390, 320]) {
          await page.setViewportSize({ width, height: 1100 });
          const overflow = await page.evaluate(() =>
            [...document.querySelectorAll('.b1,.nw-card')]
              .filter((e) => e.scrollWidth > e.clientWidth + 1)
              .map((e) => ({
                text: e.textContent.slice(0, 90),
                width: e.clientWidth,
                scroll: e.scrollWidth,
              })),
          );
          assert.deepEqual(overflow, [], language + '/' + name + ' 卡片无横向溢出 ' + width);
        }
      }
      assert.equal(await page.locator('#nw-scan').innerText(), defs.L_UI_REFRESH);
      report.push({ language, controls: counts });
      console.log(
        'PASS DISPLAY_LANGUAGE=L_' +
          language +
          ': ' +
          Object.entries(counts)
            .map(([p, n]) => p + '=' + n)
            .join(', '),
      );
    }
    fs.writeFileSync(
      path.join(out, 'pages-languages-report.json'),
      JSON.stringify(report, null, 2),
    );
  } finally {
    await browser.close();
  }
})().catch((e) => {
  console.error(e);
  process.exit(1);
});
