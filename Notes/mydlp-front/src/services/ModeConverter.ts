import { MODE } from '@/models/Device'

export const modeToText = (mode: MODE): string => {
  switch (mode) {
    case MODE.RO:
      return 'Только чтение'
    case MODE.RW:
      return 'Полный доступ'
  }
}