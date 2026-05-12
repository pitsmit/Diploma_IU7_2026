import type { Device } from '@/models/Device'
import { MODE } from '@/models/Device'
import type { WhitelistDevice } from '@/models/WhitelistDevice'

/**
 * MOCK DEVICES
 */
const mockDevices: Device[] = [
  {
    mountpath: '/dev/sda1',
    mode: MODE.RO,
    info: {
      manufacturer: 'Samsung',
      name: 'SSD 870',
      serial: 'SN123456',
      vendorId: '0x1A2B',
      productId: '0x3C4D'
    }
  },
  {
    mountpath: '/dev/sdb1',
    mode: MODE.RO,
    info: {
      manufacturer: 'Kingston',
      name: 'USB Drive',
      serial: 'SN987654',
      vendorId: '0x5E6F',
      productId: '0x7A8B'
    }
  }
]

/**
 * MOCK WHITELIST
 */
let mockWhitelist: WhitelistDevice[] = []

/**
 * GET DEVICES
 */
export const fetchDevices = (): Promise<Device[]> =>
  new Promise((resolve) => {
    setTimeout(() => resolve(structuredClone(mockDevices)), 300)
  })

/**
 * GET WHITELIST
 */
export const fetchWhitelist = (): Promise<WhitelistDevice[]> =>
  new Promise((resolve) => {
    setTimeout(() => {
      resolve(
        mockWhitelist.map((w) => ({
          ...w,
          info: { ...w.info },
          trustedUntil: new Date(w.trustedUntil)
        }))
      )
    }, 300)
  })

/**
 * ADD TO WHITELIST
 */
export const addToWhitelist = (device: Device): Promise<void> =>
  new Promise((resolve) => {
    setTimeout(() => {
      const exists = mockWhitelist.some(
        (w) => w.info.serial === device.info.serial
      )

      if (!exists) {
        mockWhitelist.push({
          id: Date.now(),
          info: { ...device.info },
          trustedUntil: new Date('2026-12-31')
        })
      }

      resolve()
    }, 200)
  })

/**
 * REMOVE FROM WHITELIST
 */
export const removeFromWhitelist = (id: number): Promise<void> =>
  new Promise((resolve) => {
    setTimeout(() => {
      mockWhitelist = mockWhitelist.filter(
        (w) => w.id !== id
      )
      resolve()
    }, 200)
  })