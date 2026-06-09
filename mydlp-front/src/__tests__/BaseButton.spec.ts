import { describe, it, expect } from 'vitest'
import { mount } from '@vue/test-utils'

import BaseButton from '@/components/BaseButton.vue'

describe('BaseButton', () => {
  // ARRANGE
  it('renders button text', () => {
    const wrapper = mount(BaseButton, {
      props: {
        text: 'Нажми'
      }
    })

    expect(wrapper.text())
      .toContain('Нажми')
  })

  it('emits click event', async () => {
    const wrapper = mount(BaseButton, {
      props: {
        text: 'Click'
      }
    })

  // ACT
  await wrapper.trigger('click')

  // ASSERT
  expect(wrapper.emitted())
      .toHaveProperty('click')
  })
})