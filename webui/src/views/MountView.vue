<script setup lang="ts">
import type { MountState } from '../domain/device'

defineProps<{ state: MountState | null; actionsAllowed: boolean }>()
</script>

<template>
  <section>
    <div class="section-heading">
      <div>
        <p class="eyebrow">Mount</p>
        <h2>Motion preview</h2>
      </div>
    </div>
    <div class="mount-layout">
      <article class="panel target-panel">
        <p class="eyebrow">Target coordinates</p>
        <div class="coordinates">
          <div><span>RA</span><strong>{{ state?.targetRa || '—' }}</strong></div>
          <div><span>DEC</span><strong>{{ state?.targetDec || '—' }}</strong></div>
        </div>
      </article>
      <article class="panel">
        <dl class="state-list">
          <div><dt>Home / Park</dt><dd>{{ state?.homePark || '—' }}</dd></div>
          <div><dt>Goto</dt><dd>{{ state?.gotoStatus || '—' }}</dd></div>
          <div><dt>Tracking</dt><dd>{{ state?.tracking || '—' }}</dd></div>
          <div><dt>Guide</dt><dd>{{ state?.guideStatus || '—' }}</dd></div>
        </dl>
      </article>
    </div>
    <p v-if="!actionsAllowed" class="read-only-note">Actions are disabled until a recognized, connected controller provides capabilities</p>
  </section>
</template>
