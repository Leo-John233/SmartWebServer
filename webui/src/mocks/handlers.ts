import { controllerSettingsPage } from './controllerSettings'
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
const controllerPages = new Map<string, string>()

export const handlers = [
  http.get('/auxiliary.htm', ({ request }) => response(request, auxiliaryPage)),
  ...Object.keys(formsByPath).map((path) =>
    http.all(path, async ({ request }) => {
      const scenario = getScenario(request)
      const pageKey = scenario.id + path
      let html =
        path === '/index.htm'
          ? (controllerPages.get(pageKey) ?? controllerSettingsPage(scenario.config))
          : formsByPath[path]
      const params =
        request.method === 'POST'
          ? new URLSearchParams(await request.text())
          : new URL(request.url).searchParams
      if (params.size) {
        const doc = new DOMParser().parseFromString(html, 'text/html')
        for (const input of doc.querySelectorAll<HTMLInputElement | HTMLSelectElement>(
          'input[name], select[name]',
        )) {
          if (!params.has(input.name)) continue
          if (input instanceof HTMLSelectElement) {
            for (const option of input.options)
              option.toggleAttribute('selected', option.value === params.get(input.name))
          } else if (input.type !== 'password') input.setAttribute('value', params.get(input.name)!)
        }
        const revertedAxis = Number(params.get('revert'))
        if (path === '/index.htm' && revertedAxis >= 1 && revertedAxis <= 9) {
          const defaults = new DOMParser().parseFromString(
            controllerSettingsPage(scenario.config),
            'text/html',
          )
          const selector = 'input[name="a' + revertedAxis + 'p1value"]'
          const original = doc.querySelector(selector)?.closest('form')
          const replacement = defaults.querySelector(selector)?.closest('form')
          replacement
            ?.querySelectorAll('input, select, button')
            .forEach((node) => node.setAttribute('disabled', ''))
          if (original && replacement) original.replaceWith(replacement)
        }
        html = doc.body.innerHTML
        if (path === '/index.htm') controllerPages.set(pageKey, html)
        else formsByPath[path] = html
      }
      return response(request, html)
    }),
  ),
  http.post('/__mock/reset', () => {
    resetMockDevice()
    controllerPages.clear()
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
          const params = new URL(request.url).searchParams
          if (scope === 'controller' && params.get('boot') === 'reset') {
            const key = getScenario(request).id + '/index.htm'
            const html = controllerPages.get(key)
            if (html) {
              const doc = new DOMParser().parseFromString(html, 'text/html')
              doc.querySelectorAll('[disabled]').forEach((node) => node.removeAttribute('disabled'))
              controllerPages.set(key, doc.body.innerHTML)
            }
          }
          applyLegacyAction(request, scope, Object.fromEntries(params))
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
