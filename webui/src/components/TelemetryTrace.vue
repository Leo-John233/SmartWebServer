<script setup lang="ts">
import { computed, ref, watch } from 'vue'
import { useUiLocale } from '../i18n'

const props = defineProps<{
  label: string
  value?: string
  updatedAt?: number | null
  unit?: string
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
  const maximum = Math.max(1, ...samples.value.map((sample) => Math.abs(sample.value ?? 0)))
  const step = 10 ** Math.floor(Math.log10(maximum))
  return Math.ceil(maximum / step) * step
})
const duration = computed(() => {
  const first = samples.value[0]?.time ?? 0
  const last = samples.value.at(-1)?.time ?? first
  return Math.max(1000, last - first)
})
const plotted = computed(() => {
  const last = samples.value.at(-1)?.time ?? 0
  return samples.value.map((sample) => ({
    x: 56 + 528 * (1 - (last - sample.time) / duration.value),
    y: sample.value === null ? null : 80 - (sample.value / scale.value) * 58,
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
      <span>{{ value?.trim() || '—' }} {{ unit }}</span>
    </div>
    <svg
      viewBox="0 0 600 180"
      role="img"
      :aria-label="`${label} · ${ui.t('Recent samples')}`"
    >
      <g
        v-for="(tick, index) in [scale, 0, -scale]"
        :key="index"
      >
        <line
          x1="56"
          x2="584"
          :y1="22 + index * 58"
          :y2="22 + index * 58"
          class="chart-grid"
        />
        <text
          x="46"
          :y="26 + index * 58"
          text-anchor="end"
        >
          {{ Number(tick.toPrecision(3)) }}
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
    <small class="muted-text"
      >{{ ui.t('Recent samples') }} · {{ samples.length }} / 120<span v-if="unit">
        · {{ unit }}</span
      ></small
    >
  </article>
</template>

<style scoped>
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
  stroke-dasharray: 3 5;
  vector-effect: non-scaling-stroke;
}
</style>
