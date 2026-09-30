<script setup lang="ts">
import { computed, ref, toRef, watch } from 'vue'
import type { DeviceAction, GuideDirection, SlewRate, TrackingMode } from '../api/deviceActions'
import { useHoldMotion } from '../composables/useHoldMotion'
import type { Capabilities, DeviceSnapshot } from '../domain/device'
import { useUiLocale } from '../i18n'

const props = defineProps<{
  snapshot: DeviceSnapshot | null
  capabilities: Capabilities
  actionsAllowed: boolean
  actionPending: boolean
  sendMotion: (action: DeviceAction) => Promise<void>
}>()
const emit = defineEmits<{
  action: [action: DeviceAction]
  safetyAction: [action: DeviceAction]
}>()

const locked = computed(() => !props.actionsAllowed || props.actionPending)
const state = computed(() => props.snapshot?.mount ?? null)
const values = computed(() => props.snapshot?.values ?? {})
const selected = computed(() => props.snapshot?.selected ?? {})
const activeSection = ref('observe')
const sections = [
  { id: 'observe', label: 'Observe' },
  { id: 'targets', label: 'Targets & alignment' },
  { id: 'settings', label: 'Motion settings' },
]
const guideRate = ref(2)
const trackingMode = ref<TrackingMode>('sidereal')
const alignStars = ref(1)
const catalog = ref(0)
const catalogText = ref('')
watch(
  () => props.snapshot?.values.cat_data,
  (text) => {
    if (text !== undefined) catalogText.value = text.replaceAll('\r', '\n')
  },
)
const extraGroups: { title: string; key: 'goto' | 'rr'; options: [string, string][] }[] = [
  {
    title: 'Goto buzzer',
    key: 'goto',
    options: [
      ['On', 'bzr_on'],
      ['Off', 'bzr_off'],
    ],
  },
  {
    title: 'Automatic meridian flip',
    key: 'goto',
    options: [
      ['On', 'af_on'],
      ['Off', 'af_off'],
      ['Flip now', 'af_now'],
    ],
  },
  {
    title: 'Pause at meridian',
    key: 'goto',
    options: [
      ['On', 'mp_on'],
      ['Off', 'mp_off'],
    ],
  },
  {
    title: 'Preferred pier side',
    key: 'goto',
    options: [
      ['East', 'pps_e'],
      ['West', 'pps_w'],
      ['Best', 'pps_b'],
      ['Auto', 'pps_a'],
    ],
  },
  {
    title: 'Tracking compensation',
    key: 'rr',
    options: [
      ['Full compensation', 'otk'],
      ['Refraction', 'on'],
      ['Off', 'off'],
      ['Dual axis', 'don'],
      ['Single axis', 'doff'],
    ],
  },
]
const longitude = ref('')
const latitude = ref('')
const utcOffset = ref('+00:00')
const limits = ref({
  horizon: 0,
  overhead: 90,
  east: 0,
  west: 0,
  axis1Backlash: 0,
  axis2Backlash: 0,
})
const ui = useUiLocale()

function send(action: DeviceAction): void {
  emit('action', action)
}

function confirmSend(message: string, action: DeviceAction): void {
  if (window.confirm(ui.t(message))) send(action)
}

function sendMotion(action: DeviceAction): Promise<void> {
  return props.sendMotion(action)
}

const hold = useHoldMotion<GuideDirection>(sendMotion, toRef(props, 'actionsAllowed'), {
  start: (direction) => ({ type: 'mount.guide.start', direction }),
  stop: (direction) => ({ type: 'mount.guide.stop', direction }),
})

function changeSection(id: string): void {
  hold.stop()
  activeSection.value = id
}

function setSlewRate(rate: SlewRate): void {
  send({ type: 'mount.goto.rate', rate })
}

function setTrackingMode(mode: TrackingMode): void {
  send({ type: 'mount.tracking.mode', mode })
}

function saveLocation(): void {
  send({
    type: 'mount.site.location',
    longitude: longitude.value,
    latitude: latitude.value,
    utcOffset: utcOffset.value,
  })
}

function saveLimits(): void {
  send({ type: 'mount.limits', ...limits.value })
}
</script>

