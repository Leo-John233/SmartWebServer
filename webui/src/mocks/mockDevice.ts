import { parseLegacyText, type LegacyDocument } from '../protocol/legacyText'
import { getMockScenario } from './scenarios'

export type Scope = 'controller' | 'mount' | 'rotator' | 'focuser' | 'auxiliary' | 'encoders'
// 每个模拟场景独立保存状态，切换场景时互不影响
const states = new Map<string, Record<Scope, LegacyDocument>>()
const catalogs = new Map<string, string>()
export const scopes: Scope[] = [
  'controller',
  'mount',
  'rotator',
  'focuser',
  'auxiliary',
  'encoders',
]
export function getScenario(request: Request) {
  return getMockScenario(request.headers.get('X-WebUI-Scenario'))
}
export function getScenarioState(request: Request) {
  const source = getScenario(request)
  if (!states.has(source.id)) {
    const texts = [
      source.controllerText,
      source.mountText,
      source.rotatorText,
      source.focuserText,
      source.auxiliaryText,
      source.encoderText,
    ]
    const documents = Object.fromEntries(
      scopes.map((scope, index) => [scope, parseLegacyText(texts[index])]),
    ) as Record<Scope, LegacyDocument>
    Object.assign(documents.mount.values, {
      gto_i1: '05:32:10',
      gto_i2: '-04° 50′ 12″',
      gto_az1: '152° 18′',
      gto_az2: '48° 20′',
      gto_rate: 'Normal',
      guide_rate: '1x',
      align_progress: 'Ready',
      pec_sta: 'Stopped',
      cat_message: 'No object',
      lib_free: '200',
      site_long: '+000:00:00',
      site_lat: '+00:00:00',
    })
    if (source.id === 'all-subsystems')
      Object.assign(documents.controller.values, {
        svoD: '0.4',
        svoP: '20',
        units: 'asec',
        sgV: '240',
        sgT: '120',
        sgS: 'Armed',
      })
    states.set(source.id, documents)
  }
  return states.get(source.id)!
}
export function setToggle(document: LegacyDocument, on: string, off: string, value: boolean) {
  document.selected[on] = value
  document.selected[off] = !value
}
// 将原固件参数转换为模拟状态变化，不发送任何真实设备命令
export function applyLegacyAction(
  request: Request,
  scope: Scope,
  params: Record<string, string>,
): boolean {
  const document = getScenarioState(request)[scope]
  const values = document.values
  let accepted = false
  for (const [key, value] of Object.entries(params)) {
    accepted = true
    if (key === 'goto') {
      if (value === 'go' || value === 'mp_cnt') {
        values.gto_status = 'Slewing'
        document.enabled.gto_active = true
      } else if (value === 'stop') {
        values.gto_status = 'Inactive'
        values.guide_sta = 'Inactive'
        document.enabled.gto_active = false
      } else if (['vs', 's', 'n', 'f', 'vf'].includes(value)) values.gto_rate = value
      else document.selected['goto_' + value] = true
    } else if (key === 'pk') {
      values.hp_sta = value === 'p' ? 'Parked' : value === 'u' ? 'Ready' : 'Park position saved'
      if (value === 'p') {
        values.track = 'Tracking off'
        setToggle(document, 'trk_on', 'trk_off', false)
      }
    } else if (key === 'qb') values.hp_sta = 'At home'
    else if (key === 'ha') setToggle(document, 'auto_on', 'auto_off', value === '1')
    else if (key === 'al')
      values.align_progress =
        value === 'n' ? 'Star accepted' : value === 'q' ? 'Cancelled' : 'Aligning: ' + value
    else if (key === 'rp') values.align_progress = 'Polar refinement'
    else if (key === 'dr' && /^F[1-6]$/.test(value)) {
      for (let i = 1; i <= 6; i++) document.selected['foc' + i + '_sel'] = value === 'F' + i
      values.foc_sel = 'Focuser ' + value[1]
    } else if (key === 'dr' && /^T/.test(value)) {
      const enabled = value !== 'T0'
      values.track = enabled
        ? 'Tracking ' +
          ({ Ts: 'sidereal', Tl: 'lunar', Th: 'solar', Tk: 'King' }[value] ?? 'sidereal')
        : 'Tracking off'
      setToggle(document, 'trk_on', 'trk_off', enabled)
    } else if (key === 'dr' && /^R/.test(value)) values.guide_rate = value.slice(1)
    else if (key === 'dr')
      values.guide_sta = value.endsWith('1') ? 'Moving ' + value[0] : 'Inactive'
    else if (key === 'pec')
      values.pec_sta =
        { play: 'Playing', stop: 'Stopped', rec: 'Recording', clr: 'Cleared', wrt: 'Saved' }[
          value
        ] ?? value
    else if (key === 'lib_index') {
      values.catalog = value
      values.cat_message = Number(value) ? 'M42 · Orion nebula' : 'No object'
      values.cat_data = ''
    } else if (key === 'lib') {
      if (value === 'cat_download')
        values.cat_data = (
          catalogs.get(getScenario(request).id + ':' + values.catalog) ??
          '$Observing\nM42,DN,05:35:17,-05*23:28'
        ).replaceAll('\n', '\r')
      else if (value === 'clear') {
        for (const k of catalogs.keys())
          if (k.startsWith(getScenario(request).id + ':')) catalogs.delete(k)
        values.cat_data = ''
        values.cat_message = 'No object'
        values.lib_message = 'Library cleared'
      } else if (value === 'cat_select') {
        values.gto_t1 = '05:35:17'
        values.gto_t2 = '-05° 23′ 28″'
        values.lib_message = 'Target selected'
      } else
        values.cat_message = values.cat_message.startsWith('M42')
          ? 'M31 · Andromeda galaxy'
          : 'M42 · Orion nebula'
    } else if (key === 'cat_upload') {
      const text = value === 'DELETE' ? '' : value.replaceAll('_', ' ')
      catalogs.set(getScenario(request).id + ':' + values.catalog, text)
      values.cat_data = text.replaceAll('\n', '\r')
      values.lib_message = 'Catalog saved'
    } else if (key === 'rs') values.rotatorpos = value + '°'
    else if (key === 'fs') values.focuserpos = value
    else if (key === 'rot' || key === 'foc') {
      const prefix = key === 'rot' ? 'rot' : 'foc'
      if (['stop', 'home', 'reset'].includes(value)) values[prefix + '_sta'] = 'Inactive'
      else if (['vs', 's', 'n', 'f', 'vf'].includes(value)) values[prefix + '_rate'] = value
      else if (value === 'on' || value === 'off')
        setToggle(document, 'rot_on', 'rot_off', value === 'on')
      else {
        values[prefix + '_sta'] = 'Moving'
        if (key === 'foc')
          values.focuserpos = String(
            Number(values.focuserpos) + (value.startsWith('in') ? -100 : 100),
          )
        else
          values.rotatorpos =
            String(parseFloat(values.rotatorpos) + (value.startsWith('ccw') ? -1 : 1)) + '°'
      }
    } else if (key.startsWith('foc_')) values[key.replace('foc_', 'f_')] = value
    else if (/^x\d+v\d$/.test(key)) {
      values[key] = value
      if (key.endsWith('v1'))
        setToggle(document, 'sw' + key[1] + '_on', 'sw' + key[1] + '_off', Number(value) > 0)
    } else if (key === 'enc') {
      if (value === 'on' || value === 'off')
        setToggle(document, 'enc_as_on', 'enc_as_off', value === 'on')
      else {
        values.diff_a1 = '0.00°'
        values.diff_a2 = '0.00°'
      }
    } else values[key] = value
  }
  return accepted
}
export function serializeLegacyState(document: LegacyDocument) {
  return [
    ...Object.entries(document.values).map(([k, values]) => k + '|' + values),
    ...Object.entries(document.selected).map(
      ([k, values]) => k + '|' + (values ? 'selected' : 'unselected'),
    ),
    ...Object.entries(document.enabled).map(
      ([k, values]) => k + '|' + (values ? 'enabled' : 'disabled'),
    ),
  ].join('\n')
}

export function resetMockDevice(): void {
  states.clear()
  catalogs.clear()
}
