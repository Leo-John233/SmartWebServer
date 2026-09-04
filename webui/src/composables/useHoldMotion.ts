import { onBeforeUnmount, onMounted, ref, watch, type Ref } from 'vue'
import { onBeforeRouteLeave } from 'vue-router'
import type { DeviceAction, GuideDirection } from '../api/deviceActions'

export function useHoldMotion(send: (action: DeviceAction) => Promise<void>, enabled: Ref<boolean>) {
  const activeDirection = ref<GuideDirection | null>(null)
  let queue = Promise.resolve()

  function enqueue(action: DeviceAction): void {
    queue = queue.then(() => send(action)).catch(() => undefined)
  }

  function stopDirection(direction: GuideDirection): void {
    enqueue({ type: 'mount.guide.stop', direction })
    window.setTimeout(() => enqueue({ type: 'mount.guide.stop', direction }), 150)
    window.setTimeout(() => enqueue({ type: 'mount.guide.stop', direction }), 500)
  }

  function stop(): void {
    const direction = activeDirection.value
    activeDirection.value = null
    if (direction) stopDirection(direction)
  }

  function start(direction: GuideDirection): void {
    if (!enabled.value || activeDirection.value === direction) return
    stop()
    activeDirection.value = direction
    enqueue({ type: 'mount.guide.start', direction })
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
