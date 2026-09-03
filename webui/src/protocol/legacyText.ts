export type LegacyDelimiter = '|' | '&'
export type LegacyEntryKind = 'value' | 'selection' | 'availability'

export interface LegacyEntry {
  id: string
  value: string
  delimiter: LegacyDelimiter
  kind: LegacyEntryKind
}

export interface LegacyDocument {
  entries: LegacyEntry[]
  values: Record<string, string>
  selected: Record<string, boolean>
  enabled: Record<string, boolean>
}

function delimiterIndex(line: string): { index: number; delimiter: LegacyDelimiter } | null {
  const pipe = line.indexOf('|')
  const ampersand = line.indexOf('&')
  if (pipe < 0 && ampersand < 0) return null
  if (pipe < 0) return { index: ampersand, delimiter: '&' }
  if (ampersand < 0 || pipe < ampersand) return { index: pipe, delimiter: '|' }
  return { index: ampersand, delimiter: '&' }
}

function entryKind(value: string): LegacyEntryKind {
  if (value === 'selected' || value === 'unselected') return 'selection'
  if (value === 'enabled' || value === 'disabled') return 'availability'
  return 'value'
}

export function parseLegacyText(text: string): LegacyDocument {
  const document: LegacyDocument = { entries: [], values: {}, selected: {}, enabled: {} }

  for (const rawLine of text.split(/\r?\n/)) {
    const line = rawLine.trim()
    if (!line) continue
    const match = delimiterIndex(line)
    if (!match) continue
    const id = line.slice(0, match.index).trim()
    const value = line.slice(match.index + 1).trim()
    if (!id) continue
    const kind = entryKind(value)
    document.entries.push({ id, value, delimiter: match.delimiter, kind })
    if (kind === 'selection') document.selected[id] = value === 'selected'
    else if (kind === 'availability') document.enabled[id] = value === 'enabled'
    else document.values[id] = value
  }

  return document
}

export function mergeLegacyDocuments(...documents: LegacyDocument[]): LegacyDocument {
  return documents.reduce<LegacyDocument>((merged, document) => ({
    entries: [...merged.entries, ...document.entries],
    values: { ...merged.values, ...document.values },
    selected: { ...merged.selected, ...document.selected },
    enabled: { ...merged.enabled, ...document.enabled },
  }), { entries: [], values: {}, selected: {}, enabled: {} })
}
