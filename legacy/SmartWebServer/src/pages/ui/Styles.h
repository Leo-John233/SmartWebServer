#pragma once
#include "Palette.h"

// 只负责排版与外观，不读取或发送设备命令。
const char html_shared_styles[] PROGMEM = R"UI(
<style>
:root {
  color-scheme: dark;
  --bg: )UI" COLOR_BODY_BACKGROUND R"UI(;
  --panel: )UI" SWS_COLOR_PANEL R"UI(;
  --field: )UI" COLOR_LIGHT_BACKGROUND R"UI(;
  --border: )UI" COLOR_BORDER R"UI(;
  --accent: )UI" COLOR_CONTROL R"UI(;
  --sidebar-width: 156px;
  --nav-active: )UI" COLOR_LINK_SELECTED_BACKGROUND R"UI(;
  --text: )UI" SWS_COLOR_TEXT R"UI(;
  --muted: )UI" SWS_COLOR_MUTED R"UI(;
  --readout: )UI" SWS_COLOR_READOUT R"UI(;
}

* {
  box-sizing: border-box;
}
body {
  margin: 0;
  padding: 16px;
  background: var(--bg);
  color: var(--text);
  font:
    15px/1.6 system-ui,
    -apple-system,
    'Segoe UI',
    sans-serif;
}

.t,
.b {
  width: 100%;
  min-width: 0;
  max-width: none;
  margin: 0;
  padding: 20px;
  border: 1px solid var(--border);
  background: #131313;
  color: var(--text);
}

.t {
  border-radius: 16px 16px 0 0;
}
.b {
  border-top: 0;
  border-radius: 0 0 16px 16px;
  display: flex;
  flex-wrap: wrap;
  align-items: flex-start;
  gap: 16px;
}

.t table {
  width: 100%;
}
.t td {
  overflow-wrap: anywhere;
}
.t font {
  font-size: 24px;
}
.t a:link,
.t a:visited {
  border: 1px solid var(--border);
  background: #1f1f1f;
  color: var(--muted);
  border-radius: 9px;
  padding: 7px 14px;
  margin: 10px 6px 0 0;
  text-decoration: none;
}

.t a:hover {
  background: var(--field) !important;
  color: var(--text);
}
.t a:focus-visible {
  outline: 2px solid #9b9b9b;
  outline-offset: 3px;
}

.b1 {
  float: none;
  flex: 1 1 310px;
  width: auto !important;
  max-width: 100%;
  min-width: 0;
  min-height: 0 !important;
  margin: 0;
  padding: 18px;
  border: 1px solid var(--border);
  border-radius: 14px;
  background: var(--panel);
  overflow-x: auto;
}

.b1 > div:first-child {
  font-weight: 600;
}
.b1 table {
  max-width: 100%;
}
.b1 canvas {
  display: block;
  max-width: 100%;
  height: auto;
}
.clear {
  flex-basis: 100%;
  height: 0;
}
.b > hr {
  width: 100%;
}

input,
select,
textarea,
button {
  font: inherit;
  max-width: 100%;
  border: 1px solid #4c4c4c;
  border-radius: 8px;
  color: var(--text);
  box-shadow: none;
}

input,
select,
textarea {
  text-align: left;
  background: var(--field);
  padding: 8px 10px;
  margin: 3px;
  font-weight: 400;
}
input[type='number'] {
  min-width: 5.5em;
  text-align: left;
}
input[type='checkbox'],
input[type='radio'] {
  width: 18px;
  height: 18px;
  accent-color: var(--accent);
  vertical-align: middle;
  padding: 0;
}

button {
  min-height: 40px;
  background: var(--accent);
  padding: 8px 14px;
  margin: 3px;
  cursor: pointer;
  font-weight: 500;
}
button:hover:not(:disabled) {
  background-image: linear-gradient(rgb(185 185 185 / 8%), rgb(185 185 185 / 8%));
}
button[data-selected='true']:not(:disabled):not([role='switch']) {
  background-image: linear-gradient(rgb(185 185 185 / 22%), rgb(185 185 185 / 22%)) !important;
  box-shadow: none;
}
button:not(:disabled):active {
  background-image: linear-gradient(rgb(185 185 185 / 32%), rgb(185 185 185 / 32%)) !important;
  box-shadow: none;
}
button:disabled {
  background: #292929;
  color: #7e7e7e;
  border-color: #3b3b3b;
  cursor: default;
}

input:focus-visible,
select:focus-visible,
textarea:focus-visible,
button:focus-visible,
summary:focus-visible {
  outline: 2px solid #9b9b9b;
  outline-offset: 2px;
}
hr {
  border: 0;
  border-top: 1px solid var(--border);
  margin: 16px 0;
}

a:link,
a:visited {
  color: #a8a8a8;
  background: transparent;
  border-color: var(--border);
}
.c {
  color: var(--readout);
}
.y {
  color: #d6b786;
}
.gb {
  font-family: inherit;
  border-radius: 10px;
}
.collapsible {
  width: 100%;
  text-align: left;
  background: #252525;
  border: 1px solid var(--border);
  border-radius: 9px;
  padding: 10px;
  color: #b5b5b5;
}
.active,
.collapsible:hover {
  background: #303030;
}
.content {
  background: #1c1c1c;
  border-radius: 0 0 9px 9px;
  padding: 12px;
}

.sws-network-layout {
  display: grid;
  grid-template-columns: repeat(2, minmax(0, 1fr));
  gap: 18px;
  width: 100%;
  min-width: 0;
}
.sws-network-layout > .nw-grid {
  grid-column: 1/-1;
  margin: 0;
}
.sws-network-layout > .b1 {
  width: 100% !important;
}
.sws-network-layout > #nw-message {
  grid-column: 1/-1;
  margin: 0;
}
.sws-account {
  grid-column: 1/-1;
  border: 1px solid var(--border);
  border-radius: 14px;
  background: var(--panel);
  padding: 20px;
  min-width: 0;
}
.sws-account h2 { margin:0 0 8px; font-size:20px; overflow-wrap:anywhere; }
.sws-account-note { margin:8px 0 0; color:var(--muted); font-size:14px; line-height:1.7; }
.sws-account-controls { display:flex; flex-wrap:wrap; gap:12px; align-items:flex-end; max-width:760px; margin-top:18px; }
.sws-account-controls form { flex:1 1 300px; min-width:0; }
.sws-account-controls label { display:block; margin-bottom:8px; font-size:15px; }
.sws-password-actions { display:flex; gap:10px; }
.sws-account .sws-password-actions input { flex:1; width:0; min-width:0; min-height:44px; margin:0; }
.sws-account-controls button { min-height:44px; margin:0; }
.sws-account-controls .sws-logout-form { flex:0 0 auto; }
button.sws-secondary { background:#303030; border-color:var(--border); }
.sws-maintenance { display:grid; grid-template-columns:minmax(0,1fr) auto; align-items:center; gap:20px; padding:18px; margin-top:18px; border:1px solid var(--border); border-radius:12px; background:var(--field); }
.sws-maintenance h3 { margin:0; font-size:17px; line-height:1.5; }
.sws-maintenance-copy ul { margin:10px 0 0; padding-left:20px; color:var(--muted); font-size:14px; line-height:1.8; }
.sws-maintenance-copy li + li { margin-top:6px; }
.sws-maintenance-actions { display:flex; flex-direction:column; gap:10px; }
.sws-maintenance-actions button { min-height:44px; margin:0; font-size:15px; }
@media(max-width:800px) {
  .sws-maintenance { grid-template-columns:minmax(0,1fr); padding:16px; gap:16px; }
  .sws-maintenance-actions { flex-direction:row; flex-wrap:wrap; }
  .sws-maintenance-actions button { flex:1 1 200px; }
}
@media(max-width:480px) {
  .sws-account { padding:16px; }
  .sws-account-controls, .sws-password-actions { display:grid; grid-template-columns:minmax(0,1fr); }
  .sws-account .sws-password-actions input, .sws-account-controls button { width:100%; }
}

html.sws-embedded body {
  padding: 0;
}
html.sws-embedded .t {
  display: none;
}
html.sws-embedded .b {
  border: 0;
  border-radius: 0;
  padding: 20px;
}

#sws-frame {
  display: block;
  width: 100%;
  border: 1px solid var(--border);
  border-top: 0;
  border-radius: 0 0 16px 16px;
  background: #131313;
}
#sws-load {
  margin: 0;
  color: var(--muted);
  font-size: 13px;
}
#sws-load:empty {
  display: none;
}
html.sws-shell body {
  overflow: hidden;
}

@media (min-width: 1500px) {
  .b1 {
    flex-basis: 340px;
  }
}

@media (max-width: 700px) {
  body {
    padding: 8px;
  }
  .t,
  .b,
  html.sws-embedded .b {
    padding: 14px;
  }
  .t td {
    display: block;
    text-align: left;
  }
  .t td:last-child {
    font-size: 12px;
  }
  .t a:link,
  .t a:visited {
    padding: 7px 10px;
    margin-right: 4px;
  }
  .b1 {
    flex-basis: 100%;
    padding: 14px;
  }
  .sws-network-layout {
    grid-template-columns: 1fr;
  }
  .sws-network-layout > .b1 {
    grid-column: 1;
  }
  .nw-grid {
    grid-template-columns: 1fr !important;
  }
}

/* 桌面使用侧栏与内容区，手机使用底部导航并为触控留出空间 */
html.sws-shell body {
  padding: 0;
  height: 100dvh;
  overflow: hidden;
}
.sws-shell .t {
  position: fixed;
  inset: 0 auto 0 0;
  width: var(--sidebar-width);
  border: 0;
  border-right: 1px solid #292929;
  border-radius: 0;
  background: #141414;
  padding: 30px 16px;
  display: flex;
  flex-direction: column;
  gap: 30px;
}
.sws-shell .t table,
.sws-shell .t tbody,
.sws-shell .t tr,
.sws-shell .t td {
  display: block;
  width: 100%;
  text-align: center;
}
.sws-shell .t td:last-child {
  font-size: 11px;
  font-weight: 400;
  color: #808080;
  margin-top: 9px;
}
.sws-shell .t font {
  font-size: 24px;
}
.sws-nav {
  display: flex;
  flex-direction: column;
  gap: 10px;
}
.sws-shell .t a:link,
.sws-shell .t a:visited {
  display: flex;
  align-items: center;
  gap: 12px;
  margin: 0;
  padding: 14px;
  border: 0;
  border-radius: 10px;
  background: transparent;
  color: #9f9f9f;
  font-size: 14px;
}
.sws-shell .t a[aria-current='page'] {
  background: var(--nav-active) !important;
  color: #fff;
  box-shadow: none;
}
@media (min-width: 701px) {
  .sws-nav {
    flex: 1;
    min-height: 0;
    align-items: center;
    overflow-y: auto;
  }
  .sws-shell .t a:link,
  .sws-shell .t a:visited {
    flex-shrink: 0;
    flex-direction: column;
    justify-content: center;
    width: 112px;
    min-height: 84px;
    gap: 6px;
    padding: 12px 8px;
    border-radius: 12px;
    text-align: center;
    line-height: 1.4;
    font-size: 13px;
  }
}
.sws-nav svg {
  width: 28px;
  height: 28px;
  fill: currentColor;
  stroke: none;
  flex-shrink: 0;
}
.sws-sidebar-note {
  width: 100%;
  text-align: center;
  margin-top: auto;
  color: var(--muted);
  font-size: 12px;
  line-height: 1.8;
}

