import { defineConfig, loadEnv } from 'vite'
import vue from '@vitejs/plugin-vue'

const devicePaths = [
  '/index.htm',
  '/mount.htm',
  '/net.htm',
  '/auxiliary.htm',
  '/libraryHelp.htm',
  '/ui-config.json',
  '/api',
  '/index-ajax-get.txt',
  '/index.txt',
  '/mount-ajax-get.txt',
  '/mount-ajax.txt',
  '/rotator-ajax-get.txt',
  '/rotator-ajax.txt',
  '/focuser-ajax-get.txt',
  '/focuser-ajax.txt',
  '/auxiliary-ajax-get.txt',
  '/auxiliary-ajax.txt',
  '/enc-ajax-get.txt',
  '/enc-ajax.txt',
]

export default defineConfig(({ mode }) => {
  const env = loadEnv(mode, process.cwd(), '')
  const target = env.VITE_DEVICE_URL || 'http://192.168.0.1'
  const proxy =
    mode === 'device'
      ? Object.fromEntries(devicePaths.map((path) => [path, { target, changeOrigin: true }]))
      : undefined

  return {
    base: mode === 'embed' ? '/ui/' : '/',
    plugins: [vue()],
    server: { host: '127.0.0.1', port: 5173, proxy },
    preview: { host: '127.0.0.1', port: 4173 },
    build: {
      target: 'es2020',
      sourcemap: false,
      cssCodeSplit: true,
      assetsInlineLimit: 0,
    },
  }
})
