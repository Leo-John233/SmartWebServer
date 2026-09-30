import { mount } from '@vue/test-utils'
import { describe, expect, it } from 'vitest'
import TelemetryTrace from '../src/components/TelemetryTrace.vue'

describe('telemetry trace', () => {
  it('uses the original StallGuard range and threshold', () => {
    const wrapper = mount(TelemetryTrace, {
      props: {
        label: 'StallGuard',
        value: '240',
        updatedAt: 1000,
        range: 'stallguard',
        threshold: '120',
      },
    })
    expect(wrapper.text()).toContain('1023')
    expect(wrapper.text()).not.toContain('-1023')
    expect(wrapper.find('.threshold-line').exists()).toBe(true)
    expect(Number(wrapper.find('circle').attributes('cy'))).toBeGreaterThan(100)
  })
  it('spaces samples by timestamp and breaks lines for missing values', async () => {
    const wrapper = mount(TelemetryTrace, {
      props: { label: 'Error', value: '1', updatedAt: 1000 },
    })
    await wrapper.setProps({ value: '2', updatedAt: 2000 })
    await wrapper.setProps({ value: '', updatedAt: 3000 })
    await wrapper.setProps({ value: '1', updatedAt: 5000 })
    const path = wrapper.find('path').attributes('d') ?? ''
    expect(path.match(/M/g)).toHaveLength(2)
    expect(path.match(/L/g)).toHaveLength(1)
    expect(path).toContain('L188.0,')
    expect(wrapper.text()).toContain('−4 s')
    expect(wrapper.find('circle').attributes('cx')).toBe('584')
  })

  it('does not invent a sample without a timestamp', () => {
    const wrapper = mount(TelemetryTrace, { props: { label: 'Error', value: '0' } })
    expect(wrapper.text()).toContain('等待遥测数据')
    expect(wrapper.find('circle').exists()).toBe(false)
  })
})
