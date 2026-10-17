const output = require('./test-output.cjs');
const fs = require('node:fs'), path = require('node:path'), assert = require('node:assert/strict');
const { chromium } = require('playwright');
const templates = require('./page-templates.cjs')(path.join(__dirname, '..'));
const cards = require('./mount-page.cjs')(), layout = templates['ui/Layouts.h'];
const scripts = templates['htmlScripts.h'];
const body = layout.html_layout_mount_begin + cards.Guide + layout.html_layout_mount_tracking + cards.Tracking +
  layout.html_layout_mount_location + cards.Site + cards.Limits + layout.html_layout_mount_target + cards.Goto +
  layout.html_layout_mount_home + cards.HomePark + layout.html_layout_mount_library + cards.Library +
  layout.html_layout_mount_advanced + cards.Align + cards.Pec + layout.html_layout_mount_end;
const clock = scripts.html_script_ajax_date_time_return + '<script>' + scripts.html_script_ajax.match(/function pad[\s\S]*?(?=var autoTimer)/)[0] + '</script>';
const html = '<!doctype html><meta charset="utf-8"><meta name="viewport" content="width=device-width,initial-scale=1">' +
  templates['Appearance.h'].html_shared_appearance +
  '<script>window.commands=[];function s(k,v){commands.push([k,v])}function g(v){s("dr",v)}function autoFastRun(){}</script>' +
  clock + '<div class="b">' + body + '</div>' + scripts.html_script_collapsible;
