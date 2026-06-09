export const modeToText = (mode: string): string => {
  switch (mode) {
    case "RO":
      return 'Только чтение'
    case "RW":
      return 'Полный доступ'
  }
  return "Неопределено"
}