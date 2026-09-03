import { describe, expect, it } from 'vitest'
import replayData from '../fixtures/serial-replays.json'
import { mockScenarios } from '../src/mocks/scenarios'

describe('mock scenarios', () => {
  it('covers operational and failure states', () => {
    expect(mockScenarios.map((scenario) => scenario.id)).toEqual(expect.arrayContaining([
      'normal', 'disconnected', 'goto', 'tracking', 'parked', 'fault', 'limit', 'minimal-subsystems', 'all-subsystems',
    ]))
  })

  it('reuses every connected compatibility boundary', () => {
    const versions = mockScenarios.map((scenario) => scenario.config.controller.version)
    for (const replay of replayData.scenarios.filter((item) => item.transport.connected && !item.expected.safeReadOnly)) {
      expect(versions).toContain(replay.controller.version)
    }
  })

  it('receives feature boundaries as capabilities', () => {
    const boundary = mockScenarios.find((scenario) => scenario.config.controller.version === '10.28a')
    expect(boundary?.config.capabilities.meridianFlipHomeModes).toBe(true)
  })
})
