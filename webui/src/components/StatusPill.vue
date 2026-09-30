<script setup lang="ts">
import { computed } from 'vue'
import { useUiLocale } from '../i18n'
const ui = useUiLocale()
import type { ConnectionState, MountActivity } from '../domain/device'

const props = defineProps<{ connection: ConnectionState; activity: MountActivity }>()
const label = computed(() => (props.connection === 'connected' ? props.activity : props.connection))
</script>

<template>
  <span
    class="status-pill"
    :class="[connection, activity]"
    >{{ ui.t(label) }}</span
  >
</template>
