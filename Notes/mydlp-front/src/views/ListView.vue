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
        row.mountPoint,
        modeToText(row.mode),
        row.info.vendorName,
        row.info.productName,
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

const load = () => {
  store.loadDevices()
}

const goToWhitelist = () => {
  router.push('/whitelist/')
}

const add = (device: Device) => {
  store.addDeviceToWhitelist(device)
}

onMounted(() => {
  load()
})

const columns = [
  {
    key: 'mountPoint',
    title: 'Путь монтирования',
    width: '2fr'
  },
  {
    key: 'mode',
    title: 'Режим',
    width: '1fr'
  },
  {
    key: 'vendorName',
    title: 'Производитель',
    width: '2fr'
  },
  {
    key: 'productName',
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
    title: 'Идентификатор производителя',
    width: '2fr'
  },
  {
    key: 'productId',
    title: 'Идентификатор продукта',
    width: '2fr'
  }
]
</script>