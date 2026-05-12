<template>
  <div
    class="list-row"
    :style="gridStyle"
  >
    <div
      v-for="(value, index) in values"
      :key="index"
      class="list-row__item"
    >
      {{ value }}
    </div>

    <div class="list-row__action">
      <img
        :src="icon"
        class="icon"
        @click="handleClick"
      />
    </div>
  </div>
</template>

<script setup lang="ts">
import { computed } from 'vue'

import { MODE } from '@/models/Device'
import type { TableColumn } from '@/models/TableColumn'

const props = defineProps<{
  values: string[]
  columns: TableColumn[]
  mode?: MODE
  variant: 'device' | 'whitelist'
}>()

const emit = defineEmits<{
  (e: 'add'): void
  (e: 'delete'): void
}>()

const gridStyle = computed(() => ({
  gridTemplateColumns: `${props.columns.map(c => c.width).join(' ')} 60px`
}))

const icon = computed(() => {
  if (props.variant === 'whitelist') {
    return new URL('@/assets/icons/trash.png', import.meta.url).href
  }

  return props.mode === MODE.RW
    ? new URL('@/assets/icons/check.png', import.meta.url).href
    : new URL('@/assets/icons/plus.png', import.meta.url).href
})

const handleClick = () => {
  if (props.variant === 'whitelist') {
    emit('delete')
    return
  }

  if (props.mode === MODE.RO) {
    emit('add')
  }
}
</script>

<style scoped lang="scss">
@use '@/assets/styles/variables.scss' as *;

.list-row {
  display: grid;
  width: 100%;
}

.list-row__item {
  display: flex;
  align-items: center;
  justify-content: center;

  padding: 12px;

  background-color: $c-lst-background;

  border-bottom: $s-stroke solid $c-border;
  border-right: $s-stroke solid $c-border;

  &:first-child {
    border-left: $s-stroke solid $c-border;
  }
}

.list-row__action {
  width: 60px;

  display: flex;
  align-items: center;
  justify-content: center;

  background-color: $c-lst-background;

  border-bottom: $s-stroke solid $c-border;
  border-right: $s-stroke solid $c-border;
}

.icon {
  width: 18px;
  height: 18px;
  object-fit: contain;
  cursor: pointer;
}
</style>