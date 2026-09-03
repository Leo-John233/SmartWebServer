import { readFileSync } from 'node:fs'
import { extname } from 'node:path'
import { spawnSync } from 'node:child_process'

const supportedExtensions = new Set([
  '.c', '.cc', '.cpp', '.cxx', '.h', '.hpp', '.ino',
  '.js', '.mjs', '.cjs', '.ts', '.tsx', '.vue',
  '.css', '.scss', '.html'
])

const options = new Set(process.argv.slice(2))
const cached = options.has('--cached')
const workingTree = options.has('--working-tree') || !cached

if (cached && options.has('--working-tree')) {
  console.error('Choose either --cached or --working-tree')
  process.exit(2)
}

function runGit(args, allowDifference = false) {
  const result = spawnSync('git', args, { encoding: 'utf8' })
  if (result.error) throw result.error
  if (result.status !== 0 && !(allowDifference && result.status === 1)) {
    throw new Error(result.stderr.trim() || `git ${args.join(' ')} failed`)
  }
  return result.stdout
}

function isSupported(path) {
  return supportedExtensions.has(extname(path).toLowerCase())
}

function createState() {
  return { block: null, quote: null }
}

function commentFragments(line, state) {
  const fragments = []
  let fragment = ''
  let i = 0

  while (i < line.length) {
    if (state.block === 'c') {
      const end = line.indexOf('*/', i)
      if (end === -1) {
        fragments.push(line.slice(i))
        return fragments
      }
      fragments.push(line.slice(i, end))
      state.block = null
      i = end + 2
      continue
    }

    if (state.block === 'html') {
      const end = line.indexOf('-->', i)
      if (end === -1) {
        fragments.push(line.slice(i))
        return fragments
      }
      fragments.push(line.slice(i, end))
      state.block = null
      i = end + 3
      continue
    }

    const char = line[i]
    const next = line[i + 1]

    if (state.quote) {
      if (char === '\\') {
        i += 2
        continue
      }
      if (char === state.quote) state.quote = null
      i += 1
      continue
    }

    if (char === '"' || char === "'" || char === '`') {
      state.quote = char
      i += 1
      continue
    }

    if (char === '/' && next === '/') {
      fragments.push(line.slice(i + 2))
      break
    }

    if (char === '/' && next === '*') {
      state.block = 'c'
      i += 2
      continue
    }

    if (line.startsWith('<!--', i)) {
      state.block = 'html'
      i += 4
      continue
    }

    i += 1
  }

  if (/^\s*\*(?!\/)/.test(line) && fragments.length === 0) {
    fragment = line.replace(/^\s*\*\s?/, '')
    fragments.push(fragment)
  }

  if (state.quote !== '`') state.quote = null
  return fragments
}

function hasForbiddenEnding(fragment) {
  const text = fragment.trim().replace(/\*\/$/, '').replace(/-->$/, '').trim()
  return text.length > 0 && /[.。]$/.test(text)
}

const violations = []

function inspectLine(path, lineNumber, line, state, report) {
  const fragments = commentFragments(line, state)
  if (!report) return
  for (const fragment of fragments) {
    if (hasForbiddenEnding(fragment)) {
      violations.push({ path, lineNumber, text: line.trim() })
    }
  }
}

function inspectDiff(diff) {
  let path = ''
  let lineNumber = 0
  let state = createState()

  for (const rawLine of diff.split(/\r?\n/)) {
    if (rawLine.startsWith('+++ b/')) {
      path = rawLine.slice(6)
      continue
    }
    if (rawLine.startsWith('+++ /dev/null')) {
      path = ''
      continue
    }
    if (rawLine.startsWith('@@')) {
      const match = /\+(\d+)(?:,\d+)?/.exec(rawLine)
      lineNumber = match ? Number(match[1]) : 0
      state = createState()
      continue
    }
    if (!path || !isSupported(path)) continue
    if (rawLine.startsWith('+') && !rawLine.startsWith('+++')) {
      inspectLine(path, lineNumber, rawLine.slice(1), state, true)
      lineNumber += 1
      continue
    }
    if (rawLine.startsWith(' ')) {
      inspectLine(path, lineNumber, rawLine.slice(1), state, false)
      lineNumber += 1
    }
  }
}

function inspectUntracked(path) {
  if (!isSupported(path)) return
  const state = createState()
  const lines = readFileSync(path, 'utf8').split(/\r?\n/)
  lines.forEach((line, index) => inspectLine(path, index + 1, line, state, true))
}

const diffArgs = ['diff', '--no-ext-diff', '--unified=3']
if (cached) diffArgs.push('--cached')
diffArgs.push('--')
inspectDiff(runGit(diffArgs))

if (workingTree) {
  const untracked = runGit(['ls-files', '--others', '--exclude-standard', '-z'])
  for (const path of untracked.split('\0').filter(Boolean)) inspectUntracked(path)
}

if (violations.length > 0) {
  for (const violation of violations) {
    console.error(`${violation.path}:${violation.lineNumber}: comment ends with a period: ${violation.text}`)
  }
  process.exit(1)
}

console.log(`Comment endings passed for ${cached ? 'staged changes' : 'working tree changes'}`)
