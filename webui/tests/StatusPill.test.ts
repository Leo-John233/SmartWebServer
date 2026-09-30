import { mount } from '@vue/test-utils'
import { describe, expect, it } from 'vitest'
import StatusPill from '../src/components/StatusPill.vue'

describe('StatusPill', () => {
  it('shows activity for a connected device', () => {
    const wrapper = mount(StatusPill, { props: { connection: 'connected', activity: 'goto' } })
    expect(wrapper.text()).toBe('正在转向')
    expect(wrapper.classes()).toContain('goto')
  })

  it('prioritizes a disconnected state', () => {
    const wrapper = mount(StatusPill, {
      props: { connection: 'disconnected', activity: 'tracking' },
    })
    expect(wrapper.text()).toBe('连接中断')
  })
})