.sws-topbar {
  margin-left: var(--sidebar-width);
  padding: 25px 30px 20px;
  background: #101010;
  border-bottom: 1px solid #242424;
  display: flex;
  align-items: center;
  justify-content: space-between;
  gap: 20px;
}
.sws-topbar h1 {
  margin: 0;
  color: #e2e2e2;
  font-size: 25px;
  font-weight: 600;
  text-align: left;
}
.sws-topbar p {
  margin: 5px 0 0;
  color: #8c8c8c;
  font-size: 13px;
}
.sws-brand-small {
  font-size: 12px;
  color: #959595;
  letter-spacing: 0.05em;
}
.sws-shell #sws-frame {
  margin-left: var(--sidebar-width);
  width: calc(100% - var(--sidebar-width));
  border: 0;
  border-radius: 0;
  background: #101010;
}
.sws-shell #sws-load {
  position: fixed;
  right: 28px;
  top: 8px;
  font-size: 11px;
  color: #8e8e8e;
  z-index: 3;
}

html.sws-embedded body {
  background: #101010;
}
html.sws-embedded .b {
  background: transparent;
  padding: 26px 30px;
  gap: 24px;
}
.b1 {
  border-color: #303030;
  background: #191919;
  border-radius: 14px;
}
.b1 > div:first-child {
  font-size: 16px;
  color: #d8d8d8;
}
h2.sws-section-title {
  margin: 0;
  font-size: 17px;
  font-weight: 600;
  color: #c7c7c7;
}
.sws-section-head {
  width: 100%;
  display: flex;
  align-items: baseline;
  justify-content: space-between;
  gap: 12px;
  margin: 0 0 14px;
}
.sws-section-head p {
  font-size: 12px;
  color: #8a8a8a;
  margin: 0;
}

.sws-controller {
  width: 100%;
  display: flex;
  flex-direction: column;
  gap: 26px;
  min-width: 0;
}
.sws-overview-grid {
  display: grid;
  grid-template-columns: repeat(auto-fit, minmax(min(100%, 350px), 1fr));
  gap: 18px;
}
.sws-overview-grid > .b1 {
  padding: 22px 24px;
  background: linear-gradient(120deg, #1c1c1c, #191919);
  border-color: #3a3a3a;
}
.sws-axis-grid {
  display: grid;
  grid-template-columns: repeat(auto-fit, minmax(min(100%, 300px), 1fr));
  gap: 18px;
}
.sws-axis-grid > .b1 {
  padding: 22px;
  line-height: 1.9;
}
.sws-axis-grid .content {
  margin-top: 10px;
}
.sws-axis-grid .b1 > hr:first-of-type {
  margin-top: 10px;
}

.sws-more {
  width: 100%;
  border: 1px solid #303030;
  border-radius: 14px;
  padding: 18px 22px;
  background: #151515;
}
.sws-more > summary {
  cursor: pointer;
  color: #bababa;
  font-size: 14px;
}
.sws-more[open] > summary {
  margin-bottom: 18px;
}
.sws-settings-grid {
  display: grid;
  grid-template-columns: repeat(auto-fit, minmax(min(100%, 310px), 1fr));
  gap: 18px;
  min-width: 0;
}

.sws-mount {
  width: 100%;
  display: flex;
  flex-direction: column;
  gap: 24px;
  min-width: 0;
}
.sws-control-grid {
  display: grid;
  grid-template-columns: minmax(290px, 0.9fr) minmax(340px, 1.4fr);
  grid-template-areas: 'guide target' 'guide tracking';
  gap: 18px;
  align-items: stretch;
}
.sws-guide {
  grid-area: guide;
}
.sws-target {
  grid-area: target;
}
.sws-tracking {
  grid-area: tracking;
}
.sws-guide > .b1,
.sws-target > .b1,
.sws-tracking > .b1 {
  height: 100%;
  width: 100% !important;
  padding: 24px;
}
.sws-guide > .b1 {
  background: radial-gradient(ellipse at center, #202020, #191919 75%);
  text-align: center !important;
}
.sws-guide .gb {
  width: 64px;
  height: 58px;
  border-radius: 12px;
  background: #282828;
  border-color: #494949;
  color: #cfcfcf;
  font-size: 23px;
}
.sws-guide .gb:hover {
  background-image: linear-gradient(rgb(185 185 185 / 8%), rgb(185 185 185 / 8%));
}
.sws-guide .collapsible {
  margin-top: 20px;
}
.sws-guide .sws-pad {
  display: grid;
  grid-template-columns: repeat(3, 64px);
  gap: 8px;
  justify-content: center;
  margin: 32px auto 26px;
}
.sws-pad .gb {
  margin: 0;
}
.sws-pad .gb:first-of-type {
  grid-column: 2;
}
.sws-pad .gb:nth-of-type(2) {
  grid-column: 1;
}
.sws-pad .gb:last-of-type {
  grid-column: 2;
}

.sws-target .b1 {
  line-height: 1.9;
}
.sws-tracking .b1 {
  line-height: 1.9;
}

@media (min-width: 1700px) {
  .sws-control-grid {
    grid-template-columns: minmax(320px, 0.7fr) minmax(440px, 1.3fr);
  }
}

@media (max-width: 900px) {
  .sws-control-grid {
    grid-template-columns: 1fr;
    grid-template-areas: 'guide' 'target' 'tracking';
  }
  .sws-guide > .b1 {
    padding: 20px;
  }
  .sws-axis-grid {
    grid-template-columns: 1fr;
  }
}

@media (max-width: 700px) {
  .sws-shell .t {
    inset: auto 0 0;
    width: 100%;
    height: calc(74px + env(safe-area-inset-bottom));
    padding: 5px 8px calc(5px + env(safe-area-inset-bottom));
    border: 0;
    border-top: 1px solid #303030;
    z-index: 10;
    background: #161616;
    display: block;
  }
  .sws-shell .t table,
  .sws-sidebar-note {
    display: none !important;
  }
  .sws-nav {
    display: flex;
    flex-direction: row;
    gap: 4px;
    overflow-x: auto;
    height: 64px;
  }
  .sws-shell .t a:link,
  .sws-shell .t a:visited {
    flex: 1 0 64px;
    flex-direction: column;
    justify-content: center;
    gap: 3px;
    padding: 5px 8px;
    font-size: 11px;
    border-radius: 10px;
    min-width: 64px;
  }
  .sws-shell .t a[aria-current='page'] {
    box-shadow: none;
  }
  .sws-nav svg {
    width: 21px;
    height: 21px;
  }
  .sws-topbar {
    margin-left: 0;
    padding: 14px 18px;
    gap: 8px;
  }
  .sws-topbar h1 {
    font-size: 20px;
  }
  .sws-topbar p {
    font-size: 11px;
    max-width: 230px;
  }
  .sws-brand-small {
    font-size: 11px;
  }
  .sws-shell #sws-frame {
    margin-left: 0;
    width: 100%;
  }
  .sws-shell #sws-load {
    right: 12px;
    top: 2px;
    font-size: 10px;
  }
  html.sws-embedded .b {
    padding: 18px 14px;
  }
  .sws-overview-grid,
  .sws-settings-grid {
    grid-template-columns: 1fr;
  }
  .sws-section-head {
    align-items: flex-start;
    flex-direction: column;
    gap: 4px;
  }
  .sws-more {
    padding: 16px;
  }
  .sws-control-grid {
    gap: 14px;
  }
  .sws-guide .sws-pad {
    margin: 22px auto;
  }
  .sws-target > .b1,
  .sws-tracking > .b1 {
    padding: 18px;
  }
  .sws-overview-grid > .b1,
  .sws-axis-grid > .b1 {
    padding: 18px;
  }
}

/* 轴参数使用原有描述，标签与输入框对齐且允许长名称换行 */
.sws-parameter {
  display: grid;
  grid-template-columns: minmax(0, 1fr) minmax(90px, 1fr);
  gap: 12px;
  align-items: center;
  margin: 10px 0;
  text-align: left;
}
.sws-parameter > span {
  order: -1;
  overflow-wrap: anywhere;
}
.sws-parameter > input,
.sws-parameter > select {
  width: 100% !important;
  min-width: 0;
  margin: 0 !important;
}
.sws-document pre {
  white-space: pre-wrap;
  overflow-wrap: anywhere;
}

