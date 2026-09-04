import { computed, ref } from 'vue'

export type UiLocale = 'en' | 'zh'

const translations: Record<string, string> = {
  'Observatory console': '天文台控制台',
  Scenario: '场景',
  Controller: '控制器',
  Mount: '赤道仪',
  Rotator: '旋转器',
  Focuser: '调焦器',
  Auxiliary: '辅助设备',
  Encoders: '编码器',
  Network: '网络',
  'Legacy UI': '旧版界面',
  'Controller overview': '控制器概览',
  'Observation control': '观测控制',
  'Camera orientation': '相机方向',
  'Focus control': '调焦控制',
  'Power & accessories': '供电与附件',
  'Axis feedback': '轴反馈',
  Connectivity: '连接设置',
  'Stop all motion': '停止全部运动',
  Stop: '停止',
  Save: '保存',
  'Save settings': '保存设置',
  On: '开启',
  Off: '关闭',
  'Find home': '寻找原点',
  Park: '驻停',
  Unpark: '解除驻停',
  'Start goto': '开始转向',
  'Sync here': '同步当前位置',
  'State is stale · motion controls are locked': '状态已过期 · 运动控制已锁定',
  'Device configuration is unavailable, actions remain disabled': '设备配置不可用，操作保持禁用',
  'Device connection interrupted': '设备连接已中断',
}

const savedLocale = localStorage.getItem('smart-webui-locale')
const locale = ref<UiLocale>(savedLocale === 'zh' ? 'zh' : 'en')

export function useUiLocale() {
  function setLocale(value: UiLocale): void {
    locale.value = value
    localStorage.setItem('smart-webui-locale', value)
    document.documentElement.lang = value === 'zh' ? 'zh-CN' : 'en'
  }

  return {
    locale: computed(() => locale.value),
    t: (text: string) => locale.value === 'zh' ? translations[text] ?? text : text,
    setLocale,
    toggleLocale: () => setLocale(locale.value === 'zh' ? 'en' : 'zh'),
  }
}
