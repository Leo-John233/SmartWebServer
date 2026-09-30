import type { UiConfig } from './device'

// 名称只依据控制器协议识别结果，不能用网页服务端版本替代控制器版本
export function controllerIdentity(controller: UiConfig['controller']) {
  const name =
    controller.protocolFamily === 'onstepx'
      ? 'OnStepX'
      : controller.protocolFamily === 'onstep4'
        ? 'OnStep'
        : ''
  return { name, version: name ? controller.version.trim() : '' }
}
