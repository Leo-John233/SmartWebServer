# SmartWebServer 开发维护指南

面向 OnStep 和 OnStepX 的网页控制与网络通信固件，支持 ESP8266 和 ESP32

本分支保留原生网页与控制协议，并维护中文界面、网页配网、坐标输入和板载功能扩展

项目介绍见 [仓库首页](../README.md)，本文说明工程配置、开发维护和本地验证，历史修改见 [修改日志](CHANGELOG.md)

## 快速开始

### 准备工程

1. 使用 Arduino IDE 打开根目录的 [SmartWebServer.ino](../SmartWebServer.ino)
2. 安装与硬件匹配的 ESP8266 或 ESP32 开发板支持包
3. 检查 [Config.h](../Config.h)、[Extended.config.h](../Extended.config.h) 和 [引脚映射](../src/pinmaps/)
4. 选择正确的开发板与串口，完成编译和烧录

固件不依赖 Node 或 Vue，网页资源随固件编译并由设备提供

| 配置项 | 当前默认值 | 说明 |
| --- | --- | --- |
| `OPERATIONAL_MODE` | `WIFI` | 网络工作模式 |
| `SERIAL_BAUD_DEFAULT` | `9600` | 与主控开始通信时使用的串口速率 |
| `SERIAL_BAUD` | `115200` | 目标工作串口速率，需要与主控配置匹配 |
| `DISPLAY_LANGUAGE` | `L_cn` | 网页显示语言，修改后重新编译 |
| `DISPLAY_WEATHER` | `OFF` | 环境数据显示开关 |
| `RA_BRAKE_PROTECTION` | `ON` | 赤经轴抱闸保护，需要匹配的硬件与驱动反馈 |

烧录前必须核对抱闸配置、引脚占用和实际接线，具体要求见下方的网络活动灯与赤经轴抱闸说明

### 编译命令

在仓库根目录执行，与实际开发板匹配的两个示例目标如下

```powershell
# ESP8266 NodeMCU 1.0
arduino-cli compile --clean --jobs 1 --warnings all --fqbn esp8266:esp8266:nodemcuv2 .

# ESP32 Dev Module
arduino-cli compile --clean --jobs 1 --warnings all --fqbn esp32:esp32:esp32 .
```

验证两个平台时依次执行，构建目录与临时文件放在本地忽略目录 `tests/output/` 下

### 连接设备

以下默认值仅适用于设备尚未保存自定义网络设置的情况

| 项目 | 默认值 |
| --- | --- |
| 设备热点名称 | `ONSTEPX` |
| 热点连接密码 | `password` |
| 热点访问地址 | <http://192.168.0.1/> |
| 网络页面管理密码 | `password` |

连接热点后打开设备地址，先确认与 OnStep 或 OnStepX 主控的通信状态，再使用控制功能

热点密码与网络页面管理密码独立设置，实际名称、密码和地址以设备保存的配置为准

升级前记录 Wi-Fi 和其他重要设置，存储格式变化时可能需要重新配置

## 代码与文档

| 路径 | 内容 |
| --- | --- |
| [README.md](../README.md) | 项目介绍与文档入口 |
| [DEVELOPMENT.md](DEVELOPMENT.md) | 工程配置与开发维护指南 |
| [CHANGELOG.md](CHANGELOG.md) | 按日期整理的修改与验证记录 |
| [demo/README.md](../demo/README.md) | 本地演示说明与运行方法 |
| [SmartWebServer.ino](../SmartWebServer.ino) | 固件入口与初始化 |
| [Config.h](../Config.h) | 主要编译配置 |
| [Extended.config.h](../Extended.config.h) | 网络和管理配置 |
| [src/pages/](../src/pages/) | 页面、表单和状态接口 |
| [src/extensions/](../src/extensions/) | 界面、配网、抱闸、状态与坐标扩展 |
| [src/locales/](../src/locales/) | 多语言资源 |
| [src/pinmaps/](../src/pinmaps/) | 平台引脚映射 |
| [src/extensions/README.md](../src/extensions/README.md) | 扩展接入点与上游合并说明 |

本分支跟踪固件入口、配置、固件源码、各级 README、开发指南、修改日志、许可证和 Git 忽略规则

