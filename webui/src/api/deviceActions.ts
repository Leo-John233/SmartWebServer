export type GuideDirection = 'north' | 'south' | 'east' | 'west'
export type SlewRate = 'verySlow' | 'slow' | 'normal' | 'fast' | 'veryFast'
export type TrackingMode = 'sidereal' | 'lunar' | 'solar' | 'king'

export type DeviceAction =
  | { type: 'mount.park' }
  | { type: 'mount.unpark' }
  | { type: 'mount.setPark' }
  | { type: 'mount.home'; mode: 'find' | 'reset' }
  | { type: 'mount.autoHome'; enabled: boolean }
  | { type: 'mount.align.start'; stars: number }
  | { type: 'mount.align.accept' }
  | { type: 'mount.align.cancel' }
  | { type: 'mount.align.refine' }
  | { type: 'mount.goto.start' }
  | { type: 'mount.goto.stop' }
  | { type: 'mount.goto.rate'; rate: SlewRate }
  | { type: 'mount.goto.flip'; mode: 'off' | 'visit' | 'pause' }
  | { type: 'mount.guide.start'; direction: GuideDirection }
  | { type: 'mount.guide.stop'; direction: GuideDirection }
  | { type: 'mount.guide.stopAll' }
  | { type: 'mount.guide.rate'; rate: number }
  | { type: 'mount.sync' }
  | { type: 'mount.tracking'; enabled: boolean }
  | { type: 'mount.tracking.mode'; mode: TrackingMode }
  | { type: 'mount.tracking.adjust'; adjustment: 'faster' | 'slower' | 'reset' }
  | { type: 'mount.pec'; command: 'play' | 'stop' | 'record' | 'clear' | 'write' }
  | { type: 'mount.coordinateOrigin' }
  | { type: 'mount.library.select'; catalog: number }
  | { type: 'mount.library.move'; direction: 'previous' | 'next' }
  | { type: 'mount.library.target' }
  | { type: 'mount.site.time'; date: Date }
  | { type: 'mount.site.location'; longitude: string; latitude: string; utcOffset: string }
  | { type: 'mount.limits'; horizon: number; overhead: number; east: number; west: number; axis1Backlash: number; axis2Backlash: number }

interface LegacyRequest {
  path: string
  params: Record<string, string>
}

const directionValue: Record<GuideDirection, string> = { north: 'n', south: 's', east: 'e', west: 'w' }
const slewRateValue: Record<SlewRate, string> = { verySlow: 'vs', slow: 's', normal: 'n', fast: 'f', veryFast: 'vf' }
const trackingModeValue: Record<TrackingMode, string> = { sidereal: 'Ts', lunar: 'Tl', solar: 'Th', king: 'Tk' }

function splitCoordinate(value: string): [string, string, string] {
  const match = value.trim().match(/^([+-]?\d+)(?:[:°*\s]+)(\d+)(?:[:′'\s]+)?(\d+)?/)
  return match ? [match[1], match[2], match[3] ?? '0'] : ['', '', '']
}

function splitOffset(value: string): [string, string] {
  const match = value.trim().match(/^([+-]?\d+)(?::(\d+))?$/)
  return match ? [match[1], match[2] ?? '0'] : ['', '']
}

export function toLegacyRequest(action: DeviceAction): LegacyRequest {
  const path = '/mount-ajax-get.txt'
  switch (action.type) {
    case 'mount.park': return { path, params: { pk: 'p' } }
    case 'mount.unpark': return { path, params: { pk: 'u' } }
    case 'mount.setPark': return { path, params: { pk: 's' } }
    case 'mount.home': return { path, params: { qb: action.mode === 'find' ? 'hc' : 'hf' } }
    case 'mount.autoHome': return { path, params: { ha: action.enabled ? '1' : '0' } }
    case 'mount.align.start': return { path, params: { al: String(action.stars) } }
    case 'mount.align.accept': return { path, params: { al: 'n' } }
    case 'mount.align.cancel': return { path, params: { al: 'q' } }
    case 'mount.align.refine': return { path, params: { rp: 'a' } }
    case 'mount.goto.start': return { path, params: { goto: 'go' } }
    case 'mount.goto.stop': return { path, params: { goto: 'stop' } }
    case 'mount.goto.rate': return { path, params: { goto: slewRateValue[action.rate] } }
    case 'mount.goto.flip': return { path, params: { goto: `mh_${action.mode}` } }
    case 'mount.guide.start': return { path, params: { dr: `${directionValue[action.direction]}1` } }
    case 'mount.guide.stop': return { path, params: { dr: `${directionValue[action.direction]}0` } }
    case 'mount.guide.stopAll': return { path, params: { dr: 'q' } }
    case 'mount.guide.rate': return { path, params: { dr: `R${Math.min(9, Math.max(0, action.rate))}` } }
    case 'mount.sync': return { path, params: { dr: 'sy' } }
    case 'mount.tracking': return { path, params: { dr: action.enabled ? 'T1' : 'T0' } }
    case 'mount.tracking.mode': return { path, params: { dr: trackingModeValue[action.mode] } }
    case 'mount.tracking.adjust': return { path, params: { tk: action.adjustment === 'faster' ? 'f' : action.adjustment === 'slower' ? '-' : 'r' } }
    case 'mount.pec': return { path, params: { pec: action.command === 'record' ? 'rec' : action.command === 'clear' ? 'clr' : action.command === 'write' ? 'wrt' : action.command } }
    case 'mount.coordinateOrigin': return { path, params: { ob: 'set' } }
    case 'mount.library.select': return { path, params: { lib_index: String(action.catalog) } }
    case 'mount.library.move': return { path, params: { lib: action.direction === 'previous' ? 'cat_prev' : 'cat_next' } }
    case 'mount.library.target': return { path, params: { lib: 'cat_select' } }
    case 'mount.site.time': return {
      path,
      params: {
        dm: String(action.date.getUTCMonth()), dd: String(action.date.getUTCDate()), dy: String(action.date.getUTCFullYear()),
        th: String(action.date.getUTCHours()), tm: String(action.date.getUTCMinutes()), ts: String(action.date.getUTCSeconds()),
      },
    }
    case 'mount.site.location': {
      const [g1, g2, g3] = splitCoordinate(action.longitude)
      const [t1, t2, t3] = splitCoordinate(action.latitude)
      const [u1, u2] = splitOffset(action.utcOffset)
      return { path, params: { g1, g2, g3, t1, t2, t3, u1, u2 } }
    }
    case 'mount.limits': return {
      path,
      params: {
        hl: String(action.horizon), ol: String(action.overhead), el: String(action.east), wl: String(action.west),
        b1: String(action.axis1Backlash), b2: String(action.axis2Backlash),
      },
    }
  }
}
