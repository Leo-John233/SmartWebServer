export interface FirmwareField {
  name: string
  label: string
  type: string
  value: string
  checked: boolean
  disabled: boolean
  min?: string
  max?: string
  step?: string
  options: { value: string; label: string }[]
}
export interface FirmwareForm {
  title: string
  path: string
  fields: FirmwareField[]
  command?: boolean
  buttons: { name: string; value: string; label: string }[]
}
const allowedPaths = ['/index.htm', '/mount.htm', '/net.htm']

// 只读取原固件的表单描述，不执行返回页面中的脚本
export function parseFirmwareForms(html: string, path: string): FirmwareForm[] {
  if (!allowedPaths.includes(path)) throw new Error('Unsupported settings page')
  const doc = new DOMParser().parseFromString(html, 'text/html')
  const forms: FirmwareForm[] = [...doc.querySelectorAll('form')].flatMap((form, index) => {
    const target = new URL(form.getAttribute('action') || path, 'http://device' + path)
    if (target.origin !== 'http://device' || !allowedPaths.includes(target.pathname)) return []
    const fields = [
      ...form.querySelectorAll<HTMLInputElement | HTMLSelectElement>('input[name], select[name]'),
    ]
      .map((input) => {
        const before = input.previousSibling?.textContent?.trim() ?? ''
        const after = input.nextSibling?.textContent?.trim() ?? ''
        const label =
          input.labels?.[0]?.textContent?.trim() ||
          (before.length > 2 ? before : after) ||
          input.name
        return {
          name: input.name,
          label: label.slice(0, 100),
          type: input instanceof HTMLSelectElement ? 'select' : input.type,
          value: input.type === 'password' ? '' : input.value,
          checked: input instanceof HTMLInputElement && input.checked,
          disabled: input.disabled,
          min: input.getAttribute('min') ?? undefined,
          max: input.getAttribute('max') ?? undefined,
          step: input.getAttribute('step') ?? undefined,
          options:
            input instanceof HTMLSelectElement
              ? [...input.options].map((o) => ({ value: o.value, label: o.textContent ?? o.value }))
              : [],
        }
      })
      .filter((f) => !['submit', 'button'].includes(f.type))
    const buttons = [
      ...form.querySelectorAll<HTMLButtonElement>('button[type="submit"], button:not([type])'),
    ].map((b) => ({ name: b.name, value: b.value, label: b.textContent?.trim() || 'Save' }))
    if (!fields.length && !buttons.some((b) => b.name)) return []
    const title =
      form.closest('.b1')?.firstElementChild?.textContent?.trim() ||
      fields.find((f) => f.type !== 'hidden')?.label ||
      buttons[0]?.label ||
      String(index + 1)
    return [
      {
        title,
        path: target.pathname,
        fields,
        buttons: buttons.length ? buttons : [{ name: '', value: '', label: 'Save' }],
      },
    ]
  })
  if (path === '/index.htm') {
    for (const button of doc.querySelectorAll('button[onpointerdown]')) {
      const match = button
        .getAttribute('onpointerdown')
        ?.match(/s\('((?:boot|svax|sgax))','([a-z0-9]+)'\)/)
      if (!match) continue
      forms.push({
        title: button.closest('.b1')?.firstElementChild?.textContent?.trim() || 'Controller',
        path: '/index-ajax-get.txt',
        fields: [],
        buttons: [
          { name: match[1], value: match[2], label: button.textContent?.trim() || match[1] },
        ],
        command: true,
      } as FirmwareForm)
    }
  }
  return forms
}

// 按浏览器表单规则提交，未选中的复选框不发送
export function firmwareFormValues(
  form: FirmwareForm,
  button: FirmwareForm['buttons'][number],
): URLSearchParams {
  const values = new URLSearchParams()
  for (const field of form.fields) {
    if (field.disabled || (field.type === 'checkbox' && !field.checked)) continue
    values.append(field.name, field.value)
  }
  if (button.name) values.set(button.name, button.value)
  return values
}