测试、对照和演示脚本仅保留在本地，依赖、缓存及编译产物不加入版本控制

下文中的测试与预览命令仅适用于已保留相应工具的环境，仅检出仓库时可直接编译固件

## 网页与设备控制

- 控制器、赤道仪、网络、调焦器、旋转器、辅助设备和编码器页面按设备能力显示
- 页面采用灰黑与暗红主题，桌面使用侧栏，手机使用底部导航和更多菜单
- 标签使用原生整页链接，支持表单提交和浏览器前进后退
- 跟踪、方向移动、导星、回零、停放、校准、限位、星表和 PEC 沿用原有控制接口
- 新增 JNOW 赤经赤纬输入，设备依次接受赤经和赤纬后才执行 GOTO 或同步
- 坐标校验失败或连接中断时显示结果，不自动重发运动命令
- 位置与限位采用只读和编辑两种状态，取消恢复原值，保存使用原始参数接口
- 编码器、环境数据和其他附件功能取决于编译配置、硬件与主控能力
- 显示语言在编译时选择，界面文字集中维护在 `src/locales/`

### 页面加载与状态刷新

样式、脚本和正文在同一次 HTML 响应中输出，不增加独立 CSS 或 JavaScript 资源请求

- 完整样式位于正文前，导航先输出，界面增强脚本随后输出
- Flash 资源分段进入共用缓冲，单段不超过 1024 字节
- 同一页面最多保留一个在途状态请求，操作命令优先于下一次状态读取
- 操作结束后进入 5 秒快速反馈窗口，状态刷新间隔为 250 ms
- 页面隐藏、离开或开始切换时暂停旧页面轮询，返回页面后恢复
- 慢响应不叠加请求，失败后逐步降低读取频率，恢复后重新读取
- 客户端断开后停止后续模板和只读参数查询，已提交的控制操作继续按原流程处理
- 运动停止确认未完成时等待停止队列，最多等待 5 秒，超时留在原页供重试

首屏传输与本地浏览器测试结果不能直接代表实机切页耗时

## 网页配网

### 使用流程

1. 连接设备热点，进入网络页面并输入管理密码
2. 查看当前连接状态，需要更新网络列表时点击刷新
3. 选择目标网络，输入密码，隐藏网络通过其他网络手动添加
4. 按需要选择 DHCP 或填写静态 IP、网关和子网掩码
5. 点击连接，等待实际连接结果
6. 连接成功后，可使用设备在路由器网络中的 IP 访问网页

### 保存与显示规则

- 新网络的参数和密码先用于连接验证，成功后才替换原有凭据
- 验证期间保存其他设置，不会提前写入待验证的密码
- STA 开关意图单独提交，重启后恢复已保存的开关状态
- 原样保存已连接网络不会重复写入设置或主动断开连接
- 已提交的密码只在当前页面内暂存，刷新或离开页面后清除
- 设备状态接口只返回是否记住密码，不返回密码内容
- 成功提示显示 5 秒，错误提示显示 8 秒，普通状态轮询不重播旧提示
- 网络名称最多 31 字节，STA 密码最多 63 字节，热点密码最多 31 字节

### 扫描与连接恢复

| 场景 | 当前行为 |
| --- | --- |
| 进入网络页或切页返回 | 只读取状态，不自动扫描或重新连接 |
| 首次开启 STA 且没有已知网络 | 自动搜索可用网络 |
| 已有已知网络 | 开启后直接尝试连接 |
| 手动连接或后台连接进行中 | 暂停扫描，保留 SDK 处理短暂失败的机会 |
| 单次连接 | 只发起一次连接，等待窗口为 25 秒 |
| 手动配网失败 | 停止本次尝试，保留热点与 STA 接口，等待用户处理 |
| 已保存网络短暂断线 | 先允许 SDK 恢复，30 秒后补充后台连接 |
| 后台连接超时 | 再等待 30 秒后允许恢复，热点有客户端时暂缓主动重连 |
| 用户关闭 STA | 停止后台重连并关闭 STA 接口 |

热点与路由器应使用不同网段，静态地址和连接后的 DHCP 地址都会进行冲突检查

AP 与 STA 可以同时工作，连接路由器后热点会跟随路由器信道，手机短暂断开时需要重新连接

