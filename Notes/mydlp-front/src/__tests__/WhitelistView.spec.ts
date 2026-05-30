import { mount } from '@vue/test-utils'
import { describe, it, expect, vi, beforeEach } from 'vitest'

import WhitelistPage from '@/views/WhitelistView.vue'

// ----------------------
// mocks store
// ----------------------
const loadWhitelistMock = vi.fn()
const removeDeviceMock = vi.fn()
const updateValidMock = vi.fn()

vi.mock('@/stores/device.store', () => ({
  useDeviceStore: () => ({
    whitelist: [
      {
        id: 1,
        validTo: '2025-12-31',
        info: {
          vendorName: 'Vendor',
          productName: 'Device',
          serial: 'ABC',
          vendorId: '111',
          productId: '222'
        }
      }
    ],
    loadWhitelist: loadWhitelistMock,
    removeDeviceFromWhitelist: removeDeviceMock,
    updateValid: updateValidMock
  })
}))

// ----------------------
// mock router
// ----------------------
const pushMock = vi.fn()

vi.mock('vue-router', () => ({
  useRouter: () => ({
    push: pushMock
  })
}))

// ----------------------
// test suite
// ----------------------
describe('WhitelistPage.vue', () => {
  beforeEach(() => {
    vi.clearAllMocks()
  })

  const factory = () =>
    mount(WhitelistPage, {
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
            template: `
              <div data-test="row">
                <button data-test="delete" @click="$emit('delete')">del</button>
                <button data-test="update" @click="$emit('update-date', '2026-01-01')">
                  update
                </button>
              </div>
            `,
            props: ['values', 'variant', 'editableDate']
          }
        }
      }
    })

  it('вызывает loadWhitelist при монтировании', () => {
    factory()
    expect(loadWhitelistMock).toHaveBeenCalledTimes(1)
  })

  it('рендерит заголовок', () => {
    const wrapper = factory()
    expect(wrapper.text()).toContain('Доверенные устройства')
  })

  it('переходит на список устройств', async () => {
    const wrapper = factory()

    const buttons = wrapper.findAll('button')
    await buttons[0].trigger('click')

    expect(pushMock).toHaveBeenCalledWith('/list/')
  })

  it('вызывает loadWhitelist по кнопке', async () => {
    const wrapper = factory()

    loadWhitelistMock.mockClear()

    const buttons = wrapper.findAll('button')
    await buttons[1].trigger('click')

    expect(loadWhitelistMock).toHaveBeenCalledTimes(2)
  })

  it('удаляет устройство из whitelist', async () => {
    const wrapper = factory()

    const row = wrapper.find('[data-test="row"]')
    await row.find('[data-test="delete"]').trigger('click')

    expect(removeDeviceMock).toHaveBeenCalledWith(1)
  })

  it('обновляет дату доверия', async () => {
    const wrapper = factory()

    const row = wrapper.find('[data-test="row"]')
    await row.find('[data-test="update"]').trigger('click')

    expect(updateValidMock).toHaveBeenCalledWith(1, '2026-01-01')
  })

  it('рендерит строки whitelist', () => {
    const wrapper = factory()
    const rows = wrapper.findAll('[data-test="row"]')

    expect(rows.length).toBe(1)
  })
})