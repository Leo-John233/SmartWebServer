import replayData from '../../fixtures/serial-replays.json'
import type { Capabilities, MountActivity, ProtocolFamily, UiConfig } from '../domain/device'

export type ScenarioId = 'normal' | 'disconnected' | 'goto' | 'tracking' | 'parked' | 'fault' | 'limit' | 'minimal-subsystems' | 'all-subsystems'

interface ReplayScenario {
  id: string
  controller: { protocolFamily: ProtocolFamily; version: string; name: string }
  expected: { safeReadOnly: boolean; capabilities: Omit<Capabilities, 'encoders'> }
}

export interface MockScenario {
  id: ScenarioId
  label: string
  disconnected?: boolean
  config: UiConfig
  controllerText: string
  mountText: string
  activity: MountActivity
}

const replays = replayData.scenarios as ReplayScenario[]

function replay(id: string): ReplayScenario {
  const match = replays.find((item) => item.id === id)
  if (!match) throw new Error(`Missing replay fixture: ${id}`)
  return match
}

function configFromReplay(id: string, overrides: Partial<Capabilities> = {}): UiConfig {
  const source = replay(id)
  return {
    schemaVersion: 1,
    firmwareVersion: '2.10h',
    controller: source.controller,
    locale: 'en',
    pollIntervalMs: 500,
    capabilities: { ...source.expected.capabilities, encoders: false, ...overrides },
    safeReadOnly: source.expected.safeReadOnly,
  }
}

function controllerText(message = ''): string {
  return [
    'fw_name|Smart Web Server',
    'fw_ver&2.10h',
    'workload|18%',
    'weather|12.4 °C / 42%',
    'axis_summary|Axis 1 ready · Axis 2 ready',
    `message|${message}`,
  ].join('\n')
}

function mountText(activity: MountActivity): string {
  const goto = activity === 'goto'
  const tracking = activity === 'tracking'
  const parked = activity === 'parked'
  const status = activity === 'fault' ? 'Fault: motor driver' : activity === 'limit' ? 'Limit reached' : goto ? 'Slewing || East' : 'Inactive || Best'
  return [
    `hp_sta|${parked ? 'Parked' : 'At home'}`,
    `gto_status|${status}`,
    `gto_active|${goto ? 'enabled' : 'disabled'}`,
    `track|${tracking ? 'Tracking sidereal' : 'Tracking off'}`,
    `trk_on|${tracking ? 'selected' : 'unselected'}`,
    `trk_off|${tracking ? 'unselected' : 'selected'}`,
    'guide_sta|Inactive',
    'gto_t1|05:35:17',
    'gto_t2|-05° 23′ 28″',
  ].join('\n')
}

function makeScenario(id: ScenarioId, label: string, activity: MountActivity, replayId = 'onstep-4.24-connected', overrides: Partial<Capabilities> = {}): MockScenario {
  const message = activity === 'fault' ? 'Controller reported a fault' : activity === 'limit' ? 'A configured travel limit is active' : ''
  return { id, label, activity, config: configFromReplay(replayId, overrides), controllerText: controllerText(message), mountText: mountText(activity) }
}

export const mockScenarios: MockScenario[] = [
  makeScenario('normal', 'OnStep 4.24 normal', 'idle'),
  { ...makeScenario('disconnected', 'Connection interrupted', 'idle'), disconnected: true },
  makeScenario('goto', 'Goto in progress', 'goto', 'onstepx-10.20-connected'),
  makeScenario('tracking', 'Sidereal tracking', 'tracking', 'onstepx-10.15-connected'),
  makeScenario('parked', 'Mount parked', 'parked'),
  makeScenario('fault', 'Controller fault', 'fault', 'onstepx-10.26-connected'),
  makeScenario('limit', 'Travel limit', 'limit', 'onstepx-10.28-connected'),
  makeScenario('minimal-subsystems', 'Mount only', 'idle', 'onstep-4.24-connected', { focuserCount: 0 }),
  makeScenario('all-subsystems', 'All subsystems', 'tracking', 'onstepx-10.28-connected', { rotator: true, focuserCount: 6, auxiliary: true, encoders: true }),
]

export function getMockScenario(id: string | null): MockScenario {
  return mockScenarios.find((scenario) => scenario.id === id) ?? mockScenarios[0]
}