/* 其余设备页采用主操作与次要设置分区，帮助与错误页保持同一视觉语言 */
.sws-device-layout {
  display: flex;
  flex-direction: column;
  gap: 24px;
  width: 100%;
  min-width: 0;
}
.sws-device-controls {
  display: grid;
  grid-template-columns: minmax(300px, 1.3fr) minmax(280px, 1fr);
  gap: 18px;
}
.sws-device-controls > section {
  min-width: 0;
}
.sws-device-controls > section > .b1 {
  height: 100%;
  padding: 24px;
}
.sws-device-primary .b1 {
  background: radial-gradient(ellipse at top, #202020, #191919 80%);
}
.sws-device-selector > .b1 {
  background: #1c1c1c;
  padding: 22px;
}
.sws-device-layout .gb {
  min-width: 64px;
  min-height: 58px;
}
.sws-device-primary button {
  min-width: 44px;
  min-height: 44px;
}
.sws-device-layout .content {
  padding: 14px;
}

.sws-aux-grid {
  display: grid;
  grid-template-columns: repeat(2, minmax(0, 1fr));
  gap: 18px;
  align-items: start;
}
.sws-aux-grid > .sws-aux-card {
  display: block;
  text-align: left;
  padding: 20px;
  overflow-wrap: anywhere;
}
.sws-aux-head {
  display: flex;
  align-items: center;
  justify-content: space-between;
  gap: 16px;
  padding-bottom: 14px;
  margin-bottom: 6px;
  border-bottom: 1px solid #2c2c2c;
}
.sws-aux-title {
  min-width: 0;
}
.sws-aux-title > span {
  display: block;
  color: var(--muted);
  font-size: 12px;
  font-weight: 400;
  line-height: 1.6;
}
.sws-aux-title h2 {
  font-size: 16px;
  font-weight: 600;
  line-height: 1.6;
  margin: 2px 0 0;
  color: #d8d8d8;
}
.sws-aux-power {
  display: flex;
  flex-wrap: wrap;
  justify-content: flex-end;
  gap: 6px;
  max-width: 45%;
  font-size: 12px;
  font-weight: 500;
  line-height: 1.5;
  font-variant-numeric: tabular-nums;
}
.sws-aux-power:empty {
  display: none;
}
.sws-aux-power > span {
  white-space: nowrap;
  padding: 4px 8px;
  border-radius: 6px;
  background: var(--field);
}
.sws-aux-row {
  display: grid;
  grid-template-columns: minmax(110px, 1fr) minmax(0, 1.7fr) 66px;
  align-items: center;
  gap: 12px;
  min-height: 54px;
  padding: 8px 0;
}
.sws-aux-row + .sws-aux-row {
  border-top: 1px solid #252525;
}
.sws-aux-label {
  font-size: 13px;
  line-height: 1.7;
  color: var(--muted);
  font-weight: 400;
}
.sws-aux-controls {
  display: flex;
  align-items: center;
  gap: 6px;
  min-width: 0;
}
.sws-aux-value {
  font-size: 13px;
  line-height: 1.6;
  text-align: right;
  color: var(--readout);
  font-variant-numeric: tabular-nums;
  white-space: nowrap;
}
.sws-aux-controls button {
  flex: 1;
  min-width: 0;
  min-height: 38px;
  margin: 0;
  padding: 7px 10px;
  font-size: 13px;
  border-radius: 7px;
}
.sws-aux-controls input[type='range'] {
  width: 100%;
  max-width: 100%;
  min-width: 0;
  margin: 0;
  padding: 0;
  height: 38px;
  accent-color: #962525;
  background: transparent;
  border: 0;
  cursor: pointer;
}
@media (max-width: 800px) {
  .sws-aux-grid {
    grid-template-columns: 1fr;
  }
}
@media (max-width: 500px) {
  .sws-aux-grid > .sws-aux-card {
    padding: 16px;
  }
  .sws-aux-head {
    gap: 10px;
    padding-bottom: 12px;
  }
  .sws-aux-power {
    gap: 4px;
    max-width: 46%;
  }
  .sws-aux-power > span {
    padding: 3px 6px;
  }
  .sws-aux-row {
    grid-template-columns: 90px minmax(0, 1fr) 54px;
    gap: 8px;
  }
  .sws-aux-label {
    font-size: 12px;
  }
  .sws-aux-controls button {
    font-size: 12px;
    padding: 6px 4px;
  }
  .sws-aux-value {
    font-size: 12px;
  }
}

.sws-document {
  max-width: 960px;
  margin: 30px auto;
  padding: 32px;
  background: #191919;
  border: 1px solid #373737;
  border-radius: 18px;
}
.sws-document header {
  border-bottom: 1px solid #303030;
  padding-bottom: 22px;
  margin-bottom: 26px;
}
.sws-document h1 {
  text-align: left;
  font-size: 28px;
  color: #d1d1d1;
}
.sws-document h2 {
  font-size: 18px;
  color: #c4c4c4;
  margin-top: 28px;
}
.sws-document p {
  color: #9e9e9e;
  line-height: 1.8;
}
.sws-document pre {
  padding: 20px;
  background: #0f0f0f;
  border: 1px solid #303030;
  border-radius: 12px;
  overflow: auto;
  color: #c4c4c4;
}
.sws-document table {
  width: 100%;
  border-collapse: collapse;
}
.sws-document td,
.sws-document th {
  text-align: left;
  padding: 10px;
  border-bottom: 1px solid #303030;
}
.sws-document a {
  display: inline-block;
  padding: 10px 16px;
  border: 1px solid #4c4c4c;
  border-radius: 9px;
  background: #2f2f2f;
  text-decoration: none;
}
.sws-empty {
  max-width: 620px;
  margin: 12vh auto;
  text-align: center;
}
.sws-empty h1 {
  text-align: center;
}
.sws-notice {
  width: 100%;
  padding: 24px;
  background: #1d1d1d;
  border: 1px solid #494949;
  border-radius: 14px;
}
.sws-notice h2 {
  font-size: 20px;
}
.sws-notice li {
  margin: 12px 0;
}

@media (max-width: 800px) {
  .sws-device-controls {
    grid-template-columns: 1fr;
  }
  .sws-device-controls > section > .b1 {
    padding: 18px;
  }
  .sws-aux-grid {
    grid-template-columns: 1fr;
  }
}

@media (max-width: 700px) {
  .sws-document {
    margin: 10px 0;
    padding: 18px;
  }
  .sws-document h1 {
    font-size: 23px;
  }
  .sws-document pre {
    font-size: 12px;
  }
  .sws-notice {
    padding: 18px;
  }
}

.sws-extra-nav {
  display: contents;
}
.sws-nav-more {
  display: none;
}

@media (max-width: 700px) {
  .sws-nav {
    overflow: visible;
  }
  .sws-nav-more {
    display: flex;
    flex: 1 0 64px;
    flex-direction: column;
    justify-content: center;
    align-items: center;
    gap: 3px;
    padding: 5px 8px;
    margin: 0;
    border: 0;
    border-radius: 10px;
    background: transparent;
    color: #9f9f9f;
    font-size: 11px;
  }
  .sws-nav-more[data-active='true'] {
    background: var(--nav-active);
    color: #fff;
    box-shadow: none;
  }
  .sws-nav-more svg {
    fill: currentColor;
    stroke: none;
  }
  .sws-extra-nav {
    display: none;
    position: fixed;
    bottom: calc(84px + env(safe-area-inset-bottom));
    left: 12px;
    right: 12px;
    padding: 10px;
    background: #1f1f1f;
    border: 1px solid #424242;
    border-radius: 14px;
    box-shadow: 0 -12px 36px #0008;
  }
  .sws-nav[data-expanded='true'] .sws-extra-nav {
    display: grid;
    grid-template-columns: repeat(2, minmax(0, 1fr));
    gap: 8px;
  }
  .sws-shell .t .sws-extra-nav a {
    min-height: 64px;
    flex-direction: row;
    justify-content: flex-start;
    font-size: 13px;
    gap: 10px;
  }
}

/* 状态指标合并展示，常用控制采用左右分区并随窄屏改为单列 */
.sws-overview-grid {
  display: block;
}
.sws-metrics {
  display: grid;
  grid-template-columns: repeat(auto-fit, minmax(155px, 1fr));
  gap: 12px;
}
.sws-metric {
  display: flex;
  flex-direction: column;
  gap: 8px;
  background: #1d1d1d;
  border-radius: 10px;
  padding: 16px;
}
.sws-metric > span {
  font-size: 12px;
  color: var(--muted);
}
.sws-metric > strong {
  font-size: 20px;
  font-weight: 600;
}
.sws-overview-grid > .b1 {
  padding: 20px;
}
.sws-overview-grid .content {
  margin-top: 12px;
}

.sws-control-grid {
  grid-template-columns: repeat(2, minmax(0, 1fr));
  grid-template-areas: 'tracking guide' 'target target';
  align-items: start;
}
.sws-guide > .b1,
.sws-tracking > .b1 {
  min-height: 390px !important;
  height: auto;
}
.sws-target > .b1 {
  height: auto;
}
.sws-home > .b1 {
  width: 100% !important;
  padding: 22px;
}
.sws-home button {
  min-width: 120px;
}
.sws-home > .b1 > .collapsible {
  margin-top: 12px;
}
.sws-guide > .b1 {
  background: var(--panel);
}

.sws-choice {
  display: flex;
  flex-direction: column;
  gap: 8px;
  margin: 18px 0;
  text-align: left;
}
.sws-choice > span {
  font-size: 14px;
  font-weight: 600;
  color: var(--text);
}
.sws-choice select {
  margin: 0;
  width: 100%;
  min-height: 46px;
  padding: 10px 12px;
}
.sws-choice-originals {
  display: none !important;
}
.sws-tracking .content {
  margin-top: 12px;
}
.sws-track-switch {
  display: flex;
  justify-content: space-between;
  align-items: center;
  margin: 8px 0 20px;
}
.sws-switch {
  width: 52px;
  min-height: 28px;
  height: 28px;
  padding: 3px;
  border-radius: 18px;
  background: #333333;
  border: 0;
  position: relative;
}
.sws-switch::after {
  content: '';
  display: block;
  width: 22px;
  height: 22px;
  border-radius: 50%;
  background: #acacac;
  transition: transform 0.15s;
}
.sws-switch[aria-checked='true'] {
  background: var(--accent);
}
.sws-switch[aria-checked='true']::after {
  transform: translateX(24px);
  background: #d4d4d4;
}

/* 四个方向保持原来的按住移动语义，中间停止与定位区同步分别保留 */
.sws-guide .sws-pad {
  touch-action: none;
  -webkit-touch-callout: none;
  -webkit-user-select: none;
  user-select: none;
  position: relative;
  display: block;
  width: 272px;
  height: 272px;
  margin: 20px auto;
  border-radius: 50%;
  overflow: hidden;
  background: #0f0f0f;
}
.sws-guide .sws-pad .gb {
  position: absolute;
  inset: 0;
  width: 100%;
  height: 100%;
  border: 0;
  border-radius: 0;
  margin: 0;
  background: #2a2a2a;
  font-size: 20px;
  padding: 0;
  display: block;
  min-width: 0;
}
.sws-pad .gb:nth-of-type(1) {
  clip-path: polygon(0 0, 100% 0, 50% 50%);
  padding-bottom: 190px !important;
}
.sws-pad .gb:nth-of-type(2) {
  clip-path: polygon(0 0, 50% 50%, 0 100%);
  padding-right: 190px !important;
}
.sws-pad .gb:nth-of-type(4) {
  clip-path: polygon(100% 0, 100% 100%, 50% 50%);
  padding-left: 190px !important;
}
.sws-pad .gb:nth-of-type(5) {
  clip-path: polygon(0 100%, 50% 50%, 100% 100%);
  padding-top: 190px !important;
}
.sws-guide .sws-pad .gb:hover {
  background-image: linear-gradient(rgb(185 185 185 / 8%), rgb(185 185 185 / 8%));
}
.sws-guide .sws-pad .sws-pad-stop {
  inset: calc(50% - 49px);
  width: 98px;
  height: 98px;
  border: 10px solid #191919;
  border-radius: 50%;
  z-index: 2;
  background: var(--accent);
  font-size: 17px;
}
.sws-sync {
  min-width: 90px;
}
.sws-guide > .b1 > .collapsible,
.sws-guide > .b1 > .content {
  display: none;
}
.sws-guide .collapsible,
.sws-guide .content {
  display: none !important;
}
.sws-home-actions {
  display: grid;
  grid-template-columns: 1fr 1fr;
  gap: 18px;
}
.sws-home-actions > div {
  display: flex;
  gap: 8px;
  padding: 14px;
  background: #1d1d1d;
  border-radius: 12px;
}
.sws-home-actions button {
  flex: 1;
  min-width: 0;
  margin: 0;
}
.sws-actions > br {
  display: none;
}
.sws-actions {
  display: flex;
  flex-wrap: wrap;
  gap: 8px;
  margin: 18px 0;
}
.sws-actions > button {
  margin: 0;
}
.sws-guide .sws-pad::after {
  content: '';
  position: absolute;
  inset: 0;
  pointer-events: none;
  background:
    linear-gradient(45deg, transparent 49%, #191919 49%, #191919 51%, transparent 51%),
    linear-gradient(-45deg, transparent 49%, #191919 49%, #191919 51%, transparent 51%);
}
.sws-pad-stop {
  z-index: 3 !important;
}

.sws-mount .sws-settings-grid {
  align-items: start;
}
.sws-mount .b1 .content {
  line-height: 1.8;
}
.sws-mount .b1 .content button {
  margin-top: 5px;
  margin-bottom: 5px;
}
.sws-mount .b1 .content br + br {
  display: none;
}

@media (max-width: 900px) {
  .sws-control-grid {
    grid-template-columns: 1fr;
    grid-template-areas: 'guide' 'tracking' 'target';
  }
  .sws-guide > .b1,
  .sws-tracking > .b1 {
    min-height: 0 !important;
  }
}

@media (max-width: 700px) {
  .sws-home-actions {
    grid-template-columns: 1fr;
  }
  .sws-metric {
    min-width: 0;
    overflow-wrap: anywhere;
  }
}

@media (max-width: 400px) {
  .sws-metrics {
    grid-template-columns: repeat(2, minmax(0, 1fr));
  }
  .sws-metric {
    padding: 12px;
  }
  .sws-metric > strong {
    font-size: 17px;
  }
  .sws-guide .sws-pad {
    width: 240px;
    height: 240px;
  }
  .sws-pad .gb:nth-of-type(1) {
    padding-bottom: 165px !important;
  }
  .sws-pad .gb:nth-of-type(2) {
    padding-right: 165px !important;
  }
  .sws-pad .gb:nth-of-type(4) {
    padding-left: 165px !important;
  }
  .sws-pad .gb:nth-of-type(5) {
    padding-top: 165px !important;
  }
}

/* 按任务分组设置卡片，桌面最多两列，手机保持单列与足够的触控间距 */
.sws-card-marker {
  display: none;
}
.sws-mount .sws-settings-grid {
  grid-template-columns: repeat(2, minmax(0, 1fr));
  gap: 22px;
}
.sws-mount .sws-more {
  padding: 0;
  border: 0;
  background: transparent;
}
.sws-mount .sws-more > summary {
  padding: 8px 0;
}
.sws-mount .b1 {
  text-align: left !important;
  padding: 24px;
  overflow-wrap: anywhere;
}
.sws-mount .sws-guide > .b1 {
  text-align: center !important;
}
.sws-mount .content {
  padding: 18px;
  margin-top: 8px;
  background: #1b1b1b;
}
.sws-mount .content > br:first-child,
.sws-mount .content > br:last-child {
  display: none;
}
.sws-mount .collapsible {
  margin: 0;
  background: #202020;
  border-color: #3a3a3a;
  min-height: 48px;
}
.sws-mount .collapsible::after {
  content: '+';
  float: right;
}
.sws-mount .collapsible.active::after {
  content: '−';
}
.sws-mount .b1 > hr {
  margin: 18px 0;
}
.sws-mount button {
  min-height: 44px;
}
.sws-mount button:not(.gb) {
  font-size: 14px;
}
.sws-mount .content p {
  margin: 10px 0;
  font-size: 13px;
  line-height: 1.8;
  color: var(--muted);
}

.sws-field-group {
  border: 0;
  padding: 0;
  margin: 0 0 22px;
  min-width: 0;
}
.sws-field-group legend {
  padding: 0;
  margin-bottom: 10px;
  color: #c1c1c1;
  font-size: 13px;
  line-height: 1.7;
}
.sws-unit-inputs {
  display: flex;
  align-items: center;
  gap: 4px;
}
.sws-unit-inputs input {
  flex: 1;
  width: 0 !important;
  min-width: 0;
  margin: 0;
  padding: 10px 6px;
  text-align: center;
}
.sws-unit-inputs select {
  min-width: 76px;
  margin: 0;
}
.sws-help {
  font-size: 12px;
  color: var(--muted);
}
.sws-number-field {
  display: grid;
  grid-template-columns: minmax(0, 1fr) 112px;
  gap: 14px;
  align-items: center;
  margin: 14px 0;
  font-size: 13px;
}
.sws-number-field input {
  min-width: 0;
  width: 100% !important;
  margin: 0;
  padding: 10px;
}
.sws-clock-action {
  margin: 18px 0;
}
.sws-clock-action button {
  margin: 0;
}
.sws-browser-time {
  font-size: 12px;
  color: var(--muted);
  padding: 12px 14px;
  border: 1px solid #2f2f2f;
  border-radius: 8px;
  margin-bottom: 14px;
}
.sws-site #date_ut,
.sws-site #time_ut {
  font-size: 18px;
  font-variant-numeric: tabular-nums;
}
.sws-site #site_lat,
.sws-site #site_long {
  font-variant-numeric: tabular-nums;
}
.sws-site .sws-help {
  margin-bottom: 0;
}

.sws-catalog-select {
  display: flex;
  align-items: center;
  justify-content: space-between;
  gap: 14px;
}
.sws-catalog-select select {
  width: 60%;
  margin: 0;
}
.sws-library #lib_message,
.sws-library #cat_message {
  width: 100% !important;
  margin: 14px 0 !important;
  padding: 12px 14px !important;
  border: 1px solid #2f2f2f !important;
  border-radius: 8px;
  background: #1d1d1d !important;
  font-size: 13px;
}
.sws-library #lib_free {
  font-size: 12px;
  font-weight: 400;
  float: none !important;
  text-align: left !important;
  margin: 6px 0;
  color: var(--muted);
}
.sws-editor-label {
  display: block;
  font-size: 13px;
  margin-bottom: 10px;
}
.sws-library textarea {
  display: block;
  width: 100%;
  min-height: 200px;
  margin: 0;
  padding: 14px;
  resize: vertical;
  background: #131313 !important;
  color: var(--text) !important;
  border-color: #4c4c4c !important;
  line-height: 1.65;
}
.sws-library a {
  display: inline-block;
  padding: 8px 12px;
  border: 1px solid #3a3a3a;
  border-radius: 8px;
  text-decoration: none;
  font-size: 13px;
}
.sws-library .content > br {
  display: none;
}
.sws-library #lib_clear {
  background: transparent;
  color: #a4a4a4;
  border-color: #4a4a4a;
}
.sws-library .sws-actions {
  margin-bottom: 10px;
}

