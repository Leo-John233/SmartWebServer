export type GuideDirection = 'north' | 'south' | 'east' | 'west'
export type SlewRate = 'verySlow' | 'slow' | 'normal' | 'fast' | 'veryFast'
export type TrackingMode = 'sidereal' | 'lunar' | 'solar' | 'king'

export type DeviceAction =
  | { type: 'mount.option'; key: 'goto' | 'rr' | 'hc1' | 'hc2' | 'hr' | 'spwr'; value: string }
  | { type: 'mount.library.download' }
  | { type: 'mount.library.upload'; text: string }
  | { type: 'mount.library.clear' }
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
  | {
      type: 'mount.limits'
      horizon: number
      overhead: number
      east: number
      west: number
      axis1Backlash: number
      axis2Backlash: number
    }
  | { type: 'rotator.move'; direction: 'counterclockwise' | 'clockwise'; fast: boolean }
  | { type: 'rotator.stop' }
  | { type: 'rotator.home'; mode: 'find' | 'reset' }
  | { type: 'rotator.position'; degrees: number }
  | { type: 'rotator.rate'; rate: SlewRate }
  | { type: 'rotator.derotate'; command: 'on' | 'off' | 'reverse' | 'parallactic' }
  | { type: 'rotator.backlash'; steps: number }
  | { type: 'focuser.select'; index: number }
  | { type: 'focuser.move'; direction: 'in' | 'out'; fast: boolean }
  | { type: 'focuser.stop' }
  | { type: 'focuser.home'; mode: 'find' | 'reset' }
  | { type: 'focuser.position'; steps: number }
  | { type: 'focuser.rate'; rate: SlewRate }
  | {
      type: 'focuser.settings'
      backlash: number
      compensationEnabled: boolean
      deadband: number
      coefficient: number
    }
  | { type: 'auxiliary.value'; feature: number; value: 1 | 2 | 3 | 4; setting: number }
  | { type: 'encoders.sync'; direction: 'controllerToEncoder' | 'encoderToController' | 'origin' }
  | { type: 'encoders.autoSync'; enabled: boolean }

interface LegacyRequest {
  path: string
  params: Record<string, string>
}