<template>
  <section>
    <div class="section-heading">
      <div>
        <p class="eyebrow">{{ ui.t('Mount') }}</p>
        <h2>{{ ui.t('Observation control') }}</h2>
      </div>
      <button
        class="danger-button emergency"
        :disabled="!actionsAllowed"
        @click="emit('safetyAction', { type: 'mount.goto.stop' })"
      >
        {{ ui.t('Stop all motion') }}
      </button>
    </div>

    <div class="telemetry-strip">
      <span
        >{{ ui.t('Current RA') }} <strong>{{ values.gto_i1 || '—' }}</strong></span
      >
      <span
        >{{ ui.t('Current Dec') }} <strong>{{ values.gto_i2 || '—' }}</strong></span
      >
      <span
        >{{ ui.t('Azimuth') }} <strong>{{ values.gto_az1 || '—' }}</strong></span
      >
      <span
        >{{ ui.t('Altitude') }} <strong>{{ values.gto_az2 || '—' }}</strong></span
      >
      <span
        >{{ ui.t('Tracking') }} <strong>{{ ui.t(state?.tracking || '—') }}</strong></span
      >
    </div>
    <nav
      class="section-tabs"
      :aria-label="ui.t('Observation control')"
    >
      <button
        v-for="section in sections"
        :key="section.id"
        :aria-pressed="activeSection === section.id"
        @click="changeSection(section.id)"
      >
        {{ ui.t(section.label) }}
      </button>
    </nav>
    <div
      v-show="activeSection === 'observe'"
      class="mount-dashboard"
    >
      <article class="panel tracking-panel">
        <div class="panel-title">
          <h3>{{ ui.t('Device tracking') }}</h3>
          <button
            class="toggle-switch"
            role="switch"
            :aria-label="ui.t('Device tracking')"
            :aria-checked="!!selected.trk_on"
            :disabled="locked"
            @click="send({ type: 'mount.tracking', enabled: !selected.trk_on })"
          >
            <span></span>
          </button>
        </div>
        <label class="field"
          ><span>{{ ui.t('Tracking rate') }}</span
          ><select
            v-model="trackingMode"
            :disabled="locked"
            @change="setTrackingMode(trackingMode)"
          >
            <option value="sidereal">{{ ui.t('Sidereal') }}</option>
            <option value="lunar">{{ ui.t('Lunar') }}</option>
            <option value="solar">{{ ui.t('Solar') }}</option>
            <option value="king">{{ ui.t('King') }}</option>
          </select></label
        >
        <label class="field"
          ><span>{{ ui.t('Movement rate') }}</span
          ><select
            v-model.number="guideRate"
            :disabled="locked"
            @change="send({ type: 'mount.guide.rate', rate: guideRate })"
          >
            <option
              v-for="(rate, index) in [
                '0.25×',
                '0.5×',
                '1×',
                '2×',
                '4×',
                '8×',
                '20×',
                '48×',
                '½ Max',
                'Max',
              ]"
              :key="index"
              :value="index"
            >
              {{ rate }}
            </option>
          </select></label
        >
        <div class="tracking-readout">
          <span>{{ ui.t('Device status') }}</span
          ><strong>{{ ui.t(state?.tracking || '—') }}</strong>
        </div>
      </article>

      <article class="panel guide-panel">
        <div class="panel-title">
          <div>
            <p class="eyebrow">{{ ui.t('Hold to move') }}</p>
            <h3>{{ ui.t('Guide pad') }}</h3>
          </div>
          <span class="status-text">{{ ui.t(state?.guideStatus || '—') }}</span>
        </div>
        <div
          class="guide-pad"
          @pointerleave="hold.stop"
        >
          <button
            class="north"
            :aria-label="ui.t('N')"
            :class="{ active: hold.activeDirection.value === 'north' }"
            :disabled="locked"
            @pointerdown.prevent="hold.start('north')"
            @keydown.space.prevent="hold.start('north')"
            @keydown.enter.prevent="hold.start('north')"
            @keyup.space.prevent="hold.stop"
            @keyup.enter.prevent="hold.stop"
            @blur="hold.stop"
            @contextmenu.prevent
            @pointerup="hold.stop"
            @pointercancel="hold.stop"
          >
            <span aria-hidden="true">▲</span>
          </button>
          <button
            class="west"
            :aria-label="ui.t('W')"
            :class="{ active: hold.activeDirection.value === 'west' }"
            :disabled="locked"
            @pointerdown.prevent="hold.start('west')"
            @keydown.space.prevent="hold.start('west')"
            @keydown.enter.prevent="hold.start('west')"
            @keyup.space.prevent="hold.stop"
            @keyup.enter.prevent="hold.stop"
            @blur="hold.stop"
            @contextmenu.prevent
            @pointerup="hold.stop"
            @pointercancel="hold.stop"
          >
            <span aria-hidden="true">◀</span>
          </button>
          <button
            class="guide-stop"
            :disabled="!actionsAllowed"
            @click="emit('safetyAction', { type: 'mount.guide.stopAll' })"
            aria-label="STOP"
          >
            STOP
          </button>
          <button
            class="east"
            :aria-label="ui.t('E')"
            :class="{ active: hold.activeDirection.value === 'east' }"
            :disabled="locked"
            @pointerdown.prevent="hold.start('east')"
            @keydown.space.prevent="hold.start('east')"
            @keydown.enter.prevent="hold.start('east')"
            @keyup.space.prevent="hold.stop"
            @keyup.enter.prevent="hold.stop"
            @blur="hold.stop"
            @contextmenu.prevent
            @pointerup="hold.stop"
            @pointercancel="hold.stop"
          >
            <span aria-hidden="true">▶</span>
          </button>
          <button
            class="south"
            :aria-label="ui.t('S')"
            :class="{ active: hold.activeDirection.value === 'south' }"
            :disabled="locked"
            @pointerdown.prevent="hold.start('south')"
            @keydown.space.prevent="hold.start('south')"
            @keydown.enter.prevent="hold.start('south')"
            @keyup.space.prevent="hold.stop"
            @keyup.enter.prevent="hold.stop"
            @blur="hold.stop"
            @contextmenu.prevent
            @pointerup="hold.stop"
            @pointercancel="hold.stop"
          >
            <span aria-hidden="true">▼</span>
          </button>
        </div>
        <p class="motion-hint">{{ ui.t('Hold to move') }}</p>
      </article>
    </div>

    <p
      v-if="!actionsAllowed"
      class="read-only-note"
    >
      {{ ui.t('Actions are locked until fresh state is available from a recognized controller') }}
    </p>

    <article
      v-show="activeSection !== 'settings'"
      class="panel target-panel"
    >
      <div class="panel-title">
        <div>
          <p class="eyebrow">{{ ui.t('Current target') }}</p>
          <h3>{{ ui.t('Coordinates') }}</h3>
        </div>
        <span class="status-text">{{ ui.t(state?.gotoStatus || '—') }}</span>
      </div>
      <div class="coordinates">
        <div>
          <span>{{ ui.t('Right ascension') }}</span
          ><strong>{{ state?.targetRa || '—' }}</strong>
        </div>
        <div>
          <span>{{ ui.t('Declination') }}</span
          ><strong>{{ state?.targetDec || '—' }}</strong>
        </div>
      </div>
      <div class="button-row">
        <button
          class="primary-button"
          :disabled="locked"
          @click="send({ type: 'mount.goto.start' })"
        >
          {{ ui.t('Start goto') }}</button
        ><button
          :disabled="locked"
          @click="send({ type: 'mount.sync' })"
        >
          {{ ui.t('Sync here') }}</button
        ><button
          :disabled="locked || !snapshot?.enabled.gto_active"
          @click="send({ type: 'mount.option', key: 'goto', value: 'mp_cnt' })"
        >
          {{ ui.t('Continue goto') }}
        </button>
      </div>
    </article>

    <div
      class="control-grid"
      :class="{ 'observation-home': activeSection === 'observe' }"
    >
      <details
        v-show="activeSection === 'observe'"
        class="panel control-card"
        open
      >
        <summary>
          <span
            ><small>{{ ui.t('Position') }}</small
            >{{ ui.t('Home & park') }}</span
          ><strong>{{ ui.t(state?.homePark || '—') }}</strong>
        </summary>
        <div class="control-body">
          <div class="button-row">
            <button
              :disabled="locked"
              @click="send({ type: 'mount.home', mode: 'find' })"
            >
              {{ ui.t('Find home') }}</button
            ><button
              :disabled="locked"
              @click="confirmSend('Reset home?', { type: 'mount.home', mode: 'reset' })"
            >
              {{ ui.t('Reset home') }}</button
            ><button
              :disabled="locked"
              @click="send({ type: 'mount.park' })"
            >
              {{ ui.t('Park') }}</button
            ><button
              :disabled="locked"
              @click="send({ type: 'mount.unpark' })"
            >
              {{ ui.t('Unpark') }}
            </button>
          </div>
          <button
            class="quiet-button"
            :disabled="locked"
            @click="confirmSend('Set the current position as park?', { type: 'mount.setPark' })"
          >
            {{ ui.t('Set park position') }}
          </button>
          <div
            v-if="capabilities.autoHome"
            class="segmented"
          >
            <span>{{ ui.t('Automatic home') }}</span
            ><button
              :class="{ selected: selected.auto_on }"
              :disabled="locked"
              @click="send({ type: 'mount.autoHome', enabled: true })"
            >
              {{ ui.t('On') }}</button
            ><button
              :class="{ selected: selected.auto_off }"
              :disabled="locked"
              @click="send({ type: 'mount.autoHome', enabled: false })"
            >
              {{ ui.t('Off') }}
            </button>
          </div>
        </div>
      </details>

      <details
        v-show="activeSection === 'settings'"
        class="panel control-card"
        open
      >
        <summary>
          <span
            ><small>{{ ui.t('Motion') }}</small
            >{{ ui.t('Tracking fine tuning') }}</span
          ><strong>{{ ui.t(state?.tracking || '—') }}</strong>
        </summary>
        <div class="control-body">
          <div class="button-row">
            <button
              :disabled="locked"
              @click="send({ type: 'mount.tracking.adjust', adjustment: 'slower' })"
            >
              − 0.02 Hz</button
            ><button
              :disabled="locked"
              @click="send({ type: 'mount.tracking.adjust', adjustment: 'reset' })"
            >
              {{ ui.t('Reset') }}</button
            ><button
              :disabled="locked"
              @click="send({ type: 'mount.tracking.adjust', adjustment: 'faster' })"
            >
              + 0.02 Hz
            </button>
          </div>
        </div>
      </details>

      <details
        v-show="activeSection === 'settings'"
        class="panel control-card"
      >
        <summary>
          <span
            ><small>{{ ui.t('Motion') }}</small
            >{{ ui.t('Slew profile') }}</span
          ><strong>{{ ui.t(values.gto_rate || 'Normal') }}</strong>
        </summary>
        <div class="control-body">
          <div class="segmented five">
            <button
              :disabled="locked"
              @click="setSlewRate('verySlow')"
            >
              {{ ui.t('Very slow') }}</button
            ><button
              :disabled="locked"
              @click="setSlewRate('slow')"
            >
              {{ ui.t('Slow') }}</button
            ><button
              :disabled="locked"
              @click="setSlewRate('normal')"
            >
              {{ ui.t('Normal') }}</button
            ><button
              :disabled="locked"
              @click="setSlewRate('fast')"
            >
              {{ ui.t('Fast') }}</button
            ><button
              :disabled="locked"
              @click="setSlewRate('veryFast')"
            >
              {{ ui.t('Very fast') }}
            </button>
          </div>
          <div
            v-if="capabilities.meridianFlipHomeModes"
            class="field-group"
          >
            <span>{{ ui.t('Meridian flip home mode') }}</span>
            <div class="button-row">
              <button
                :disabled="locked"
                @click="send({ type: 'mount.goto.flip', mode: 'off' })"
              >
                {{ ui.t('Direct') }}</button
              ><button
                :disabled="locked"
                @click="send({ type: 'mount.goto.flip', mode: 'visit' })"
              >
                {{ ui.t('Visit') }}</button
              ><button
                :disabled="locked"
                @click="send({ type: 'mount.goto.flip', mode: 'pause' })"
              >
                {{ ui.t('Pause') }}
              </button>
            </div>
          </div>
        </div>
      </details>

      <details
        v-show="activeSection === 'targets'"
        class="panel control-card"
      >
        <summary>
          <span
            ><small>{{ ui.t('Model') }}</small
            >{{ ui.t('Alignment') }}</span
          ><strong>{{ ui.t(values.align_progress || 'Ready') }}</strong>
        </summary>
        <div class="control-body">
          <label class="field"
            ><span>{{ ui.t('Alignment stars') }}</span
            ><select
              v-model.number="alignStars"
              :disabled="locked"
            >
              <option
                v-for="count in 9"
                :key="count"
                :value="count"
              >
                {{ count }}
              </option>
            </select></label
          >
          <div class="button-row">
            <button
              :disabled="locked"
              @click="send({ type: 'mount.align.start', stars: alignStars })"
            >
              {{ ui.t('Start') }}</button
            ><button
              :disabled="locked"
              @click="send({ type: 'mount.align.accept' })"
            >
              {{ ui.t('Accept') }}</button
            ><button
              :disabled="locked"
              @click="emit('safetyAction', { type: 'mount.align.cancel' })"
            >
              {{ ui.t('Cancel') }}
            </button>
          </div>
          <button
            class="quiet-button"
            :disabled="locked"
            @click="
              confirmSend('Start polar alignment refinement?', { type: 'mount.align.refine' })
            "
          >
            {{ ui.t('Refine polar alignment') }}
          </button>
        </div>
      </details>

      <details
        v-show="activeSection === 'settings'"
        class="panel control-card"
      >
        <summary>
          <span
            ><small>{{ ui.t('Site') }}</small
            >{{ ui.t('Time & location') }}</span
          ><strong>{{ values.time_lst || '—' }} LST</strong>
        </summary>
        <div class="control-body">
          <dl class="compact-list">
            <div>
              <dt>UTC</dt>
              <dd>{{ values.date_ut }} {{ values.time_ut }}</dd>
            </div>
            <div>
              <dt>{{ ui.t('Longitude') }}</dt>
              <dd>{{ values.site_long || '—' }}</dd>
            </div>
            <div>
              <dt>{{ ui.t('Latitude') }}</dt>
              <dd>{{ values.site_lat || '—' }}</dd>
            </div>
          </dl>
          <button
            :disabled="locked"
            @click="send({ type: 'mount.site.time', date: new Date() })"
          >
            {{ ui.t('Use browser time') }}
          </button>
          <div class="form-grid">
            <label class="field"
              ><span>{{ ui.t('Longitude D:M:S') }}</span
              ><input
                v-model="longitude"
                placeholder="+000:00:00" /></label
            ><label class="field"
              ><span>{{ ui.t('Latitude D:M:S') }}</span
              ><input
                v-model="latitude"
                placeholder="+00:00:00" /></label
            ><label class="field"
              ><span>{{ ui.t('UTC offset') }}</span
              ><input
                v-model="utcOffset"
                placeholder="+08:00"
            /></label>
          </div>
          <button
            :disabled="locked || !longitude || !latitude"
            @click="saveLocation"
          >
            {{ ui.t('Save location') }}
          </button>
        </div>
      </details>

      <details
        v-show="activeSection === 'targets'"
        class="panel control-card"
      >
        <summary>
          <span
            ><small>{{ ui.t('Catalog') }}</small
            >{{ ui.t('Object library') }}</span
          ><strong>{{ values.cat_message || 'No object' }}</strong>
        </summary>
        <div class="control-body">
          <label class="field"
            ><span>{{ ui.t('Catalog') }}</span
            ><select
              v-model.number="catalog"
              :disabled="locked"
              @change="send({ type: 'mount.library.select', catalog })"
            >
              <option
                v-for="index in 16"
                :key="index - 1"
                :value="index - 1"
              >
                {{ index - 1 || 'None' }}
              </option>
            </select></label
          >
          <div class="button-row">
            <button
              :disabled="locked"
              @click="send({ type: 'mount.library.move', direction: 'previous' })"
            >
              {{ ui.t('Previous') }}</button
            ><button
              :disabled="locked"
              @click="send({ type: 'mount.library.target' })"
            >
              {{ ui.t('Set target') }}</button
            ><button
              :disabled="locked"
              @click="send({ type: 'mount.library.move', direction: 'next' })"
            >
              {{ ui.t('Next') }}
            </button>
          </div>
          <small class="muted-text">{{
            values.lib_free || ui.t('Catalog availability pending')
          }}</small>
          <p role="status">{{ values.lib_message }}</p>
          <label class="field"
            ><span>{{ ui.t('Catalog editor') }}</span
            ><textarea
              v-model="catalogText"
              spellcheck="false"
              placeholder="$MyCatalog&#10;M42,DN,05:35:17,-05*23:28"
            ></textarea>
          </label>
          <small>{{ ui.t('Catalog format help') }}</small>
          <div class="button-row">
            <button
              :disabled="locked || !catalog"
              @click="send({ type: 'mount.library.download' })"
            >
              {{ ui.t('Read catalog') }}
            </button>
            <button
              :disabled="locked || !catalog || !catalogText.trim()"
              @click="
                confirmSend('Replace this catalog?', {
                  type: 'mount.library.upload',
                  text: catalogText,
                })
              "
            >
              {{ ui.t('Upload catalog') }}
            </button>
            <button
              :disabled="locked || !catalog"
              @click="
                confirmSend('Clear this catalog?', { type: 'mount.library.upload', text: '' })
              "
            >
              {{ ui.t('Clear catalog') }}
            </button>
            <button
              :disabled="locked"
              @click="confirmSend('Clear the entire library?', { type: 'mount.library.clear' })"
            >
              {{ ui.t('Clear library') }}
            </button>
          </div>
        </div>
      </details>

      <details
        v-show="activeSection === 'settings'"
        class="panel control-card"
      >
        <summary>
          <span
            ><small>{{ ui.t('Correction') }}</small
            >PEC</span
          ><strong>{{ values.pec_sta || '—' }}</strong>
        </summary>
        <div class="control-body">
          <div class="button-row wrap">
            <button
              :disabled="locked"
              @click="send({ type: 'mount.pec', command: 'play' })"
            >
              {{ ui.t('Play') }}</button
            ><button
              :disabled="locked"
              @click="send({ type: 'mount.pec', command: 'stop' })"
            >
              {{ ui.t('Stop') }}</button
            ><button
              :disabled="locked"
              @click="confirmSend('Start PEC recording?', { type: 'mount.pec', command: 'record' })"
            >
              {{ ui.t('Record') }}</button
            ><button
              :disabled="locked"
              @click="confirmSend('Clear PEC data?', { type: 'mount.pec', command: 'clear' })"
            >
              {{ ui.t('Clear') }}</button
            ><button
              :disabled="locked"
              @click="confirmSend('Write PEC to memory?', { type: 'mount.pec', command: 'write' })"
            >
              {{ ui.t('Write to memory') }}
            </button>
          </div>
        </div>
      </details>

      <details
        v-show="activeSection === 'settings'"
        class="panel control-card"
      >
        <summary>
          <span
            ><small>{{ ui.t('Safety') }}</small
            >{{ ui.t('Limits & origin') }}</span
          ><strong>{{ ui.t('Controller guarded') }}</strong>
        </summary>
        <div class="control-body">
          <div class="form-grid six">
            <label class="field"
              ><span>{{ ui.t('Horizon °') }}</span
              ><input
                v-model.number="limits.horizon"
                type="number"
                min="-30"
                max="30" /></label
            ><label class="field"
              ><span>{{ ui.t('Overhead °') }}</span
              ><input
                v-model.number="limits.overhead"
                type="number"
                min="60"
                max="90" /></label
            ><label class="field"
              ><span>{{ ui.t('East min') }}</span
              ><input
                v-model.number="limits.east"
                type="number"
                min="-270"
                max="270" /></label
            ><label class="field"
              ><span>{{ ui.t('West min') }}</span
              ><input
                v-model.number="limits.west"
                type="number"
                min="-270"
                max="270" /></label
            ><label class="field"
              ><span>{{ ui.t('Axis 1 backlash') }}</span
              ><input
                v-model.number="limits.axis1Backlash"
                type="number"
                min="0"
                max="3600" /></label
            ><label class="field"
              ><span>{{ ui.t('Axis 2 backlash') }}</span
              ><input
                v-model.number="limits.axis2Backlash"
                type="number"
                min="0"
                max="3600"
            /></label>
          </div>
          <div class="button-row">
            <button
              :disabled="locked"
              @click="saveLimits"
            >
              {{ ui.t('Save limits') }}</button
            ><button
              class="quiet-button"
              :disabled="locked"
              @click="
                confirmSend('Set the current coordinate origin?', {
                  type: 'mount.coordinateOrigin',
                })
              "
            >
              {{ ui.t('Set coordinate origin') }}
            </button>
          </div>
        </div>
      </details>
      <details
        v-show="activeSection === 'settings'"
        class="panel control-card"
      >
        <summary>
          <span>{{ ui.t('Advanced motion settings') }}</span>
        </summary>
        <div class="control-body">
          <div
            v-for="group in extraGroups"
            :key="group.title"
            class="field-group"
          >
            <span>{{ ui.t(group.title) }}</span>
            <div class="button-row">
              <button
                v-for="[label, value] in group.options"
                :key="value"
                :disabled="locked"
                @click="send({ type: 'mount.option', key: group.key, value })"
              >
                {{ ui.t(label) }}
              </button>
            </div>
          </div>
        </div>
      </details>
    </div>
  </section>
</template>
