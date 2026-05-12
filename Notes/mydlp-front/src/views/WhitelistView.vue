<template>
  <div class="page">
    <div class="actions">
      <BaseButton
        text="Подключённые устройства"
        @click="goToList"
      />

      <BaseButton
        text="Доверенные устройства"
        @click="load"
      />
    </div>

    <ListHeader
      :columns="columns"
      :has-action="true"
    />

    <ListRow
      v-for="row in store.whitelist"
      :key="row.id"
      :columns="columns"
      :values="[
        row.info.manufacturer,
        row.info.name,
        row.info.serial,
        row.info.vendorId,
        row.info.productId,
        row.trustedUntil.toISOString().split('T')[0] ?? ''
      ]"
      variant="whitelist"
      @delete="remove(row.id)"
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

import type { TableColumn } from '@/models/TableColumn'

const store = useDeviceStore()
const router = useRouter()

const columns: TableColumn[] = [
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
    title: 'Идентификатор производителя',
    width: '1fr'
  },
  {
    key: 'productId',
    title: 'Идентификатор продукта',
    width: '1fr'
  },
  {
    key: 'trustedUntil',
    title: 'Доверено до',
    width: '2fr'
  }
]

/**
 * load whitelist
 */
const load = () => {
  store.loadWhitelist()
}

/**
 * navigate back to devices list
 */
const goToList = () => {
  router.push('/list/')
}

/**
 * remove device from whitelist
 */
const remove = (id: number) => {
  store.removeDeviceFromWhitelist(id)
}

/**
 * initial load
 */
onMounted(() => {
  store.loadWhitelist()
})
</script>