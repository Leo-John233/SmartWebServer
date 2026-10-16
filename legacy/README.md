# 原版 SWS 固件

Arduino 工程入口：`SmartWebServer/SmartWebServer.ino`

本目录保留原版网页与控制协议实现，不包含 Vue 静态资源或 Vue API 适配层
这里的“原版”指原网页实现，保留仓库已经完成的协议修复，并非回退到历史上游版本

- 配置文件：`SmartWebServer/Config.h` 和 `SmartWebServer/Extended.config.h`
- 网页首页：设备地址 `/`
- 直接用 Arduino IDE 打开本目录的工程即可编译，不需要 Node 或 Vue
- 也可在仓库根目录运行 `node scripts/build-firmware.mjs legacy`，生成 `build/legacy/SmartWebServer`
- 使用 Arduino CLI：`node scripts/build-firmware.mjs legacy --compile --fqbn <开发板标识>`

共用的控制协议由此目录维护，Vue 构建按需复制到自己的输出目录，构建过程不改写本目录

## 网页配网

- 连接设备热点，进入网络页面并输入管理密码
- 打开连接路由器开关，设备会搜索附近 Wi-Fi，也可点击搜索按钮重新搜索
- 点选网络后填写密码，再点击保存并连接，无需重启
- 点选网络默认使用 DHCP，需要固定 IP 时可展开高级设置
- 设备热点与路由器连接可以同时使用，配网时会保留热点作为恢复入口
- 连接路由器后热点会跟随路由器信道，手机可能短暂断线，重新连接即可
- 热点与路由器应使用不同网段，页面会检查静态地址并在连接后检查 DHCP 地址
- 更改热点名称或地址后，需要用新名称或新地址重新访问
- 配网页面支持网络名称和密码各最多 31 字节，中文名称会占用多个字节
- Wi-Fi 名称、密码和开关保存在设备中，重启后继续使用

## 配网验证

- `node legacy/tests/network-wifi-backend.cjs` 编译并执行配网处理测试，需要本机安装 g++
- `node legacy/tests/network-wifi-ui.cjs <输出目录>` 验证扫描与连接交互，并生成桌面和手机预览，需要 webui 的 Playwright 依赖和 Edge 浏览器
- 支持 ESP8266 和 ESP32 编译验证，浏览器测试中的无线数据来自模拟接口
- 刷入设备后还需验证真实网络扫描、密码错误恢复、DHCP 地址、路由器掉线和热点信道切换

## 原版更新记录

- 已合并 `origin/SmartWebServer-Original` 的 `193a818`，固件版本更新到 2.10h
- 原版根目录代码映射到 `legacy/SmartWebServer`，保留开发分支的配网页面、设备参数和编译产物忽略规则
- 配网接口已适配上游新版键值存储，路由器参数和密码分开保存
- 新版首次挂载旧版存储失败时会重新初始化，刷写前请记下设备中保存的 Wi-Fi 和其他设置，刷写后检查并重新设置
- 合并前的本地版本保存在 `codex/backup-before-original-update` 分支
- 本次已通过配网后台回归、浏览器交互及 320/390 像素布局检查，并完成 ESP8266 和 ESP32 编译