.sws-option-grid {
  display: grid;
  grid-template-columns: repeat(2, minmax(0, 1fr));
  gap: 14px;
}
.sws-option-row {
  min-width: 0;
  padding: 16px;
  border: 1px solid #313131;
  border-radius: 10px;
  font-size: 13px;
  background: #1c1c1c;
  line-height: 1.8;
}
.sws-option-row > br:last-child {
  display: none;
}
.sws-option-row button {
  margin: 8px 4px 0 0 !important;
  min-width: 48px;
}
.sws-target .sws-option-row {
  min-height: 104px;
}
.sws-homepark .sws-option-row button {
  min-width: 72px;
}
.sws-pec .content {
  font-size: 13px;
}
.sws-align .content {
  font-size: 13px;
}
.sws-align #align_lr,
.sws-align #align_ud {
  font-size: 20px;
}
.sws-advanced-grid .b1 {
  height: auto;
}
.sws-mount .sws-actions {
  gap: 10px;
}
.sws-target .sws-actions button {
  height: 44px !important;
  min-width: 90px;
}
.sws-target [style='display: table; text-align: left;'] {
  width: 100%;
  font-size: 14px;
  font-variant-numeric: tabular-nums;
}

@media (max-width: 1100px) {
  .sws-mount .sws-settings-grid {
    grid-template-columns: 1fr;
  }
}

@media (max-width: 700px) {
  .sws-mount .b1 {
    padding: 18px;
  }
  .sws-mount .content {
    padding: 14px;
  }
  .sws-option-grid {
    grid-template-columns: 1fr;
  }
  .sws-number-field {
    grid-template-columns: minmax(0, 1fr) 90px;
    gap: 10px;
  }
  .sws-target [style='display: table; text-align: left;'] {
    font-size: 12px;
  }
  .sws-target .sws-actions button {
    flex: 1 1 40%;
    min-width: 0;
  }
  .sws-home-actions > div {
    padding: 10px;
  }
  .sws-mount .content button {
    min-height: 48px;
  }
  .sws-library .sws-actions button {
    flex: 1 1 auto;
  }
  .sws-target > .b1 {
    overflow-x: hidden;
  }
}

