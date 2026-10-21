#pragma once

// 网络页使用独立的响应式样式，不加载外部字体和脚本
// 新版通信与固件更新卡片限制在屏幕宽度内，避免手机页面横向滚动
const char html_network_css[] PROGMEM = R"NET(
<style>
.sws-network-entry.nw-card {
  grid-column: 1/-1;
  display: block;
  width: 100%;
  max-width: 960px;
  margin: 0 auto;
  padding: 24px;
}
.sws-network-entry h2 { font-size: 20px; line-height: 1.5; margin: 0; }
.sws-entry-note { margin: 10px 0 0; color: var(--muted); font-size: 14px; line-height: 1.8; }
.sws-login-card form { margin: 20px 0 0; max-width: 560px; }
.sws-login-card label { margin: 0 0 8px; font-size: 14px; }
.sws-login-actions { display: flex; align-items: stretch; gap: 10px; }
.sws-login-actions input:not([type='checkbox']) { flex: 1; min-width: 0; width: 0; min-height: 46px; text-align: left; }
.sws-login-actions button { flex: 0 0 auto; margin: 0; min-height: 46px; padding: 8px 20px; }
.sws-network-help { margin-top: 24px; padding-top: 24px; border-top: 1px solid var(--border); }
.sws-network-help p { margin: 12px 0 0; color: var(--muted); font-size: 14px; line-height: 1.85; overflow-wrap: anywhere; }
.sws-network-help br { display: none; }
.sws-network-help b { display: inline; }
@media(max-width:800px) {
  .sws-network-entry.nw-card { padding: 20px; }
}

.nw-grid {
  display: grid;
  grid-template-columns: repeat(2, minmax(0, 1fr));
  gap: 18px;
  margin: 22px 0;
}

.nw-card {
  border: 1px solid var(--border, #383838);
  background: var(--panel, #191919);
  border-radius: 14px;
  padding: 20px;
  min-width: 0;
}

.nw-card h2 {
  margin: 0 0 8px;
  font-size: 20px;
}
.nw-note {
  color: #a9a9a9;
  font-size: 13px;
  line-height: 1.7;
  margin: 8px 0 16px;
}

.nw-card label, .nw-card .nw-toggle {
  display: block;
  margin: 14px 0 6px;
}
.nw-card input:not([type='checkbox']),
.nw-card select {
  display: block;
  width: 100%;
  margin: 0;
}

.nw-card input[type='checkbox'] {
  width: 20px;
  height: 20px;
  vertical-align: middle;
  accent-color: var(--accent);
  margin: 0 8px 0 0;
}

.nw-card .nw-toggle {
  display: flex;
  align-items: center;
  font-weight: 600;
}
.nw-card details {
  margin: 16px 0;
  padding: 12px;
  border: 1px solid #373737;
  border-radius: 9px;
}

.nw-card summary {
  cursor: pointer;
}
.nw-actions {
  display: flex;
  flex-wrap: wrap;
  gap: 8px;
  margin-top: 16px;
}
.nw-secondary {
  background: #282828;
}

.nw-state {
  padding: 16px;
  background: #202020;
  border: 1px solid #353535;
  border-radius: 12px;
  overflow-wrap: anywhere;
}

.nw-state-top {
  display: flex;
  align-items: center;
  justify-content: space-between;
  gap: 12px;
  color: #9e9e9e;
  font-size: 12px;
}
.nw-badge {
  padding: 4px 9px;
  border-radius: 6px;
  background: #2e2e2e;
  color: #bbbbbb;
  font-size: 12px;
  white-space: nowrap;
}
.nw-badge[data-connected='true'] {
  background: #343434;
  color: #bfbfbf;
}

.nw-state-name {
  display: block;
  font-size: 18px;
  font-weight: 600;
  color: #e5e5e5;
  margin: 14px 0 16px;
}
.nw-state-values {
  display: flex;
  gap: 28px;
  flex-wrap: wrap;
  margin: 0;
}
.nw-state-values div {
  min-width: 0;
}
.nw-state-values dt {
  color: #939393;
  font-size: 12px;
  margin-bottom: 5px;
}
.nw-state-values dd {
  margin: 0;
  color: #cfcfcf;
  font-size: 14px;
  font-variant-numeric: tabular-nums;
}

/* 网络列表将名称放在左侧，锁和信号放在右侧，勾选只表示已经连接 */
/* 访问地址直接显示在已连接网络下方，断开后隐藏旧地址 */
.nw-access {
  display: flex;
  align-items: baseline;
  flex-wrap: wrap;
  gap: 8px 16px;
  margin: 12px 0 0;
  padding: 0 16px;
}
.nw-access dt {
  color: #9d9d9d;
  font-size: 13px;
}
.nw-access dd {
  margin: 0;
  color: #e4e4e4;
  font-size: 15px;
  font-variant-numeric: tabular-nums;
  overflow-wrap: anywhere;
}

.nw-networks {
  overflow: hidden;
  margin-top: 12px;
  border: 1px solid #373737;
  border-radius: 14px;
  background: #1d1d1d;
}

#nw-results {
  max-height: 360px;
  overflow: auto;
}
.nw-networks:empty {
  display: none;
}

.nw-row {
  display: flex;
  align-items: center;
  gap: 12px;
  width: 100%;
  min-height: 72px;
  text-align: left;
  background: transparent;
  border: 0;
  border-bottom: 1px solid #2f2f2f;
  border-radius: 0;
  margin: 0;
  padding: 14px 16px;
  font-weight: 400;
}

.nw-row:last-child {
  border-bottom: 0;
}
.nw-row:hover {
  background: #282828;
}

.nw-network-copy {
  display: flex;
  flex: 1;
  min-width: 0;
  flex-direction: column;
  gap: 5px;
}
.nw-network-name {
  font-size: 16px;
  overflow-wrap: anywhere;
  color: #e4e4e4;
}
.nw-network-meta {
  font-size: 12px;
  color: #9d9d9d;
}

.nw-wifi {
  width: 25px;
  height: 25px;
  flex-shrink: 0;
  fill: none;
  stroke: currentColor;
  stroke-width: 1.8;
  stroke-linecap: round;
  stroke-linejoin: round;
}
.nw-lock {
  width: 14px;
  height: 18px;
  flex-shrink: 0;
  fill: none;
  stroke: currentColor;
  stroke-width: 1.8;
}

.nw-selected {
  width: 20px;
  color: #afafaf;
  font-size: 22px;
  flex-shrink: 0;
  text-align: center;
}
.nw-info {
  border: 1.5px solid #868686;
  border-radius: 50%;
  width: 19px;
  height: 19px;
  text-align: center;
  line-height: 19px;
  font-size: 13px;
  color: #acacac;
  flex-shrink: 0;
}

.nw-list-heading {
  display: flex;
  align-items: center;
  justify-content: space-between;
  margin-top: 20px;
  gap: 12px;
}
.nw-list-heading strong {
  font-size: 14px;
  color: #aaaaaa;
}
.nw-list-heading button {
  margin: 0;
  padding: 7px 12px;
  min-height: 36px;
  font-size: 13px;
}

.nw-switch-line {
  display: flex;
  justify-content: space-between;
  align-items: center;
  gap: 16px;
  margin: 18px 0;
}
.nw-switch-line > span {
  margin: 0;
}
.nw-switch {
  appearance: none;
  position: relative;
  width: 44px !important;
  height: 26px !important;
  border-radius: 18px;
  padding: 0;
  background: #494949;
  cursor: pointer;
  margin: 0 !important;
  flex-shrink: 0;
}
.nw-switch:before {
  content: '';
  position: absolute;
  top: 3px;
  left: 3px;
  width: 18px;
  height: 18px;
  border-radius: 50%;
  background: #fff;
  transition: transform 0.15s;
}
.nw-switch:checked {
  background: var(--accent);
  border-color: var(--accent);
}
.nw-switch:checked:before {
  transform: translateX(18px);
}

/* 点选网络后弹出独立连接窗口，密码和高级设置不再常驻列表 */
#nw-dialog {
  box-sizing: border-box;
  width: calc(100% - 24px);
  max-width: 440px;
  max-height: 85dvh;
  overflow: auto;
  color: #cdcdcd;
  background: #1d1d1d;
  border: 1px solid #4d4d4d;
  border-radius: 18px;
  padding: 22px;
}
#nw-dialog::backdrop {
  background: #0009;
}

