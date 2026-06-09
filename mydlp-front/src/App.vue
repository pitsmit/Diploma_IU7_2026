<template>
  <AppContainer>
    <router-view />
  </AppContainer>
</template>

<script setup lang="ts">
import { onMounted } from 'vue'
import AppContainer from '@/components/AppContainer.vue'
import { useDeviceStore } from '@/stores/device.store'

const store = useDeviceStore()

onMounted(() => {
  store.initWebSocket()
})
</script>