/* 时间与位置独立排列，长说明与操作保持清楚的间距 */
.sws-location-values {
  display: grid;
  grid-template-columns: 1fr 1fr;
  gap: 12px;
  margin: 16px 0;
}
.sws-location-values > div {
  display: flex;
  flex-direction: column;
  gap: 6px;
  padding: 12px;
  background: #1d1d1d;
  border-radius: 8px;
  font-size: 12px;
  color: var(--muted);
}
.sws-location-values strong {
  font-size: 15px;
  font-weight: 500;
}
.sws-device-time {
  margin-bottom: 6px;
}
.sws-sidereal-time {
  font-size: 13px;
  color: var(--muted);
}
.sws-homepark form {
  max-width: 640px;
}
.sws-homepark .sws-number-field {
  grid-template-columns: 1fr 150px;
}
.sws-pec-write {
  margin-top: 18px;
  padding-top: 16px;
  border-top: 1px solid #303030;
}
.sws-library #lib_free {
  display: block;
  clear: both;
  padding-top: 8px;
}
.sws-mount .b1 > .sws-card-marker {
  display: none;
}
.sws-mount .b1 > .sws-card-marker + br {
  display: none;
}

@media (max-width: 400px) {
  .sws-location-values {
    grid-template-columns: 1fr;
  }
  .sws-homepark .sws-number-field {
    grid-template-columns: 1fr 100px;
  }
}

.sws-option-grid > br {
  display: none;
}
.sws-limit-values {
  display: grid;
  grid-template-columns: minmax(0, 1fr);
  gap: 10px;
  clear: both;
  min-width: 0;
}
.sws-limit-values > div > span {
  min-width: 0;
}
.sws-mount .b1 > hr {
  clear: both;
}
.sws-limit-values > div {
  display: flex;
  justify-content: space-between;
  gap: 16px;
  padding: 10px 12px;
  background: #1d1d1d;
  border-radius: 8px;
  font-size: 12px;
}
.sws-limit-values strong {
  font-size: 16px;
}
.sws-site .sws-field-group:last-of-type {
  margin-bottom: 8px;
}

/* 同行卡片自动等高，紧凑显示状态与表单，手机恢复自然高度 */
.sws-mount {
  gap: 18px;
}
.sws-control-grid {
  align-items: stretch;
  gap: 18px;
}
.sws-guide,
.sws-tracking {
  display: flex;
  min-width: 0;
}
.sws-guide > .b1,
.sws-tracking > .b1 {
  height: 100%;
  flex: 1;
  min-height: 0 !important;
}
.sws-guide > .b1 {
  display: flex;
  flex-direction: column;
}
.sws-guide .sws-pad {
  margin: auto;
  flex-shrink: 0;
}
.sws-guide > .b1 > hr {
  width: 100%;
}

/* 方位控制与跟踪标题下方仅保留间距 */
.sws-guide > .b1 > hr:first-of-type,
.sws-tracking > .b1 > hr:first-of-type {
  border: 0;
  height: 0;
}
.sws-choice {
  margin: 12px 0;
  gap: 6px;
}
.sws-choice select {
  min-height: 42px;
  padding: 8px 12px;
}
.sws-track-switch {
  margin: 4px 0 12px;
}
.sws-mount button.sws-switch {
  width: 44px !important;
  min-width: 44px;
  min-height: 26px !important;
  height: 26px !important;
  padding: 0;
  border: 1px solid #4c4c4c;
  border-radius: 18px;
  background: #494949;
  flex-shrink: 0;
}
.sws-mount button.sws-switch::after {
  position: absolute;
  top: 3px;
  left: 3px;
  width: 18px;
  height: 18px;
  background: #fff;
}
.sws-mount button.sws-switch[aria-checked='true'] {
  background: var(--accent);
  border-color: var(--accent);
}
.sws-mount button.sws-switch[aria-checked='true']::after {
  transform: translateX(18px);
  background: #fff;
}

.sws-mount .b1 {
  padding: 18px;
}
.sws-mount .b1 > hr {
  margin: 12px 0;
}
.sws-mount .content {
  padding: 12px;
  margin-top: 6px;
}
.sws-mount .collapsible {
  min-height: 40px;
  padding: 8px 12px;
}
.sws-mount .sws-settings-grid {
  gap: 18px;
  align-items: stretch;
}
.sws-site-library > .b1 {
  height: 100%;
}
.sws-location-values {
  margin: 10px 0;
  gap: 8px;
}
.sws-location-values > div {
  padding: 10px;
}
.sws-clock-action {
  margin: 12px 0;
}
.sws-library #lib_free {
  padding-top: 2px;
  margin: 0;
}
.sws-library #lib_message,
.sws-library #cat_message {
  margin: 10px 0 !important;
  padding: 9px 12px !important;
}
.sws-limit-values {
  gap: 4px;
}
.sws-limit-values > div {
  padding: 6px 10px;
  background: transparent;
  border-bottom: 1px solid #292929;
  border-radius: 0;
}
.sws-option-grid {
  gap: 10px;
}
.sws-option-row {
  padding: 12px;
  min-height: 0 !important;
}
.sws-home-actions {
  gap: 18px;
}
.sws-home-actions > div {
  display: block;
  padding: 16px;
  background: #1d1d1d;
}
.sws-home-actions h3,
.sws-coordinate-form h3 {
  font-size: 15px;
  font-weight: 600;
  margin: 0 0 12px;
}
.sws-action-pair {
  display: flex;
  gap: 10px;
}
.sws-action-pair > br {
  display: none;
}
.sws-action-pair button {
  flex: 1;
  min-width: 0;
}
.sws-secondary {
  background: #2b2b2b !important;
  border-color: #383838 !important;
}
.sws-secondary:hover {
  background-image: linear-gradient(rgb(185 185 185 / 8%), rgb(185 185 185 / 8%));
}
.sws-unpark-row {
  margin-top: 8px;
  text-align: right;
}
.sws-unpark-row button {
  min-height: 32px !important;
  min-width: 0;
  padding: 4px 10px;
  background: transparent;
  color: var(--muted);
}

.sws-pier-status {
  clear: both;
  text-align: right;
  font-size: 12px;
  color: var(--muted);
  padding: 4px 0;
}
/* 卡片状态统一为柔和红色标签，空状态不显示底色 */
:is(#gto_status, #hp_sta, #align_progress, #guide_sta, #track, #foc_sta, #rot_sta) {
  display: inline-block;
  max-width: 100%;
  box-sizing: border-box;
  padding: 2px 10px;
  border-radius: 6px;
  background: #272727;
  color: var(--readout);
  font-size: 12px;
  font-weight: 600;
  line-height: 1.6;
  overflow-wrap: anywhere;
}

:is(#gto_status, #hp_sta, #align_progress, #guide_sta, #track, #foc_sta, #rot_sta):empty {
  display: none;
}

.sws-position-grid {
  display: grid;
  grid-template-columns: repeat(3, minmax(0, 1fr));
  gap: 8px;
  margin: 12px 0;
}
.sws-position-grid > div {
  container-type: inline-size;
  min-height: 86px;
  padding: 8px 12px;
  background: #1d1d1d;
  border-radius: 8px;
  display: flex;
  flex-direction: column;
  gap: 4px;
}
.sws-position-grid span {
  font-size: 12px;
  color: var(--muted);
}
.sws-position-grid strong {
  font-size: clamp(15px, 12cqi, 24px);
  line-height: 1.3;
  font-weight: 600;
  color: var(--readout);
  font-variant-numeric: tabular-nums;
}
.sws-coordinate-form {
  padding: 16px;
  background: #1d1d1d;
  border-radius: 10px;
  margin: 14px 0;
}
.sws-coordinate-grid {
  display: grid;
  grid-template-columns: repeat(2, minmax(0, 1fr));
  gap: 14px 18px;
  align-items: end;
}
.sws-coordinate-system {
  display: flex;
  flex-direction: column;
  gap: 8px;
}
.sws-coordinate-grid input {
  margin: 0;
  min-width: 0;
  width: 100% !important;
}
.sws-coordinate-grid fieldset {
  margin: 0;
  padding: 0;
  border: 0;
  min-width: 0;
}
.sws-coordinate-grid legend {
  font-size: 14px;
  font-weight: 600;
  padding: 0;
  margin-bottom: 8px;
}
.sws-coordinate-actions {
  display: flex;
  gap: 10px;
}
.sws-coordinate-actions button {
  flex: 1;
  margin: 0;
  min-height: 44px;
}
.sws-coordinate-units {
  display: flex;
  gap: 6px;
  align-items: center;
  min-width: 0;
}
.sws-coordinate-unit {
  flex: 1;
  min-width: 0;
  position: relative;
}
.sws-coordinate-unit input {
  padding: 10px 28px 10px 8px;
  text-align: center;
  appearance: textfield;
  -moz-appearance: textfield;
}
.sws-coordinate-unit input::-webkit-inner-spin-button {
  appearance: none;
}
.sws-coordinate-unit span {
  position: absolute;
  right: 9px;
  top: 50%;
  transform: translateY(-50%);
  pointer-events: none;
  color: var(--muted);
  font-size: 12px;
}
.sws-coordinate-units select {
  width: 56px;
  min-width: 56px;
  padding: 10px 5px;
  margin: 0;
}
.sws-coordinate-message {
  font-size: 13px;
  margin: 10px 0 0 !important;
  color: #b1b1b1;
}
.sws-coordinate-message:empty {
  display: none;
}
.sws-target .sws-actions {
  margin: 12px 0;
}

@media (max-width: 900px) {
  .sws-guide,
  .sws-tracking {
    display: block;
  }
  .sws-guide > .b1,
  .sws-tracking > .b1 {
    height: auto;
  }
  .sws-guide .sws-pad {
    margin: 14px auto;
  }
  .sws-site-library > .b1 {
    height: auto;
  }
}

@media (max-width: 700px) {
  .sws-coordinate-grid {
    grid-template-columns: 1fr;
  }
  .sws-coordinate-actions {
    grid-row: 4;
  }
  .sws-coordinate-form {
    padding: 12px;
  }
  .sws-position-grid {
    grid-template-columns: repeat(2, minmax(0, 1fr));
  }
  .sws-position-grid > div:nth-child(2) {
    order: 2;
  }
  .sws-position-grid > div:nth-child(3) {
    order: 4;
  }
  .sws-position-grid > div:nth-child(4) {
    order: 1;
  }
  .sws-position-grid > div:nth-child(5) {
    order: 3;
  }
  .sws-position-grid > div:nth-child(6) {
    order: 5;
  }
  .sws-home-actions > div {
    padding: 12px;
  }
  .sws-mount .b1 {
    padding: 16px;
  }
  .sws-coordinate-unit input {
    padding-left: 4px;
    padding-right: 22px;
  }
  .sws-coordinate-unit span {
    right: 5px;
  }
  .sws-coordinate-units select {
    width: 44px;
    min-width: 44px;
  }
  .sws-mount button.sws-switch {
    min-height: 26px !important;
  }
}