.nw-dialog-head {
  display: flex;
  align-items: center;
  justify-content: space-between;
  gap: 12px;
}
.nw-dialog-head h2 {
  overflow-wrap: anywhere;
  font-size: 20px;
  margin: 0;
}
.nw-dialog-head button {
  background: transparent;
  border: 0;
  font-size: 22px;
  padding: 4px 10px;
}

.nw-password-line {
  display: flex;
  align-items: stretch;
  border: 1px solid #585858;
  border-radius: 9px;
  overflow: hidden;
  background: #1f1f1f;
}
.nw-card .nw-password-line input {
  min-width: 0;
  border: 0;
  border-radius: 0;
  flex: 1;
}
.nw-password-line button {
  background: transparent;
  border: 0;
  border-radius: 0;
  padding: 8px 12px;
  color: #afafaf;
  font-size: 13px;
  white-space: nowrap;
}

/* 使用统一的显示按钮，避免浏览器自带的密码图标重复出现 */
#nw-password::-ms-reveal {
  display: none;
}

#nw-dialog .nw-actions button {
  flex: 1;
}
#nw-dialog-error {
  color: #c1c1c1;
  overflow-wrap: anywhere;
}
#nw-dialog-error:empty {
  display: none;
}

[hidden] {
  display: none !important;
}

.nw-feedback {
  margin: 12px 0 0;
  padding: 10px 12px;
  border: 1px solid var(--border);
  border-left: 3px solid #777;
  border-radius: 8px;
  background: var(--field);
  color: var(--text);
  font-size: 13px;
  line-height: 1.7;
  overflow-wrap: anywhere;
}
.nw-feedback.nw-error {
  border-left-color: var(--accent);
}

@media (max-width: 650px) {
  .nw-grid {
    grid-template-columns: 1fr;
  }
  .nw-card {
    padding: 16px;
  }
  .nw-actions button {
    flex: 1;
  }
  input {
    max-width: 100%;
  }
}

/* 只有开关本体响应切换，文字和空白保持普通指针 */
.nw-switch-line,.nw-switch-line>span,.nw-card .nw-toggle { cursor:default; -webkit-tap-highlight-color:transparent; }
.nw-card :is(a,button,input,select,textarea,label,summary,[role='switch']) { -webkit-tap-highlight-color:transparent; }
.nw-card :is(button,summary,[role='switch']) { -webkit-user-select:none; user-select:none; }
.nw-card :is(a,button,input,select,textarea,summary):focus:not(:focus-visible) { outline:none; }
</style>
)NET";
