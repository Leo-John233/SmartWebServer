// 对照原版 Strings_en.h 和控制器协议维护参数术语
const terms: Record<string, string> = {
  'Steps/degree': '每度步数',
  'Min limit, degs': '最小位置限位（°）',
  'Max limit, degs': '最大位置限位（°）',
  'Steps/um': '每微米步数',
  'Min limit, um': '最小位置限位（μm）',
  'Max limit, um': '最大位置限位（μm）',
  Reverse: '电机方向反转',
  'Reverse direction': '电机方向反转',
  Microsteps: '运行微步细分',
  'Microsteps Goto': '转向微步细分',
  'Decay mode': '运行电流衰减模式',
  'Decay mode Goto': '转向电流衰减模式',
  'mA Hold': '保持电流（mA）',
  'mA Run': '运行电流（mA）',
  'mA Goto': '转向电流（mA）',
  '256x Interpolate': '256 微步插值',
  'P Tracking': '跟踪比例增益 P',
  'I Tracking': '跟踪积分增益 I',
  'D Tracking': '跟踪微分增益 D',
  'P Slewing': '转向比例增益 P',
  'I Slewing': '转向积分增益 I',
  'D Slewing': '转向微分增益 D',
  'Rads/count': '每编码器计数对应弧度',
  'Steps/count ratio': '步数与编码器计数比',
  'Max accel, %/s/s': '最大加速度（%/s²）',
  'Min power, %': '最小输出功率（%）',
  'Max power, %': '最大输出功率（%）',
  'Mount type': '支架类型',
  'German equatorial': '德式赤道仪',
  'Equatorial fork': '叉式赤道仪',
  'Alt-azimuth': '经纬仪',
  'Mount select 1 GEM, 2 EQ Fork, or 3 Alt/Azm':
    '支架类型（1 德式赤道仪 / 2 叉式赤道仪 / 3 经纬仪）',
  Mixed: '混合衰减',
  SpreadCycle: 'SpreadCycle 精密斩波',
  StealthChop: 'StealthChop 静音斩波',
  Upload: '应用参数',
  'Revert to Defaults': '恢复默认参数',
  'Reboot OnStep': '重启控制器',
  'Wipe all settings and Reboot OnStep': '清除全部设置并重启',
  'Enable Advanced Configuration': '启用高级配置',
  'Disable Advanced Configuration (all defaults on reboot)': '禁用高级配置（重启后恢复默认值）',
}

export function translateFirmwareText(text: string): string | undefined {
  const clean = text
    .replace(/\s+/g, ' ')
    .trim()
    .replace(/[:：!]$/, '')
  const immediate = clean.endsWith(' (i)')
  const base = immediate ? clean.slice(0, -4) : clean
  if (terms[base]) return terms[base] + (immediate ? '（立即生效）' : '')
  const axis = base.match(/^Axis\s*(\d+)(?: (RA\/Azm|Dec\/Alt|Rotator|Focuser\s*(\d+)))?$/)
  if (axis) {
    const role =
      axis[2] === 'RA/Azm'
        ? '赤经 / 方位轴'
        : axis[2] === 'Dec/Alt'
          ? '赤纬 / 高度轴'
          : axis[2] === 'Rotator'
            ? '旋转器'
            : axis[3]
              ? `调焦器 ${axis[3]}`
              : ''
    return `轴 ${axis[1]}` + (role ? ` · ${role}` : '')
  }
  return undefined
}
