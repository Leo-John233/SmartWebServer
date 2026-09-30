<script setup lang="ts">
import { onMounted, ref } from 'vue'
import {
  parseFirmwareForms,
  firmwareFormValues,
  type FirmwareForm,
} from '../protocol/firmwareForms'
import { useUiLocale } from '../i18n'
const props = defineProps<{ actionsAllowed: boolean }>()
const ui = useUiLocale()
const path = ref('/index.htm')
const forms = ref<FirmwareForm[]>([])
const busy = ref(false)
const message = ref('')
async function load(): Promise<void> {
  busy.value = true
  try {
    const response = await fetch(path.value)
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
    const response = await fetch(form.path, {
      method: 'POST',
      body: firmwareFormValues(form, button),
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
onMounted(load)
</script>
<template>
  <section>
    <div class="section-heading">
      <div>
        <p class="eyebrow">{{ ui.t('Device settings') }}</p>
        <h2>{{ ui.t('Full settings') }}</h2>
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
    <div class="control-grid">
      <form
        v-for="(form, index) in forms"
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
                {{ ui.t(option.label) }}
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
            :disabled="busy || !actionsAllowed"
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
