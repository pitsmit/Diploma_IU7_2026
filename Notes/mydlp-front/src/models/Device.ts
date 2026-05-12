import type { DeviceInfo } from '@/models/DeviceInfo'

export enum MODE {
  RO = 0,
  RW = 1
}

export type Device = {
  mountpath: string
  mode: MODE
  info: DeviceInfo
}