<script setup lang="ts">
import { computed, ref, toRef } from 'vue'
import type { DeviceAction, SlewRate } from '../api/deviceActions'
import { useHoldMotion } from '../composables/useHoldMotion'
import type { DeviceSnapshot } from '../domain/device'
import { useUiLocale } from '../i18n'

type Direction = 'counterclockwise' | 'clockwise'
const props = defineProps<{
  snapshot: DeviceSnapshot | null
  actionsAllowed: boolean
  actionPending: boolean
  sendMotion: (action: DeviceAction) => Promise<void>
}>()
const emit = defineEmits<{ action: [action: DeviceAction]; safetyAction: [action: DeviceAction] }>()
const values = computed(() => props.snapshot?.values ?? {})
const locked = computed(() => !props.actionsAllowed || props.actionPending)
const position = ref(0)
const fastMotion = ref(false)
const backlash = ref(0)
const ui = useUiLocale()

function sendMotion(action: DeviceAction): Promise<void> {
  return props.sendMotion(action)
}

const hold = useHoldMotion<Direction>(sendMotion, toRef(props, 'actionsAllowed'), {
  start: (direction) => ({ type: 'rotator.move', direction, fast: fastMotion.value }),
  stop: () => ({ type: 'rotator.stop' }),
})

function setRate(rate: SlewRate): void {
  emit('action', { type: 'rotator.rate', rate })
}
</script>

<template>
  <section>
    <div class="section-heading">
      <div>
        <p class="eyebrow">{{ ui.t('Rotator') }}</p>
        <h2>{{ ui.t('Camera orientation') }}</h2>
      </div>
      <button
        class="danger-button"
        :disabled="!actionsAllowed"
        @click="emit('safetyAction', { type: 'rotator.stop' })"
      >
        {{ ui.t('Stop') }}
      </button>
    </div>
    <div class="subsystem-hero">
      <article class="panel hero-value">
        <span>{{ ui.t('Position') }}</span
        ><strong>{{ values.rotatorpos || '—' }}</strong
        ><small>{{ values.rot_sta || 'Unavailable' }}</small>
      </article>
      <article class="panel control-body always-visible">
        <label class="check-field"
          ><input
            v-model="fastMotion"
            type="checkbox"
          />{{ ui.t('Fast manual motion') }}</label
        >
        <div class="dual-hold">
          <button
            :disabled="locked"
            @pointerdown.prevent="hold.start('counterclockwise')"
            @pointerup="hold.stop"
            @pointercancel="hold.stop"
            @pointerleave="hold.stop"
          >
            {{ ui.t('↺ Counterclockwise') }}</button
          ><button
            :disabled="locked"
            @pointerdown.prevent="hold.start('clockwise')"
            @pointerup="hold.stop"
            @pointercancel="hold.stop"
            @pointerleave="hold.stop"
          >
            {{ ui.t('Clockwise ↻') }}
          </button>
        </div>
        <label class="field"
          ><span>{{ ui.t('Go to angle') }}</span>
          <div class="inline-field">
            <input
              v-model.number="position"
              type="number"
              min="-360"
              max="360"
              step="0.1"
            /><button
              :disabled="locked"
              @click="emit('action', { type: 'rotator.position', degrees: position })"
            >
              {{ ui.t('Go') }}
            </button>
          </div></label
        >
      </article>
    </div>
    <div class="control-grid">
      <article class="panel control-body always-visible">
        <h3>{{ ui.t('Home') }}</h3>
        <div class="button-row">
          <button
            :disabled="locked"
            @click="emit('action', { type: 'rotator.home', mode: 'find' })"
          >
            {{ ui.t('Find home') }}</button
          ><button
            :disabled="locked"
            @click="emit('action', { type: 'rotator.home', mode: 'reset' })"
          >
            {{ ui.t('Reset home') }}
          </button>
        </div>
      </article>
      <article class="panel control-body always-visible">
        <h3>{{ ui.t('Goto rate') }}</h3>
        <div class="button-row wrap">
          <button
            v-for="rate in ['verySlow', 'slow', 'normal', 'fast', 'veryFast'] as SlewRate[]"
            :key="rate"
            :disabled="locked"
            @click="setRate(rate)"
          >
            {{ ui.t(rate) }}
          </button>
        </div>
      </article>
      <article class="panel control-body always-visible">
        <h3>{{ ui.t('Field derotation') }}</h3>
        <div class="button-row wrap">
          <button
            :disabled="locked"
            @click="emit('action', { type: 'rotator.derotate', command: 'on' })"
          >
            {{ ui.t('On') }}</button
          ><button
            :disabled="locked"
            @click="emit('action', { type: 'rotator.derotate', command: 'off' })"
          >
            {{ ui.t('Off') }}</button
          ><button
            :disabled="locked"
            @click="emit('action', { type: 'rotator.derotate', command: 'reverse' })"
          >
            {{ ui.t('Reverse') }}</button
          ><button
            :disabled="locked"
            @click="emit('action', { type: 'rotator.derotate', command: 'parallactic' })"
          >
            {{ ui.t('Parallactic') }}
          </button>
        </div>
      </article>
      <article class="panel control-body always-visible">
        <h3>{{ ui.t('Backlash') }}</h3>
        <label class="field"
          ><span>{{ ui.t('Steps') }}</span>
          <div class="inline-field">
            <input
              v-model.number="backlash"
              type="number"
              min="0"
              max="32767"
            /><button
              :disabled="locked"
              @click="emit('action', { type: 'rotator.backlash', steps: backlash })"
            >
              {{ ui.t('Save') }}
            </button>
          </div></label
        >
      </article>
    </div>
  </section>
</template>
