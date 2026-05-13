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
        row.info.vendorName,
        row.info.productName,
        row.info.serial,
        row.info.vendorId,
        row.info.productId,
        row.validTo
      ]"
      variant="whitelist"
      editable-date
      @delete="remove(row.id)"
      @update-date="updateDate(row.id, $event)"
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

const load = () => {
  store.loadWhitelist()
}

const goToList = () => {
  router.push('/list/')
}

const remove = (id: number) => {
  store.removeDeviceFromWhitelist(id)
}

const updateDate = (
  id: number,
  value: string
) => {
  store.updateValid(id, value)
}

onMounted(() => {
  load()
})

const columns: TableColumn[] = [
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
  },
  {
    key: 'validTo',
    title: 'Доверено до',
    width: '2fr'
  }
]
</script>