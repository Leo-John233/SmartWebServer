import { gunzipSync, gzipSync } from 'node:zlib'
import { mkdirSync, readFileSync, readdirSync, writeFileSync } from 'node:fs'
import { basename, dirname, join, relative, resolve, sep } from 'node:path'
import { fileURLToPath } from 'node:url'

const scriptDirectory = fileURLToPath(new URL('.', import.meta.url))
const webuiDirectory = resolve(scriptDirectory, '..')
const distDirectory = join(webuiDirectory, 'dist', 'embed')
const outputPath = resolve(webuiDirectory, 'firmware', 'webui', 'WebAssets.generated.h')

function listFiles(directory) {
  return readdirSync(directory, { withFileTypes: true }).flatMap((entry) => {
    const entryPath = join(directory, entry.name)
    return entry.isDirectory() ? listFiles(entryPath) : [entryPath]
  })
}

function identifier(path) {
  return path.replace(/[^a-zA-Z0-9]+/g, '_').replace(/^_|_$/g, '')
}

function byteLines(buffer) {
  const bytes = [...buffer].map((byte) => `0x${byte.toString(16).padStart(2, '0')}`)
  const lines = []
  for (let offset = 0; offset < bytes.length; offset += 16)
    lines.push(`  ${bytes.slice(offset, offset + 16).join(', ')}`)
  return lines.join(',\n')
}

const buildFiles = listFiles(distDirectory)
const sourceFiles = buildFiles.filter(
  (path) =>
    path.endsWith('index.html') || /assets[\\/]index-[a-zA-Z0-9_-]+\.(?:css|js)$/.test(path),
)
const htmlFiles = sourceFiles.filter((path) => path.endsWith('index.html'))
const scriptFiles = sourceFiles.filter((path) => path.endsWith('.js'))
const styleFiles = sourceFiles.filter((path) => path.endsWith('.css'))

if (htmlFiles.length !== 1 || scriptFiles.length !== 1 || styleFiles.length !== 1) {
  throw new Error(
    `Expected one HTML, CSS and JavaScript entry, found ${htmlFiles.length}/${styleFiles.length}/${scriptFiles.length}`,
  )
}

const orderedFiles = [htmlFiles[0], styleFiles[0], scriptFiles[0]]
const assets = orderedFiles.map((filePath, index) => {
  const relativePath = relative(distDirectory, filePath).split(sep).join('/')
  const requestPath = index === 0 ? '/ui/' : `/ui/${relativePath}`
  const source = readFileSync(filePath)
  const compressed = gzipSync(source, { level: 9 })
  if (!gunzipSync(compressed).equals(source))
    throw new Error(`Gzip verification failed for ${relativePath}`)
  return {
    id: identifier(relativePath),
    requestPath,
    mimeType: filePath.endsWith('.html')
      ? 'text/html; charset=utf-8'
      : filePath.endsWith('.css')
        ? 'text/css; charset=utf-8'
        : 'text/javascript; charset=utf-8',
    sourceLength: source.length,
    compressed,
    immutable: index !== 0,
  }
})

const declarations = assets
  .map(
    (asset) => `
static const char ${asset.id}_path[] PROGMEM = "${asset.requestPath}";
static const char ${asset.id}_mime[] PROGMEM = "${asset.mimeType}";
static const uint8_t ${asset.id}_gzip[] PROGMEM = {
${byteLines(asset.compressed)}
};`,
  )
  .join('\n')

const records = assets
  .map(
    (asset) =>
      `  { ${asset.id}_path, ${asset.id}_mime, ${asset.id}_gzip, ${asset.compressed.length}, ${asset.sourceLength}, ${asset.immutable} }`,
  )
  .join(',\n')
const scriptPath = assets.find((asset) => asset.requestPath.endsWith('.js')).requestPath
const stylePath = assets.find((asset) => asset.requestPath.endsWith('.css')).requestPath

const header = `#pragma once

#include <Arduino.h>

#define WEB_UI_INDEX_PATH "/ui/"
#define WEB_UI_SCRIPT_PATH "${scriptPath}"
#define WEB_UI_STYLE_PATH "${stylePath}"

namespace webui_assets {

struct WebAsset {
  const char* path;
  const char* mimeType;
  const uint8_t* data;
  size_t compressedLength;
  size_t sourceLength;
  bool immutable;
};
${declarations}

static const WebAsset assets[] PROGMEM = {
${records}
};

static const size_t assetCount = sizeof(assets) / sizeof(assets[0]);

}
`

mkdirSync(dirname(outputPath), { recursive: true })
writeFileSync(outputPath, header)

for (const asset of assets) {
  console.log(
    `${basename(asset.requestPath)}: ${asset.sourceLength} -> ${asset.compressed.length} bytes`,
  )
}
