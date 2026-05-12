import { defineStore } from 'pinia'
import { ref, computed } from 'vue'

import type { Device } from '@/models/Device'
import { MODE } from '@/models/Device'
import type { WhitelistDevice } from '@/models/WhitelistDevice'

import {
  fetchDevices,
  fetchWhitelist,
  addToWhitelist,
  removeFromWhitelist
} from '@/api/api'

export const useDeviceStore = defineStore('device', () => {
  /**
   * state
   */
  const devices = ref<Device[]>([])
  const whitelist = ref<WhitelistDevice[]>([])
  const loading = ref(false)

  /**
   * load devices
   */
  const loadDevices = async () => {
    loading.value = true

    const data = await fetchDevices()

    /**
     * нормализация гарантирует реактивность + безопасные объекты
     */
    devices.value = data.map((d) => ({
      ...d,
      info: { ...d.info }
    }))

    loading.value = false
  }

  /**
   * load whitelist
   */
  const loadWhitelist = async () => {
    loading.value = true

    const data = await fetchWhitelist()

    whitelist.value = data.map((w) => ({
      ...w,
      info: { ...w.info },
      trustedUntil: new Date(w.trustedUntil)
    }))

    loading.value = false
  }

  /**
   * add device → whitelist
   */
  const addDeviceToWhitelist = async (device: Device) => {
    await addToWhitelist(device)

    const exists = whitelist.value.some(
      (w) => w.info.serial === device.info.serial
    )

    if (!exists) {
      whitelist.value.push({
        id: Date.now(),
        info: { ...device.info },
        trustedUntil: new Date('2026-12-31')
      })
    }

    /**
     * обновляем mode локально (RO → RW)
     */
    const target = devices.value.find(
      (d) => d.info.serial === device.info.serial
    )

    if (target) {
      target.mode = MODE.RW
    }
  }

  /**
   * remove from whitelist
   */
  const removeDeviceFromWhitelist = async (id: number) => {
    await removeFromWhitelist(id)

    whitelist.value = whitelist.value.filter(
      (w) => w.id !== id
    )
  }

  return {
    devices,
    whitelist,
    loading,

    loadDevices,
    loadWhitelist,

    addDeviceToWhitelist,
    removeDeviceFromWhitelist
  }
})