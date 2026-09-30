import { onBeforeUnmount, onMounted, ref, watch, type Ref } from 'vue'
import { onBeforeRouteLeave } from 'vue-router'
import type { DeviceAction } from '../api/deviceActions'

interface HoldActions<T extends string> {
  start: (direction: T) => DeviceAction
  stop: (direction: T) => DeviceAction
}

export function useHoldMotion<T extends string>(
  send: (action: DeviceAction) => Promise<void>,
  enabled: Ref<boolean>,
  actions: HoldActions<T>,
) {
  const activeDirection = ref<T | null>(null)
  let queue = Promise.resolve()

  function enqueue(action: DeviceAction): void {
    queue = queue.then(() => send(action)).catch(() => undefined)
  }

  function stopDirection(direction: T): void {
    enqueue(actions.stop(direction))
    window.setTimeout(() => enqueue(actions.stop(direction)), 150)
    window.setTimeout(() => enqueue(actions.stop(direction)), 500)
  }

  function stop(): void {
    const direction = activeDirection.value
    activeDirection.value = null
    if (direction) stopDirection(direction)
  }

  function start(direction: T): void {
    if (!enabled.value || activeDirection.value === direction) return
    stop()
    activeDirection.value = direction
    enqueue(actions.start(direction))
  }

  function onVisibilityChange(): void {
    if (document.hidden) stop()
  }

  watch(enabled, (allowed) => {
    if (!allowed) stop()
  })
  onMounted(() => {
    window.addEventListener('blur', stop)
    document.addEventListener('visibilitychange', onVisibilityChange)
  })
  onBeforeRouteLeave(() => stop())
  onBeforeUnmount(() => {
    stop()
    window.removeEventListener('blur', stop)
    document.removeEventListener('visibilitychange', onVisibilityChange)
  })

  return { activeDirection, start, stop }
}
