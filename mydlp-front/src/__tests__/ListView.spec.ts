import { mount } from '@vue/test-utils'
import { describe, it, expect, vi, beforeEach } from 'vitest'

import DevicesPage from '@/views/ListView.vue'

// мок store
const loadDevicesMock = vi.fn()
const addDeviceToWhitelistMock = vi.fn()

vi.mock('@/stores/device.store', () => ({
  useDeviceStore: () => ({
    devices: [
      {
        mountPoint: '/dev/sda1',
        mode: 'read',
        info: {
          vendorName: 'TestVendor',
          productName: 'TestDevice',
          serial: 'ABC123',
          vendorId: '1111',
          productId: '2222'
        }
      }
    ],
    loadDevices: loadDevicesMock,
    addDeviceToWhitelist: addDeviceToWhitelistMock
  })
}))

// мок router
const pushMock = vi.fn()

vi.mock('vue-router', () => ({
  useRouter: () => ({
    push: pushMock
  })
}))

describe('DevicesPage.vue', () => {
  beforeEach(() => {
    vi.clearAllMocks()
  })

  const factory = () =>
    mount(DevicesPage, {
      global: {
        stubs: {
          BaseButton: {
            template: `<button @click="$emit('click')">{{ text }}</button>`,
            props: ['text']
          },
          PageTitle: {
            template: `<h1>{{ text }}</h1>`,
            props: ['text']
          },
          ListHeader: {
            template: `<div data-test="header"></div>`
          },
          ListRow: {
            template: `<div data-test="row" @click="$emit('add')"></div>`,
            props: ['values', 'mode']
          }
        }
      }
    })

  it('вызывает loadDevices при монтировании', async () => {
    factory()
    expect(loadDevicesMock).toHaveBeenCalled()
  })

  it('рендерит заголовок страницы', () => {
    const wrapper = factory()
    expect(wrapper.text()).toContain('Подключённые устройства')
  })

  it('нажатие кнопки "Подключённые устройства" вызывает loadDevices', async () => {
    const wrapper = factory()

    const buttons = wrapper.findAll('button')
    await buttons[0].trigger('click')

    expect(loadDevicesMock).toHaveBeenCalledTimes(3) // mount + click
  })

  it('переходит на whitelist', async () => {
    const wrapper = factory()

    const buttons = wrapper.findAll('button')
    await buttons[1].trigger('click')

    expect(pushMock).toHaveBeenCalledWith('/whitelist/')
  })

  it('вызывает addDeviceToWhitelist при клике на строку', async () => {
    const wrapper = factory()

    const row = wrapper.find('[data-test="row"]')
    await row.trigger('click')

    expect(addDeviceToWhitelistMock).toHaveBeenCalled()
  })

  it('передаёт устройства в список', () => {
    const wrapper = factory()
    const rows = wrapper.findAll('[data-test="row"]')

    expect(rows.length).toBe(1)
  })
})