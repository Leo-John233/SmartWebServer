import { computed, onBeforeUnmount, readonly, ref } from 'vue'
import { LegacyClient } from '../api/legacyClient'
import type { DeviceAction } from '../api/deviceActions'
import { safeUiConfig, type DeviceSnapshot, type UiConfig } from '../domain/device'
import { mockScenarios, type ScenarioId } from '../mocks/scenarios'

const activeScenario = ref<ScenarioId>('normal')
const config = ref<UiConfig>(safeUiConfig)
const snapshot = ref<DeviceSnapshot | null>(null)
const error = ref('')
const loading = ref(false)
const actionPending = ref(false)
const clock = ref(Date.now())
let timer: number | undefined
let client: LegacyClient | undefined

function getClient(): LegacyClient {
  client ??= new LegacyClient({ scenario: () => activeScenario.value })
  return client
}

async function refresh(): Promise<void> {
  clock.value = Date.now()
  loading.value = snapshot.value === null
  try {
    const nextConfig = await getClient().getConfig()
    const nextSnapshot = await getClient().getSnapshot(nextConfig)
    config.value = nextConfig
    snapshot.value = nextSnapshot
    error.value =
      nextConfig.safeReadOnly || nextConfig.controller.protocolFamily === 'unknown'
        ? 'Device configuration is unavailable, actions remain disabled'
        : ''
  } catch {
    error.value = 'Device connection interrupted'
    if (snapshot.value) snapshot.value.controller.connection = 'disconnected'
  } finally {
    loading.value = false
  }
}

async function sendAction(action: DeviceAction, safetyAction = false): Promise<void> {
  if (!safetyAction && !actionsAllowed.value) return
  actionPending.value = true
  try {
    await getClient().performAction(action)
    await refresh()
  } catch {
    error.value = 'The device did not accept the action'
  } finally {
    actionPending.value = false
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

const stateFresh = computed(() => {
  const updatedAt = snapshot.value?.controller.updatedAt
  return (
    updatedAt !== null &&
    updatedAt !== undefined &&
    clock.value - updatedAt <= Math.max(config.value.pollIntervalMs * 4, 2500)
  )
})

const actionsAllowed = computed(
  () =>
    snapshot.value?.controller.connection === 'connected' &&
    config.value.controller.protocolFamily !== 'unknown' &&
    !config.value.safeReadOnly &&
    stateFresh.value,
)

export function useDeviceSession() {
  onBeforeUnmount(() => window.clearTimeout(timer))
  return {
    activeScenario: readonly(activeScenario),
    config: readonly(config),
    snapshot: readonly(snapshot),
    error: readonly(error),
    loading: readonly(loading),
    actionPending: readonly(actionPending),
    stateFresh,
    scenarios: mockScenarios,
    actionsAllowed,
    start,
    refresh,
    selectScenario,
    performMotion: async (action: DeviceAction) => {
      const stopping = ['mount.guide.stop', 'focuser.stop', 'rotator.stop'].includes(action.type)
      if (!stopping && !actionsAllowed.value) return
      await getClient().performAction(action)
    },
    performAction: (action: DeviceAction) => sendAction(action),
    performSafetyAction: (action: DeviceAction) => sendAction(action, true),
  }
}
