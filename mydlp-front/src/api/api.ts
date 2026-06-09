import type { Device } from '@/models/Device'
import type { WhitelistDevice } from '@/models/WhitelistDevice'

const base: string = '/api/v1'

export const fetchDevices = async (): Promise<Device[]> => {
  const response = await fetch(`${base}/list`)

  if (!response.ok) {
    throw new Error('Failed to fetch devices')
  }

  return response.json()
}

export const fetchWhitelist = async (): Promise<WhitelistDevice[]> => {
  const response = await fetch(`${base}/whitelist`)

  if (!response.ok) {
    throw new Error('Failed to fetch whitelist')
  }

  return response.json()
}

export const addToWhitelist = async (
  device: Device
): Promise<number> => {
  const response = await fetch(
    `${base}/whitelist`,
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

export const removeFromWhitelist = async (
  id: number
): Promise<void> => {
  const response = await fetch(
    `${base}/whitelist/${id}`,
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
    `${base}/whitelist/${id}`,
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