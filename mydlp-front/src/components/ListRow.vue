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
      <template v-if="editableDate && index === values.length - 1">
      <input
        type="date"
        class="date-input"
        :min="minDate"
        :value="value === 'Бессрочно' ? '' : value"
        @change="emit('update-date', ($event.target as HTMLInputElement).value)"
      />
    </template>

    <template v-else>
      {{ value }}
    </template>
    </div>

    <div 
    :title="tooltipText" 
    @click="handleClick"
    class="list-row__action">
      <img
        :src="icon"
        class="icon"
      />
    </div>
  </div>
</template>

<script setup lang="ts">
import { computed } from 'vue'

import type { TableColumn } from '@/models/TableColumn'

const props = defineProps<{
  values: string[]
  columns: TableColumn[]
  mode?: string
  variant: 'device' | 'whitelist'

  editableDate?: boolean
}>()

const emit = defineEmits<{
  (e: 'add'): void
  (e: 'delete'): void
  (e: 'update-date', value: string): void
}>()

const minDate = computed(() => {
  return new Date().toISOString().split('T')[0]
})

const gridStyle = computed(() => ({
  gridTemplateColumns: `${props.columns.map(c => c.width).join(' ')} 60px`
}))

const icon = computed(() => {
  if (props.variant === 'whitelist') {
    return new URL('@/assets/icons/trash.png', import.meta.url).href
  }

  return props.mode === "RW"
    ? new URL('@/assets/icons/check.png', import.meta.url).href
    : new URL('@/assets/icons/plus.png', import.meta.url).href
})

const handleClick = () => {
  if (props.variant === 'whitelist') {
    emit('delete')
    return
  }

  if (props.mode === "RO") {
    emit('add')
  }
}

const tooltipText = computed(() => {
  if (props.variant === 'whitelist') {
    return 'Удалить'
  }

  return props.mode === "RW"
    ? 'Уже добавлено'
    : 'Добавить'
})
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

  padding: 10px;
  white-space: normal;
  overflow-wrap: anywhere;
  word-break: break-word;
  text-align: center;
  min-width: 0;

  background-color: $c-lst-background;

  border-bottom: $s-stroke solid $c-border;
  border-right: $s-stroke solid $c-border;

  &:first-child {
    border-left: $s-stroke solid $c-border;
  }
}

.list-row__action {
  display: flex;
  align-items: center;
  justify-content: center;

  background-color: $c-lst-background;

  border-bottom: $s-stroke solid $c-border;
  border-right: $s-stroke solid $c-border;

  cursor: pointer;
}

.icon {
  width: 18px;
  height: 18px;
  object-fit: contain;
}

.date-input {
  width: 100%;
  border: none;
  background: transparent;
  text-align: center;
  font: inherit;
  outline: none;
}
</style>