import { createApp } from 'vue'
import App from './App.vue'
import { router } from './router'
import './style.css'

async function enableMocking(): Promise<void> {
  if (import.meta.env.MODE !== 'mock') return
  const { worker } = await import('./mocks/browser')
  await worker.start({ onUnhandledRequest: 'bypass' })
}

void enableMocking().then(() => createApp(App).use(router).mount('#app'))
