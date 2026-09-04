<script setup lang="ts">
import { computed, ref, toRef } from 'vue'
import type { DeviceAction, GuideDirection, SlewRate, TrackingMode } from '../api/deviceActions'
import { useHoldMotion } from '../composables/useHoldMotion'
import type { Capabilities, DeviceSnapshot } from '../domain/device'

const props = defineProps<{
  snapshot: DeviceSnapshot | null
  capabilities: Capabilities
  actionsAllowed: boolean
  actionPending: boolean
}>()
const emit = defineEmits<{
  action: [action: DeviceAction]
  safetyAction: [action: DeviceAction]
}>()

const locked = computed(() => !props.actionsAllowed || props.actionPending)
const state = computed(() => props.snapshot?.mount ?? null)
const values = computed(() => props.snapshot?.values ?? {})
const selected = computed(() => props.snapshot?.selected ?? {})
const guideRate = ref(2)
const alignStars = ref(1)
const catalog = ref(0)
const longitude = ref('')
const latitude = ref('')
const utcOffset = ref('+00:00')
const limits = ref({ horizon: 0, overhead: 90, east: 0, west: 0, axis1Backlash: 0, axis2Backlash: 0 })

function send(action: DeviceAction): void {
  emit('action', action)
}

function confirmSend(message: string, action: DeviceAction): void {
  if (window.confirm(message)) send(action)
}

function sendMotion(action: DeviceAction): Promise<void> {
  if (action.type === 'mount.guide.stop') emit('safetyAction', action)
  else emit('action', action)
  return Promise.resolve()
}

const hold = useHoldMotion<GuideDirection>(sendMotion, toRef(props, 'actionsAllowed'), {
  start: (direction) => ({ type: 'mount.guide.start', direction }),
  stop: (direction) => ({ type: 'mount.guide.stop', direction }),
})

function setSlewRate(rate: SlewRate): void {
  send({ type: 'mount.goto.rate', rate })
}

function setTrackingMode(mode: TrackingMode): void {
  send({ type: 'mount.tracking.mode', mode })
}

function saveLocation(): void {
  send({ type: 'mount.site.location', longitude: longitude.value, latitude: latitude.value, utcOffset: utcOffset.value })
}

function saveLimits(): void {
  send({ type: 'mount.limits', ...limits.value })
}
</script>

