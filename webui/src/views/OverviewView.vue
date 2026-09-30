<script setup lang="ts">
import { computed } from 'vue'
import TelemetryTrace from '../components/TelemetryTrace.vue'
import type { Capabilities, DeviceSnapshot } from '../domain/device'
import { useUiLocale } from '../i18n'

const props = defineProps<{ snapshot: DeviceSnapshot | null; capabilities: Capabilities }>()
const state = computed(() => props.snapshot?.controller ?? null)
const monitorValues = computed(() =>
  Object.entries(props.snapshot?.values ?? {}).filter(([key]) =>
    /^(?:dvr_stat|svo|sg|units|servo|stall|signal|last_err)/.test(key),
  ),
)
const ui = useUiLocale()
const monitorLabels: Record<string, string> = {
  svoD: 'Servo error',
  svoP: 'Servo output (%)',
  units: 'Measurement unit',
  sgV: 'StallGuard value',
  sgT: 'Trigger threshold',
  sgS: 'StallGuard state',
  sgA: 'StallGuard axis',
  sgB: 'Fault duration (ms)',
}
</script>

<template>
  <section>
    <div class="section-heading">
      <div>
        <p class="eyebrow">{{ ui.t('Controller') }}</p>
        <h2>{{ ui.t('Controller overview') }}</h2>
      </div>
      <span class="live-dot">{{ ui.t('Live ·') }} {{ ui.t(state?.activity ?? 'connecting') }}</span>
    </div>
    <div class="metric-grid">
      <article class="metric-card">
        <span>{{ ui.t('Firmware') }}</span>
        <strong>{{ state?.controllerVersion || '—' }}</strong>
        <small>{{ state?.product || 'Waiting for device' }}</small>
      </article>
      <article class="metric-card">
        <span>{{ ui.t('Workload') }}</span>
        <strong>{{ state?.workload || snapshot?.values.work_load || '—' }}</strong>
        <small>{{ ui.t('Controller workload') }}</small>
      </article>
      <article class="metric-card">
        <span>{{ ui.t('Controller temperature') }}</span>
        <strong>{{ snapshot?.values.tphd_m || '—' }}</strong>
        <small>{{ ui.t('Internal MCU sensor') }}</small>
      </article>
      <article class="metric-card">
        <span>{{ ui.t('Axes') }}</span>
        <strong>{{ capabilities.axisCount || '—' }}</strong>
        <small>{{ state?.axisSummary || 'No axis data' }}</small>
      </article>
    </div>
    <article
      v-if="
        ['tphd_t', 'tphd_h', 'tphd_p', 'tphd_d'].some(
          (key) => snapshot?.values[key] && snapshot.values[key] !== '?',
        )
      "
      class="panel environment-panel"
    >
      <h3>{{ ui.t('Observing conditions') }}</h3>
      <dl class="environment-values">
        <div
          v-for="[key, label] in [
            ['tphd_t', 'Ambient temperature'],
            ['tphd_h', 'Relative humidity'],
            ['tphd_p', 'Air pressure'],
            ['tphd_d', 'Dew point'],
          ]"
          :key="key"
        >
          <dt>{{ ui.t(label) }}</dt>
          <dd>{{ snapshot?.values[key] || '—' }}</dd>
        </div>
      </dl>
    </article>
    <div
      class="control-grid"
      v-if="snapshot?.values.svoD !== undefined || snapshot?.values.sgV !== undefined"
    >
      <TelemetryTrace
        v-if="snapshot?.values.svoD !== undefined"
        :label="ui.t('Servo error')"
        :value="snapshot.values.svoD"
        :unit="ui.t(snapshot.values.units || '')"
        :updated-at="snapshot.controller.updatedAt"
      />
      <TelemetryTrace
        v-if="snapshot?.values.sgV !== undefined"
        :label="ui.t('StallGuard')"
        :value="snapshot.values.sgV"
        range="stallguard"
        :threshold="snapshot.values.sgT"
        :updated-at="snapshot.controller.updatedAt"
      />
    </div>
    <article
      v-if="monitorValues.length"
      class="panel monitor-panel"
    >
      <div>
        <p class="eyebrow">{{ ui.t('Telemetry') }}</p>
        <h3>{{ ui.t('Active monitors') }}</h3>
      </div>
      <dl class="compact-list">
        <div
          v-for="[key, value] in monitorValues"
          :key="key"
        >
          <dt>{{ ui.t(monitorLabels[key] || key.replaceAll('_', ' ')) }}</dt>
          <dd>{{ ui.t(value || '—') }}</dd>
        </div>
      </dl>
    </article>
    <article class="panel capability-panel">
      <div>
        <p class="eyebrow">{{ ui.t('Server capability contract') }}</p>
        <h3>{{ ui.t('Detected subsystems') }}</h3>
      </div>
      <div class="chip-row">
        <span v-if="capabilities.mount">{{ ui.t('Mount') }}</span>
        <span v-if="capabilities.rotator">{{ ui.t('Rotator') }}</span>
        <span v-if="capabilities.focuserCount"
          >{{ ui.t('Focusers ×') }} {{ capabilities.focuserCount }}</span
        >
        <span v-if="capabilities.auxiliary">{{ ui.t('Auxiliary') }}</span>
        <span v-if="capabilities.encoders">{{ ui.t('Encoders') }}</span>
        <span v-if="!capabilities.mount && !capabilities.rotator && !capabilities.focuserCount">{{
          ui.t('Read-only')
        }}</span>
      </div>
    </article>
  </section>
</template>
