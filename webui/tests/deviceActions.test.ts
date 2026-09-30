import { describe, expect, it } from 'vitest'
import { toLegacyRequest } from '../src/api/deviceActions'

describe('legacy action adapter', () => {
  it('maps semantic mount actions without exposing controller commands', () => {
    expect(toLegacyRequest({ type: 'mount.park' })).toEqual({
      path: '/mount-ajax-get.txt',
      params: { pk: 'p' },
    })
    expect(toLegacyRequest({ type: 'mount.guide.start', direction: 'west' }).params).toEqual({
      dr: 'w1',
    })
    expect(toLegacyRequest({ type: 'mount.tracking.mode', mode: 'sidereal' }).params).toEqual({
      dr: 'Ts',
    })
  })

  it('preserves high precision site fields for firmware-side version handling', () => {
    const request = toLegacyRequest({
      type: 'mount.site.location',
      longitude: '+120:30:45',
      latitude: '-30:15:20',
      utcOffset: '+08:00',
    })
    expect(request.params).toMatchObject({
      g1: '+120',
      g2: '30',
      g3: '45',
      t1: '-30',
      t2: '15',
      t3: '20',
    })
  })

  it('routes subsystem actions through their legacy adapters', () => {
    expect(toLegacyRequest({ type: 'rotator.position', degrees: 45 }).path).toBe(
      '/rotator-ajax-get.txt',
    )
    expect(toLegacyRequest({ type: 'focuser.select', index: 2 }).params).toEqual({ dr: 'F2' })
    expect(
      toLegacyRequest({ type: 'auxiliary.value', feature: 3, value: 2, setting: 50 }).params,
    ).toEqual({ x3v2: '50' })
    expect(toLegacyRequest({ type: 'encoders.autoSync', enabled: true }).params).toEqual({
      enc: 'on',
    })
  })
})
