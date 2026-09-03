import { computed, onBeforeUnmount, readonly, ref } from 'vue'
import { LegacyClient } from '../api/legacyClient'
import { safeUiConfig, type DeviceSnapshot, type UiConfig } from '../domain/device'
import { mockScenarios, type ScenarioId } from '../mocks/scenarios'

const activeScenario = ref<ScenarioId>('normal')
const config = ref<UiConfig>(safeUiConfig)
const snapshot = ref<DeviceSnapshot | null>(null)
const error = ref('')
const loading = ref(false)
let timer: number | undefined
let client: LegacyClient | undefined

function getClient(): LegacyClient {
  client ??= new LegacyClient({ scenario: () => activeScenario.value })
  return client
}

async function refresh(): Promise<void> {
  loading.value = snapshot.value === null
  try {
    const nextConfig = await getClient().getConfig()
    const nextSnapshot = await getClient().getSnapshot(nextConfig)
    config.value = nextConfig
    snapshot.value = nextSnapshot
    error.value = nextConfig.safeReadOnly ? 'Device configuration is unavailable, actions remain disabled' : ''
  } catch {
    error.value = 'Device connection interrupted'
    if (snapshot.value) snapshot.value.controller.connection = 'disconnected'
  } finally {
    loading.value = false
  }
}

function schedule(): void {
  window.clearTimeout(timer)
  timer = window.setTimeout(async () => {
    await refresh()
    schedule()
  }, config.value.pollIntervalMs)
}

async function start(): Promise<void> {
  await refresh()
  schedule()
}

async function selectScenario(id: ScenarioId): Promise<void> {
  activeScenario.value = id
  await refresh()
  schedule()
}

export function useDeviceSession() {
  onBeforeUnmount(() => window.clearTimeout(timer))
  return {
    activeScenario: readonly(activeScenario),
    config: readonly(config),
    snapshot: readonly(snapshot),
    error: readonly(error),
    loading: readonly(loading),
    scenarios: mockScenarios,
    actionsAllowed: computed(() => snapshot.value?.controller.connection === 'connected' && !config.value.safeReadOnly),
    start,
    refresh,
    selectScenario,
  }
}
