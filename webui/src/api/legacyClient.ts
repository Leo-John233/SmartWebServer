import { controllerIdentity } from '../domain/controllerIdentity'
import type { DeviceSnapshot, MountActivity, NetworkConfig, UiConfig } from '../domain/device'
import { safeUiConfig } from '../domain/device'
import { mergeLegacyDocuments, parseLegacyText, type LegacyDocument } from '../protocol/legacyText'
import { toLegacyRequest, type DeviceAction } from './deviceActions'

export interface LegacyClientOptions {
  fetcher?: typeof fetch
  scenario?: () => string
}

function textValue(document: LegacyDocument, keys: string[], fallback = '—'): string {
  for (const key of keys) {
    if (document.values[key]) return document.values[key]
  }
  return fallback
}

function inferActivity(document: LegacyDocument): MountActivity {
  const status =
    `${document.values.gto_status ?? ''} ${document.values.hp_sta ?? ''} ${document.values.track ?? ''}`.toLowerCase()
  if (status.includes('fault') || status.includes('error')) return 'fault'
  if (status.includes('limit')) return 'limit'
  if (status.includes('slew') || document.enabled.gto_active) return 'goto'
  if (status.includes('parked')) return 'parked'
  if (document.selected.trk_on || (status.includes('tracking') && !status.includes('tracking off')))
    return 'tracking'
  return 'idle'
}

export class LegacyClient {
  private readonly fetcher: typeof fetch
  private readonly scenario: () => string
  private apiAvailable = false
  // 星表和提示只下发一次，需要在后续轮询中保留
  private catalogState: Record<string, string> = {}

  constructor(options: LegacyClientOptions = {}) {
    this.fetcher = options.fetcher ?? ((input, init) => globalThis.fetch(input, init))
    this.scenario = options.scenario ?? (() => 'normal')
  }

  private async request(path: string, init: RequestInit = {}): Promise<Response> {
    const headers = new Headers(init.headers)
    headers.set('X-WebUI-Scenario', this.scenario())
    return this.fetcher(path, {
      ...init,
      headers,
      signal: init.signal ?? AbortSignal.timeout(5000),
    })
  }

  async getConfig(): Promise<UiConfig> {
    try {
      const apiResponse = await this.request('/api/v1/capabilities')
      if (apiResponse.ok) {
        this.apiAvailable = true
        return (await apiResponse.json()) as UiConfig
      }
    } catch {
      this.apiAvailable = false
    }
    try {
      const response = await this.request('/ui-config.json')
      if (!response.ok) return safeUiConfig
      return (await response.json()) as UiConfig
    } catch {
      return safeUiConfig
    }
  }

  async performAction(action: DeviceAction): Promise<void> {
    if (this.apiAvailable) {
      const body = new URLSearchParams({ action: action.type })
      for (const [key, value] of Object.entries(action)) {
        if (key !== 'type')
          body.set(key, value instanceof Date ? value.toISOString() : String(value))
      }
      const apiResponse = await this.request('/api/v1/actions', {
        method: 'POST',
        headers: { 'Content-Type': 'application/x-www-form-urlencoded' },
        body,
      })
      if (apiResponse.ok) return
      if (apiResponse.status !== 404 && apiResponse.status !== 501)
        throw new Error('Device rejected the action')
    }
    const request = toLegacyRequest(action)
    const query = new URLSearchParams(request.params)
    const response = await this.request(`${request.path}?${query}`)
    if (!response.ok) throw new Error('Device rejected the action')
  }

  async getSnapshot(config: UiConfig): Promise<DeviceSnapshot> {
    // 状态查询使用轮询地址，命令地址可能返回空响应
    const paths = ['/index.txt']
    if (config.capabilities.mount) paths.push('/mount-ajax.txt')
    if (config.capabilities.rotator) paths.push('/rotator-ajax.txt')
    if (config.capabilities.focuserCount) paths.push('/focuser-ajax.txt')
    if (config.capabilities.auxiliary) paths.push('/auxiliary-ajax.txt')
    if (config.capabilities.encoders) paths.push('/enc-ajax.txt')
    const requests = paths.map(async (path) => {
      const response = await this.request(path)
      if (!response.ok) throw new Error(`Legacy endpoint failed: ${path}`)
      return parseLegacyText(await response.text())
    })
    const document = mergeLegacyDocuments(...(await Promise.all(requests)))
    if (document.entries.length === 0) throw new Error('Legacy endpoints returned no state')
    return this.snapshotFromDocument(config, document)
  }

  async getNetwork(): Promise<NetworkConfig> {
    const response = await this.request('/api/v1/network')
    if (!response.ok) throw new Error('Network settings are unavailable')
    return (await response.json()) as NetworkConfig
  }

  async updateNetwork(values: Record<string, string | number | boolean>): Promise<void> {
    const body = new URLSearchParams()
    for (const [key, value] of Object.entries(values)) body.set(key, String(value))
    const response = await this.request('/api/v1/network', {
      method: 'POST',
      headers: { 'Content-Type': 'application/x-www-form-urlencoded' },
      body,
    })
    if (!response.ok) throw new Error('Network action failed')
  }

  private snapshotFromDocument(config: UiConfig, document: LegacyDocument): DeviceSnapshot {
    for (const key of ['cat_data', 'lib_message']) {
      if (key in document.values) this.catalogState[key] = document.values[key]
      else if (key in this.catalogState) document.values[key] = this.catalogState[key]
    }
    const now = Date.now()

    return {
      controller: {
        connection: 'connected',
        activity: inferActivity(document),
        product: controllerIdentity(config.controller).name,
        firmwareVersion: config.firmwareVersion || textValue(document, ['fw_ver']),
        controllerVersion: config.controller.version || textValue(document, ['version']),
        workload: textValue(document, ['workload', 'work_load', 'work']),
        weather: textValue(document, ['weather', 'tphd_t', 'temp']),
        axisSummary: textValue(document, ['axis_summary', 'axis']),
        message: textValue(document, ['message'], ''),
        updatedAt: now,
      },
      mount: {
        homePark: textValue(document, ['hp_sta']),
        gotoStatus: textValue(document, ['gto_status']),
        tracking: textValue(document, ['track']),
        guideStatus: textValue(document, ['guide_sta']),
        targetRa: textValue(document, ['gto_t1']),
        targetDec: textValue(document, ['gto_t2']),
      },
      values: document.values,
      selected: document.selected,
      enabled: document.enabled,
    }
  }
}
