import type { Device } from '@/models/Device'
import type { WhitelistDevice } from '@/models/WhitelistDevice'

/*const mockDevices: Device[] = [
  {
    mountPoint: '/dev/sda1',
    mode: MODE.RO,
    info: {
      vendorName: 'Samsввввввввввввввукаааааааааааааааааааааааааааааааung',
      productName: 'SSD 870',
      serial: 'SN123456',
      vendorId: '0x1A2B',
      productId: '0x3C4D'
    }
  },
  {
    mountPoint: '/dev/sdb1',
    mode: MODE.RO,
    info: {
      vendorName: 'Kingston',
      productName: 'USB Drive',
      serial: 'SN987654',
      vendorId: '0x5E6F',
      productId: '0x7A8B'
    }
  }
]
let mockWhitelist: WhitelistDevice[] = []*/

/**
 * GET DEVICES
 */
export const fetchDevices = async (): Promise<Device[]> => {
  const response = await fetch('http://localhost:8080/api/v1/list')

  if (!response.ok) {
    throw new Error('Failed to fetch devices')
  }

  return response.json()
}

/**
 * GET WHITELIST
 */
export const fetchWhitelist = async (): Promise<WhitelistDevice[]> => {
  const response = await fetch('http://localhost:8080/api/v1/whitelist')

  if (!response.ok) {
    throw new Error('Failed to fetch whitelist')
  }

  return response.json()
}

/**
 * ADD TO WHITELIST
 */
export const addToWhitelist = async (
  device: Device
): Promise<number> => {
  const response = await fetch(
    'http://localhost:8080/api/v1/whitelist',
    {
      method: 'POST',
      headers: {
        'Content-Type': 'application/json'
      },
      body: JSON.stringify(device)
    }
  )

  if (!response.ok) {
    throw new Error('Failed to add device to whitelist')
  }

  const data = await response.json()

  return data.id
}

/**
 * REMOVE FROM WHITELIST
 */
export const removeFromWhitelist = async (
  id: number
): Promise<void> => {
  const response = await fetch(
    `http://localhost:8080/api/v1/whitelist/${id}`,
    {
      method: 'DELETE'
    }
  )

  if (!response.ok) {
    throw new Error('Failed to remove device from whitelist')
  }
}

export const updateValidTo = async (
  id: number,
  validTo: string
): Promise<void> => {
  const response = await fetch(
    `http://localhost:8080/api/v1/whitelist/${id}`,
    {
      method: 'PATCH',
      headers: {
        'Content-Type': 'application/json'
      },
      body: JSON.stringify({ validTo })
    }
  )

  if (!response.ok) {
    throw new Error('Failed to update validTo')
  }
}