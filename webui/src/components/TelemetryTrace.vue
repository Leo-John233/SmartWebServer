<script setup lang="ts">
import { computed, ref, watch } from 'vue'
const props = defineProps<{
  label: string
  value?: string
  updatedAt?: number | null
  unit?: string
}>()
const samples = ref<number[]>([])
watch(
  () => props.updatedAt,
  () => {
    const value = Number(props.value)
    if (props.value && Number.isFinite(value)) samples.value = [...samples.value.slice(-119), value]
  },
  { immediate: true },
)
const scale = computed(() => Math.max(1, ...samples.value.map(Math.abs)))
const points = computed(() =>
  samples.value
    .map((value, index) => `${(index * 600) / 119},${70 - (value * 60) / scale.value}`)
    .join(' '),
)
</script>
<template>
  <article class="panel trace-panel">
    <div class="panel-title">
      <h3>{{ label }}</h3>
      <span>{{ value ?? '—' }} {{ unit }}</span>
    </div>
    <svg
      viewBox="0 0 600 140"
      role="img"
      :aria-label="label"
      preserveAspectRatio="none"
    >
      <path
        d="M0 70H600"
        stroke="var(--line)"
      />
      <polyline
        :points="points"
        fill="none"
        stroke="var(--accent)"
        stroke-width="2"
        vector-effect="non-scaling-stroke"
      />
    </svg>
    <small class="muted-text">±{{ scale.toFixed(1) }} {{ unit }} · {{ samples.length }}/120</small>
  </article>
</template>
