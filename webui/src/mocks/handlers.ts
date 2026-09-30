import { delay, http, HttpResponse } from 'msw'
import { toLegacyRequest, type DeviceAction } from '../api/deviceActions'
import {
  getScenario,
  getScenarioState,
  applyLegacyAction,
  serializeLegacyState,
  resetMockDevice,
  scopes,
  type Scope,
} from './mockDevice'
import { formsByPath, auxiliaryPage } from './mockPages'

// 网络处理与模拟设备状态分开，便于单独添加和调试接口
async function response(request: Request, body: unknown) {
  await delay(35)
  if (getScenario(request).disconnected) return HttpResponse.error()
  return typeof body === 'string'
    ? HttpResponse.text(body)
    : HttpResponse.json(body as Record<string, unknown>)
}
const network = {
  schemaVersion: 1,
  authenticated: true,
  restartRequired: false,
  station: { enabled: true, ssid: 'Observatory', dhcp: true, ip: '192.168.1.44' },
  accessPoint: { enabled: true, ssid: 'SmartWebServer', channel: 6, ip: '192.168.0.1' },
}
export const handlers = [
  http.get('/auxiliary.htm', ({ request }) => response(request, auxiliaryPage)),
  ...Object.keys(formsByPath).map((path) =>
    http.all(path, async ({ request }) => {
      if (request.method === 'POST') {
        const params = new URLSearchParams(await request.text())
        const doc = new DOMParser().parseFromString(formsByPath[path], 'text/html')
        for (const input of doc.querySelectorAll<HTMLInputElement>('input[name]')) {
          if (input.type !== 'password' && params.has(input.name))
            input.setAttribute('value', params.get(input.name)!)
          if (input.type === 'checkbox') input.toggleAttribute('checked', params.has(input.name))
        }
        formsByPath[path] = doc.body.innerHTML
      }
      return response(request, formsByPath[path])
    }),
  ),
  http.post('/__mock/reset', () => {
    resetMockDevice()
    return HttpResponse.json({ ok: true })
  }),
  ...['/api/v1/capabilities', '/ui-config.json'].map((path) =>
    http.get(path, ({ request }) => response(request, getScenario(request).config)),
  ),
  ...scopes.flatMap((scope) =>
    ['get', 'poll'].map((mode) =>
      http.get(
        mode === 'poll' && scope === 'controller'
          ? '/index.txt'
          : '/' +
              (scope === 'controller' ? 'index' : scope === 'encoders' ? 'enc' : scope) +
              (mode === 'poll' ? '-ajax.txt' : '-ajax-get.txt'),
        ({ request }) => {
          applyLegacyAction(request, scope, Object.fromEntries(new URL(request.url).searchParams))
          return response(request, serializeLegacyState(getScenarioState(request)[scope]))
        },
      ),
    ),
  ),
  http.get('/api/v1/state', ({ request }) => {
    const scope = new URL(request.url).searchParams.get('scope') as Scope
    return response(request, { schemaVersion: 1, scope, ...getScenarioState(request)[scope] })
  }),
  http.post('/api/v1/actions', async ({ request }) => {
    const params = new URLSearchParams(await request.text())
    const action: Record<string, unknown> = { type: params.get('action') }
    for (const [key, value] of params)
      if (key !== 'action')
        action[key] =
          value === 'true'
            ? true
            : value === 'false'
              ? false
              : key === 'date'
                ? new Date(value)
                : value
    const legacy = toLegacyRequest(action as DeviceAction)
    if (!legacy) return HttpResponse.json({ ok: false }, { status: 501 })
    const scope = action.type!.toString().split('.')[0] as Scope
    applyLegacyAction(request, scope, legacy.params)
    return response(request, { ok: true })
  }),
  http.get('/api/v1/network', ({ request }) => response(request, network)),
  http.post('/api/v1/network', async ({ request }) => {
    const params = new URLSearchParams(await request.text())
    if (params.get('action') === 'logout') network.authenticated = false
    else if (params.get('action') === 'login') network.authenticated = !!params.get('password')
    else if (!network.authenticated) return HttpResponse.json({ ok: false }, { status: 401 })
    else if (params.get('action') === 'save') {
      Object.assign(network.station, {
        enabled: params.get('stationEnabled') === 'true',
        ssid: params.get('stationSsid') ?? '',
        dhcp: params.get('stationDhcp') === 'true',
      })
      Object.assign(network.accessPoint, {
        enabled: params.get('accessPointEnabled') === 'true',
        ssid: params.get('accessPointSsid') ?? '',
        channel: Number(params.get('accessPointChannel')),
      })
      network.restartRequired = true
    } else if (params.get('action') === 'reboot') network.restartRequired = false
    return response(request, { ok: true })
  }),
]
