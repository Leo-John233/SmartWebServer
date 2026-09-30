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
  let motionSequence = 0

  function enqueue(action: DeviceAction): void {
    queue = queue.then(() => send(action)).catch(() => undefined)
  }

  function stopDirection(direction: T): void {
    enqueue(actions.stop(direction))
    // 新的移动开始后，旧操作的延迟停止不能打断它
    const sequence = motionSequence
    for (const delay of [150, 500]) {
      window.setTimeout(() => {
        if (sequence === motionSequence) enqueue(actions.stop(direction))
      }, delay)
    }
  }

  function stop(): void {
    const direction = activeDirection.value
    activeDirection.value = null
    if (direction) stopDirection(direction)
  }

  function start(direction: T): void {
    if (!enabled.value || activeDirection.value === direction) return
    stop()
    motionSequence += 1
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
    window.addEventListener('pointerup', stop)
    window.addEventListener('pointercancel', stop)
    document.addEventListener('visibilitychange', onVisibilityChange)
  })
  onBeforeRouteLeave(() => stop())
  onBeforeUnmount(() => {
    stop()
    window.removeEventListener('blur', stop)
    window.removeEventListener('pointerup', stop)
    window.removeEventListener('pointercancel', stop)
    document.removeEventListener('visibilitychange', onVisibilityChange)
  })

  return { activeDirection, start, stop }
}
