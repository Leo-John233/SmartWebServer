import { describe, expect, it } from 'vitest'
import { parseLegacyText } from '../src/protocol/legacyText'

describe('parseLegacyText', () => {
  it('parses both delimiters and preserves value separators', () => {
    const document = parseLegacyText('name|OnStep\nmessage&Ready | tracking\n')
    expect(document.values).toEqual({ name: 'OnStep', message: 'Ready | tracking' })
    expect(document.entries.map((entry) => entry.delimiter)).toEqual(['|', '&'])
  })

  it('maps selection and availability tokens', () => {
    const document = parseLegacyText('on|selected\noff|unselected\nstart|enabled\nstop|disabled')
    expect(document.selected).toEqual({ on: true, off: false })
    expect(document.enabled).toEqual({ start: true, stop: false })
    expect(document.values).toEqual({})
  })

  it('ignores malformed and empty lines', () => {
    const document = parseLegacyText('\ninvalid\n|missing-id\nvalid|ok\n')
    expect(document.entries).toHaveLength(1)
    expect(document.values.valid).toBe('ok')
  })
})
