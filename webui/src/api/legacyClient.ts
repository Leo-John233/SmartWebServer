import type { DeviceSnapshot, MountActivity, UiConfig } from '../domain/device'
import { safeUiConfig } from '../domain/device'
import { mergeLegacyDocuments, parseLegacyText, type LegacyDocument } from '../protocol/legacyText'

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
  const status = `${document.values.gto_status ?? ''} ${document.values.hp_sta ?? ''} ${document.values.track ?? ''}`.toLowerCase()
  if (status.includes('fault') || status.includes('error')) return 'fault'
  if (status.includes('limit')) return 'limit'
  if (status.includes('slew') || document.enabled.gto_active) return 'goto'
  if (status.includes('parked')) return 'parked'
  if (document.selected.trk_on || (status.includes('tracking') && !status.includes('tracking off'))) return 'tracking'
  return 'idle'
}

export class LegacyClient {
  private readonly fetcher: typeof fetch
  private readonly scenario: () => string

  constructor(options: LegacyClientOptions = {}) {
    this.fetcher = options.fetcher ?? ((input, init) => globalThis.fetch(input, init))
    this.scenario = options.scenario ?? (() => 'normal')
  }

  private async request(path: string): Promise<Response> {
    return this.fetcher(path, { headers: { 'X-WebUI-Scenario': this.scenario() } })
  }

  async getConfig(): Promise<UiConfig> {
    try {
      const response = await this.request('/ui-config.json')
      if (!response.ok) return safeUiConfig
      return await response.json() as UiConfig
    } catch {
      return safeUiConfig
    }
  }

  async getSnapshot(config: UiConfig): Promise<DeviceSnapshot> {
    const requests = ['/index-ajax-get.txt', '/mount-ajax-get.txt'].map(async (path) => {
      const response = await this.request(path)
      if (!response.ok) throw new Error(`Legacy endpoint failed: ${path}`)
      return parseLegacyText(await response.text())
    })
    const document = mergeLegacyDocuments(...await Promise.all(requests))
    if (document.entries.length === 0) throw new Error('Legacy endpoints returned no state')
    const now = Date.now()

    return {
      controller: {
        connection: 'connected',
        activity: inferActivity(document),
        product: textValue(document, ['fw_name', 'controller']),
        firmwareVersion: config.firmwareVersion || textValue(document, ['fw_ver']),
        controllerVersion: config.controller.version || textValue(document, ['version']),
        workload: textValue(document, ['workload', 'work']),
        weather: textValue(document, ['weather', 'temp']),
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
