<template>
  <div
    class="list-header"
    :style="gridStyle"
  >
    <div
      v-for="column in columns"
      :key="column.key"
      class="list-header__item"
    >
      {{ column.title }}
    </div>

    <div
      v-if="hasAction"
      class="list-header__action"
    />
  </div>
</template>

<script setup lang="ts">
import { computed } from 'vue'

import type { TableColumn } from '@/models/TableColumn'

const props = defineProps<{
  columns: TableColumn[]
  hasAction?: boolean
}>()

const gridStyle = computed(() => ({
  gridTemplateColumns: props.hasAction
    ? `${props.columns.map(c => c.width).join(' ')} 60px`
    : props.columns.map(c => c.width).join(' ')
}))
</script>

<style scoped lang="scss">
@use '@/assets/styles/variables.scss' as *;

.list-header {
  display: grid;
  width: 100%;
}

.list-header__item {
  display: flex;
  align-items: center;
  justify-content: center;

  padding: 10px;
  min-width: 0;

  background-color: $c-accent;

  color: $c-text;
  font-size: $s-txt;

  border-top: $s-stroke solid $c-border;
  border-bottom: $s-stroke solid $c-border;
  border-right: $s-stroke solid $c-border;

  overflow: hidden;
  text-align: center;

  white-space: normal;
  overflow-wrap: anywhere;
  word-break: break-word;

  &:first-child {
    border-left: $s-stroke solid $c-border;
    border-top-left-radius: $s-round;
  }
}

.list-header__action {
  display: flex;
  align-items: center;
  justify-content: center;

  background-color: $c-accent;

  border-top: $s-stroke solid $c-border;
  border-bottom: $s-stroke solid $c-border;
  border-right: $s-stroke solid $c-border;

  border-top-right-radius: $s-round;
}
</style>