/* 跟踪高级设置按功能分组，按钮保持等宽并适应窄屏 */
.sws-tracking-setting {
  border: 0;
  border-bottom: 1px solid #303030;
  padding: 0 0 14px;
  margin: 0 0 14px;
  min-width: 0;
}
.sws-tracking-setting:last-child {
  border-bottom: 0;
  padding-bottom: 0;
  margin-bottom: 0;
}
.sws-tracking-setting legend {
  float: none;
  width: 100%;
  padding: 0;
  margin: 0 0 8px;
  color: var(--muted);
  font-size: 13px;
  line-height: 1.6;
}
.sws-tracking-options {
  display: flex;
  gap: 8px;
}
.sws-mount .content .sws-tracking-options button {
  flex: 1;
  min-width: 0;
  margin: 0;
  padding: 9px 8px;
  border-radius: 8px;
  line-height: 1.5;
  white-space: normal;
}
.sws-frequency-options button {
  background: #292929;
  border-color: #3c3c3c;
}
.sws-frequency-options button:hover {
  background-image: linear-gradient(rgb(185 185 185 / 8%), rgb(185 185 185 / 8%));
}
@media (max-width: 420px) {
  .sws-frequency-options {
    flex-wrap: wrap;
  }
  .sws-frequency-options button {
    flex-basis: calc(50% - 8px) !important;
  }
  .sws-frequency-options button:last-child {
    flex-basis: 100% !important;
  }
}

/* 时间概览使用统一的信息格，地点输入按实际内容限制宽度 */
.sws-time-summary {
  display: grid;
  grid-template-columns: repeat(2, minmax(0, 1fr));
  gap: 8px;
  margin: 12px 0 8px;
}
.sws-time-summary > div {
  display: flex;
  flex-direction: column;
  gap: 6px;
  padding: 12px;
  background: #1d1d1d;
  border: 0;
  border-radius: 8px;
  margin: 0;
  min-width: 0;
}
.sws-time-summary .sws-browser-time {
  grid-column: 1/-1;
  flex-direction: row;
  align-items: center;
  justify-content: space-between;
  flex-wrap: wrap;
  background: transparent;
  padding: 0 0 8px;
  border-bottom: 1px solid #2f2f2f;
  border-radius: 0;
}
.sws-time-label {
  font-size: 12px;
  color: var(--muted);
  font-weight: 400;
}
.sws-time-summary strong,
.sws-site #date_ut,
.sws-site #time_ut {
  font-size: 15px;
  font-weight: 500;
  line-height: 1.6;
  font-variant-numeric: tabular-nums;
}
.sws-time-summary #datetime {
  font-size: 13px;
  color: var(--text);
}
.sws-site .sws-location-values {
  grid-template-columns: repeat(2, minmax(0, 1fr));
  margin: 8px 0;
}
.sws-site .sws-clock-action {
  display: flex;
  justify-content: flex-end;
  margin: 12px 0;
}
.sws-site .sws-clock-action button {
  font-weight: 500;
  font-size: 13px;
}
.sws-site-fields {
  display: grid;
  grid-template-columns: repeat(auto-fit, minmax(min(100%, 250px), 1fr));
  gap: 16px;
}
.sws-site-fields .sws-field-group {
  margin: 0 !important;
}
.sws-site-fields .sws-field-group:last-child {
  grid-column: 1/-1;
}
.sws-site-fields legend {
  font-size: 12px;
  margin-bottom: 8px;
}
.sws-site-fields .sws-unit-inputs {
  max-width: 360px;
  gap: 4px;
}
.sws-site-fields .sws-unit-inputs input {
  height: 42px;
  padding: 8px 4px;
  font-size: 14px;
}
.sws-site-fields .sws-field-group:last-child .sws-unit-inputs {
  max-width: 220px;
}
.sws-site-fields .sws-unit-inputs select {
  height: 42px;
  min-width: 68px;
  padding: 8px;
  font-size: 14px;
}
.sws-site .sws-site-fields .sws-help {
  margin: 8px 0 0;
  font-size: 12px;
  line-height: 1.6;
}
.sws-site-submit {
  display: flex;
  justify-content: flex-end;
  margin-top: 16px;
  padding-top: 12px;
  border-top: 1px solid #2f2f2f;
}
.sws-site-submit button {
  margin: 0 !important;
  min-width: 100px;
}
@media (max-width: 420px) {
  .sws-time-summary,
  .sws-site .sws-location-values {
    grid-template-columns: 1fr;
  }
  .sws-site-fields {
    gap: 14px;
  }
  .sws-site .sws-clock-action button {
    width: 100%;
  }
  .sws-site .sws-clock-action small {
    width: 100%;
  }
  .sws-site-submit button {
    width: 100%;
  }
}

/* 时间与位置的信息框等宽等高，数值统一居中 */
.sws-time-summary > .sws-device-time,
.sws-time-summary > .sws-sidereal-time,
.sws-site .sws-location-values > div {
  box-sizing: border-box;
  min-height: 88px;
  padding: 12px;
  gap: 6px;
  display: flex;
  flex-direction: column;
  align-items: stretch;
  justify-content: flex-start;
  line-height: 1.6;
}
.sws-time-summary .sws-device-time > div,
.sws-time-summary #time_lst,
.sws-site .sws-location-values strong {
  display: flex;
  flex: 1;
  align-items: center;
  justify-content: center;
  text-align: center;
  width: 100%;
  gap: 5px;
  flex-wrap: wrap;
  font-size: 15px;
  line-height: 1.6;
}

/* 定位概览采用左侧读数与右侧操作的双栏布局 */
.sws-goto-overview {
  display: grid;
  grid-template-columns: minmax(0, 1fr) 260px;
  gap: 18px;
  align-items: stretch;
  margin: 12px 0;
}
.sws-goto-overview .sws-position-grid {
  margin: 0;
  min-width: 0;
}
.sws-goto-overview .sws-position-grid strong {
  text-align: center;
  margin: auto 0;
}
.sws-target .sws-goto-overview .sws-actions {
  display: grid;
  grid-template-columns: repeat(2, minmax(0, 1fr));
  grid-template-rows: repeat(2, minmax(44px, 1fr));
  gap: 8px;
  margin: 0;
  align-items: stretch;
}
.sws-target .sws-goto-overview .sws-actions button {
  width: 100% !important;
  height: 100% !important;
  min-width: 0;
  margin: 0;
  padding: 10px 8px;
  white-space: normal;
  line-height: 1.5;
}
.sws-target .sws-goto-overview .sws-actions button:nth-of-type(n + 3) {
  background: #292929;
  border-color: #3c3c3c;
}
@media (max-width: 1000px) {
  .sws-goto-overview {
    grid-template-columns: 1fr;
  }
  .sws-target .sws-goto-overview .sws-actions {
    grid-template-columns: repeat(4, minmax(0, 1fr));
    grid-template-rows: 1fr;
  }
}
@media (max-width: 700px) {
  .sws-goto-overview {
    gap: 12px;
  }
  .sws-target .sws-goto-overview .sws-actions {
    grid-template-columns: repeat(2, minmax(0, 1fr));
    grid-template-rows: repeat(2, minmax(44px, 1fr));
  }
}

/* 方位控制标题与其他卡片一致靠左，方向盘继续居中 */
.sws-mount .sws-guide > .b1 > div:first-child {
  text-align: left;
  align-self: stretch;
}

/* 跟踪卡片从开关开始显示，移除重复标题及标题占位 */
.sws-tracking > .b1 > div:first-child,
.sws-tracking > .b1 > br:first-of-type,
.sws-tracking > .b1 > hr,
.sws-guide #guide_rate,
.sws-guide #guide_sta,
.sws-tracking #track {
  display: none;
}

/* 设备跟踪开关行与方位控制标题采用统一层级 */
.sws-track-switch {
  min-height: 28px;
  margin: 0 0 16px;
  clear: both;
}
.sws-track-switch > span,
.sws-mount .sws-guide > .b1 > div:first-child {
  font-size: 16px;
  font-weight: 600;
  line-height: 28px;
}

/* 隐藏旧页头的名称区域，版本信息由原始服务端数据移至页头右侧 */
.sws-shell .t td:last-child {
  display: none;
}
.sws-version {
  font-size: 11px;
  line-height: 1.5;
  color: var(--muted);
  text-align: right;
  overflow-wrap: anywhere;
  max-width: 50%;
}
@media (max-width: 700px) {
  .sws-version {
    font-size: 10px;
    max-width: 45%;
  }
  .sws-topbar {
    align-items: flex-start;
  }
}