const directionValue: Record<GuideDirection, string> = {
  north: 'n',
  south: 's',
  east: 'e',
  west: 'w',
}
const slewRateValue: Record<SlewRate, string> = {
  verySlow: 'vs',
  slow: 's',
  normal: 'n',
  fast: 'f',
  veryFast: 'vf',
}
const trackingModeValue: Record<TrackingMode, string> = {
  sidereal: 'Ts',
  lunar: 'Tl',
  solar: 'Th',
  king: 'Tk',
}

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
    case 'mount.option':
      return { path, params: { [action.key]: action.value } }
    case 'mount.library.download':
      return { path, params: { lib: 'cat_download' } }
    case 'mount.library.upload':
      return { path, params: { cat_upload: action.text.replaceAll(' ', '_') || 'DELETE' } }
    case 'mount.library.clear':
      return { path, params: { lib: 'clear' } }

    case 'mount.park':
      return { path, params: { pk: 'p' } }
    case 'mount.unpark':
      return { path, params: { pk: 'u' } }
    case 'mount.setPark':
      return { path, params: { pk: 's' } }
    case 'mount.home':
      return { path, params: { qb: action.mode === 'find' ? 'hc' : 'hf' } }
    case 'mount.autoHome':
      return { path, params: { ha: action.enabled ? '1' : '0' } }
    case 'mount.align.start':
      return { path, params: { al: String(action.stars) } }
    case 'mount.align.accept':
      return { path, params: { al: 'n' } }
    case 'mount.align.cancel':
      return { path, params: { al: 'q' } }
    case 'mount.align.refine':
      return { path, params: { rp: 'a' } }
    case 'mount.goto.start':
      return { path, params: { goto: 'go' } }
    case 'mount.goto.stop':
      return { path, params: { goto: 'stop' } }
    case 'mount.goto.rate':
      return { path, params: { goto: slewRateValue[action.rate] } }
    case 'mount.goto.flip':
      return { path, params: { goto: `mh_${action.mode}` } }
    case 'mount.guide.start':
      return { path, params: { dr: `${directionValue[action.direction]}1` } }
    case 'mount.guide.stop':
      return { path, params: { dr: `${directionValue[action.direction]}0` } }
    case 'mount.guide.stopAll':
      return { path, params: { dr: 'q' } }
    case 'mount.guide.rate':
      return { path, params: { dr: `R${Math.min(9, Math.max(0, action.rate))}` } }
    case 'mount.sync':
      return { path, params: { dr: 'sy' } }
    case 'mount.tracking':
      return { path, params: { dr: action.enabled ? 'T1' : 'T0' } }
    case 'mount.tracking.mode':
      return { path, params: { dr: trackingModeValue[action.mode] } }
    case 'mount.tracking.adjust':
      return {
        path,
        params: {
          tk: action.adjustment === 'faster' ? 'f' : action.adjustment === 'slower' ? '-' : 'r',
        },
      }
    case 'mount.pec':
      return {
        path,
        params: {
          pec:
            action.command === 'record'
              ? 'rec'
              : action.command === 'clear'
                ? 'clr'
                : action.command === 'write'
                  ? 'wrt'
                  : action.command,
        },
      }
    case 'mount.coordinateOrigin':
      return { path, params: { ob: 'set' } }
    case 'mount.library.select':
      return { path, params: { lib_index: String(action.catalog) } }
    case 'mount.library.move':
      return { path, params: { lib: action.direction === 'previous' ? 'cat_prev' : 'cat_next' } }
    case 'mount.library.target':
      return { path, params: { lib: 'cat_select' } }
    case 'mount.site.time':
      return {
        path,
        params: {
          dm: String(action.date.getUTCMonth()),
          dd: String(action.date.getUTCDate()),
          dy: String(action.date.getUTCFullYear()),
          th: String(action.date.getUTCHours()),
          tm: String(action.date.getUTCMinutes()),
          ts: String(action.date.getUTCSeconds()),
        },
      }
    case 'mount.site.location': {
      const [g1, g2, g3] = splitCoordinate(action.longitude)
      const [t1, t2, t3] = splitCoordinate(action.latitude)
      const [u1, u2] = splitOffset(action.utcOffset)
      return { path, params: { g1, g2, g3, t1, t2, t3, u1, u2 } }
    }
    case 'mount.limits':
      return {
        path,
        params: {
          hl: String(action.horizon),
          ol: String(action.overhead),
          el: String(action.east),
          wl: String(action.west),
          b1: String(action.axis1Backlash),
          b2: String(action.axis2Backlash),
        },
      }
    case 'rotator.move':
      return {
        path: '/rotator-ajax-get.txt',
        params: {
          rot: `${action.direction === 'counterclockwise' ? 'ccw' : 'cw'}${action.fast ? 'f' : ''}`,
        },
      }
    case 'rotator.stop':
      return { path: '/rotator-ajax-get.txt', params: { rot: 'stop' } }
    case 'rotator.home':
      return {
        path: '/rotator-ajax-get.txt',
        params: { rot: action.mode === 'find' ? 'home' : 'reset' },
      }
    case 'rotator.position':
      return { path: '/rotator-ajax-get.txt', params: { rs: String(action.degrees) } }
    case 'rotator.rate':
      return { path: '/rotator-ajax-get.txt', params: { rot: slewRateValue[action.rate] } }
    case 'rotator.derotate':
      return {
        path: '/rotator-ajax-get.txt',
        params: {
          rot:
            action.command === 'reverse'
              ? 'rev'
              : action.command === 'parallactic'
                ? 'par'
                : action.command,
        },
      }
    case 'rotator.backlash':
      return { path: '/rotator-ajax-get.txt', params: { b3: String(action.steps) } }
    case 'focuser.select':
      return { path: '/focuser-ajax-get.txt', params: { dr: `F${action.index}` } }
    case 'focuser.move':
      return {
        path: '/focuser-ajax-get.txt',
        params: { foc: `${action.direction === 'in' ? 'in' : 'out'}${action.fast ? 'f' : ''}` },
      }
    case 'focuser.stop':
      return { path: '/focuser-ajax-get.txt', params: { foc: 'stop' } }
    case 'focuser.home':
      return { path: '/focuser-ajax-get.txt', params: { dr: action.mode === 'find' ? 'Fh' : 'FH' } }
    case 'focuser.position':
      return { path: '/focuser-ajax-get.txt', params: { fs: String(action.steps) } }
    case 'focuser.rate':
      return { path: '/focuser-ajax-get.txt', params: { foc: slewRateValue[action.rate] } }
    case 'focuser.settings':
      return {
        path: '/focuser-ajax-get.txt',
        params: {
          foc_bl: String(action.backlash),
          foc_tcf_en: action.compensationEnabled ? '1' : '0',
          foc_tcf_db: String(action.deadband),
          foc_tcf_coef: String(action.coefficient),
        },
      }
    case 'auxiliary.value':
      return {
        path: '/auxiliary-ajax-get.txt',
        params: { [`x${action.feature}v${action.value}`]: String(action.setting) },
      }
    case 'encoders.sync':
      return {
        path: '/enc-ajax-get.txt',
        params: {
          enc:
            action.direction === 'controllerToEncoder'
              ? 'o2e'
              : action.direction === 'encoderToController'
                ? 'e2o'
                : 'zro',
        },
      }
    case 'encoders.autoSync':
      return { path: '/enc-ajax-get.txt', params: { enc: action.enabled ? 'on' : 'off' } }
  }
}
