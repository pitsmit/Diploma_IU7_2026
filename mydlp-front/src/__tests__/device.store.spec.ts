import {
  describe,
  it,
  expect,
  vi,
  beforeEach
} from 'vitest'

import {
  setActivePinia,
  createPinia
} from 'pinia'

import { useDeviceStore } from '@/stores/device.store'

import {
  fetchWhitelist,
  removeFromWhitelist,
  updateValidTo
} from '@/api/api'

vi.mock('@/api/api', () => ({
  fetchDevices: vi.fn(),
  fetchWhitelist: vi.fn(),
  addToWhitelist: vi.fn(),
  removeFromWhitelist: vi.fn(),
  updateValidTo: vi.fn()
}))

vi.mock('@/api/websocket', () => ({
  ws: {}
}))

import {
  fetchDevices,
  addToWhitelist
} from '@/api/api'

describe('device store', () => {
  beforeEach(() => {
    setActivePinia(createPinia())
  })

  it('loads devices', async () => {
    vi.mocked(fetchDevices).mockResolvedValue([
      {
        mountPoint: '/mnt/usb',
        mode: 'RO',
        info: {
          serial: '123'
        }
      }
    ] as any)

    const store = useDeviceStore()

    await store.loadDevices()

    expect(store.devices.length)
      .toBe(1)

    expect(store.devices[0].mountPoint)
      .toBe('/mnt/usb')
  })

  it('adds device to whitelist', async () => {
    vi.mocked(addToWhitelist)
      .mockResolvedValue(1)

    const store = useDeviceStore()

    store.devices = [
      {
        mode: 'RO',
        info: {
          serial: '123'
        }
      }
    ] as any

    await store.addDeviceToWhitelist({
      info: {
        serial: '123'
      }
    } as any)

    expect(store.whitelist.length)
      .toBe(1)

    expect(store.devices[0].mode)
      .toBe('RW')
  })
})

it('loads whitelist', async () => {
  vi.mocked(fetchWhitelist).mockResolvedValue([
    {
      id: 1,
      validTo: null,

      info: {
        serial: '123'
      }
    }
  ] as any)

  const store = useDeviceStore()

  await store.loadWhitelist()

  expect(store.whitelist.length)
    .toBe(1)

  expect(store.whitelist[0].validTo)
    .toBe('Бессрочно')
})

it('does not duplicate whitelist device', async () => {
  vi.mocked(addToWhitelist)
    .mockResolvedValue(1)

  const store = useDeviceStore()

  store.whitelist = [
    {
      id: 1,

      info: {
        serial: '123'
      },

      validTo: 'Бессрочно'
    }
  ] as any

  await store.addDeviceToWhitelist({
    info: {
      serial: '123'
    }
  } as any)

  expect(store.whitelist.length)
    .toBe(1)
})

it('adds to whitelist even if device not found in devices list', async () => {
  vi.mocked(addToWhitelist)
    .mockResolvedValue(1)

  const store = useDeviceStore()

  store.devices = []

  await store.addDeviceToWhitelist({
    info: {
      serial: '123'
    }
  } as any)

  expect(store.whitelist.length)
    .toBe(1)
})

it('removes device from whitelist', async () => {
  vi.mocked(removeFromWhitelist)
    .mockResolvedValue(undefined)

  const store = useDeviceStore()

  store.whitelist = [
    {
      id: 1,
      info: {
        serial: '123'
      }
    },
    {
      id: 2,
      info: {
        serial: '456'
      }
    }
  ] as any

  await store.removeDeviceFromWhitelist(1)

  expect(store.whitelist.length)
    .toBe(1)

  expect(store.whitelist[0].id)
    .toBe(2)
})

it('updates validTo', async () => {
  vi.mocked(updateValidTo)
    .mockResolvedValue(undefined)

  const store = useDeviceStore()

  store.whitelist = [
    {
      id: 1,
      validTo: 'Бессрочно',

      info: {
        serial: '123'
      }
    }
  ] as any

  await store.updateValid(
    1,
    '2026-12-31'
  )

  expect(store.whitelist[0].validTo)
    .toBe('2026-12-31')
})

it('does nothing if whitelist target not found', async () => {
  vi.mocked(updateValidTo)
    .mockResolvedValue(undefined)

  const store = useDeviceStore()

  store.whitelist = []

  await store.updateValid(
    999,
    '2026-12-31'
  )

  expect(store.whitelist.length)
    .toBe(0)
})