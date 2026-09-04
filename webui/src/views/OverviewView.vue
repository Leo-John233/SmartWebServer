<script setup lang="ts">
import { computed } from 'vue'
import type { Capabilities, DeviceSnapshot } from '../domain/device'

const props = defineProps<{ snapshot: DeviceSnapshot | null; capabilities: Capabilities }>()
const state = computed(() => props.snapshot?.controller ?? null)
const monitorValues = computed(() => Object.entries(props.snapshot?.values ?? {}).filter(([key]) =>
  /^(?:dvr_stat|servo|stall|tphd_|signal|last_err)/.test(key)))
</script>

<template>
  <section>
    <div class="section-heading">
      <div>
        <p class="eyebrow">Controller</p>
        <h2>Controller overview</h2>
      </div>
      <span class="live-dot">Live · {{ state?.activity ?? 'connecting' }}</span>
    </div>
    <div class="metric-grid">
      <article class="metric-card">
        <span>Firmware</span>
        <strong>{{ state?.firmwareVersion || '—' }}</strong>
        <small>{{ state?.product || 'Waiting for device' }}</small>
      </article>
      <article class="metric-card">
        <span>Workload</span>
        <strong>{{ state?.workload || snapshot?.values.work_load || '—' }}</strong>
        <small>ESP web workload</small>
      </article>
      <article class="metric-card">
        <span>Weather</span>
        <strong>{{ state?.weather || snapshot?.values.tphd_t || '—' }}</strong>
        <small>Ambient telemetry</small>
      </article>
      <article class="metric-card">
        <span>Axes</span>
        <strong>{{ capabilities.axisCount || '—' }}</strong>
        <small>{{ state?.axisSummary || 'No axis data' }}</small>
      </article>
    </div>
    <article v-if="monitorValues.length" class="panel monitor-panel">
      <div>
        <p class="eyebrow">Telemetry</p>
        <h3>Active monitors</h3>
      </div>
      <dl class="compact-list">
        <div v-for="([key, value]) in monitorValues" :key="key"><dt>{{ key.replaceAll('_', ' ') }}</dt><dd>{{ value || '—' }}</dd></div>
      </dl>
    </article>
    <article class="panel capability-panel">
      <div>
        <p class="eyebrow">Server capability contract</p>
        <h3>Detected subsystems</h3>
      </div>
      <div class="chip-row">
        <span v-if="capabilities.mount">Mount</span>
        <span v-if="capabilities.rotator">Rotator</span>
        <span v-if="capabilities.focuserCount">Focusers × {{ capabilities.focuserCount }}</span>
        <span v-if="capabilities.auxiliary">Auxiliary</span>
        <span v-if="capabilities.encoders">Encoders</span>
        <span v-if="!capabilities.mount && !capabilities.rotator && !capabilities.focuserCount">Read-only</span>
      </div>
    </article>
  </section>
</template>
