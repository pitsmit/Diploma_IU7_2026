import { describe, it, expect } from 'vitest'
import { mount } from '@vue/test-utils'

import ListRow from '@/components/ListRow.vue'

describe('ListRow', () => {
  const columns = [
    {
      key: 'name',
      title: 'Имя',
      width: '1fr'
    }
  ]

  it('renders values', () => {
    const wrapper = mount(ListRow, {
      props: {
        values: ['USB Device'],
        columns,
        variant: 'device'
      }
    })

    expect(wrapper.text())
      .toContain('USB Device')
  })

  it('emits add event for RO device', async () => {
    const wrapper = mount(ListRow, {
      props: {
        values: ['USB'],
        columns,
        mode: 'RO',
        variant: 'device'
      }
    })

    await wrapper.find('.list-row__action')
      .trigger('click')

    expect(wrapper.emitted())
      .toHaveProperty('add')
  })

  it('does not emit add event for RW device', async () => {
    const wrapper = mount(ListRow, {
      props: {
        values: ['USB'],
        columns,
        mode: 'RW',
        variant: 'device'
      }
    })

    await wrapper.find('.list-row__action')
      .trigger('click')

    expect(wrapper.emitted('add'))
      .toBeUndefined()
  })

  it('emits delete event for whitelist row', async () => {
    const wrapper = mount(ListRow, {
      props: {
        values: ['USB'],
        columns,
        variant: 'whitelist'
      }
    })

    await wrapper.find('.list-row__action')
      .trigger('click')

    expect(wrapper.emitted())
      .toHaveProperty('delete')
  })
})