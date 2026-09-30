<script setup lang="ts">
import { computed, ref, watch } from 'vue'
import { useUiLocale } from '../i18n'

const props = defineProps<{
  label: string
  value?: string
  updatedAt?: number | null
  unit?: string
  range?: 'signed' | 'stallguard'
  threshold?: string
}>()
const ui = useUiLocale()
const samples = ref<{ time: number; value: number | null }[]>([])

watch(
  () => props.updatedAt,
  (time) => {
    if (time === null || time === undefined) return
    const number = Number(props.value)
    const value = props.value?.trim() && Number.isFinite(number) ? number : null
    samples.value = [...samples.value.slice(-119), { time, value }]
  },
  { immediate: true },
)

// 横轴采用真实采样时间，缺失值保留断点，避免画出不存在的连续数据
const scale = computed(() => {
  if (props.range === 'stallguard') return 1023
  const maximum = Math.max(1, ...samples.value.map((sample) => Math.abs(sample.value ?? 0)))
  const step = 10 ** Math.floor(Math.log10(maximum))
  return Math.ceil(maximum / step) * step
})
const duration = computed(() => {
  const first = samples.value[0]?.time ?? 0
  const last = samples.value.at(-1)?.time ?? first
  return Math.max(1000, last - first)
})
const lowerBound = computed(() => (props.range === 'stallguard' ? 0 : -scale.value))
const ticks = computed(() =>
  Array.from(
    { length: 5 },
    (_, index) => scale.value - ((scale.value - lowerBound.value) * index) / 4,
  ),
)
function yPosition(value: number): number {
  return 22 + (116 * (scale.value - value)) / (scale.value - lowerBound.value)
}
const thresholdValue = computed(() => {
  const value = Number(props.threshold)
  return props.threshold?.trim() && Number.isFinite(value) && value >= 0 && value <= 1023
    ? value
    : null
})
const plotted = computed(() => {
  const last = samples.value.at(-1)?.time ?? 0
  return samples.value.map((sample) => ({
    x: 56 + 528 * (1 - (last - sample.time) / duration.value),
    y: sample.value === null ? null : yPosition(sample.value),
  }))
})
const path = computed(() => {
  let connected = false
  return plotted.value
    .map(({ x, y }) => {
      if (y === null) {
        connected = false
        return ''
      }
      const command = connected ? 'L' : 'M'
      connected = true
      return `${command}${x.toFixed(1)},${y.toFixed(1)}`
    })
    .join(' ')
})
const lastPoint = computed(() => plotted.value.at(-1))
const hasValues = computed(() => samples.value.some((sample) => sample.value !== null))
</script>

<template>
  <article class="panel trace-panel">
    <div class="panel-title">
      <h3>{{ label }}</h3>
      <div class="trace-reading">
        <strong>{{ value?.trim() || '—' }}</strong
        ><span>{{ unit }}</span>
      </div>
    </div>
    <svg
      viewBox="0 0 600 180"
      role="img"
      :aria-label="`${label} · ${ui.t('Recent samples')}`"
    >
      <g
        v-for="(tick, index) in ticks"
        :key="index"
      >
        <line
          x1="56"
          x2="584"
          :y1="22 + index * 29"
          :y2="22 + index * 29"
          class="chart-grid"
        />
        <text
          x="46"
          :y="26 + index * 29"
          text-anchor="end"
        >
          {{ range === 'stallguard' ? Math.round(tick) : Number(tick.toPrecision(3)) }}
        </text>
      </g>
      <text
        x="56"
        y="164"
      >
        −{{ (duration / 1000).toFixed(0) }} s
      </text>
      <text
        x="584"
        y="164"
        text-anchor="end"
      >
        {{ ui.t('Now') }}
      </text>
      <line
        v-if="thresholdValue !== null"
        x1="56"
        x2="584"
        :y1="yPosition(thresholdValue)"
        :y2="yPosition(thresholdValue)"
        class="threshold-line"
      />
      <path
        :d="path"
        fill="none"
        stroke="var(--accent)"
        stroke-width="2"
        vector-effect="non-scaling-stroke"
      />
      <circle
        v-if="lastPoint && lastPoint.y !== null"
        :cx="lastPoint.x"
        :cy="lastPoint.y"
        r="3"
        fill="var(--accent)"
      />
      <text
        v-if="!hasValues"
        x="320"
        y="70"
        text-anchor="middle"
      >
        {{ ui.t('Waiting for telemetry') }}
      </text>
    </svg>
    <div class="trace-legend">
      <span><i class="sample-key"></i>{{ ui.t('Measured value') }}</span>
      <span v-if="thresholdValue !== null"
        ><i class="threshold-key"></i>{{ ui.t('Trigger threshold') }} {{ thresholdValue }}</span
      >
      <span class="sample-count">{{ ui.t('Recent samples') }} {{ samples.length }}</span>
    </div>
  </article>
</template>

<style scoped>
.trace-panel {
  overflow: hidden;
}
.trace-panel .panel-title {
  border-bottom: 0;
  padding-bottom: 0;
}
.trace-reading {
  display: flex;
  align-items: baseline;
  gap: 6px;
  font-variant-numeric: tabular-nums;
}
.trace-reading strong {
  font-size: 28px;
  font-weight: 550;
  color: var(--accent);
}
.trace-reading span {
  font-size: 12px;
  color: var(--muted);
}
.trace-legend {
  display: flex;
  flex-wrap: wrap;
  gap: 12px;
  color: var(--muted);
  font-size: 12px;
  margin-top: 14px;
}
.trace-legend span {
  display: inline-flex;
  align-items: center;
  gap: 6px;
}
.trace-legend i {
  display: inline-block;
  width: 16px;
  border-top: 2px solid var(--accent);
}
.trace-legend .threshold-key {
  border-top: 2px dashed var(--warning);
}
.sample-count {
  margin-left: auto;
}
.threshold-line {
  stroke: var(--warning);
  stroke-dasharray: 6 4;
  vector-effect: non-scaling-stroke;
}
.trace-panel svg {
  display: block;
  width: 100%;
  height: auto;
  margin-top: 16px;
}
.trace-panel text {
  fill: var(--muted);
  font-size: 12px;
  font-variant-numeric: tabular-nums;
}
.chart-grid {
  stroke: var(--line);
  opacity: 0.55;
  vector-effect: non-scaling-stroke;
}
</style>
