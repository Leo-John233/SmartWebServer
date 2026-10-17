const testOutput = require('./test-output.cjs');
// 验证真实编码器页面的同步请求、选中反馈、读数尺寸和手机布局。
const assert = require('node:assert/strict');
const fs = require('node:fs');
const path = require('node:path');
const { chromium } = require('playwright');
const root = path.join(__dirname, '..');
const templates = require('./page-templates.cjs')(root);
const body = require('./encoders-page.cjs')({ absolute: true });
const appearance = templates['Appearance.h'].html_shared_appearance;
const collapse = templates['htmlScripts.h'].html_script_collapsible;
const output = process.argv[2] || testOutput;
fs.mkdirSync(output, { recursive: true });

(async () => {
  const browser = await chromium.launch({ channel: 'msedge', headless: true });
  try {
    const page = await browser.newPage({ viewport: { width: 1440, height: 1000 } });
    const errors = [],
      requests = [];
    let automatic = false;
    page.on('pageerror', (error) => errors.push(error.message));
    await page.route('http://encoders.test/**', (route) => {
      const url = new URL(route.request().url());
      if (url.pathname === '/enc-ajax-get.txt') {
        const action = url.searchParams.get('enc');
        requests.push(action);
        if (action === 'on' || action === 'off') automatic = action === 'on';
        return route.fulfill({ body: '' });
      }
      if (url.pathname === '/enc-ajax.txt')
        return route.fulfill({
          body:
            'enc_as_on|' +
            (automatic ? 'selected' : 'unselected') +
            '\n' +
            'enc_as_off|' +
            (automatic ? 'unselected' : 'selected') +
            '\n' +
            'enc_a1|+120°59′59.9″\nraw_a1|+120°59′59.9″\ndiff_a1|-12.5\n' +
            'enc_a2|-89°59′59.9″\nraw_a2|-89°59′59.9″\ndiff_a2|+12.5\n',
        });
      return route.fulfill({
        contentType: 'text/html; charset=utf-8',
        body:
          '<!doctype html><html><head><meta charset="utf-8">' +
          appearance +
          '</head><body><div class="b">' +
          body +
          '</div>' +
          collapse +
          '</body></html>',
      });
    });
    await page.goto('http://encoders.test/enc.htm');
    await page.waitForFunction(
      () => document.getElementById('enc_as_off').dataset.selected === 'true',
    );
    const syncButton = page.getByRole('button', { name: '同步 OnStep -> 编码器', exact: true });
    const normalBackground = await syncButton.evaluate((e) => getComputedStyle(e).backgroundColor);
    await syncButton.hover();
    await page.mouse.down();
    assert.equal(
      await syncButton.evaluate((e) => getComputedStyle(e).backgroundColor),
      normalBackground,
      '按下时保留按钮基底色',
    );
    assert.match(
      await syncButton.evaluate((e) => getComputedStyle(e).backgroundImage),
      /rgba.*0\.32/,
      '按下时叠加半透明灰色遮罩',
    );
    await page.mouse.up();
    await page.getByRole('button', { name: '同步 编码器 -> OnStep', exact: true }).click();
    await page.locator('#enc_as_on').click();
    await page.mouse.move(0, 0);
    await page.waitForFunction(
      () => document.getElementById('enc_as_on').dataset.selected === 'true',
    );
    const colors = await page.evaluate(() => ({
      selected: getComputedStyle(document.getElementById('enc_as_on')).backgroundColor,
      normal: getComputedStyle(document.getElementById('enc_as_off')).backgroundColor,
      overlay: getComputedStyle(document.getElementById('enc_as_on')).backgroundImage,
      value: getComputedStyle(document.getElementById('enc_a1')).color,
      text: getComputedStyle(document.body).getPropertyValue('--text').trim(),
    }));
    assert.equal(colors.selected, colors.normal, '选中状态不切换基底色');
    assert.match(colors.overlay, /rgba.*0\.22/, '后台选中状态采用透明灰色遮罩');
    assert.equal(colors.value, 'rgb(212, 212, 212)', '读数使用主题浅灰');
    await page.locator('#enc_as_off').click();
    await page.waitForFunction(
      () => document.getElementById('enc_as_off').dataset.selected === 'true',
    );
    assert(
      requests.includes('o2e') &&
        requests.includes('e2o') &&
        requests.includes('on') &&
        requests.includes('off'),
    );
    for (const width of [1440, 390, 320]) {
      await page.setViewportSize({ width, height: 1000 });
      for (const button of await page.locator('.collapsible').all()) {
        if (!(await button.evaluate((e) => e.classList.contains('active')))) await button.click();
      }
      const failures = await page.evaluate(() =>
        [
          ...document.querySelectorAll(
            '.b1,.sws-encoder-metric,.sws-encoder-metric strong,.sws-encoder-field',
          ),
        ]
          .filter((e) => e.scrollWidth > e.clientWidth + 1)
          .map((e) => e.textContent.trim()),
      );
      assert.deepEqual(failures, [], '长坐标和表单不溢出 ' + width);
      const structure = await page.locator('.sws-encoder-axis').evaluateAll((nodes) =>
        nodes.map((e) => {
          const r = e.getBoundingClientRect();
          return { x: r.x, y: r.y, width: r.width };
        }),
      );
      if (width === 1440) assert.equal(structure[0].y, structure[1].y, '两轴卡片顶部齐平');
      else assert.equal(structure[0].x, structure[1].x, '手机两轴卡片改为单列');
      await page.screenshot({
        path: path.join(output, 'encoders-layout-' + width + '.png'),
        fullPage: true,
      });
    }
    assert.deepEqual(errors, []);
    console.log(
      'PASS: real encoder controls, sync requests, visible selected state, neutral readouts, long coordinates and 1440/390/320 layouts',
    );
  } finally {
    await browser.close();
  }
})().catch((error) => {
  console.error(error);
  process.exitCode = 1;
});
