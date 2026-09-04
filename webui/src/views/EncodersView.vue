<script setup lang="ts">
import { computed } from 'vue'
import type { DeviceAction } from '../api/deviceActions'
import type { DeviceSnapshot } from '../domain/device'
import { useUiLocale } from '../i18n'

const props = defineProps<{ snapshot: DeviceSnapshot | null; actionsAllowed: boolean; actionPending: boolean }>()
const emit = defineEmits<{ action: [action: DeviceAction] }>()
const values = computed(() => props.snapshot?.values ?? {})
const selected = computed(() => props.snapshot?.selected ?? {})
const locked = computed(() => !props.actionsAllowed || props.actionPending)
const ui = useUiLocale()
</script>

<template>
  <section>
    <div class="section-heading"><div><p class="eyebrow">{{ ui.t('Encoders') }}</p><h2>{{ ui.t('Axis feedback') }}</h2></div><span class="live-dot">Two-axis comparison</span></div>
    <div class="encoder-grid">
      <article v-for="axis in 2" :key="axis" class="panel hero-value"><span>Axis {{ axis }}</span><strong>{{ values[`enc_a${axis}`] || '—' }}</strong><small>Controller {{ values[`raw_a${axis}`] || '—' }} · Difference {{ values[`diff_a${axis}`] || '—' }}</small></article>
    </div>
    <article class="panel sync-panel">
      <div><p class="eyebrow">Synchronization</p><h3>Choose the trusted position source</h3></div>
      <div class="button-row wrap"><button :disabled="locked" @click="emit('action', { type: 'encoders.sync', direction: 'controllerToEncoder' })">Controller → encoders</button><button :disabled="locked" @click="emit('action', { type: 'encoders.sync', direction: 'encoderToController' })">Encoders → controller</button><button :disabled="locked" @click="emit('action', { type: 'encoders.sync', direction: 'origin' })">Set origin</button></div>
      <div class="segmented compact"><span>Automatic sync</span><button :class="{ selected: selected.enc_as_on }" :disabled="locked" @click="emit('action', { type: 'encoders.autoSync', enabled: true })">On</button><button :class="{ selected: selected.enc_as_off }" :disabled="locked" @click="emit('action', { type: 'encoders.autoSync', enabled: false })">Off</button></div>
    </article>
  </section>
</template>
