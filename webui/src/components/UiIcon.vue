<script setup lang="ts">
import { computed } from 'vue'
import {
  Telescope,
  Settings,
  SlidersHorizontal,
  RotateCw,
  Focus,
  Power,
  Crosshair,
  Wifi,
} from '@lucide/vue'
const props = defineProps<{ name: string }>()
// 统一使用同一套图标，按需引入以控制固件资源体积
const icons = {
  telescope: Telescope,
  settings: Settings,
  control: SlidersHorizontal,
  rotate: RotateCw,
  focus: Focus,
  power: Power,
  encoder: Crosshair,
  network: Wifi,
}
const icon = computed(() => icons[props.name as keyof typeof icons] ?? Telescope)
</script>
<template>
  <component
    :is="icon"
    class="ui-icon"
    :size="24"
    :stroke-width="1.75"
    aria-hidden="true"
  />
</template>
