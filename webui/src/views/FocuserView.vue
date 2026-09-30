<script setup lang="ts">
import { computed, ref, toRef } from 'vue'
import type { DeviceAction, SlewRate } from '../api/deviceActions'
import { useHoldMotion } from '../composables/useHoldMotion'
import type { Capabilities, DeviceSnapshot } from '../domain/device'
import { useUiLocale } from '../i18n'

type Direction = 'in' | 'out'
const props = defineProps<{
  snapshot: DeviceSnapshot | null
  capabilities: Capabilities
  actionsAllowed: boolean
  actionPending: boolean
  sendMotion: (action: DeviceAction) => Promise<void>
}>()
const emit = defineEmits<{ action: [action: DeviceAction]; safetyAction: [action: DeviceAction] }>()
const values = computed(() => props.snapshot?.values ?? {})
const locked = computed(() => !props.actionsAllowed || props.actionPending)
const position = ref(0)
const fastMotion = ref(false)
const settings = ref({ backlash: 0, compensationEnabled: false, deadband: 1, coefficient: 0 })
const ui = useUiLocale()

function sendMotion(action: DeviceAction): Promise<void> {
  return props.sendMotion(action)
}

const hold = useHoldMotion<Direction>(sendMotion, toRef(props, 'actionsAllowed'), {
  start: (direction) => ({ type: 'focuser.move', direction, fast: fastMotion.value }),
  stop: () => ({ type: 'focuser.stop' }),
})
</script>

<template>
  <section>
    <div class="section-heading">
      <div>
        <p class="eyebrow">{{ ui.t('Focuser') }}</p>
        <h2>{{ ui.t('Focus control') }}</h2>
      </div>
      <button
        class="danger-button"
        :disabled="!actionsAllowed"
        @click="emit('safetyAction', { type: 'focuser.stop' })"
      >
        {{ ui.t('Stop') }}
      </button>
    </div>
    <div class="subsystem-hero">
      <article class="panel hero-value">
        <span>{{ ui.t('Position') }}</span
        ><strong>{{ values.focuserpos || '—' }}</strong
        ><small>{{ values.foc_sta || values.foc_sel || 'Unavailable' }}</small>
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
            @pointerdown.prevent="hold.start('in')"
            @pointerup="hold.stop"
            @pointercancel="hold.stop"
            @pointerleave="hold.stop"
          >
            {{ ui.t('Move in') }}</button
          ><button
            :disabled="locked"
            @pointerdown.prevent="hold.start('out')"
            @pointerup="hold.stop"
            @pointercancel="hold.stop"
            @pointerleave="hold.stop"
          >
            {{ ui.t('Move out') }}
          </button>
        </div>
        <label class="field"
          ><span>{{ ui.t('Go to position') }}</span>
          <div class="inline-field">
            <input
              v-model.number="position"
              type="number"
              min="-500000"
              max="500000"
            /><button
              :disabled="locked"
              @click="emit('action', { type: 'focuser.position', steps: position })"
            >
              {{ ui.t('Go') }}
            </button>
          </div></label
        >
      </article>
    </div>
    <div class="control-grid">
      <article class="panel control-body always-visible">
        <h3>{{ ui.t('Active focuser') }}</h3>
        <div class="button-row wrap">
          <button
            v-for="index in capabilities.focuserCount"
            :key="index"
            :disabled="locked"
            @click="emit('action', { type: 'focuser.select', index })"
          >
            {{ index }}
          </button>
        </div>
      </article>
      <article class="panel control-body always-visible">
        <h3>{{ ui.t('Home & rate') }}</h3>
        <div class="button-row">
          <button
            :disabled="locked"
            @click="emit('action', { type: 'focuser.home', mode: 'find' })"
          >
            {{ ui.t('Find home') }}</button
          ><button
            :disabled="locked"
            @click="emit('action', { type: 'focuser.home', mode: 'reset' })"
          >
            {{ ui.t('Reset home') }}
          </button>
        </div>
        <div class="button-row wrap">
          <button
            v-for="rate in ['verySlow', 'slow', 'normal', 'fast', 'veryFast'] as SlewRate[]"
            :key="rate"
            :disabled="locked"
            @click="emit('action', { type: 'focuser.rate', rate })"
          >
            {{ ui.t(rate) }}
          </button>
        </div>
      </article>
      <article class="panel control-body always-visible wide-card">
        <h3>{{ ui.t('Temperature compensation') }}</h3>
        <p class="muted-text">
          {{ values.f_temp || 'Temperature unavailable' }} · {{ values.f_bl || 'No backlash data' }}
        </p>
        <div class="form-grid">
          <label class="field"
            ><span>{{ ui.t('Backlash') }}</span
            ><input
              v-model.number="settings.backlash"
              type="number"
              min="0"
              max="32767" /></label
          ><label class="field"
            ><span>{{ ui.t('Deadband') }}</span
            ><input
              v-model.number="settings.deadband"
              type="number"
              min="1"
              max="32767" /></label
          ><label class="field"
            ><span>{{ ui.t('Coefficient') }}</span
            ><input
              v-model.number="settings.coefficient"
              type="number"
              min="-999"
              max="999"
              step="0.1"
          /></label>
        </div>
        <label class="check-field"
          ><input
            v-model="settings.compensationEnabled"
            type="checkbox"
          />
          {{ ui.t('Enable compensation') }}</label
        ><button
          :disabled="locked"
          @click="emit('action', { type: 'focuser.settings', ...settings })"
        >
          {{ ui.t('Save settings') }}
        </button>
      </article>
    </div>
  </section>
</template>