<template>
  <section>
    <div class="section-heading">
      <div><p class="eyebrow">Mount</p><h2>Observation control</h2></div>
      <button class="danger-button emergency" :disabled="!actionsAllowed" @click="emit('safetyAction', { type: 'mount.goto.stop' })">Stop all motion</button>
    </div>

    <div class="mount-dashboard">
      <article class="panel target-panel">
        <div class="panel-title"><div><p class="eyebrow">Current target</p><h3>Coordinates</h3></div><span class="status-text">{{ state?.gotoStatus || '—' }}</span></div>
        <div class="coordinates">
          <div><span>Right ascension</span><strong>{{ state?.targetRa || '—' }}</strong></div>
          <div><span>Declination</span><strong>{{ state?.targetDec || '—' }}</strong></div>
        </div>
        <div class="button-row"><button class="primary-button" :disabled="locked" @click="send({ type: 'mount.goto.start' })">Start goto</button><button :disabled="locked" @click="send({ type: 'mount.sync' })">Sync here</button></div>
      </article>

      <article class="panel guide-panel">
        <div class="panel-title"><div><p class="eyebrow">Hold to move</p><h3>Guide pad</h3></div><span class="status-text">{{ state?.guideStatus || '—' }}</span></div>
        <div class="guide-pad" @pointerleave="hold.stop">
          <button class="north" :class="{ active: hold.activeDirection.value === 'north' }" :disabled="locked" @pointerdown.prevent="hold.start('north')" @pointerup="hold.stop" @pointercancel="hold.stop">N</button>
          <button class="west" :class="{ active: hold.activeDirection.value === 'west' }" :disabled="locked" @pointerdown.prevent="hold.start('west')" @pointerup="hold.stop" @pointercancel="hold.stop">W</button>
          <button class="guide-stop" :disabled="!actionsAllowed" @click="emit('safetyAction', { type: 'mount.guide.stopAll' })">■</button>
          <button class="east" :class="{ active: hold.activeDirection.value === 'east' }" :disabled="locked" @pointerdown.prevent="hold.start('east')" @pointerup="hold.stop" @pointercancel="hold.stop">E</button>
          <button class="south" :class="{ active: hold.activeDirection.value === 'south' }" :disabled="locked" @pointerdown.prevent="hold.start('south')" @pointerup="hold.stop" @pointercancel="hold.stop">S</button>
        </div>
        <label class="range-field"><span>Guide rate {{ guideRate }}</span><input v-model.number="guideRate" :disabled="locked" type="range" min="0" max="9" @change="send({ type: 'mount.guide.rate', rate: guideRate })"></label>
      </article>
    </div>

    <p v-if="!actionsAllowed" class="read-only-note">Actions are locked until fresh state is available from a recognized controller</p>

    <div class="control-grid">
      <details class="panel control-card" open>
        <summary><span><small>Position</small>Home &amp; park</span><strong>{{ state?.homePark || '—' }}</strong></summary>
        <div class="control-body">
          <div class="button-row"><button :disabled="locked" @click="send({ type: 'mount.home', mode: 'find' })">Find home</button><button :disabled="locked" @click="send({ type: 'mount.park' })">Park</button><button :disabled="locked" @click="send({ type: 'mount.unpark' })">Unpark</button></div>
          <button class="quiet-button" :disabled="locked" @click="confirmSend('Set the current position as park?', { type: 'mount.setPark' })">Set park position</button>
          <div v-if="capabilities.autoHome" class="segmented"><span>Automatic home</span><button :class="{ selected: selected.auto_on }" :disabled="locked" @click="send({ type: 'mount.autoHome', enabled: true })">On</button><button :class="{ selected: selected.auto_off }" :disabled="locked" @click="send({ type: 'mount.autoHome', enabled: false })">Off</button></div>
        </div>
      </details>

      <details class="panel control-card" open>
        <summary><span><small>Motion</small>Tracking</span><strong>{{ state?.tracking || '—' }}</strong></summary>
        <div class="control-body">
          <div class="segmented compact"><button :class="{ selected: selected.trk_on }" :disabled="locked" @click="send({ type: 'mount.tracking', enabled: true })">On</button><button :class="{ selected: selected.trk_off }" :disabled="locked" @click="send({ type: 'mount.tracking', enabled: false })">Off</button></div>
          <div class="button-row wrap"><button :disabled="locked" @click="setTrackingMode('sidereal')">Sidereal</button><button :disabled="locked" @click="setTrackingMode('lunar')">Lunar</button><button :disabled="locked" @click="setTrackingMode('solar')">Solar</button><button :disabled="locked" @click="setTrackingMode('king')">King</button></div>
          <div class="button-row"><button :disabled="locked" @click="send({ type: 'mount.tracking.adjust', adjustment: 'slower' })">− 0.02 Hz</button><button :disabled="locked" @click="send({ type: 'mount.tracking.adjust', adjustment: 'reset' })">Reset</button><button :disabled="locked" @click="send({ type: 'mount.tracking.adjust', adjustment: 'faster' })">+ 0.02 Hz</button></div>
        </div>
      </details>

      <details class="panel control-card">
        <summary><span><small>Motion</small>Slew profile</span><strong>{{ values.gto_rate || 'Normal' }}</strong></summary>
        <div class="control-body">
          <div class="segmented five"><button :disabled="locked" @click="setSlewRate('verySlow')">Very slow</button><button :disabled="locked" @click="setSlewRate('slow')">Slow</button><button :disabled="locked" @click="setSlewRate('normal')">Normal</button><button :disabled="locked" @click="setSlewRate('fast')">Fast</button><button :disabled="locked" @click="setSlewRate('veryFast')">Very fast</button></div>
          <div v-if="capabilities.meridianFlipHomeModes" class="field-group"><span>Meridian flip home mode</span><div class="button-row"><button :disabled="locked" @click="send({ type: 'mount.goto.flip', mode: 'off' })">Direct</button><button :disabled="locked" @click="send({ type: 'mount.goto.flip', mode: 'visit' })">Visit</button><button :disabled="locked" @click="send({ type: 'mount.goto.flip', mode: 'pause' })">Pause</button></div></div>
        </div>
      </details>

      <details class="panel control-card">
        <summary><span><small>Model</small>Alignment</span><strong>{{ values.align_progress || 'Ready' }}</strong></summary>
        <div class="control-body">
          <label class="field"><span>Alignment stars</span><select v-model.number="alignStars" :disabled="locked"><option v-for="count in 9" :key="count" :value="count">{{ count }}</option></select></label>
          <div class="button-row"><button :disabled="locked" @click="send({ type: 'mount.align.start', stars: alignStars })">Start</button><button :disabled="locked" @click="send({ type: 'mount.align.accept' })">Accept</button><button :disabled="locked" @click="emit('safetyAction', { type: 'mount.align.cancel' })">Cancel</button></div>
          <button class="quiet-button" :disabled="locked" @click="confirmSend('Start polar alignment refinement?', { type: 'mount.align.refine' })">Refine polar alignment</button>
        </div>
      </details>

      <details class="panel control-card">
        <summary><span><small>Site</small>Time &amp; location</span><strong>{{ values.time_lst || '—' }} LST</strong></summary>
        <div class="control-body">
          <dl class="compact-list"><div><dt>UTC</dt><dd>{{ values.date_ut }} {{ values.time_ut }}</dd></div><div><dt>Longitude</dt><dd>{{ values.site_long || '—' }}</dd></div><div><dt>Latitude</dt><dd>{{ values.site_lat || '—' }}</dd></div></dl>
          <button :disabled="locked" @click="send({ type: 'mount.site.time', date: new Date() })">Use browser time</button>
          <div class="form-grid"><label class="field"><span>Longitude D:M:S</span><input v-model="longitude" placeholder="+000:00:00"></label><label class="field"><span>Latitude D:M:S</span><input v-model="latitude" placeholder="+00:00:00"></label><label class="field"><span>UTC offset</span><input v-model="utcOffset" placeholder="+08:00"></label></div>
          <button :disabled="locked || !longitude || !latitude" @click="saveLocation">Save location</button>
        </div>
      </details>

      <details class="panel control-card">
        <summary><span><small>Catalog</small>Object library</span><strong>{{ values.cat_message || 'No object' }}</strong></summary>
        <div class="control-body">
          <label class="field"><span>Catalog</span><select v-model.number="catalog" :disabled="locked" @change="send({ type: 'mount.library.select', catalog })"><option v-for="index in 16" :key="index - 1" :value="index - 1">{{ index - 1 || 'None' }}</option></select></label>
          <div class="button-row"><button :disabled="locked" @click="send({ type: 'mount.library.move', direction: 'previous' })">Previous</button><button :disabled="locked" @click="send({ type: 'mount.library.target' })">Set target</button><button :disabled="locked" @click="send({ type: 'mount.library.move', direction: 'next' })">Next</button></div>
          <small class="muted-text">{{ values.lib_free || 'Catalog availability pending' }}</small>
        </div>
      </details>

      <details class="panel control-card">
        <summary><span><small>Correction</small>PEC</span><strong>{{ values.pec_sta || '—' }}</strong></summary>
        <div class="control-body"><div class="button-row wrap"><button :disabled="locked" @click="send({ type: 'mount.pec', command: 'play' })">Play</button><button :disabled="locked" @click="send({ type: 'mount.pec', command: 'stop' })">Stop</button><button :disabled="locked" @click="confirmSend('Start PEC recording?', { type: 'mount.pec', command: 'record' })">Record</button><button :disabled="locked" @click="confirmSend('Clear PEC data?', { type: 'mount.pec', command: 'clear' })">Clear</button></div></div>
      </details>

      <details class="panel control-card">
        <summary><span><small>Safety</small>Limits &amp; origin</span><strong>Controller guarded</strong></summary>
        <div class="control-body">
          <div class="form-grid six"><label class="field"><span>Horizon °</span><input v-model.number="limits.horizon" type="number" min="-30" max="30"></label><label class="field"><span>Overhead °</span><input v-model.number="limits.overhead" type="number" min="60" max="90"></label><label class="field"><span>East min</span><input v-model.number="limits.east" type="number" min="-270" max="270"></label><label class="field"><span>West min</span><input v-model.number="limits.west" type="number" min="-270" max="270"></label><label class="field"><span>Axis 1 backlash</span><input v-model.number="limits.axis1Backlash" type="number" min="0" max="3600"></label><label class="field"><span>Axis 2 backlash</span><input v-model.number="limits.axis2Backlash" type="number" min="0" max="3600"></label></div>
          <div class="button-row"><button :disabled="locked" @click="saveLimits">Save limits</button><button class="quiet-button" :disabled="locked" @click="confirmSend('Set the current coordinate origin?', { type: 'mount.coordinateOrigin' })">Set coordinate origin</button></div>
        </div>
      </details>
    </div>
  </section>
</template>
