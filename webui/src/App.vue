<script setup lang="ts">
import { computed, onMounted, ref, watchEffect } from 'vue'
import { RouterLink, RouterView } from 'vue-router'
import UiIcon from './components/UiIcon.vue'
import StatusPill from './components/StatusPill.vue'
import { useDeviceSession } from './composables/useDeviceSession'
import type { ScenarioId } from './mocks/scenarios'
import { useUiLocale } from './i18n'

import { controllerIdentity } from './domain/controllerIdentity'

const session = useDeviceSession()
const identity = computed(() => controllerIdentity(session.config.value.controller))
const mockMode = import.meta.env.MODE === 'mock'
const ui = useUiLocale()
const nightMode = ref(localStorage.getItem('smart-webui-night') === 'true')
watchEffect(() => {
  document.documentElement.dataset.theme = nightMode.value ? 'night' : 'dark'
  localStorage.setItem('smart-webui-night', String(nightMode.value))
})

watchEffect(() => {
  document.title = [identity.value.name || ui.t('Controller'), identity.value.version]
    .filter(Boolean)
    .join(' · ')
})

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
      <RouterLink
        class="brand"
        to="/"
      >
        <span class="brand-mark"><UiIcon name="telescope" /></span>
        <span
          ><strong>{{ identity.name || ui.t('Controller') }}</strong
          ><small>{{ identity.version || ui.t('Waiting for identification') }}</small></span
        >
      </RouterLink>
      <div class="topbar-status">
        <label
          v-if="mockMode"
          class="scenario-select"
        >
          <span>{{ ui.t('Scenario') }}</span>
          <select
            :value="session.activeScenario.value"
            @change="changeScenario"
          >
            <option
              v-for="scenario in session.scenarios"
              :key="scenario.id"
              :value="scenario.id"
            >
              {{ ui.t(scenario.label) }}
            </option>
          </select>
        </label>
        <StatusPill
          :connection="session.snapshot.value?.controller.connection ?? 'connecting'"
          :activity="session.snapshot.value?.controller.activity ?? 'idle'"
        />
        <button
          class="locale-button"
          type="button"
          :aria-pressed="nightMode"
          @click="nightMode = !nightMode"
        >
          {{ ui.t(nightMode ? 'Dark mode' : 'Night vision') }}
        </button>
        <button
          class="locale-button"
          type="button"
          @click="ui.toggleLocale"
        >
          {{ ui.locale.value === 'zh' ? 'EN' : '中文' }}
        </button>
      </div>
    </header>

    <div class="workspace">
      <aside class="sidebar">
        <nav>
          <RouterLink to="/overview"
            ><UiIcon name="telescope" /><span class="nav-label">{{
              ui.t('Device')
            }}</span></RouterLink
          >
          <RouterLink
            v-if="session.config.value.capabilities.mount"
            to="/mount"
            ><UiIcon name="control" /><span class="nav-label">{{
              ui.t('Control')
            }}</span></RouterLink
          >
          <RouterLink
            v-if="session.config.value.capabilities.rotator"
            to="/rotator"
            ><UiIcon name="rotate" /><span class="nav-label">{{
              ui.t('Rotator')
            }}</span></RouterLink
          >
          <RouterLink
            v-if="session.config.value.capabilities.focuserCount"
            to="/focuser"
            ><UiIcon name="focus" /><span class="nav-label">{{ ui.t('Focuser') }}</span></RouterLink
          >
          <RouterLink
            v-if="session.config.value.capabilities.auxiliary"
            to="/auxiliary"
            ><UiIcon name="power" /><span class="nav-label">{{
              ui.t('Auxiliary')
            }}</span></RouterLink
          >
          <RouterLink
            v-if="session.config.value.capabilities.encoders"
            to="/encoders"
            ><UiIcon name="encoder" /><span class="nav-label">{{
              ui.t('Encoders')
            }}</span></RouterLink
          >
          <RouterLink to="/settings"
            ><UiIcon name="settings" /><span class="nav-label">{{
              ui.t('Settings')
            }}</span></RouterLink
          >
          <RouterLink to="/network"
            ><UiIcon name="network" /><span class="nav-label">{{
              ui.t('Network')
            }}</span></RouterLink
          >
        </nav>
        <div class="controller-meta">
          <span>{{ identity.name || ui.t('Controller') }}</span>
          <strong>{{ identity.version || ui.t('Waiting for identification') }}</strong>
          <a
            class="legacy-sidebar-link"
            href="/index.htm"
            >{{ ui.t('Legacy UI') }} ↗</a
          >
        </div>
      </aside>

      <main>
        <div
          v-if="mockMode"
          class="simulation-banner"
          role="status"
        >
          ● {{ ui.t('Simulation · no hardware connected') }}
        </div>
        <div
          v-if="session.error.value"
          class="error-banner"
          role="alert"
        >
          {{ ui.t(session.error.value) }}
        </div>
        <div
          v-else-if="!session.stateFresh.value"
          class="warning-banner"
          role="status"
        >
          {{ ui.t('State is stale · motion controls are locked') }}
        </div>
        <div
          v-if="session.loading.value"
          class="loading-card"
        >
          {{ ui.t('Connecting to controller…') }}
        </div>
        <RouterView
          v-else
          v-slot="{ Component }"
        >
          <component
            :is="Component"
            :snapshot="session.snapshot.value"
            :capabilities="session.config.value.capabilities"
            :actions-allowed="session.actionsAllowed.value"
            :action-pending="session.actionPending.value"
            :send-motion="session.performMotion"
            @action="session.performAction"
            @safety-action="session.performSafetyAction"
          />
        </RouterView>
      </main>
    </div>
  </div>
</template>
