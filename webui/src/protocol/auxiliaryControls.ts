export interface AuxiliaryControl {
  key: string
  label: string
  value: number
  min: number
  max: number
}
export interface AuxiliaryFeature {
  id: number
  name: string
  switches: { label: string; value: number }[]
  controls: AuxiliaryControl[]
}
// 沿用固件下发的范围和单位，避免除露与快门参数发生偏差
export function parseAuxiliaryControls(html: string): AuxiliaryFeature[] {
  const document = new DOMParser().parseFromString(html, 'text/html')
  return [...document.querySelectorAll('.b1')].flatMap((tile) => {
    const controls: AuxiliaryControl[] = []
    const switches: AuxiliaryFeature['switches'] = []
    let id = 0
    for (const input of tile.querySelectorAll<HTMLInputElement>('input[type="range"]')) {
      const match = input.getAttribute('onchange')?.match(/sz\('x([1-8])v([1-4])',this.value\)/)
      if (!match) continue
      id = Number(match[1])
      const label = input.parentElement?.previousElementSibling?.textContent?.trim() || 'Output'
      controls.push({
        key: 'x' + match[1] + 'v' + match[2],
        label,
        value: Number(input.value),
        min: Number(input.min),
        max: Number(input.max),
      })
    }
    for (const button of tile.querySelectorAll('button')) {
      const match = button.getAttribute('onpointerdown')?.match(/s\('x([1-8])v1','([01])'\)/)
      if (!match) continue
      id = Number(match[1])
      switches.push({
        label: button.textContent?.trim() || (match[2] === '1' ? 'On' : 'Off'),
        value: Number(match[2]),
      })
    }
    return id
      ? [
          {
            id,
            name: tile.firstElementChild?.textContent?.trim() || String(id),
            switches,
            controls,
          },
        ]
      : []
  })
}
