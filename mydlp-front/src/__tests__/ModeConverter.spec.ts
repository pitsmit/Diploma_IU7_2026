import { describe, it, expect } from 'vitest'

import { modeToText } from '@/services/ModeConverter'

describe('modeToText', () => {
  it('returns text for RO', () => {
    expect(modeToText('RO'))
      .toBe('Только чтение')
  })

  it('returns text for RW', () => {
    expect(modeToText('RW'))
      .toBe('Полный доступ')
  })

  it('returns fallback text', () => {
    expect(modeToText('XXX'))
      .toBe('Неопределено')
  })
})