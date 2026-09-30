import { defineConfig, devices } from '@playwright/test'
export default defineConfig({
  testDir: './e2e-mock',
  fullyParallel: false,
  workers: 1,
  reporter: 'line',
  use: { baseURL: 'http://127.0.0.1:4175', trace: 'retain-on-failure' },
  webServer: {
    command:
      'node ./node_modules/vite/bin/vite.js preview --host 127.0.0.1 --port 4175 --strictPort',
    url: 'http://127.0.0.1:4175',
  },
  projects: [
    { name: 'desktop', use: { ...devices['Desktop Chrome'] } },
    { name: 'phone', use: { ...devices['iPhone 13'], defaultBrowserType: 'chromium' } },
    {
      name: 'tablet',
      use: { viewport: { width: 820, height: 1180 }, isMobile: true, hasTouch: true },
    },
  ],
})
