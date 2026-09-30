import { expect, test } from '@playwright/test'

test('observe without hardware, persist theme and exercise all pages', async ({ page }, info) => {
  const errors: string[] = []
  page.on('pageerror', (error) => errors.push(error.message))
  await page.goto('/')
  await expect(page.getByRole('heading', { name: '观测控制' })).toBeVisible()
  await expect(page.locator('.brand strong')).toHaveText('OnStep')
  await expect(page.getByText('模拟预览 · 未连接真实硬件')).toBeVisible()
  await expect(page.getByRole('button', { name: '开始转向' })).toBeEnabled()
  await page.getByRole('button', { name: '开始转向' }).click()
  await expect(page.locator('.status-pill')).toHaveText('正在转向')
  await page.getByRole('button', { name: '停止全部运动' }).click()
  await expect(page.locator('.status-pill')).toHaveText('待机')
  await page.getByRole('switch', { name: '设备跟踪' }).click()
  await expect(page.locator('.status-pill')).toHaveText('正在跟踪')
  await page.getByRole('button', { name: '夜间红光' }).click()
  await expect(page.locator('html')).toHaveAttribute('data-theme', 'night')
  await page.reload()
  await expect(page.locator('html')).toHaveAttribute('data-theme', 'night')
  await page.getByRole('button', { name: 'EN', exact: true }).click()
  await expect(page.getByRole('heading', { name: 'Observation control' })).toBeVisible()
  await page.getByRole('button', { name: '中文', exact: true }).click()
  await page.locator('.scenario-select select').selectOption('all-subsystems')
  await expect(page.locator('.brand strong')).toHaveText('OnStepX')
  await expect(page).toHaveTitle(/OnStepX/)
  for (const route of [
    '/overview',
    '/rotator',
    '/focuser',
    '/auxiliary',
    '/encoders',
    '/network',
    '/settings',
    '/mount',
  ]) {
    await page.goto('/#' + route)
    await expect(page.locator('main h2')).toBeVisible()
    await expect
      .poll(() => page.evaluate(() => document.documentElement.scrollWidth <= window.innerWidth))
      .toBe(true)
  }
  await page.screenshot({
    path: 'test-results/observation-' + info.project.name + '.png',
    fullPage: true,
  })
  await page.locator('.scenario-select select').selectOption('disconnected')
  await expect(page.getByRole('alert')).toContainText('设备连接已中断')
  await expect(page.getByRole('button', { name: '开始转向' })).toBeDisabled()
  expect(errors).toEqual([])
})

test('catalog and settings round trip', async ({ page }) => {
  await page.goto('/')
  await expect(page.getByRole('button', { name: '开始转向' })).toBeEnabled()
  await page.getByRole('button', { name: '目标与校准', exact: true }).click()
  const catalog = page
    .locator('details')
    .filter({ has: page.locator('summary', { hasText: '天体库' }) })
  await catalog.locator('summary').click()
  await catalog.locator('select').selectOption('1')
  await catalog.getByRole('button', { name: '读取星表' }).click()
  await expect(catalog.locator('textarea')).toContainText('')
  await expect(catalog.locator('textarea')).toHaveValue(/M42/)
  await catalog.locator('textarea').fill('$MyList\nM31,GAL,00:42:44,+41*16:09')
  page.on('dialog', (dialog) => dialog.accept())
  await catalog.getByRole('button', { name: '上传星表' }).click()
  await expect(catalog.getByRole('status')).toHaveText('Catalog saved')
  await page.goto('/#/settings')
  await expect(page.getByLabel('每度步数')).toHaveValue('12800')
  await page.getByLabel('每度步数').fill('13000')
  await page.locator('form').first().getByRole('button', { name: '保存', exact: true }).click()
  await expect(page.getByLabel('每度步数')).toHaveValue('13000')
})

test('release sends a stop after the start request', async ({ page }) => {
  const commands: string[] = []
  page.on('request', (request) => {
    if (request.url().endsWith('/api/v1/actions'))
      commands.push(new URLSearchParams(request.postData() ?? '').get('action') ?? '')
  })
  await page.goto('/')
  const north = page.locator('.guide-pad .north')
  await expect(north).toBeEnabled()
  await north.dispatchEvent('pointerdown', { pointerId: 1 })
  await expect.poll(() => commands.includes('mount.guide.start')).toBe(true)
  await north.dispatchEvent('pointerup', { pointerId: 1 })
  await expect.poll(() => commands.includes('mount.guide.stop')).toBe(true)
  expect(commands.indexOf('mount.guide.stop')).toBeGreaterThan(
    commands.indexOf('mount.guide.start'),
  )
})

test('keyboard holds and rapid restarts do not receive an old stop', async ({ page }) => {
  const commands: string[] = []
  page.on('request', (request) => {
    if (request.url().endsWith('/api/v1/actions'))
      commands.push(new URLSearchParams(request.postData() ?? '').get('action') ?? '')
  })
  await page.goto('/')
  const north = page.locator('.guide-pad .north')
  await expect(north).toBeEnabled()
  await north.focus()
  await page.keyboard.down('Space')
  await expect
    .poll(() => commands.filter((command) => command === 'mount.guide.start').length)
    .toBe(1)
  await page.keyboard.up('Space')
  await expect.poll(() => commands.includes('mount.guide.stop')).toBe(true)
  await page.keyboard.down('Space')
  await expect
    .poll(() => commands.filter((command) => command === 'mount.guide.start').length)
    .toBe(2)
  const restartIndex = commands.lastIndexOf('mount.guide.start')
  // 等待超过旧停止重试窗口，检查新动作不会被上一轮计时器打断
  await page.waitForTimeout(650)
  expect(commands.slice(restartIndex + 1)).not.toContain('mount.guide.stop')
  await page.keyboard.up('Space')
  await expect.poll(() => commands.slice(restartIndex + 1).includes('mount.guide.stop')).toBe(true)
  await page.getByRole('button', { name: '运动设置', exact: true }).click()
  await expect(page.locator('.guide-pad')).toBeHidden()
  await expect(page.locator('summary', { hasText: '跟踪微调' })).toBeVisible()
})
