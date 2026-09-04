<script setup lang="ts">
import { onMounted } from 'vue'
import { RouterLink, RouterView } from 'vue-router'
import StatusPill from './components/StatusPill.vue'
import { useDeviceSession } from './composables/useDeviceSession'
import type { ScenarioId } from './mocks/scenarios'
import { useUiLocale } from './i18n'

const session = useDeviceSession()
const mockMode = import.meta.env.MODE === 'mock'
const ui = useUiLocale()

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
        <span><strong>Smart Web Server</strong><small>{{ ui.t('Observatory console') }}</small></span>
      </RouterLink>
      <div class="topbar-status">
        <label v-if="mockMode" class="scenario-select">
          <span>{{ ui.t('Scenario') }}</span>
          <select :value="session.activeScenario.value" @change="changeScenario">
            <option v-for="scenario in session.scenarios" :key="scenario.id" :value="scenario.id">{{ scenario.label }}</option>
          </select>
        </label>
        <StatusPill
          :connection="session.snapshot.value?.controller.connection ?? 'connecting'"
          :activity="session.snapshot.value?.controller.activity ?? 'idle'"
        />
        <button class="locale-button" type="button" @click="ui.toggleLocale">{{ ui.locale.value === 'zh' ? 'EN' : '中文' }}</button>
      </div>
    </header>

    <div class="workspace">
      <aside class="sidebar">
        <nav>
          <RouterLink to="/">{{ ui.t('Controller') }}</RouterLink>
          <RouterLink v-if="session.config.value.capabilities.mount" to="/mount">{{ ui.t('Mount') }}</RouterLink>
          <RouterLink v-if="session.config.value.capabilities.rotator" to="/rotator">{{ ui.t('Rotator') }}</RouterLink>
          <RouterLink v-if="session.config.value.capabilities.focuserCount" to="/focuser">{{ ui.t('Focuser') }}</RouterLink>
          <RouterLink v-if="session.config.value.capabilities.auxiliary" to="/auxiliary">{{ ui.t('Auxiliary') }}</RouterLink>
          <RouterLink v-if="session.config.value.capabilities.encoders" to="/encoders">{{ ui.t('Encoders') }}</RouterLink>
          <RouterLink to="/network">{{ ui.t('Network') }}</RouterLink>
        </nav>
        <div class="controller-meta">
          <span>{{ session.config.value.controller.protocolFamily }}</span>
          <strong>{{ session.config.value.controller.version || 'Unknown controller' }}</strong>
          <a class="legacy-sidebar-link" href="/index.htm">{{ ui.t('Legacy UI') }} ↗</a>
        </div>
      </aside>

      <main>
        <div v-if="session.error.value" class="error-banner" role="alert">{{ ui.t(session.error.value) }}</div>
        <div v-else-if="!session.stateFresh.value" class="warning-banner" role="status">{{ ui.t('State is stale · motion controls are locked') }}</div>
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
