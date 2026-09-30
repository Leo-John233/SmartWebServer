# 原版与 Vue 版目录隔离

| 内容 | 源码位置 | 独立构建输出 |
| --- | --- | --- |
| 原版 SWS 固件及网页 | [legacy/SmartWebServer](legacy/SmartWebServer) | build/legacy/SmartWebServer |
| Vue 前端 | [webui](webui) | webui/dist/production、mock、embed |
| Vue 固件适配与独立配置 | [webui/firmware](webui/firmware) | build/vue/SmartWebServer |

原版：在仓库根目录运行 `node scripts/build-firmware.mjs legacy`，或直接用 Arduino IDE 打开 `legacy/SmartWebServer/SmartWebServer.ino`

Vue 版：在 `webui` 目录运行 `npm ci` 和 `npm run firmware:prepare`，再用 Arduino IDE 打开 `build/vue/SmartWebServer/SmartWebServer.ino`

命令行编译：原版使用 `node scripts/build-firmware.mjs legacy --compile --fqbn <开发板标识>`，Vue 使用 `npm run firmware:build -- --fqbn <开发板标识>`（在 webui 目录执行）

两版分别维护自己的 Config.h / Extended.config.h，生成目录不可作为源码编辑；运行 `node --test scripts/build-firmware.test.mjs` 检查隔离性

隔离保留已有协议修复，不代表还原历史上游快照；Vue 兼容接口仍复用原版控制逻辑，当前内嵌适配仍限 ESP8266 WiFi

---

# OnStep Smart Web Server
This is the WiFi and Ethernet web server for OnStep, it is also sometimes referred to as the "SWS".
The SWS enables OnStep to be controlled from a website and/or by passing commands over IP.
The SWS is known to work from personal computers (using a web browser, ASCOM, or INDI), phones (Android and iOS), tablets, and the wireless Smart Hand Controller.

# Optionally, the SWS can also provide:
Support for encoders where it integrates tightly with OnStep; or if not using Encoders, OnStepX can use those four connections (pins) as general purpose I/O.
There is also support for using the "ACGAM R1 Bluetooth 4.0 Wireless Gamepad" over Bluetooth LE which you can read about here:
https://onstep.groups.io/g/main/wiki/26762

# Flashing The Smart Web Server
The exact flashing procedure depends on which device you will be using.  You can find instructions for setting up the SWS in its Wiki here:
https://onstep.groups.io/g/main/wiki/26881

# License
The Smart Web Server is open source free software, licensed under the GPL.

See [LICENSE.txt](./LICENSE.txt) file.

# Primary Author
[Howard Dutton](http://www.stellarjourney.com)
