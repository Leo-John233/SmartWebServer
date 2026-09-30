<script setup lang="ts">
defineProps<{ name: string }>()
const paths: Record<string, string> = {
  telescope: 'M4 11 16 4l4 7-12 7z M15 5l4-2 3 5-4 2 M6 14l-3 2-2-4 3-2 M11 17l-3 5m4-5 4 5m-5-5v5',
  settings:
    'm10 2-1 3-3 1-3-1-2 4 2 2v3l-2 2 2 4 3-1 3 1 1 3h4l1-3 3-1 3 1 2-4-2-2v-3l2-2-2-4-3 1-3-1-1-3z M16 12a4 4 0 1 1-8 0 4 4 0 0 1 8 0',
  control: 'M22 12a10 10 0 1 1-20 0 10 10 0 0 1 20 0 M8 7l9 5-9 5z',
  rotate: 'M20 7V2m0 5h-5 M20 7a9 9 0 1 0 1 8 M12 7v5l3 2',
  focus: 'M3 8V3h5m8 0h5v5M3 16v5h5m8 0h5v-5 M16 12a4 4 0 1 1-8 0 4 4 0 0 1 8 0',
  power: 'M12 2v10 M6 5a9 9 0 1 0 12 0',
  encoder: 'M3 12h18M12 3v18 M19 12a7 7 0 1 1-14 0 7 7 0 0 1 14 0',
  network: 'M2 8a16 16 0 0 1 20 0 M5 12a11 11 0 0 1 14 0 M8 16a6 6 0 0 1 8 0 M12 20h.01',
  planet: 'M19 9a8 8 0 1 0-4 10 M21 3C9 4-2 17 2 20S26 8 21 3Z',
}
</script>
<template>
  <svg
    class="ui-icon"
    viewBox="0 0 24 24"
    fill="none"
    stroke="currentColor"
    stroke-width="1.8"
    stroke-linecap="round"
    stroke-linejoin="round"
    aria-hidden="true"
  >
    <path :d="paths[name] || paths.control" />
  </svg>
</template>
