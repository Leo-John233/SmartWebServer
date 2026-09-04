export type ProtocolFamily = 'onstep4' | 'onstepx' | 'unknown'
export type ConnectionState = 'connecting' | 'connected' | 'disconnected' | 'stale'
export type MountActivity = 'idle' | 'goto' | 'tracking' | 'parked' | 'fault' | 'limit'

export interface Capabilities {
  mount: boolean
  rotator: boolean
  focuserCount: number
  auxiliary: boolean
  encoders: boolean
  axisCount: number
  highPrecisionCoordinates: boolean
  axisConfiguration: boolean
  autoHome: boolean
  meridianFlipHomeModes: boolean
  workingSlewRate: boolean
}

export interface UiConfig {
  schemaVersion: 1
  firmwareVersion: string
  controller: {
    protocolFamily: ProtocolFamily
    version: string
    name: string
  }
  locale: 'en' | 'zh'
  pollIntervalMs: number
  capabilities: Capabilities
  safeReadOnly?: boolean
}

export interface ControllerState {
  connection: ConnectionState
  activity: MountActivity
  product: string
  firmwareVersion: string
  controllerVersion: string
  workload: string
  weather: string
  axisSummary: string
  message: string
  updatedAt: number | null
}

export interface MountState {
  homePark: string
  gotoStatus: string
  tracking: string
  guideStatus: string
  targetRa: string
  targetDec: string
}

export interface DeviceSnapshot {
  controller: ControllerState
  mount: MountState
  values: Readonly<Record<string, string>>
  selected: Readonly<Record<string, boolean>>
  enabled: Readonly<Record<string, boolean>>
}

export interface NetworkConfig {
  schemaVersion: 1
  authenticated: boolean
  restartRequired: boolean
  station: { enabled: boolean; ssid: string; dhcp: boolean; ip: string }
  accessPoint: { enabled: boolean; ssid: string; channel: number; ip: string }
}

export const disabledCapabilities: Capabilities = {
  mount: false,
  rotator: false,
  focuserCount: 0,
  auxiliary: false,
  encoders: false,
  axisCount: 0,
  highPrecisionCoordinates: false,
  axisConfiguration: false,
  autoHome: false,
  meridianFlipHomeModes: false,
  workingSlewRate: false,
}

export const safeUiConfig: UiConfig = {
  schemaVersion: 1,
  firmwareVersion: '',
  controller: { protocolFamily: 'unknown', version: '', name: '' },
  locale: 'en',
  pollIntervalMs: 1000,
  capabilities: disabledCapabilities,
  safeReadOnly: true,
}
