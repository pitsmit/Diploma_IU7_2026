<template>
  <div class="page">
    <div class="actions">
      <BaseButton text="Подключённые устройства" @click="load" />
      <BaseButton text="Доверенные устройства" @click="goToWhitelist" />
    </div>

    <ListHeader
      :columns="columns"
      :has-action="true"
    />

    <ListRow
      v-for="row in store.devices"
      :key="row.info.serial"
      :columns="columns"
      :values="[
        row.mountpath,
        modeToText(row.mode),
        row.info.manufacturer,
        row.info.name,
        row.info.serial,
        row.info.vendorId,
        row.info.productId
      ]"
      :mode="row.mode"
      variant="device"
      @add="add(row)"
    />
  </div>
</template>

<script setup lang="ts">
import { onMounted } from 'vue'
import { useRouter } from 'vue-router'

import BaseButton from '@/components/BaseButton.vue'
import ListHeader from '@/components/ListHeader.vue'
import ListRow from '@/components/ListRow.vue'

import { useDeviceStore } from '@/stores/device.store'
import type { Device } from '@/models/Device'

import { modeToText } from '@/services/ModeConverter'

const store = useDeviceStore()
const router = useRouter()

/**
 * load devices
 */
const load = () => {
  store.loadDevices()
}

/**
 * navigate to whitelist page
 */
const goToWhitelist = () => {
  router.push('/whitelist/')
}

/**
 * add device to whitelist
 */
const add = (device: Device) => {
  store.addDeviceToWhitelist(device)
}

/**
 * initial load
 */
onMounted(() => {
  store.loadDevices()
})

const columns = [
  {
    key: 'mountpath',
    title: 'Путь монтирования',
    width: '2fr'
  },
  {
    key: 'mode',
    title: 'Режим',
    width: '1fr'
  },
  {
    key: 'manufacturer',
    title: 'Производитель',
    width: '2fr'
  },
  {
    key: 'name',
    title: 'Имя устройства',
    width: '2fr'
  },
  {
    key: 'serial',
    title: 'Серийный номер',
    width: '2fr'
  },
  {
    key: 'vendorId',
    title: 'Vendor ID',
    width: '1fr'
  },
  {
    key: 'productId',
    title: 'Product ID',
    width: '1fr'
  }
]
</script>