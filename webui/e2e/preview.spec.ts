import { expect, test } from '@playwright/test'

test('loads through hash routing', async ({ page }) => {
  await page.goto('/#/mount')
  await expect(page).toHaveTitle('Smart Web Server')
  await expect(page.getByRole('heading', { name: 'Observation control' })).toBeVisible()
  await expect(page.getByRole('button', { name: 'Stop all motion' })).toBeVisible()
  await expect(page).toHaveURL(/#\/mount$/)
})

test('production preview remains safe without a device', async ({ page }) => {
  await page.goto('/')
  await expect(page.getByRole('alert')).toContainText('Device connection interrupted')
})
