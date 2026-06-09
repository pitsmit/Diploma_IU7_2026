import type { DeviceInfo } from '@/models/DeviceInfo'

export type WhitelistDevice = {
  id: number
  info: DeviceInfo
  validTo: string
}