(async () => {
  const browser = await chromium.launch({ channel: 'msedge', headless: true });
  try {
    const page = await browser.newPage({ viewport: { width: 1440, height: 1100 }, timezoneId:'America/New_York' }), errors = [], submissions = [];
    let trackingReply='ok'; const trackingRequests=[];
    page.on('pageerror', e => { errors.push(e.message); console.error(e.stack); });
    await page.route('http://editors.test/**', route => {
      const url = new URL(route.request().url());
      if(url.pathname==='/mount-ajax-get.txt'){trackingRequests.push(url.searchParams.get('dr'));return route.fulfill({body:trackingReply});}
      if (url.searchParams.size) submissions.push(Object.fromEntries(url.searchParams));
      return route.fulfill({ contentType: 'text/html', body: html });
    });
    await page.goto('http://editors.test/');
    await page.clock.setFixedTime(new Date('2026-10-17T00:00:00Z'));
    assert.equal(await page.locator('#sws-local-time').inputValue(),'2026-10-16 18:08:38');
    assert.equal(await page.locator('#date_ut').count(),1);assert.equal(await page.locator('#time_lst').count(),1);
    assert(await page.locator('#date_ut').isVisible());assert(await page.locator('#time_lst').isVisible());
    assert.equal(await page.locator('#time_ut').innerText(),'10:08:38');
    assert.equal(await page.locator('#time_lst').innerText(),'19:52:26');
    const footerPosition=await page.evaluate(()=>document.querySelector('.sws-astronomy-time').getBoundingClientRect().top>=document.querySelector('.sws-clock-controls').getBoundingClientRect().bottom);
    assert(footerPosition,'世界时和恒星时位于同步区下方');
    await page.evaluate(()=>{document.getElementById('time_ut').textContent='23:59:59';});
    await page.waitForFunction(()=>document.getElementById('sws-local-time').value==='2026-10-17 07:59:59');
    await page.evaluate(()=>{document.getElementById('time_ut').textContent='10:08:38';});
    const toggle=page.locator('.sws-track-switch [role=switch]');
    await toggle.click();await page.waitForFunction(()=>document.querySelector('.sws-track-switch [role=switch]').getAttribute('aria-checked')==='true');
    assert.equal(trackingRequests.pop(),'T1');
    trackingReply='rejected';await toggle.click();await page.getByRole('status').filter({hasText:'主控拒绝'}).waitFor();
    assert.equal(await toggle.getAttribute('aria-checked'),'true');assert.equal(trackingRequests.pop(),'T0');
    trackingReply='timeout';await toggle.click();await page.getByRole('status').filter({hasText:'主控应答超时'}).waitFor();
    trackingReply='invalid';await toggle.click();await page.getByRole('status').filter({hasText:'主控应答异常'}).waitFor();
    trackingReply='ok';await toggle.click();await page.waitForFunction(()=>document.querySelector('.sws-track-switch [role=switch]').getAttribute('aria-checked')==='false');
    const site = page.locator('#sws-site-form'), limits = page.locator('#sws-limits-form');
    const degree = name => page.locator('[name='+name+']').locator('..').locator('input:not([hidden])');
    const direction = name => page.locator('[name='+name+']').locator('xpath=ancestor::fieldset').locator('.sws-unit-inputs > select');
    const timezone = page.locator('.sws-timezone-select');
    assert.equal(await timezone.inputValue(), '-480', 'UTC+08:00 对应原始设备偏移 -08:00');
    assert.equal(await degree('g1').inputValue(), '121');
    assert.equal(await direction('g1').inputValue(), '-');
    assert.equal(await degree('g1').isEditable(), false, '默认只读，点击编辑后才修改');
    const geometry = await page.evaluate(() => {
      const s=document.querySelector('.sws-site').getBoundingClientRect(), l=document.querySelector('.sws-limits').getBoundingClientRect(), t=document.querySelector('.sws-target').getBoundingClientRect();
      return { sameRow:Math.abs(s.top-l.top)<1, above:Math.max(s.bottom,l.bottom)<t.top };
    });
    assert(geometry.sameRow && geometry.above, '位置与限位并排并位于 GOTO 上方');
    assert.equal(await page.locator('.sws-target>.b1>div:first-child').innerText(), '按坐标 GOTO:');
    await page.locator('[data-edit-form=sws-site-form]').click();
    await degree('g1').fill('0'); await direction('g1').selectOption('-'); await site.locator('[name=g2]').fill('30');
    await degree('t1').fill('0'); await direction('t1').selectOption('-'); await site.locator('[name=t2]').fill('15');
    await timezone.selectOption('-840');
    await site.locator('button[type=submit]').click();
    await page.waitForURL('**/mount.htm?**');
    assert.equal(submissions[0].g1, '-0', '零度以内东经保留负号');
    assert.equal(submissions[0].t1, '-0', '零度以内南纬保留负号');
    assert.equal(submissions[0].u1, '-14'); assert.equal(submissions[0].u2, '0');
    for (const key of ['g1','g2','g3','t1','t2','t3','u1','u2']) assert(key in submissions[0], key+' 原接口参数完整');
    await page.locator('[data-edit-form=sws-site-form]').click();
    await degree('g1').fill('45'); await timezone.selectOption('330');
    await site.locator('button[type=reset]').click();
    await page.waitForFunction(() => document.querySelector('.sws-site').dataset.editing === 'false');
    assert.equal(await degree('g1').inputValue(), '121'); assert.equal(await timezone.inputValue(), '-480');
    assert.equal(submissions.length, 1, '取消不会提交或改变设备');
    await page.locator('[data-edit-form=sws-limits-form]').click();
    await degree('hl').fill('12');
    await degree('el').fill('270'); await limits.locator('[name=el]').locator('..').locator('select').selectOption('-');
    await limits.locator('.sws-backlash-editor').evaluate(e => e.open = true);
    await limits.locator('[name=b1]').fill('10'); await limits.locator('[name=b2]').fill('20');
    await degree('wl').fill('271'); assert.equal(await limits.evaluate(e => e.checkValidity()), false, '限位保留 ±270° 边界');
    await degree('wl').fill('2');
    await limits.locator('button[type=submit]').click();
    await page.waitForFunction(() => document.querySelector('.sws-limits').dataset.editing === 'false');
    await page.waitForTimeout(100);
    assert.equal(submissions[1].hl, '-12'); assert.equal(submissions[1].el, '-270'); assert.equal(submissions[1].wl, '2');
    assert.equal(submissions[1].b1, '10'); assert.equal(submissions[1].b2, '20');
    await page.evaluate(() => { document.getElementById('park').disabled=true; document.getElementById('unpark').disabled=false; });
    await page.waitForFunction(() => document.getElementById('park').hidden);
    assert.equal(await page.locator('.sws-park-toggle button:visible').count(), 1);
    await page.locator('#unpark').click(); assert.deepEqual(await page.evaluate(() => commands.pop()), ['pk','u']);
    await page.evaluate(() => { document.getElementById('park').disabled=false; document.getElementById('unpark').disabled=true; });
    await page.waitForFunction(() => !document.getElementById('park').hidden);
    await page.locator('#park').click(); assert.deepEqual(await page.evaluate(() => commands.pop()), ['pk','p']);
    await page.locator('#setpark').click(); assert.deepEqual(await page.evaluate(() => commands.pop()), ['pk','s']);
    await page.evaluate(() => document.querySelectorAll('.collapsible').forEach(e => e.click()));
    for (const [id, value, label] of [['off','mh_off','直接翻转'],['visit','mh_visit','经零位翻转'],['pause','mh_pause','到零位后暂停']]) {
      const button=page.locator('#gto_mfh_'+id); assert.equal(await button.innerText(),label);
      await button.click(); assert.deepEqual(await page.evaluate(() => commands.pop()), ['goto',value]);
    }
    await page.locator('button[name=qb][value=st]').click();
    await page.waitForURL('**/mount.htm?qb=st**');
    assert.equal(submissions[2].qb, 'st');
    for (const key of ['dm','dd','dy','th','tm','ts']) assert(/^\d+$/.test(submissions[2][key]), key+' 时间同步参数完整');
    assert.equal(submissions[2].th,'8','同步使用设备 UTC+8 而非浏览器纽约时区');
    assert.equal(submissions[2].dd,'17');
    assert(!('g1' in submissions[2]), '时间同步与位置设置各自提交');
    assert.equal(await page.locator('.sws-locate').count(),0,'原版方式不提供浏览器定位按钮');
    assert(!html.includes('getCurrentPosition'),'页面不再调用手机定位接口');
    assert.equal(await degree('g1').inputValue(),'121','显示主控返回的经度');
    const colors=await page.evaluate(()=>({site:getComputedStyle(document.querySelector('.sws-site')).backgroundColor,limits:getComputedStyle(document.querySelector('.sws-limits')).backgroundColor}));
    assert.equal(colors.site,colors.limits);
    await page.locator('.sws-site-limits').screenshot({path:path.join(output,'position-limits-readonly.png')});
    await page.locator('.sws-home-actions').screenshot({path:path.join(output,'home-park-compact.png')});
    for (const width of [1440,1024,390,320]) {
      await page.setViewportSize({width,height:1100});
      assert(await page.evaluate(() => document.documentElement.scrollWidth<=innerWidth), '页面无横向溢出 '+width);
      for (const form of [site,limits]) {
        const edit=page.locator('[data-edit-form='+await form.getAttribute('id')+']');
        if (await edit.isEnabled()) await edit.click();
      }
      const narrow=await page.locator('.sws-site-limits').evaluate(e => [...e.querySelectorAll('input:not([hidden]),select')].filter(x=>x.getClientRects().length&&x.clientWidth<42).map(x=>({name:x.name,width:x.clientWidth,html:x.outerHTML})));
      assert.deepEqual(narrow, [], '手机输入框保留可读宽度 '+width);
      await page.locator('.sws-site-limits').screenshot({path:path.join(output,'position-limits-'+width+'.png')});
    }
    assert.deepEqual(errors, []);
    console.log('PASS: actual position/limits cards, signed coordinates, UTC mapping, edit/cancel, original submissions, single park action, flip modes and responsive layout');
  } finally { await browser.close(); }
})().catch(e => { console.error(e); process.exit(1); });
