import { describe, expect, it } from 'vitest'
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
})