/* 调焦器与旋转器按位置、移动、参考位置和补偿组织 */
.sws-accessory-layout {
  gap: 18px;
}
.sws-accessory-grid {
  display: grid;
  grid-template-columns: minmax(0, 1.15fr) minmax(0, 1fr);
  gap: 18px;
  align-items: start;
}
.sws-accessory-main {
  display: flex;
  flex-direction: column;
  gap: 18px;
  min-width: 0;
}
.sws-accessory-motion {
  min-width: 0;
}
.sws-accessory-home {
  min-width: 0;
}
.sws-accessory-settings > .b1 {
  flex: 0 0 auto;
}
.sws-accessory-metrics > div:only-child {
  grid-column: 1/-1;
}
.sws-accessory-settings {
  display: flex;
  flex-direction: column;
  gap: 18px;
  min-width: 0;
}
.sws-accessory-layout .b1 {
  width: 100% !important;
  padding: 20px;
  text-align: left;
  min-width: 0;
  overflow-wrap: anywhere;
  background: var(--panel);
}
.sws-device-card-head {
  display: flex;
  justify-content: space-between;
  align-items: center;
  gap: 12px;
  margin-bottom: 16px;
}
.sws-device-card-head h2 {
  font-size: 16px;
  line-height: 1.5;
  margin: 0;
  color: #d8d8d8;
}
.sws-accessory-status {
  font-size: 12px;
  font-weight: 500;
  white-space: nowrap;
}
.sws-accessory-position {
  container-type: inline-size;
  min-height: 96px;
  display: flex;
  flex-direction: column;
  gap: 6px;
  padding: 14px;
  background: #1d1d1d;
  border-radius: 10px;
  margin-bottom: 16px;
}
.sws-accessory-position > span {
  font-size: 12px;
  color: var(--muted);
}
.sws-accessory-position strong {
  font-size: clamp(22px, 8cqi, 28px);
  line-height: 1.3;
  font-weight: 600;
  margin: auto 0;
  text-align: center;
  font-variant-numeric: tabular-nums;
}
.sws-accessory-goto {
  display: grid;
  grid-template-columns: minmax(0, 1fr) auto auto;
  gap: 10px;
  align-items: end;
}
.sws-accessory-target {
  min-width: 0;
  display: flex;
  flex-direction: column;
  gap: 6px;
  font-size: 12px;
  color: var(--muted);
}
.sws-accessory-input {
  position: relative;
  min-width: 0;
}
.sws-accessory-input input {
  width: 100%;
  min-width: 0 !important;
  margin: 0;
  padding: 10px 40px 10px 12px;
  height: 44px;
  font-size: 15px;
}
.sws-accessory-input > span {
  position: absolute;
  right: 12px;
  top: 50%;
  transform: translateY(-50%);
}
.sws-accessory-goto button {
  height: 44px;
  min-height: 44px;
  margin: 0;
  padding: 8px 14px;
  min-width: 0;
}
.sws-accessory-stop {
  background: #3e3e3e;
  border-color: #626262;
}
.sws-accessory-hint {
  font-size: 12px;
  color: var(--muted);
  line-height: 1.7;
  margin: 14px 0 10px;
}
.sws-accessory-jog {
  display: grid;
  grid-template-columns: repeat(4, minmax(0, 1fr));
  gap: 8px;
}
.sws-accessory-jog > br {
  display: none;
}
.sws-accessory-jog button {
  display: flex;
  flex-direction: column;
  align-items: center;
  justify-content: center;
  gap: 2px;
  width: 100% !important;
  min-width: 0;
  min-height: 68px;
  height: auto !important;
  padding: 8px 4px;
  margin: 0;
  background: #292929;
  border-color: #3c3c3c;
  touch-action: none;
  user-select: none;
}
.sws-accessory-jog button:hover {
  background-image: linear-gradient(rgb(185 185 185 / 8%), rgb(185 185 185 / 8%));
}
.sws-accessory-jog span {
  font-size: 22px;
  line-height: 1.3;
}
.sws-accessory-jog small {
  font-size: 12px;
  white-space: normal;
  overflow-wrap: anywhere;
}
.sws-accessory-motion-card .sws-choice {
  margin: 16px 0 8px;
}
.sws-accessory-motion-card .sws-choice > span {
  font-size: 12px;
  color: var(--muted);
  font-weight: 400;
}
.sws-accessory-live-rate {
  display: flex;
  justify-content: space-between;
  gap: 12px;
  color: var(--muted);
  font-size: 12px;
}
.sws-accessory-live-rate strong {
  font-weight: 500;
}
.sws-accessory-home-actions {
  display: grid;
  grid-template-columns: repeat(2, minmax(0, 1fr));
  gap: 10px;
}
.sws-accessory-home-actions button {
  width: 100%;
  min-height: 44px;
  margin: 0;
  padding: 10px 8px;
  white-space: normal;
}
.sws-accessory-home-actions button:last-child {
  background: #292929;
  border-color: #3c3c3c;
}
.sws-accessory-home-card .sws-accessory-hint {
  margin-top: 0;
}
.sws-accessory-layout .sws-device-selector > .b1 {
  display: flex;
  align-items: center;
  gap: 18px;
  padding: 14px 20px;
}
.sws-device-selector .sws-focuser-select {
  display: flex;
  align-items: center;
  justify-content: space-between;
  gap: 16px;
  flex: 1;
  min-width: 0;
}
.sws-focuser-select big {
  font-size: 13px;
  white-space: nowrap;
}
.sws-focuser-select-buttons {
  display: flex;
  gap: 6px;
  flex-wrap: wrap;
}
.sws-focuser-select-buttons button {
  width: 40px;
  min-height: 36px;
  padding: 4px;
  margin: 0;
}
.sws-accessory-metrics {
  display: grid;
  grid-template-columns: repeat(2, minmax(0, 1fr));
  gap: 8px;
  clear: both;
}
.sws-accessory-metrics > div {
  display: flex;
  flex-direction: column;
  gap: 6px;
  padding: 12px;
  border-radius: 8px;
  background: #1d1d1d;
  min-width: 0;
}
.sws-accessory-metrics span {
  font-size: 12px;
  color: var(--muted);
}
.sws-accessory-metrics strong {
  font-size: 17px;
  font-weight: 500;
  text-align: center;
  margin: auto 0;
}
.sws-accessory-field {
  display: grid;
  grid-template-columns: minmax(0, 1fr) 110px;
  align-items: center;
  gap: 14px;
  margin-bottom: 12px;
  font-size: 12px;
}
.sws-accessory-field input,
.sws-accessory-field select {
  width: 100%;
  min-width: 0 !important;
  margin: 0;
  padding: 9px 8px;
  text-align: center;
  height: 42px;
}
.sws-accessory-settings .content form > button {
  display: block;
  margin: 16px 0 0 auto;
  min-width: 100px;
  min-height: 44px;
}
.sws-accessory-settings .collapsible {
  margin: 0;
  min-height: 40px;
  padding: 8px 12px;
}
.sws-accessory-settings .content {
  padding: 14px;
}
.sws-accessory-settings .b1 > hr {
  margin: 14px 0;
}
.sws-accessory-settings .b1 > div:first-child {
  font-size: 15px;
}
.sws-accessory-derotate {
  display: grid;
  grid-template-columns: repeat(2, minmax(0, 1fr));
  gap: 8px;
  clear: both;
}
.sws-accessory-derotate > br {
  display: none;
}
.sws-accessory-derotate button {
  margin: 0;
  white-space: normal;
  min-height: 44px;
}
.sws-accessory-derotate button:nth-of-type(n + 3) {
  grid-column: 1/-1;
  background: #292929;
  border-color: #3c3c3c;
}

/* 附件参数卡片统一标题、折叠栏与表单间距 */
.sws-accessory-parameter-card > .sws-device-card-head {
  margin-bottom: 14px;
  flex-wrap: wrap;
}
.sws-accessory-parameter-card > .sws-accessory-metrics {
  margin-bottom: 14px;
}
.sws-accessory-parameter-card .collapsible {
  display: flex;
  align-items: center;
  justify-content: space-between;
  gap: 12px;
  width: 100%;
  margin: 0;
  min-height: 42px;
  padding: 9px 12px;
  background: var(--field);
  border: 1px solid var(--border);
  border-radius: 8px;
  font-size: 13px;
  color: var(--text);
}
.sws-accessory-parameter-card .collapsible::after {
  content: '+';
  color: var(--muted);
}
.sws-accessory-parameter-card .collapsible.active::after {
  content: '−';
}
.sws-accessory-parameter-card .collapsible:hover {
  background-image: linear-gradient(rgb(185 185 185 / 8%), rgb(185 185 185 / 8%));
}
.sws-accessory-parameter-card .content {
  padding: 14px 0 0;
  background: transparent;
  border-radius: 0;
}
.sws-accessory-parameter-card .content > br {
  display: none;
}
.sws-accessory-parameter-card .sws-accessory-field {
  font-size: 13px;
  line-height: 1.6;
  grid-template-columns: minmax(0, 1fr) 110px;
  gap: 14px;
  margin: 0;
  padding: 10px 0;
  border-bottom: 1px solid #292929;
}
.sws-accessory-parameter-card .sws-accessory-field:first-child {
  padding-top: 0;
}
.sws-accessory-parameter-card .sws-accessory-field input,
.sws-accessory-parameter-card .sws-accessory-field select {
  height: 42px;
  font-size: 14px;
}
.sws-accessory-form-actions {
  display: flex;
  justify-content: flex-end;
  padding-top: 14px;
}
.sws-accessory-form-actions button {
  margin: 0;
  min-width: 110px;
  min-height: 42px;
  padding: 8px 16px;
  font-size: 13px;
}
@media (max-width: 500px) {
  .sws-accessory-parameter-card .sws-accessory-field {
    grid-template-columns: minmax(0, 1fr) 90px;
    gap: 10px;
  }
}

