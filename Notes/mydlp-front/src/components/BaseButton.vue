<template>
  <button class="base-button" @click="$emit('click')">
    {{ text }}
  </button>
</template>

<script setup lang="ts">
defineProps<{
  text: string
}>()

defineEmits(['click'])
</script>

<style scoped lang="scss">
@use '@/assets/styles/variables.scss' as *;

.base-button {
  display: flex;
  align-items: center;
  justify-content: center;

  padding: 12px 20px;

  border: $s-stroke solid $c-border;
  border-radius: $s-round;

  background-color: $c-accent;

  color: $c-text;
  font-size: $s-txt;
  font-weight: 500;

  cursor: pointer;

  transition:
    background-color 0.2s ease,
    border-color 0.2s ease;

  &:hover {
    background-color: $c-btn-hover;
    border-color: $c-btn-hover;
  }
}
</style>