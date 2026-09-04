<script setup lang="ts">
import { computed, ref, toRef } from 'vue'
import type { DeviceAction, SlewRate } from '../api/deviceActions'
import { useHoldMotion } from '../composables/useHoldMotion'
import type { DeviceSnapshot } from '../domain/device'

type Direction = 'counterclockwise' | 'clockwise'
const props = defineProps<{ snapshot: DeviceSnapshot | null; actionsAllowed: boolean; actionPending: boolean }>()
const emit = defineEmits<{ action: [action: DeviceAction]; safetyAction: [action: DeviceAction] }>()
const values = computed(() => props.snapshot?.values ?? {})
const locked = computed(() => !props.actionsAllowed || props.actionPending)
const position = ref(0)
const backlash = ref(0)

function sendMotion(action: DeviceAction): Promise<void> {
  if (action.type === 'rotator.stop') emit('safetyAction', action)
  else emit('action', action)
  return Promise.resolve()
}

const hold = useHoldMotion<Direction>(sendMotion, toRef(props, 'actionsAllowed'), {
  start: (direction) => ({ type: 'rotator.move', direction, fast: false }),
  stop: () => ({ type: 'rotator.stop' }),
})

function setRate(rate: SlewRate): void {
  emit('action', { type: 'rotator.rate', rate })
}
</script>

<template>
  <section>
    <div class="section-heading"><div><p class="eyebrow">Rotator</p><h2>Camera orientation</h2></div><button class="danger-button" :disabled="!actionsAllowed" @click="emit('safetyAction', { type: 'rotator.stop' })">Stop</button></div>
    <div class="subsystem-hero">
      <article class="panel hero-value"><span>Position</span><strong>{{ values.rotatorpos || '—' }}</strong><small>{{ values.rot_sta || 'Unavailable' }}</small></article>
      <article class="panel control-body always-visible">
        <div class="dual-hold"><button :disabled="locked" @pointerdown.prevent="hold.start('counterclockwise')" @pointerup="hold.stop" @pointercancel="hold.stop" @pointerleave="hold.stop">↺ Counterclockwise</button><button :disabled="locked" @pointerdown.prevent="hold.start('clockwise')" @pointerup="hold.stop" @pointercancel="hold.stop" @pointerleave="hold.stop">Clockwise ↻</button></div>
        <label class="field"><span>Go to angle</span><div class="inline-field"><input v-model.number="position" type="number" min="-360" max="360" step="0.1"><button :disabled="locked" @click="emit('action', { type: 'rotator.position', degrees: position })">Go</button></div></label>
      </article>
    </div>
    <div class="control-grid">
      <article class="panel control-body always-visible"><h3>Home</h3><div class="button-row"><button :disabled="locked" @click="emit('action', { type: 'rotator.home', mode: 'find' })">Find home</button><button :disabled="locked" @click="emit('action', { type: 'rotator.home', mode: 'reset' })">Reset home</button></div></article>
      <article class="panel control-body always-visible"><h3>Goto rate</h3><div class="button-row wrap"><button v-for="rate in (['verySlow', 'slow', 'normal', 'fast', 'veryFast'] as SlewRate[])" :key="rate" :disabled="locked" @click="setRate(rate)">{{ rate }}</button></div></article>
      <article class="panel control-body always-visible"><h3>Field derotation</h3><div class="button-row wrap"><button :disabled="locked" @click="emit('action', { type: 'rotator.derotate', command: 'on' })">On</button><button :disabled="locked" @click="emit('action', { type: 'rotator.derotate', command: 'off' })">Off</button><button :disabled="locked" @click="emit('action', { type: 'rotator.derotate', command: 'reverse' })">Reverse</button><button :disabled="locked" @click="emit('action', { type: 'rotator.derotate', command: 'parallactic' })">Parallactic</button></div></article>
      <article class="panel control-body always-visible"><h3>Backlash</h3><label class="field"><span>Steps</span><div class="inline-field"><input v-model.number="backlash" type="number" min="0" max="32767"><button :disabled="locked" @click="emit('action', { type: 'rotator.backlash', steps: backlash })">Save</button></div></label></article>
    </div>
  </section>
</template>
