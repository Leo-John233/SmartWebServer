<script setup lang="ts">
import { onMounted } from 'vue'
import { RouterLink, RouterView } from 'vue-router'
import StatusPill from './components/StatusPill.vue'
import { useDeviceSession } from './composables/useDeviceSession'
import type { ScenarioId } from './mocks/scenarios'

const session = useDeviceSession()
const mockMode = import.meta.env.MODE === 'mock'

onMounted(() => {
  void session.start()
})

function changeScenario(event: Event): void {
  void session.selectScenario((event.target as HTMLSelectElement).value as ScenarioId)
}
</script>

<template>
  <div class="app-shell">
    <header class="topbar">
      <RouterLink class="brand" to="/">
        <span class="brand-mark">S</span>
        <span><strong>Smart Web Server</strong><small>Observatory console</small></span>
      </RouterLink>
      <div class="topbar-status">
        <label v-if="mockMode" class="scenario-select">
          <span>Scenario</span>
          <select :value="session.activeScenario.value" @change="changeScenario">
            <option v-for="scenario in session.scenarios" :key="scenario.id" :value="scenario.id">{{ scenario.label }}</option>
          </select>
        </label>
        <StatusPill
          :connection="session.snapshot.value?.controller.connection ?? 'connecting'"
          :activity="session.snapshot.value?.controller.activity ?? 'idle'"
        />
      </div>
    </header>

    <div class="workspace">
      <aside class="sidebar">
        <nav>
          <RouterLink to="/">Controller</RouterLink>
          <RouterLink v-if="session.config.value.capabilities.mount" to="/mount">Mount</RouterLink>
          <span v-if="session.config.value.capabilities.rotator">Rotator · soon</span>
          <span v-if="session.config.value.capabilities.focuserCount">Focuser · soon</span>
          <span v-if="session.config.value.capabilities.auxiliary">Auxiliary · soon</span>
          <span v-if="session.config.value.capabilities.encoders">Encoders · soon</span>
        </nav>
        <div class="controller-meta">
          <span>{{ session.config.value.controller.protocolFamily }}</span>
          <strong>{{ session.config.value.controller.version || 'Unknown controller' }}</strong>
        </div>
      </aside>

      <main>
        <div v-if="session.error.value" class="error-banner" role="alert">{{ session.error.value }}</div>
        <div v-else-if="!session.stateFresh.value" class="warning-banner" role="status">State is stale · motion controls are locked</div>
        <div v-if="session.loading.value" class="loading-card">Connecting to controller…</div>
        <RouterView v-else v-slot="{ Component }">
          <component
            :is="Component"
            :snapshot="session.snapshot.value"
            :capabilities="session.config.value.capabilities"
            :actions-allowed="session.actionsAllowed.value"
            :action-pending="session.actionPending.value"
            @action="session.performAction"
            @safety-action="session.performSafetyAction"
          />
        </RouterView>
      </main>
    </div>
  </div>
</template>