@media (max-width: 900px) {
  .sws-accessory-grid {
    grid-template-columns: 1fr;
  }
  .sws-accessory-layout .b1 {
    padding: 18px;
  }
}
@media (max-width: 500px) {
  .sws-accessory-layout .sws-device-selector > .b1 {
    align-items: flex-start;
    flex-direction: column;
    gap: 8px;
    padding: 14px;
  }
  .sws-device-selector .sws-focuser-select {
    width: 100%;
    flex-wrap: wrap;
    gap: 8px;
  }
  .sws-accessory-goto {
    grid-template-columns: 1fr 1fr;
    gap: 8px;
  }
  .sws-accessory-target {
    grid-column: 1/-1;
  }
  .sws-accessory-goto button {
    width: 100%;
  }
  .sws-accessory-field {
    grid-template-columns: minmax(0, 1fr) 90px;
    gap: 10px;
  }
  .sws-accessory-settings #f_temp {
    float: none !important;
    clear: both;
    text-align: left !important;
    padding-top: 6px;
    font-size: 12px;
  }
  .sws-accessory-layout .b1 {
    padding: 16px;
  }
  .sws-accessory-jog {
    gap: 6px;
  }
}
/* 编码器按同步操作、实时读数与两轴设置排列。 */
.sws-encoder-layout {
  gap: 20px;
}
.sws-encoder-layout .b1 {
  width: 100% !important;
  padding: 20px;
  text-align: left;
}
.sws-encoder-sync-grid {
  display: grid;
  grid-template-columns: minmax(0, 1.15fr) minmax(0, 1fr);
  gap: 24px;
  align-items: center;
}
.sws-encoder-sync-actions {
  display: grid;
  grid-template-columns: repeat(2, minmax(0, 1fr));
  gap: 10px;
}
.sws-encoder-sync-actions button {
  width: 100%;
  min-height: 46px;
  margin: 0;
  padding: 10px;
  white-space: normal;
  line-height: 1.5;
}
.sws-encoder-origin {
  grid-column: 1 / -1;
}
.sws-encoder-auto {
  display: flex;
  justify-content: space-between;
  align-items: center;
  gap: 16px;
  padding-left: 24px;
  border-left: 1px solid var(--border);
}
.sws-encoder-auto > span {
  font-size: 14px;
  color: var(--muted);
}
.sws-encoder-toggle {
  display: flex;
  gap: 8px;
  flex-shrink: 0;
}
.sws-encoder-toggle button {
  margin: 0;
  min-width: 48px;
  min-height: 42px;
}
.sws-encoder-grid {
  display: grid;
  grid-template-columns: repeat(2, minmax(0, 1fr));
  gap: 18px;
  align-items: start;
}
.sws-encoder-readings {
  display: grid;
  grid-template-columns: repeat(3, minmax(0, 1fr));
  gap: 8px;
  margin: 14px 0;
}
.sws-encoder-metric {
  container-type: inline-size;
  min-width: 0;
  min-height: 88px;
  padding: 12px;
  border-radius: 8px;
  background: var(--field);
  display: flex;
  flex-direction: column;
  gap: 10px;
}
.sws-encoder-metric > span {
  font-size: 12px;
  color: var(--muted);
}
.sws-encoder-metric > strong {
  min-width: 0;
  margin: auto 0;
  text-align: center;
  font-size: clamp(15px, 14cqi, 24px);
  font-weight: 600;
  line-height: 1.3;
  font-variant-numeric: tabular-nums;
}
.sws-encoder-values {
  margin: 0 0 16px;
  font-size: 13px;
}
.sws-encoder-values > div {
  display: flex;
  align-items: baseline;
  justify-content: space-between;
  gap: 14px;
  padding: 9px 0;
  border-bottom: 1px solid var(--border);
}
.sws-encoder-values dt {
  color: var(--muted);
}
.sws-encoder-values dd {
  margin: 0;
  text-align: right;
  font-size: 15px;
  font-variant-numeric: tabular-nums;
}
.sws-encoder-layout .content {
  padding: 14px 0 0;
  background: transparent;
}
.sws-encoder-layout .content > br {
  display: none;
}
.sws-encoder-field {
  display: grid;
  grid-template-columns: minmax(0, 1fr) 140px;
  align-items: center;
  gap: 16px;
  margin: 0 0 12px;
  font-size: 13px;
}
.sws-encoder-field > span {
  overflow-wrap: anywhere;
}
.sws-encoder-field input {
  margin: 0;
  width: 100%;
  min-width: 0;
  text-align: center;
}
.sws-encoder-form-actions {
  display: flex;
  justify-content: flex-end;
  gap: 8px;
  padding-top: 4px;
}
.sws-encoder-form-actions button {
  margin: 0;
  min-width: 96px;
}
@media (max-width: 1000px) {
  .sws-encoder-sync-grid {
    grid-template-columns: 1fr;
    gap: 16px;
  }
  .sws-encoder-auto {
    border-left: 0;
    border-top: 1px solid var(--border);
    padding: 16px 0 0;
  }
}
@media (max-width: 850px) {
  .sws-encoder-grid {
    grid-template-columns: 1fr;
  }
}
@media (max-width: 500px) {
  .sws-encoder-layout .b1 {
    padding: 16px;
  }
  .sws-encoder-readings {
    grid-template-columns: 1fr;
  }
  .sws-encoder-metric {
    display: grid;
    grid-template-columns: 72px minmax(0, 1fr);
    min-height: 58px;
    align-items: center;
  }
  .sws-encoder-metric > span {
    overflow-wrap: anywhere;
  }
  .sws-encoder-metric > strong {
    font-size: clamp(14px, 7cqi, 20px);
  }
  .sws-encoder-sync-actions {
    grid-template-columns: 1fr;
  }
  .sws-encoder-auto {
    align-items: flex-start;
  }
  .sws-encoder-toggle button {
    min-width: 42px;
  }
  .sws-encoder-field {
    grid-template-columns: minmax(0, 1fr) 100px;
    gap: 10px;
  }
  .sws-encoder-metric {
    padding: 10px 8px;
  }
}
/* 时间地点、限位与校准说明共用清楚的层级与紧凑表单。 */
.sws-mount .sws-advanced-grid {
  align-items: start;
}
.sws-mount .sws-advanced-grid > .b1 {
  height: auto;
  align-self: start;
}
.sws-align-correction {
  padding: 16px;
  background: var(--field);
  border: 1px solid var(--border);
  border-radius: 10px;
}
.sws-align-correction > div {
  display: flex;
  align-items: center;
  gap: 12px;
  margin-bottom: 8px;
}
.sws-align-correction strong {
  font-size: 24px;
  line-height: 1.3;
  font-weight: 600;
}
.sws-align-correction > span {
  color: var(--muted);
  font-size: 14px;
  line-height: 1.6;
}
.sws-site .content > br,
.sws-limits .content > br,
.sws-align .content > br,
.sws-pec .content > br {
  display: none;
}
.sws-site .content,
.sws-limits .content,
.sws-align .content {
  padding: 16px 0 0;
  background: transparent;
}
.sws-limit-values {
  grid-template-columns: repeat(2, minmax(0, 1fr));
  gap: 10px;
  margin: 14px 0;
}
.sws-limit-values > div {
  display: flex;
  flex-direction: column;
  justify-content: space-between;
  gap: 12px;
  min-height: 100px;
  padding: 14px;
  background: var(--field);
  border: 1px solid var(--border);
  border-radius: 10px;
}
.sws-limit-values > div > span {
  color: var(--muted);
  font-size: 14px;
  line-height: 1.6;
}
.sws-limit-values strong {
  color: var(--text);
  font-size: 24px;
  line-height: 1.25;
  font-weight: 600;
  text-align: center;
  font-variant-numeric: tabular-nums;
}
.sws-limits-form {
  display: grid;
  gap: 14px;
}
.sws-limits-form fieldset {
  margin: 0;
  padding: 14px;
  min-width: 0;
  background: #1d1d1d;
  border: 1px solid var(--border);
  border-radius: 10px;
}
.sws-limits-form legend {
  padding: 0 6px;
  font-size: 15px;
  font-weight: 600;
  color: var(--text);
}
.sws-limits .sws-number-field {
  grid-template-columns: minmax(0, 1fr) 112px;
  gap: 16px;
  margin: 12px 0;
  font-size: 14px;
  line-height: 1.6;
}
.sws-limits .sws-number-field input {
  height: 46px;
  font-size: 17px;
  text-align: center;
}
.sws-form-footer {
  display: flex;
  justify-content: flex-end;
  gap: 10px;
  padding-top: 4px;
}
.sws-form-footer button {
  margin: 0;
  min-width: 110px;
}
.sws-refine-title {
  margin: 0 0 12px;
  font-size: 16px;
  font-weight: 600;
}
.sws-refine-steps {
  counter-reset: refine-step;
  list-style: none;
  padding: 0;
  margin: 0;
  display: grid;
  gap: 10px;
}
.sws-refine-steps li {
  position: relative;
  padding: 14px 14px 14px 50px;
  background: #222;
  border: 1px solid var(--border);
  border-radius: 9px;
  font-size: 14px;
  line-height: 1.8;
  color: var(--text);
}
.sws-refine-steps li::before {
  counter-increment: refine-step;
  content: counter(refine-step);
  position: absolute;
  left: 14px;
  top: 16px;
  display: grid;
  place-items: center;
  width: 24px;
  height: 24px;
  border-radius: 50%;
  background: #3b3b3b;
  font-size: 13px;
  font-weight: 600;
}
.sws-refine-action {
  display: flex;
  justify-content: flex-end;
  margin-top: 14px;
}
.sws-refine-action button {
  margin: 0;
  min-width: 130px;
}
.sws-pec-status {
  display: flex;
  justify-content: space-between;
  align-items: center;
  gap: 14px;
  padding: 14px;
  background: var(--field);
  border: 1px solid var(--border);
  border-radius: 10px;
  font-size: 14px;
}
.sws-pec-status > span {
  color: var(--muted);
}
.sws-pec-status strong {
  font-size: 17px;
  font-weight: 500;
  color: var(--text);
}
.sws-pec > hr:nth-of-type(2) {
  margin: 14px 0;
}
.sws-time-label,
.sws-site-fields legend {
  font-size: 14px;
  line-height: 1.6;
}
.sws-time-summary > .sws-device-time,
.sws-time-summary > .sws-sidereal-time,
.sws-site .sws-location-values > div {
  container-type: inline-size;
  min-height: 112px;
  padding: 16px;
  gap: 12px;
  border: 1px solid var(--border);
  border-radius: 10px;
  background: var(--field);
}
.sws-time-summary .sws-device-time > div,
.sws-time-summary #time_lst,
.sws-site .sws-location-values strong {
  font-size: clamp(18px, 8cqi, 25px);
  line-height: 1.4;
  font-weight: 600;
}
.sws-site #date_ut,
.sws-site #time_ut {
  font-size: inherit;
  line-height: inherit;
  white-space: nowrap;
}
.sws-time-summary #datetime {
  font-size: 16px;
  font-weight: 500;
}
.sws-site .sws-location-values > div {
  font-size: 14px;
  color: var(--muted);
}
.sws-site .sws-location-values strong {
  color: var(--text);
}
.sws-site .sws-clock-action button {
  min-height: 44px;
  font-size: 15px;
}
.sws-site-fields {
  grid-template-columns: repeat(auto-fit, minmax(min(100%, 260px), 1fr));
  gap: 14px;
}
.sws-site-fields .sws-field-group {
  padding: 12px 14px 14px;
  border: 1px solid var(--border);
  border-radius: 10px;
  background: #1d1d1d;
}
.sws-site-fields legend {
  padding: 0 6px;
}
.sws-site-fields .sws-unit-inputs {
  max-width: none;
}
.sws-site-fields .sws-unit-inputs input,
.sws-site-fields .sws-unit-inputs select {
  height: 46px;
  font-size: 17px;
}
.sws-site .sws-site-fields .sws-help {
  font-size: 13px;
  line-height: 1.8;
}
@media (max-width: 600px) {
  .sws-site-fields {
    grid-template-columns: 1fr;
  }
  .sws-limit-values {
    grid-template-columns: 1fr;
  }
  .sws-limit-values > div {
    min-height: 80px;
    display: grid;
    grid-template-columns: minmax(0, 1fr) 70px;
    align-items: center;
    gap: 10px;
  }
  .sws-limit-values strong {
    font-size: 22px;
  }
  .sws-limits .sws-number-field {
    grid-template-columns: minmax(0, 1fr) 88px;
    gap: 12px;
  }
  .sws-limits-form fieldset {
    padding: 12px;
  }
  .sws-refine-action button,
  .sws-form-footer button {
    width: 100%;
  }
}
</style>
)UI";
