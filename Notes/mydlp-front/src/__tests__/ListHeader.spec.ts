import { describe, it, expect } from 'vitest'
import { mount } from '@vue/test-utils'

import ListHeader from '@/components/ListHeader.vue'

describe('ListHeader', () => {
  const columns = [
    {
      key: 'name',
      title: 'Имя',
      width: '1fr'
    },
    {
      key: 'serial',
      title: 'Serial',
      width: '2fr'
    }
  ]

  it('renders columns', () => {
    const wrapper = mount(ListHeader, {
      props: {
        columns
      }
    })

    expect(wrapper.text())
      .toContain('Имя')

    expect(wrapper.text())
      .toContain('Serial')
  })

  it('renders action column', () => {
    const wrapper = mount(ListHeader, {
      props: {
        columns,
        hasAction: true
      }
    })

    expect(
      wrapper.find('.list-header__action').exists()
    ).toBe(true)
  })
})