修改热点名称或地址后，使用新名称或新地址重新访问

DHCP 主机名由 `HOST_NAME` 转为小写，当前为 `onstep-sws`，mDNS 名称由 `MDNS_NAME` 单独配置

### 状态与诊断

登录后访问 `/net-wifi?action=status`，可读取运行时间和连接状态

ESP8266 还提供最近一次启动原因、重置信息、空闲堆内存、无线模式和热点客户端数量

诊断读取不切换无线模式、不重启热点，也不向 OnStep 串口发送诊断内容

连接与恢复时序分别维护在 `src/extensions/network/StationConnection.h` 和 `StationReconnect.h`

## 网络活动灯与赤经轴抱闸

逻辑运行在 SmartWebServer 内，关闭网页后继续采样和处理，不修改 OnStep 主控固件

### 引脚与配置

| 引脚 | 功能 | 电平与行为 |
| --- | --- | --- |
| IO2 | `LED_STATUS_PIN` 网络活动灯 | 低电平亮灯，业务收发时闪烁，失联时保留原连接提示 |
| IO5 | `RA_BRAKE_PIN` 赤经轴抱闸 | LOW 抱闸，HIGH 松闸，不再兼任第二轴编码器 A |

| `RA_BRAKE_PROTECTION` | 行为 |
| --- | --- |
| `ON` | 从引脚初始化起保持 LOW，启动保护模块并显示网页开关 |
| `OFF` | 不采样、不自动停止，隐藏保护开关，从引脚初始化起保持 HIGH |

引脚初始化前的电平由硬件决定，IO5 所需的外部下拉电阻必须保留

### 自动保护流程

1. 上电先保持抱闸，输出任务或采样任务启动失败时继续保持 LOW
2. 自动保护开启且本次运行没有故障锁定时，有效无故障反馈连续保持至少 500 ms 才松闸
3. 驱动故障、反馈无效、失联、反馈超过 1 秒未更新或关闭网页保护时，锁定本次事件并启动停止流程
4. 先发送 `:Q#` 终止运动，再发送 `:Td#` 关闭跟踪，分别记录两条命令的结果
5. 停止失败时每隔 2 秒重试，每轮连同首次最多尝试 3 次，等待期间保持当前抱闸电平
6. 两条命令均成功后，必须收到停止流程结束之后的至少两帧有效静止反馈，且跨度至少 250 ms，才输出 LOW 抱闸
7. 无效、过期或不带 `ST` 的反馈会清除未完成的静止确认，不使用固定倒计时强制抱闸
8. 故障锁定不会因反馈恢复、刷新页面或切换开关而自动解除，重新上电后重新执行启动检查

正常静止本身不会触发抱闸，单独出现 `ST` 也不会启动故障停止流程

### 手动操作与存储

- 手动解锁立即输出 HIGH，并暂停自动逻辑，不检查当前驱动或主控状态
- 手动解锁不发送运动命令，也不恢复此前的跟踪或移动
- 重新开启自动锁定后恢复自动流程，等待停止确认时可点击重试停止
- 仅活动灯和自动开关设置写入存储，故障记录保留在本次运行的内存中
- 旧故障存储记录不参与判断，手动解锁只作用于本次运行
- 网页、TCP 和串口命令仍由原通道处理，不按抱闸状态过滤运动命令

### 反馈与验证边界

状态使用现有 `:GXU1#`，正常采样间隔为 250 ms，无效反馈后每秒探测一次，单次读取超时为 200 ms

反馈必须符合 `ST`、`OA`、`OB`、`GA`、`GB`、`OT`、`PW`、`GF` 的字段定义，七类故障中任一置位均按故障处理

`ST` 不是机械轴速反馈，`GXU1` 也不能证明保持电流建立、驱动初始化完成或主控全部运动结束

当前实现未验证机械安全性，实际接线、上电电平、串口反馈和抱闸动作仍需在目标设备上检查

## 开发维护

### 扩展职责

| 目录 | 维护内容 |
| --- | --- |
| `src/extensions/ui/` | 配色、布局、导航、编辑器与界面资源输出 |
| `src/extensions/network/` | 配网处理、连接恢复和 DHCP 主机名 |
| `src/extensions/network/ui/` | 配网样式、模板和浏览器脚本 |
| `src/extensions/boardIo/` | 活动灯、抱闸策略、GPIO 和保护采样 |
| `src/extensions/status/` | 驱动反馈解析和状态文本 |
| `src/extensions/mount/` | 坐标输入校验与请求处理 |
| `src/extensions/web/` | 路由登记与网络活动通知 |

