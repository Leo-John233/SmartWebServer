<script setup lang="ts">
import { computed, ref } from 'vue'
import type { DeviceAction } from '../api/deviceActions'
import type { DeviceSnapshot } from '../domain/device'

const props = defineProps<{ snapshot: DeviceSnapshot | null; actionsAllowed: boolean; actionPending: boolean }>()
const emit = defineEmits<{ action: [action: DeviceAction] }>()
const values = computed(() => props.snapshot?.values ?? {})
const selected = computed(() => props.snapshot?.selected ?? {})
const locked = computed(() => !props.actionsAllowed || props.actionPending)
const settings = ref(Array.from({ length: 8 }, () => 0))

const activeFeatures = computed(() => Array.from({ length: 8 }, (_, index) => index + 1).filter((feature) =>
  Object.keys(values.value).some((key) => key.startsWith(`x${feature}v`) || key === `vout${feature}` || key === `iout${feature}`)
  || Object.keys(selected.value).some((key) => key.startsWith(`sw${feature}_`))))

function setFeature(feature: number, setting: number): void {
  settings.value[feature - 1] = setting
  emit('action', { type: 'auxiliary.value', feature, value: 1, setting })
}
</script>

<template>
  <section>
    <div class="section-heading"><div><p class="eyebrow">Auxiliary</p><h2>Power &amp; accessories</h2></div><span class="live-dot">{{ activeFeatures.length }} active channels</span></div>
    <div v-if="activeFeatures.length" class="feature-grid">
      <article v-for="feature in activeFeatures" :key="feature" class="panel feature-card">
        <div class="panel-title"><div><p class="eyebrow">Channel {{ feature }}</p><h3>Feature {{ feature }}</h3></div><span class="status-text">{{ values[`vout${feature}`] ? `${values[`vout${feature}`]} V` : '' }} {{ values[`iout${feature}`] ? `· ${values[`iout${feature}`]} A` : '' }}</span></div>
        <div class="segmented compact"><button :class="{ selected: selected[`sw${feature}_on`] }" :disabled="locked" @click="setFeature(feature, 255)">On</button><button :class="{ selected: selected[`sw${feature}_off`] }" :disabled="locked" @click="setFeature(feature, 0)">Off</button></div>
        <label class="range-field"><span>Output {{ values[`x${feature}v1`] || settings[feature - 1] }}</span><input v-model.number="settings[feature - 1]" :disabled="locked" type="range" min="0" max="255" @change="setFeature(feature, settings[feature - 1])"></label>
        <dl class="compact-list"><div v-for="value in [2, 3, 4]" :key="value"><dt>Value {{ value }}</dt><dd>{{ values[`x${feature}v${value}`] || '—' }}</dd></div></dl>
      </article>
    </div>
    <article v-else class="panel empty-state"><h3>No active auxiliary features</h3><p>The controller did not report any configured channels</p></article>
  </section>
</template>
