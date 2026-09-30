<script setup lang="ts">
import { onMounted, ref } from 'vue'
import { LegacyClient } from '../api/legacyClient'
import type { NetworkConfig } from '../domain/device'
import { useUiLocale } from '../i18n'

const client = new LegacyClient()
const network = ref<NetworkConfig | null>(null)
const error = ref('')
const password = ref('')
const stationPassword = ref('')
const accessPointPassword = ref('')
const saving = ref(false)
const ui = useUiLocale()

async function load(): Promise<void> {
  try {
    network.value = await client.getNetwork()
    error.value = ''
  } catch {
    error.value = 'Network settings are unavailable'
  }
}

async function submit(values: Record<string, string | number | boolean>): Promise<void> {
  saving.value = true
  try {
    await client.updateNetwork(values)
    await load()
  } catch {
    error.value = 'Network action failed'
  } finally {
    saving.value = false
  }
}

async function login(): Promise<void> {
  await submit({ action: 'login', password: password.value })
  password.value = ''
}

async function save(): Promise<void> {
  if (!network.value) return
  await submit({
    action: 'save',
    stationEnabled: network.value.station.enabled,
    stationSsid: network.value.station.ssid,
    stationPassword: stationPassword.value,
    stationDhcp: network.value.station.dhcp,
    accessPointEnabled: network.value.accessPoint.enabled,
    accessPointSsid: network.value.accessPoint.ssid,
    accessPointPassword: accessPointPassword.value,
    accessPointChannel: network.value.accessPoint.channel,
  })
  stationPassword.value = ''
  accessPointPassword.value = ''
}

function reboot(): void {
  if (window.confirm(ui.t('Restart the web server now?'))) void submit({ action: 'reboot' })
}

onMounted(() => void load())
</script>

<template>
  <section>
    <div class="section-heading">
      <div>
        <p class="eyebrow">{{ ui.t('Network') }}</p>
        <h2>{{ ui.t('Connectivity') }}</h2>
      </div>
    </div>
    <div
      v-if="error"
      class="error-banner"
      role="alert"
    >
      {{ error }}
    </div>
    <article
      v-if="network && !network.authenticated"
      class="panel login-panel"
    >
      <div>
        <p class="eyebrow">{{ ui.t('Protected settings') }}</p>
        <h3>{{ ui.t('Administrator login') }}</h3>
      </div>
      <label class="field"
        ><span>{{ ui.t('Password') }}</span
        ><input
          v-model="password"
          type="password"
          autocomplete="current-password" /></label
      ><button
        :disabled="saving || !password"
        @click="login"
      >
        {{ ui.t('Log in') }}
      </button>
    </article>
    <div
      v-else-if="network"
      class="network-grid"
    >
      <article class="panel control-body always-visible">
        <div class="panel-title">
          <div>
            <p class="eyebrow">{{ ui.t('Station') }}</p>
            <h3>{{ ui.t('Join a network') }}</h3>
          </div>
          <span class="status-text">{{ network.station.ip }}</span>
        </div>
        <label class="check-field"
          ><input
            v-model="network.station.enabled"
            type="checkbox"
          />
          {{ ui.t('Enabled') }}</label
        ><label class="field"
          ><span>SSID</span
          ><input
            v-model="network.station.ssid"
            autocomplete="off" /></label
        ><label class="field"
          ><span>{{ ui.t('New password') }}</span
          ><input
            v-model="stationPassword"
            type="password"
            autocomplete="new-password"
            placeholder="Leave blank to keep" /></label
        ><label class="check-field"
          ><input
            v-model="network.station.dhcp"
            type="checkbox"
          />
          {{ ui.t('Use DHCP') }}</label
        >
      </article>
      <article class="panel control-body always-visible">
        <div class="panel-title">
          <div>
            <p class="eyebrow">{{ ui.t('Access point') }}</p>
            <h3>{{ ui.t('Device hotspot') }}</h3>
          </div>
          <span class="status-text">{{ network.accessPoint.ip }}</span>
        </div>
        <label class="check-field"
          ><input
            v-model="network.accessPoint.enabled"
            type="checkbox"
          />
          {{ ui.t('Enabled') }}</label
        ><label class="field"
          ><span>SSID</span
          ><input
            v-model="network.accessPoint.ssid"
            autocomplete="off" /></label
        ><label class="field"
          ><span>{{ ui.t('New password') }}</span
          ><input
            v-model="accessPointPassword"
            type="password"
            autocomplete="new-password"
            placeholder="Leave blank to keep" /></label
        ><label class="field"
          ><span>{{ ui.t('Channel') }}</span
          ><input
            v-model.number="network.accessPoint.channel"
            type="number"
            min="1"
            max="14"
        /></label>
      </article>
      <div class="network-actions">
        <p
          v-if="network.restartRequired"
          class="read-only-note"
        >
          {{ ui.t('A restart is required to apply changes') }}
        </p>
        <div class="button-row">
          <button
            class="primary-button"
            :disabled="saving"
            @click="save"
          >
            {{ ui.t('Save settings') }}</button
          ><button
            class="danger-button"
            :disabled="saving"
            @click="reboot"
          >
            {{ ui.t('Restart') }}</button
          ><button
            :disabled="saving"
            @click="submit({ action: 'logout' })"
          >
            {{ ui.t('Log out') }}
          </button>
        </div>
      </div>
    </div>
  </section>
</template>
