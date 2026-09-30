import { describe, expect, it } from 'vitest'
import { controllerIdentity } from '../src/domain/controllerIdentity'

describe('controller identity', () => {
  it.each([
    ['onstep4', 'OnStep'],
    ['onstepx', 'OnStepX'],
    ['unknown', ''],
  ] as const)('uses protocol identity %s', (protocolFamily, name) => {
    expect(
      controllerIdentity({ protocolFamily, version: ' 4.24 ', name: 'Smart Web Server' }),
    ).toEqual({ name, version: name ? '4.24' : '' })
  })
})
