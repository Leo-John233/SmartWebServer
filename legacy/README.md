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
