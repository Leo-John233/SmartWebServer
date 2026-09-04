import { delay, http, HttpResponse } from 'msw'
import { parseLegacyText } from '../protocol/legacyText'
import { getMockScenario } from './scenarios'

function scenarioFrom(request: Request) {
  return getMockScenario(request.headers.get('X-WebUI-Scenario'))
}

async function respond(request: Request, body: string): Promise<Response> {
  const scenario = scenarioFrom(request)
  await delay(35)
  if (scenario.disconnected) return HttpResponse.error()
  return HttpResponse.text(body, { headers: { 'Cache-Control': 'no-store' } })
}

export const handlers = [
  http.get('/api/v1/capabilities', async ({ request }) => {
    const scenario = scenarioFrom(request)
    await delay(35)
    if (scenario.disconnected) return HttpResponse.error()
    return HttpResponse.json(scenario.config)
  }),
  http.get('/ui-config.json', async ({ request }) => {
    const scenario = scenarioFrom(request)
    await delay(35)
    if (scenario.disconnected) return HttpResponse.error()
    return HttpResponse.json(scenario.config)
  }),
  http.get('/index-ajax-get.txt', ({ request }) => respond(request, scenarioFrom(request).controllerText)),
  http.get('/mount-ajax-get.txt', ({ request }) => respond(request, scenarioFrom(request).mountText)),
  http.get('/rotator-ajax-get.txt', ({ request }) => respond(request, scenarioFrom(request).rotatorText)),
  http.get('/focuser-ajax-get.txt', ({ request }) => respond(request, scenarioFrom(request).focuserText)),
  http.get('/auxiliary-ajax-get.txt', ({ request }) => respond(request, scenarioFrom(request).auxiliaryText)),
  http.get('/enc-ajax-get.txt', ({ request }) => respond(request, scenarioFrom(request).encoderText)),
  http.get('/api/v1/state', ({ request }) => {
    const scenario = scenarioFrom(request)
    const scope = new URL(request.url).searchParams.get('scope')
    const text = scope === 'controller' ? scenario.controllerText
      : scope === 'mount' ? scenario.mountText
        : scope === 'rotator' ? scenario.rotatorText
          : scope === 'focuser' ? scenario.focuserText
            : scope === 'auxiliary' ? scenario.auxiliaryText
              : scenario.encoderText
    const document = parseLegacyText(text)
    return respond(request, JSON.stringify({ schemaVersion: 1, scope, values: document.values, selected: document.selected, enabled: document.enabled }))
  }),
  http.post('/api/v1/actions', ({ request }) => respond(request, '{"ok":true}')),
  http.get('/api/v1/network', () => HttpResponse.json({
    schemaVersion: 1,
    authenticated: true,
    restartRequired: false,
    station: { enabled: true, ssid: 'Observatory', dhcp: true, ip: '192.168.1.44' },
    accessPoint: { enabled: true, ssid: 'SmartWebServer', channel: 6, ip: '192.168.0.1' },
  })),
  http.post('/api/v1/network', () => HttpResponse.json({ ok: true })),
]
