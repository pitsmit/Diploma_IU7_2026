<!-- components/PageTitle.vue -->
<template>
  <div class="page-title">
    {{ text }}
  </div>
</template>

<script setup lang="ts">
defineProps<{
  text: string
}>()
</script>

<style scoped lang="scss">
.page-title {
  width: 100%;

  margin-bottom: 16px;

  text-align: center;

  font-size: 24px;
  font-weight: 600;
}
</style>