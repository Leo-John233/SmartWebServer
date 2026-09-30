<script setup lang="ts">
import { computed, onMounted, ref, watch } from 'vue'
import {
  parseFirmwareForms,
  firmwareFormValues,
  type FirmwareForm,
} from '../protocol/firmwareForms'
import { useUiLocale } from '../i18n'
import type { Capabilities } from '../domain/device'
const props = defineProps<{
  actionsAllowed: boolean
  capabilities: Capabilities
  scenario?: string
}>()
const selectedAxis = ref(0)
const axes = computed(() => [
  ...(props.capabilities.mount ? [1, 2] : []),
  ...(props.capabilities.rotator ? [3] : []),
  ...Array.from({ length: props.capabilities.focuserCount }, (_, index) => index + 4),
])
const visibleForms = computed(() =>
  path.value === '/index.htm'
    ? forms.value.filter((form) => (form.axis ?? 0) === selectedAxis.value)
    : forms.value,
)
function requestHeaders(): HeadersInit {
  return import.meta.env.MODE === 'mock' && props.scenario
    ? { 'X-WebUI-Scenario': props.scenario }
    : {}
}
const ui = useUiLocale()
const path = ref('/index.htm')
const forms = ref<FirmwareForm[]>([])
const busy = ref(false)
const message = ref('')
async function load(): Promise<void> {
  busy.value = true
  try {
    const response = await fetch(path.value, { headers: requestHeaders() })
    if (!response.ok) throw new Error()
    forms.value = parseFirmwareForms(await response.text(), path.value)
    message.value = forms.value.length ? '' : 'No editable settings reported'
  } catch {
    message.value = 'Unable to load device settings'
  } finally {
    busy.value = false
  }
}
async function save(form: FirmwareForm, button: FirmwareForm['buttons'][number]): Promise<void> {
  if (!props.actionsAllowed || busy.value) return
  if (!window.confirm(ui.t('Apply these device settings?'))) return
  busy.value = true
  try {
    const values = firmwareFormValues(form, button)
    const method = form.method ?? 'GET'
    const response = await fetch(method === 'GET' ? form.path + '?' + values : form.path, {
      method,
      headers: requestHeaders(),
      ...(method === 'POST' ? { body: values } : {}),
    })
    if (!response.ok) throw new Error()
    if (form.command) await load()
    else forms.value = parseFirmwareForms(await response.text(), path.value)
    message.value = 'Settings submitted; review returned values'
  } catch {
    message.value = 'Unable to save device settings'
  } finally {
    busy.value = false
  }
}
watch(() => props.scenario, load)
onMounted(load)
</script>
<template>
  <section>
    <div class="section-heading">
      <div>
        <p class="eyebrow">{{ ui.t('Device settings') }}</p>
        <h2>{{ ui.t('Device settings') }}</h2>
      </div>
    </div>
    <p class="muted-text">
      {{
        ui.t(
          'Settings follow the options reported by your firmware. Passwords left blank are preserved.',
        )
      }}
    </p>
    <label class="field"
      ><span>{{ ui.t('Settings category') }}</span
      ><select
        v-model="path"
        :disabled="busy"
        @change="load"
      >
        <option value="/index.htm">{{ ui.t('Controller and axes') }}</option>
        <option value="/mount.htm">{{ ui.t('Mount calibration and limits') }}</option>
        <option value="/net.htm">{{ ui.t('Network and update settings') }}</option>
      </select></label
    >
    <p
      v-if="message"
      role="status"
    >
      {{ ui.t(message) }}
    </p>
    <nav
      v-if="path === '/index.htm'"
      class="axis-tabs"
      :aria-label="ui.t('Controller and axes')"
    >
      <button
        :aria-pressed="selectedAxis === 0"
        @click="selectedAxis = 0"
      >
        {{ ui.t('Controller') }}
      </button>
      <button
        v-for="axis in axes"
        :key="axis"
        :aria-pressed="selectedAxis === axis"
        @click="selectedAxis = axis"
      >
        {{ ui.t('Axis ' + axis) }}
      </button>
    </nav>
    <p
      v-if="path === '/index.htm' && !visibleForms.length && !busy"
      class="read-only-note"
    >
      {{
        ui.t(
          'The firmware did not provide editable parameters. Axis configuration may be disabled or awaiting a reboot. Dynamic axis parameters in this project require OnStepX 10.26 or later.',
        )
      }}
    </p>
    <p
      v-if="selectedAxis && path === '/index.htm'"
      class="setting-help"
    >
      {{
        ui.t(
          'Apply settings separately for each axis. Most changes take effect after restarting the controller, except parameters marked as immediate.',
        )
      }}
    </p>
    <div class="control-grid settings-grid">
      <form
        v-for="(form, index) in visibleForms"
        :key="path + index"
        class="panel control-body always-visible"
        @submit.prevent="save(form, form.buttons[0])"
      >
        <h3>{{ ui.t(form.title) }}</h3>
        <template
          v-for="field in form.fields"
          :key="field.name"
        >
          <label
            v-if="field.type !== 'hidden'"
            :class="field.type === 'checkbox' ? 'check-field' : 'field'"
          >
            <span>{{ ui.t(field.label) }}</span>
            <input
              v-if="field.type === 'checkbox'"
              v-model="field.checked"
              type="checkbox"
              :disabled="busy || field.disabled"
            />
            <select
              v-else-if="field.type === 'select'"
              v-model="field.value"
              :disabled="busy || field.disabled"
            >
              <option
                v-for="option in field.options"
                :key="option.value"
                :value="option.value"
              >
                {{
                  ui.t(
                    /Decay mode/.test(field.label) && ['Fast', 'Slow'].includes(option.label)
                      ? option.label + ' decay'
                      : option.label,
                  )
                }}
              </option>
            </select>
            <input
              v-else
              v-model="field.value"
              :type="field.type"
              :min="field.min"
              :max="field.max"
              :step="field.step"
              :disabled="busy || field.disabled"
              :autocomplete="field.type === 'password' ? 'new-password' : 'off'"
            />
          </label>
        </template>
        <div class="button-row">
          <button
            v-for="button in form.buttons"
            :key="button.name + button.value"
            type="button"
            :disabled="
              busy ||
              !actionsAllowed ||
              (!!form.fields.length && form.fields.every((field) => field.disabled))
            "
            @click="
              ($event.currentTarget as HTMLButtonElement).form?.reportValidity() &&
              save(form, button)
            "
          >
            {{ ui.t(button.label) }}
          </button>
        </div>
      </form>
    </div>
  </section>
</template>
