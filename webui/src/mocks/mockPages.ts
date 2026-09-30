// 保留旧版固件字段和控件描述，供无硬件预览解析
export const formsByPath: Record<string, string> = {
  '/index.htm': '',
  '/mount.htm': `
<div class="b1">
<div>Home offsets</div>
<form action="/mount.htm">
<label>Axis 1<input name="hc1" type="number" value="0">
</label>
<label>Axis 2<input name="hc2" type="number" value="0">
</label>
<button type="submit">Save</button>
</form>
</div>
<div class="b1">
<div>PEC worm steps</div>
<form action="/mount.htm">
<label>Steps<input name="spwr" type="number" min="1" value="86400">
</label>
<button type="submit">Save</button>
</form>
</div>
`,
  '/net.htm': `
<div class="b1">
<div>Network and update settings</div>
<form action="/net.htm">
<label>Command timeout<input name="ccto" value="100" type="number" min="100" max="300">
</label>
<label>Web timeout<input name="wcto" value="100" type="number" min="100" max="300">
</label>
<button type="submit">Save</button>
</form>
</div>
<div class="b1">
<div>Wi-Fi</div>
<form action="/net.htm">
<label>SSID<input name="stssid" value="Observatory">
</label>
<label>DHCP<input name="stadhcp" value="1" type="checkbox" checked>
</label>
<label>IP 1<input name="staip1" type="number" value="192">
</label>
<label>IP 2<input name="staip2" type="number" value="168">
</label>
<label>IP 3<input name="staip3" type="number" value="1">
</label>
<label>IP 4<input name="staip4" type="number" value="44">
</label>
<button type="submit">Save</button>
</form>
</div>
<form action="/net.htm">
<label>OTA password<input name="otapwd" type="password">
</label>
<label>OTA<input name="otaen" type="checkbox" value="1">
</label>
<button type="submit">Save</button>
</form>
`,
}

export const auxiliaryPage = `
<div class="b1">
<div>Power switch</div>
<button onpointerdown="s('x1v1','1')">On</button>
<button onpointerdown="s('x1v1','0')">Off</button>
</div>
  <div class="b1">
<div>Analog output</div>
<div>Output</div>
<div>
<input type="range" min="0" max="255" value="42" onchange="sz('x2v1',this.value)">
</div>
</div>
  <div class="b1">
<div>Dew heater</div>
<button onpointerdown="s('x3v1','1')">On</button>
<button onpointerdown="s('x3v1','0')">Off</button>
<div>Dew zero</div>
<div>
<input type="range" min="-10" max="40" value="5" onchange="sz('x3v2',this.value)">
</div>
<div>Dew span</div>
<div>
<input type="range" min="-10" max="40" value="2" onchange="sz('x3v3',this.value)">
</div>
</div>
  <div class="b1">
<div>Intervalometer</div>
<button onpointerdown="s('x4v1','1')">Start</button>
<button onpointerdown="s('x4v1','0')">Stop</button>
<div>Exposure</div>
<div>
<input type="range" min="0" max="255" value="20" onchange="sz('x4v2',this.value)">
</div>
<div>Delay</div>
<div>
<input type="range" min="10" max="255" value="10" onchange="sz('x4v3',this.value)">
</div>
<div>Count</div>
<div>
<input type="range" min="1" max="255" value="10" onchange="sz('x4v4',this.value)">
</div>
</div>
`
