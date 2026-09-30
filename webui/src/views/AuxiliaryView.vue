<script setup lang="ts">
import { computed, onMounted, ref } from 'vue'
import type { DeviceAction } from '../api/deviceActions'
import type { DeviceSnapshot } from '../domain/device'
import { parseAuxiliaryControls, type AuxiliaryFeature } from '../protocol/auxiliaryControls'
import { useUiLocale } from '../i18n'
const props = defineProps<{
  snapshot: DeviceSnapshot | null
  actionsAllowed: boolean
  actionPending: boolean
}>()
const emit = defineEmits<{ action: [action: DeviceAction] }>()
const ui = useUiLocale()
const features = ref<AuxiliaryFeature[]>([])
const error = ref('')
const locked = computed(() => !props.actionsAllowed || props.actionPending)
async function load(): Promise<void> {
  try {
    const response = await fetch('/auxiliary.htm')
    if (!response.ok) throw new Error()
    features.value = parseAuxiliaryControls(await response.text())
    error.value = ''
  } catch {
    error.value = 'Unable to load device settings'
  }
}
function setValue(feature: number, value: 1 | 2 | 3 | 4, setting: number): void {
  emit('action', { type: 'auxiliary.value', feature, value, setting })
}
onMounted(load)
</script>
<template>
  <section>
    <div class="section-heading">
      <div>
        <p class="eyebrow">{{ ui.t('Auxiliary') }}</p>
        <h2>{{ ui.t('Power & accessories') }}</h2>
      </div>
      <button @click="load">{{ ui.t('Refresh') }}</button>
    </div>
    <p
      v-if="error"
      role="alert"
    >
      {{ ui.t(error) }}
    </p>
    <div class="feature-grid">
      <article
        v-for="feature in features"
        :key="feature.id"
        class="panel feature-card"
      >
        <div>
          <p class="eyebrow">{{ ui.t('Channel') }} {{ feature.id }}</p>
          <h3>{{ ui.t(feature.name.replace(/:$/, '')) }}</h3>
        </div>
        <div class="button-row">
          <button
            v-for="button in feature.switches"
            :key="button.value"
            :disabled="locked"
            :class="{
              selected: snapshot?.selected['sw' + feature.id + (button.value ? '_on' : '_off')],
            }"
            @click="setValue(feature.id, 1, button.value)"
          >
            {{ ui.t(button.label) }}
          </button>
        </div>
        <label
          v-for="control in feature.controls"
          :key="control.key"
          class="range-field"
          ><span
            >{{ ui.t(control.label) }} · {{ snapshot?.values[control.key] ?? control.value }}</span
          ><input
            v-model.number="control.value"
            type="range"
            :min="control.min"
            :max="control.max"
            :disabled="locked"
            @change="
              setValue(feature.id, Number(control.key.slice(-1)) as 1 | 2 | 3 | 4, control.value)
            "
        /></label>
        <dl class="compact-list">
          <div
            v-for="key in ['vout' + feature.id, 'iout' + feature.id, 'x' + feature.id + 'v4']"
            :key="key"
          >
            <template v-if="snapshot?.values[key]"
              ><dt>
                {{ key.startsWith('vout') ? 'V' : key.startsWith('iout') ? 'A' : ui.t('Feedback') }}
              </dt>
              <dd>{{ snapshot.values[key] }}</dd></template
            >
          </div>
        </dl>
      </article>
    </div>
    <p v-if="!features.length && !error">{{ ui.t('No active auxiliary features') }}</p>
  </section>
</template>
