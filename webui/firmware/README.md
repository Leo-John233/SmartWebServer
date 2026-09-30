# Vue 固件适配层

此目录只用于 Vue 版固件，不参与原版工程编译

- `Config.h`、`Extended.config.h`：Vue 固件的独立硬件配置
- `api/`：Vue 使用的语义 API
- `webui/`：网页资源服务、独立路由注册和生成的压缩资源

在 `webui` 目录运行 `npm run firmware:prepare`，先生成网页资源，再装配到仓库的 `build/vue/SmartWebServer`
安装 Arduino CLI 和对应开发板包后，运行 `npm run firmware:build -- --fqbn <开发板标识>` 编译
也可以用 Arduino IDE 打开装配后的 `SmartWebServer.ino`

Vue 内嵌固件目前只支持 ESP8266 WiFi，其他平台会明确报错，避免误以为已启用 Vue 页面
网页首页为 `/`，同时保留 `/ui/`；兼容表单和设备控制接口仍复用原固件实现
请修改此目录下的配置，构建输出中的配置会在下次构建时重新生成
