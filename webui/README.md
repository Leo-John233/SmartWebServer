# Smart Web Server Web UI

This directory contains the Vue 3 development environment for the ESP8266 interface

## Commands

- `npm run dev` starts the local MSW simulation at `http://127.0.0.1:5173`
- `npm run dev:device` proxies legacy requests to the configured ESP device
- `npm run test` runs protocol and component tests
- `npm run test:e2e` builds and verifies the production preview with Chromium
- `npm run build` creates the production bundle in `dist/`
- `npm run build:embed` creates the `/ui/` bundle and regenerates `src/webui/WebAssets.generated.h`
- `npm run preview` serves the latest production build at `http://127.0.0.1:4173`

## Device proxy

Copy `.env.example` to `.env.local` and change `VITE_DEVICE_URL` when the ESP is not available at the default `http://192.168.0.1` address

The browser talks to Vite on the same origin, so the device does not require CORS changes

## Firmware build

Run `npm ci` and `npm run build:embed` before compiling the ESP8266 sketch. The generated header contains the minified gzip assets and is committed with the firmware source

On ESP8266 WiFi builds, `/` and `/ui/` open the Vue interface. `/index.htm` remains the legacy fallback. Other targets keep their existing default page

The Network page never reads passwords. Leaving a password field blank preserves the stored password

## Simulation

The scenario selector covers normal, disconnected, Goto, tracking, parked, fault, limit, mount-only, and all-subsystem states

Capability boundaries come from `fixtures/serial-replays.json`; the browser does not derive features from controller version strings

The interface supports English and Chinese from the language switch in the header

## 中文开发与预览

在 `webui` 目录运行 `npm run dev`，打开终端显示的本地地址（默认 http://127.0.0.1:5173）。不需要连接 ESP8266 或 OnStep。页头的场景选择器可切换正常、断线、GoTo、跟踪、驻停、故障、限位以及全部附件。默认中文；英文和夜间红光偏好保存在当前浏览器中。

- 首页是观测控制：当前坐标、目标坐标、方向键、速度、GoTo、停止和跟踪。
- 模拟器会记录操作并返回变化后的状态，支持星表编辑、调焦器定位和网络保存；重新加载页面会重置模拟设备，不代表真实机械运动或天体计算。
- `npm run build:mock` 生成可独立预览的模拟版；`npm run test:e2e:mock` 在桌面、手机、平板尺寸检查交互。
- `npm run build:embed` 生成 ESP8266 固件使用的压缩资源，不包含模拟服务。设备连接与原版相同。
- `npm run dev:device` 通过 `.env.local` 中的 `VITE_DEVICE_URL` 代理设备，包含旧版状态、表单和附件描述地址。

## 原版功能对应

| 原版页面 | Vue 入口与迁移方式 |
| --- | --- |
| Controller | 控制器概览显示环境、驱动与监测数据；完整设置读取驱动轴参数、还原及固件开放的重启/监测操作 |
| Mount | 观测控制包含归零/驻停、校准、GoTo、方向/速度、跟踪、时间地点、PEC、星表和限位；完整设置补充归零偏移、蜗杆步数等固件条件选项 |
| Rotator | 定位、快慢手动移动、归零、速度、消旋、反向、视差角与回差 |
| Focuser | 多调焦器切换、定位、快慢手动移动、归零、速度、回差及温度补偿 |
| Auxiliary | 从原版页面读取开关、模拟输出、除露与定时快门的控件描述，Vue 渲染并保留原始范围和提交单位 |
| Encoders | 双轴位置比较、双向同步、原点与自动同步 |
| Network | 常用连接设置；完整设置保留固件提供的 IP/网关/子网、超时、管理密码、OTA 和重启选项 |

高级设置与辅助设备当前采用兼容迁移：只解析原固件的字段描述，由 Vue 创建控件，不执行返回页面的脚本，不嵌入旧页面。旧版页面处理器需要保留。字段名称由设备固件提供，部分设备专有名称仍可能显示固件语言。

完整功能的实机等价性尚未验收。模拟样例覆盖常见路径，不是所有编译开关/硬件组合的证明；Servo/StallGuard 提供数值与最近 120 次采样曲线。后续实机验证应重点检查按住/松开、转向中停止、校准、附件单位、配置保存与重启。

## 代码与提交约定

- 代码使用有意义的命名，复杂分支展开书写，Vue 模板每个属性独立换行
- 说明性注释使用中文，末尾不带中文或英文句号，重点解释协议差异和设计原因
- 模拟逻辑分为 `mockDevice.ts`（设备状态）、`mockPages.ts`（页面样本）和 `handlers.ts`（接口处理）
- 修改后运行 `npm run format`，提交前运行 `npm run format:check` 和 `npm run lint:comments`
- 每完成一个可独立说明的功能模块，执行相关检查并提交一次本地 Git
- 提交标题写明模块及改动，提交正文列出具体内容与验证结果，不将无关改动混入同一提交
- 页面源码修改完成后运行 `npm run build:embed`，同步固件内嵌资源
