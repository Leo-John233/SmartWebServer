<script setup lang="ts">
import type { Capabilities, ControllerState } from '../domain/device'

defineProps<{ state: ControllerState | null; capabilities: Capabilities }>()
</script>

<template>
  <section>
    <div class="section-heading">
      <div>
        <p class="eyebrow">Controller</p>
        <h2>System overview</h2>
      </div>
      <span class="live-dot">Live protocol preview</span>
    </div>
    <div class="metric-grid">
      <article class="metric-card">
        <span>Firmware</span>
        <strong>{{ state?.firmwareVersion || '—' }}</strong>
        <small>{{ state?.product || 'Waiting for device' }}</small>
      </article>
      <article class="metric-card">
        <span>Workload</span>
        <strong>{{ state?.workload || '—' }}</strong>
        <small>ESP web workload</small>
      </article>
      <article class="metric-card">
        <span>Weather</span>
        <strong>{{ state?.weather || '—' }}</strong>
        <small>Ambient telemetry</small>
      </article>
      <article class="metric-card">
        <span>Axes</span>
        <strong>{{ capabilities.axisCount || '—' }}</strong>
        <small>{{ state?.axisSummary || 'No axis data' }}</small>
      </article>
    </div>
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
