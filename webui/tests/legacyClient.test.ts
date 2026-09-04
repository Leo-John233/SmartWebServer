import { describe, expect, it, vi } from 'vitest'
import { LegacyClient } from '../src/api/legacyClient'

describe('LegacyClient', () => {
  it('converts legacy payloads into typed state', async () => {
    const client = new LegacyClient({ scenario: () => 'tracking' })
    const config = await client.getConfig()
    const snapshot = await client.getSnapshot(config)
    expect(snapshot.controller.connection).toBe('connected')
    expect(snapshot.controller.activity).toBe('tracking')
    expect(snapshot.mount.targetRa).toBe('05:35:17')
    expect(snapshot.selected.trk_on).toBe(true)
  })

  it('fails closed when the device is unavailable', async () => {
    const client = new LegacyClient({ scenario: () => 'disconnected' })
    const config = await client.getConfig()
    expect(config.safeReadOnly).toBe(true)
    await expect(client.getSnapshot(config)).rejects.toThrow()
  })

  it('loads only subsystems advertised by the server', async () => {
    const client = new LegacyClient({ scenario: () => 'all-subsystems' })
    const snapshot = await client.getSnapshot(await client.getConfig())
    expect(snapshot.values.rotatorpos).toBe('12.5°')
    expect(snapshot.values.focuserpos).toBe('12450')
    expect(snapshot.values.enc_a1).toBe('120.42°')
  })

  it('uses form encoded versioned actions when available', async () => {
    const fetcher = vi.fn((input: RequestInfo | URL, init?: RequestInit) => globalThis.fetch(input, init))
    const client = new LegacyClient({ fetcher, scenario: () => 'normal' })
    await client.getConfig()
    await client.performAction({ type: 'mount.park' })
    expect(fetcher).toHaveBeenLastCalledWith('/api/v1/actions', expect.objectContaining({ method: 'POST' }))
  })

  it('keeps core state equivalent to the legacy fallback', async () => {
    const apiClient = new LegacyClient({ scenario: () => 'normal' })
    const legacyClient = new LegacyClient({
      scenario: () => 'normal',
      fetcher: (input, init) => String(input).startsWith('/api/')
        ? Promise.resolve(new Response('', { status: 404 }))
        : globalThis.fetch(input, init),
    })
    const apiSnapshot = await apiClient.getSnapshot(await apiClient.getConfig())
    const legacySnapshot = await legacyClient.getSnapshot(await legacyClient.getConfig())
    expect(apiSnapshot.mount).toEqual(legacySnapshot.mount)
  })
})