原模块仍负责设备能力判断、页面参数、存储、协议和命令处理，具体接入位置见 [扩展维护说明](../src/extensions/README.md)

### 上游合并

已集成的原版对照版本为 SWS 2.10h，提交 `193a81809f5b19b3afa2a8c58f18291c2f0b0475`

原版分支为 `SmartWebServer-Original`，开发分支保留本地提交历史，后续更新通过 GitHub Pull Request 审查和合并

1. 保留上游的协议与业务修复，再按扩展接入清单核对本地功能
2. 逐项检查路由、登录限制、设备能力条件、表单名称、卡片标识和 AJAX 键值
3. 保留轴参数边界、附件目标、编码器默认值、时区和长文本缓冲区修复
4. 合并后更新已集成的原版提交编号，重新执行适用的验证

本地保留对照工具时，可运行只读检查

```powershell
node tools/upstream-audit.cjs origin/SmartWebServer-Original
```

工具只检查本地引用，不自动更新远端、不下载或合并文件

文件交集只是审查提示，不能保证后续合并没有文本冲突

## 验证说明

### 本地预览

需要本地保留演示与测试脚本，并安装 Node、g++ 和相应浏览器依赖

```powershell
npm ci --prefix tests --ignore-scripts
node demo/server.cjs
```

预览说明见 [demo/README.md](../demo/README.md)，模拟状态不代表实机数据

### 常用检查

以下命令在仓库根目录运行，浏览器检查通常还需要安装 Edge 和 Playwright 依赖

| 检查内容 | 本地命令 |
| --- | --- |
| 原版接口与命令 | `node tests/page-interfaces.cjs` |
| 模板输出与首屏 | `node tests/page-template.cjs` 和 `node tests/native-first-paint.cjs` |
| 导航与状态轮询 | `node tests/native-navigation.cjs` 和 `node tests/page-performance.cjs` |
| 配网后端与恢复 | `node tests/network-wifi-backend.cjs` 和 `node tests/network-wifi-recovery.cjs` |
| 配网界面与提示 | `node tests/network-wifi-ui.cjs <输出目录>` 和 `node tests/network-feedback.cjs` |
| 页面多语言与布局 | `node tests/pages-languages.cjs <输出目录>` 和 `node tests/locales.cjs` |
| 坐标与设备参数 | `node tests/coordinate-input.cjs` 和 `node tests/axis-page.cjs <HTML输出路径>` |
| 抱闸策略与适配 | `node tests/brake-policy.cjs` 和 `node tests/board-io-backend.cjs` |
| 重启与设置保存 | `node tests/brake-reboot.cjs` 和 `node tests/nv-critical-commit.cjs` |
| 第三方命令转发 | `node tests/tracking-transport.cjs` |

全部语言的工具链检查使用 `node tests/firmware-languages.cjs <compile_commands.json> <输出目录>`

编译清单由 Arduino CLI 的 `compile --only-compilation-database` 生成，语言切换在隔离副本中进行

程序、截图、快照和日志统一放在 `tests/output/`，该目录不提交到 Git

### 已知限制

- 编译通过只说明代码可构建，不能证明真实无线、串口和机械动作正常
- ESP-07S 曾出现热点可见但终端掉线或暂时无法加入的情况，仍需实机定位
- 切页卡顿和偶发空白需要结合设备运行时间、复位原因、连接状态与串口反馈验证
- 新增坐标输入仅支持 JNOW 赤经赤纬，尚未提供 J2000 或方位高度转换
- 不同日期的验证对应各自的源码快照，历史结果不能直接视为当前设备验证结果

## 修改记录

历史修改、回归数据和构建结果集中记录在 [修改日志](CHANGELOG.md)

## 项目来源与许可

基于 [Howard Dutton 的 SmartWebServer](https://github.com/hjd1964/SmartWebServer) 维护

使用 GNU GPL v3 许可证，具体条款见 [LICENSE](../LICENSE)
