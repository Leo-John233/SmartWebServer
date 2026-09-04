import { delay, http, HttpResponse } from 'msw'
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
]
