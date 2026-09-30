import { describe, expect, it } from 'vitest'
import { parseFirmwareForms, firmwareFormValues } from '../src/protocol/firmwareForms'
import { parseAuxiliaryControls } from '../src/protocol/auxiliaryControls'
import { LegacyClient } from '../src/api/legacyClient'

describe('firmware compatibility', () => {
  it('preserves checkboxes, axis parameter names and password blanks', () => {
    const [form] = parseFirmwareForms(
      `<div class='b1'><div>Axis 1:</div><form action='/index.htm'>
      <input name='a1p1value' type='number' value='12800' min='1' max='90000'>
      <input name='flag' type='checkbox' value='1'>
      <input name='password' type='password' value='must-not-copy'>
      <button name='revert' value='1' type='submit'>Revert</button>
    </form></div>`,
      '/index.htm',
    )
    const values = firmwareFormValues(form, form.buttons[0])
    expect(values.get('a1p1value')).toBe('12800')
    expect(values.has('flag')).toBe(false)
    expect(values.get('password')).toBe('')
    expect(values.get('revert')).toBe('1')
    expect(form.fields[0].max).toBe('90000')
  })
  it('does not accept external form destinations or execute page scripts', () => {
    expect(
      parseFirmwareForms(
        `<script>throw new Error('executed')</script><form action='https://example.org'><input name='login'></form>`,
        '/net.htm',
      ),
    ).toEqual([])
  })
  it('preserves heater ranges and switch command values from firmware', () => {
    const [feature] = parseAuxiliaryControls(
      `<div class='b1'><div>Heater:</div><button onpointerdown="s('x3v1','1')">On</button><div>Zero</div><div><input type='range' min='-10' max='40' value='5' onchange="sz('x3v2',this.value)"></div></div>`,
    )
    expect(feature.id).toBe(3)
    expect(feature.switches[0].value).toBe(1)
    expect(feature.controls[0]).toMatchObject({ key: 'x3v2', min: -10, max: 40, value: 5 })
  })
  it('uses polling endpoints, never command endpoints, to retrieve state', async () => {
    const paths: string[] = []
    const client = new LegacyClient({
      fetcher: (input, init) => {
        paths.push(String(input))
        return fetch(input, init)
      },
    })
    await client.getSnapshot(await client.getConfig())
    expect(paths).toContain('/index.txt')
    expect(paths).toContain('/mount-ajax.txt')
    expect(paths.some((path) => path.includes('-ajax-get.txt'))).toBe(false)
  })
  it('simulates an action and reports its changed state', async () => {
    const client = new LegacyClient({ scenario: () => 'all-subsystems' })
    const config = await client.getConfig()
    await client.performAction({ type: 'mount.goto.start' })
    expect((await client.getSnapshot(config)).controller.activity).toBe('goto')
    await client.performAction({ type: 'mount.goto.stop' })
    expect((await client.getSnapshot(config)).controller.activity).not.toBe('goto')
    await client.performAction({ type: 'focuser.position', steps: 14000 })
    expect((await client.getSnapshot(config)).values.focuserpos).toBe('14000')
  })
})
