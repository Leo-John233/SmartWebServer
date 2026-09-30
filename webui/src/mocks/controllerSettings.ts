import type { UiConfig } from '../domain/device'

// 模拟原版按已连接附件逐轴生成表单的行为，参数名称保留固件格式
export function controllerSettingsPage(config: UiConfig): string {
  const { mount, rotator, focuserCount } = config.capabilities
  const axes = [
    ...(mount ? [1, 2] : []),
    ...(rotator ? [3] : []),
    ...Array.from({ length: focuserCount }, (_, index) => index + 4),
  ]
  const modern =
    config.controller.protocolFamily === 'onstepx' &&
    Number.parseFloat(config.controller.version) >= 10.26
  const controller = `<div class="b1"><div>Controller</div><form action="/index.htm" method="get">
    ${modern ? '<label>Mount type<select name="mountt"><option value="1">German equatorial</option><option value="2">Equatorial fork</option><option value="3">Alt-azimuth</option></select></label><button type="submit">Save</button>' : ''}
    </form><button type="button" onpointerdown="s('boot','reset')">Reboot OnStep</button></div>`
  return (
    controller +
    axes
      .map((axis) => {
        const title =
          axis === 1
            ? 'Axis1 RA/Azm'
            : axis === 2
              ? 'Axis2 Dec/Alt'
              : axis === 3
                ? 'Axis3 Rotator'
                : `Axis${axis} Focuser${axis - 3}`
        if (!modern)
          return `<div class="b1"><div>${title}</div>Editing of this axis has been disabled.</div>`
        const linear = axis >= 4
        const parameters = [
          [linear ? 'Steps/um' : 'Steps/degree', linear ? 80 : 12800, 0.001, 1000000],
          [linear ? 'Min limit, um' : 'Min limit, degs', linear ? 0 : -180, -360000, 360000],
          [linear ? 'Max limit, um' : 'Max limit, degs', linear ? 50000 : 180, -360000, 360000],
          ['Reverse', 'boolean'],
          ['Microsteps', 'microsteps'],
          ['Microsteps Goto', 'microsteps'],
          ['mA Hold', 300, 0, 3000],
          ['mA Run', 800, 0, 3000],
          ['mA Goto', 1000, 0, 3000],
          ['Decay mode', 'decay'],
          ['Decay mode Goto', 'decay'],
          ['256x Interpolate', 'boolean'],
        ]
        const fields = parameters
          .map(([label, value, min, max], index) => {
            const name = `a${axis}p${index + 1}value`
            if (typeof value === 'number')
              return `<input type="number" name="${name}" value="${value}" min="${min}" max="${max}" step="any">&nbsp;${label}<br>`
            const options =
              value === 'boolean'
                ? [
                    [0, 'Off'],
                    [1, 'On'],
                  ]
                : value === 'decay'
                  ? [
                      [1, 'Mixed'],
                      [2, 'Fast'],
                      [3, 'Slow'],
                      [4, 'SpreadCycle'],
                      [5, 'StealthChop'],
                    ]
                  : [
                      [8, '8'],
                      [16, '16'],
                      [32, '32'],
                      [64, '64'],
                    ]
            return `<select name="${name}">${options.map(([value, label]) => `<option value="${value}">${label}</option>`).join('')}</select>&nbsp;${label}<br>`
          })
          .join('')
        return `<div class="b1"><div>${title}</div><form action="/index.htm" method="get">${fields}<button type="submit">Upload</button><button type="submit" name="revert" value="${axis}">Revert to Defaults</button></form></div>`
      })
      .join('')
  )
}
