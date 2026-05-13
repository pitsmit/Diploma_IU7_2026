import { defineStore } from 'pinia'
import { ref } from 'vue'

import type { Device } from '@/models/Device'
import type { WhitelistDevice } from '@/models/WhitelistDevice'

import {
  fetchDevices,
  fetchWhitelist,
  addToWhitelist,
  removeFromWhitelist,
  updateValidTo
} from '@/api/api'

export const useDeviceStore = defineStore('device', () => {
  /**
   * state
   */
  const devices = ref<Device[]>([])
  const whitelist = ref<WhitelistDevice[]>([])
  const loading = ref(false)

  const loadDevices = async () => {
    loading.value = true

    const data = await fetchDevices()

    devices.value = data.map((d) => ({
      ...d,
      info: { ...d.info }
    }))

    loading.value = false
  }

  const loadWhitelist = async () => {
    loading.value = true

    const data = await fetchWhitelist()

    whitelist.value = data.map((w) => ({
      ...w,
      info: { ...w.info },
      validTo: w.validTo ? w.validTo : 'Бессрочно'
    }))

    loading.value = false
  }

  const addDeviceToWhitelist = async (device: Device) => {
    const id = await addToWhitelist(device)

    const exists = whitelist.value.some(
      (w) => w.info.serial === device.info.serial
    )

    if (!exists) {
      whitelist.value.push({
        id: id,
        info: { ...device.info },
        validTo: 'Бессрочно'
      })
    }

    const target = devices.value.find(
      (d) => d.info.serial === device.info.serial
    )

    if (target) {
      target.mode = "RW"
    }
  }

  const removeDeviceFromWhitelist = async (id: number) => {
    await removeFromWhitelist(id)

    whitelist.value = whitelist.value.filter(
      (w) => w.id !== id
    )
  }

  const updateValid = async (id: number, validTo: string) => {
    await updateValidTo(id, validTo)

    const target = whitelist.value.find(
      (w) => w.id === id
    )

    if (target) {
      target.validTo = validTo
    }
  }

  return {
    devices,
    whitelist,
    loading,

    loadDevices,
    loadWhitelist,

    addDeviceToWhitelist,
    removeDeviceFromWhitelist,
    updateValid
  }
})