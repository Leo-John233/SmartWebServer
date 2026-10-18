# SWS 本地扩展维护说明

独立扩展集中在本目录，按功能分组，继续使用原版的任务调度、页面服务和 OnStep 命令通道，没有新增插件注册框架或运行时配置层。

| 目录 | 职责 |
| --- | --- |
| `ui/` | 公共主题、布局、导航、编辑器和静态资源输出 |
| `network/` | 配网处理、连接与重连时序及 DHCP 主机名 |
| `network/ui/` | 配网页面样式、模板和浏览器脚本 |
| `boardIo/` | 网络活动灯、抱闸策略、GPIO 和保护采样适配 |
| `boardIo/Page.*`、`boardIo/View.h` | 板载功能的页面接口与模板 |
| `status/` | 驱动反馈解析和有界中文状态文本格式化 |
| `mount/` | 新增坐标输入的校验和请求处理 |
| `web/` | 页面路由登记及网络活动通知 |

Arduino 会自动编译 `src/` 下的 `.cpp`，扩展实现只保留一份，不在旧目录保留实现副本或转发 `.cpp`。

## 与原版的边界

扩展可以调用原版提供的服务，原版只通过头文件和少量调用接入扩展。保护策略和连接时序不依赖页面，页面不直接控制 GPIO。

| 原版位置 | 需要保留的接入内容 |
| --- | --- |
| `SmartWebServer.ino` | `boardIo.initPins()`、`boardIo.begin()`、配网轮询和扩展路由，保持原有启动顺序 |
| `pages/Pages.h`、`pages/Page.h` | 扩展入口声明及公共资源接口 |
| `pages/htmlHeaders.h`、页面处理函数 | 公共配色和文字宏，`pageHead()` 与分区模板 |
| `pages/Page.*` | 沿用原版 `pageHeader()` 及能力条件，直接输出完整导航、当前标签和标题 |
| `pages/network/Network.cpp` | 配网渲染入口，登录、OTA 和其他原版设置仍由本文件处理 |
| `pages/index/StatusTile.cpp`、`AxisTile.cpp` | 活动灯与抱闸设置，读取同一次驱动反馈 |
| `pages/mount/Mount.cpp` | 分派 `coord_action`，坐标校验和命令顺序在扩展中维护 |
| `libApp/status/State.h`、`State.cpp`、`StateController.cpp` | 保护反馈更新声明及调用，驱动文本格式化入口 |
| `libApp/cmd/Cmd.cpp`、Wi-Fi 与以太网命令服务 | 网络活动通知，协议帧和回复语义保持原有约定 |
| `lib/wifi/webServer/WebServer.*` | 直接使用原版 ESP8266WebServer 或 ESP32 WebServer，不保留自定义子类 |
| `lib/wifi/WifiManager.cpp` | STA 开启后调用 `network/StationIdentity.h` 应用 DHCP 主机名，网页配网复用同一方法 |

相对于 `src/` 的路径见上表。本地保留 `tools/upstream-audit.cjs` 时，可用它生成差异清单；该工具不随固件仓库跟踪，实际合并按上表审查即可。

实验性网络防护、NETWORK_LOAD_GUARD 配置和自定义 WebServer 子类已删除。HTTP 请求调度、发送等待及 TCP 命令处理均沿用原版实现。

网页按原版在同一个 HTML 响应中输出样式、脚本和正文，不登记独立 CSS/JS 资源。较大的界面模板通过有界 String 分段复制，仍使用原有 sendContentAndClear；完整发送调用之间让出 OnTask 调度，避免新增抱闸采样被长页面饿死。

以下修改仍属于原模块，不能只靠搬迁目录消除合并工作：

- `Config.h`、`Config.defaults.h`、`Validate.h` 与引脚映射中的编译配置和冲突校验
- `WifiManager.*` 对候选凭据与已保存凭据的隔离
- `ShimCached.h`、`ShimDelayedCommit.h` 的关键提交和失败重试修复
- 轴参数、附件目标、编码器第二轴默认值、时区边界和长文本缓冲区修复
- 页面卡片的设备能力条件、表单参数、AJAX 标识和布局模板插入位置
- `locales/` 中的翻译与新增语言宏

这些位置需要逐段合并，不能用整份旧文件覆盖原作者的新文件，也不要把原模块的安全修复移到样式层。

## 原版对照版本与本地历史

已集成的原版对照版本为 SWS 2.10h，提交 `193a81809f5b19b3afa2a8c58f18291c2f0b0475`，对应本地远程跟踪引用 `origin/SmartWebServer-Original`。

历史上的 `35fb5ee6` 通过复制文件更新原版，开发分支继续保留此前的本地提交历史。原版提交编号用于源码差异检查，检查工具不要求开发分支包含原版提交，也不向本地历史添加上游父提交。

## 后续合并步骤

后续更新通过 GitHub 的 Pull Request 审查和合并。原版对照引用更新后，本地保留对照工具时可运行只读检查：

```powershell
node tools/upstream-audit.cjs origin/SmartWebServer-Original
```

检查脚本只读取本地 Git 引用和工作区，不下载、不合并、不改写文件。它报告上游增量、双方都修改的文件、原版接入点、独立扩展和其他本地差异，并检查本地头文件路径。文件交集是审查提示，不代表每一项都一定产生文本冲突。

准备更新 Pull Request 时保留原作者的协议和业务更新，再按上表接回扩展。审查或处理冲突时重点核对路由、登录限制、设备能力条件、表单名称与 AJAX 标识，不使用批量选择本地版本解决冲突。

在 GitHub 完成更新合并后，将本说明的编号更新到已集成的新原版提交。本地保留对照工具和测试时，同时更新其中的默认对照版本与接口回归基准。使用本地工具时可显式指定其他对照版本：

```powershell
node tools/upstream-audit.cjs <待检查原版引用> <已集成原版版本>
```

## 验证

本地测试、演示和对照脚本不加入版本控制，`tests/` 与 `tools/` 整体忽略，`demo/` 只保留 README 说明。现有本地文件继续保留；新环境缺少这些脚本时，按接入清单核对包含路径，并执行两个目标的真实固件编译。

- `node tools/upstream-audit.cjs` 检查扩展接入及本地包含路径
- `node tests/firmware-languages.cjs <compile_commands.json> <输出目录>` 检查全部十种语言，范围同时包含 `src/pages/` 与 `src/extensions/`，编译清单缺少任一页面或扩展时必须重新生成
- 抱闸检查使用 `brake-policy`、`board-io-backend`、`brake-reboot`、`controller-status-text`
- 网络检查使用 `network-wifi-backend`、`network-wifi-performance`、`network-wifi-ui`、`network-wifi-recovery`、`sta-navigation`、`sta-lifecycle`、`wifi-scan-history` 和 `esp8266-scan-behavior`
- 页面检查使用 `coordinate-input`、`network-page`、`page-performance`、`page-header`、`native-first-paint`、`native-icon-paint`、`native-navigation` 和 `locales`
- 使用 Arduino 工具链编译 ESP8266 与 ESP32，构建目录和临时文件放在忽略的输出目录

资源长度使用编译期数组大小。主题仍在原版 HTML 响应内联输出，使用原生 String / sendContentAndClear，并将新增大模板限制为每段 1024 字节。PageResourceWriter 在一次 head 内共用 1 KB String，合并相邻小片段，完成后输出不足一段的尾部；不发送空分块。每个完整发送调用结束后才让出任务；Wi-Fi 客户端断开后停止后续模板输出，各功能页在主题输出后检查连接并跳过后续正文查询。没有独立公共资源端点、缓存资源服务器、iframe 或异步页面替换层。

样式在正文前输出。网络样式仅发送给 net.htm，位置/限位编辑样式和脚本仅发送给 mount.htm，板载灯及抱闸样式和脚本仅发送给首页。公共控件和导航继续使用同一份模板。附件、编码器、辅助通道及位置/限位详细样式也分别限定到对应页面，通用卡片标题继续共用。pageHead 根据原服务器 uri() 选择内容，不增加 HTTP 请求。

普通导航在点击时暂停旧文档的只读轮询，保留浏览器原生跳转；运动停止确认及历史返回恢复仍保留。navigation-poll-lifecycle.cjs 验证慢速导航不再保留旧状态连接，page-cancel-lifecycle.cjs 执行真实赤道仪处理函数验证断线跳过正文读取。

导航、当前标签、标题和跟踪控件由固件直接生成，脚本只绑定操作与状态标识。原生跨文档过渡在浏览器支持时等待完整导航标记后替换画面，后续正文继续分段传输，不再等待页尾。导航停止确认通过 head 中立即注册的事件委托处理，正文未收齐也不会绕过确认。旧浏览器及过渡超时回退普通整页导航；该处理不保证设备网络异常时绝对零闪动。native-first-paint.cjs 使用真实固件页头验证提前绘制、部分响应、停止确认和禁用 JavaScript 后的原生导航。

| 功能 | 接口与实现边界 |
| --- | --- |
| 各标签页面与状态刷新 | 保留原版 .htm、.txt 路由与 KeyValue 状态标识 |
| 跟踪、移动、导星、回零、停泊、位置与限位 | 保留原版 mount-ajax-get.txt / mount.htm 参数及 OnStepCmd 命令通道 |
| 第三方软件 | 保留原版 CmdServer 端口和指令/回复判定；保留缓冲区越界与写入失败修复 |
| 坐标输入 | 扩展原版 mount-ajax-get.txt 参数，使用同一 OnStepCmd 通道，不增加串口客户端 |
| Wi-Fi 扫描、候选密码与连接状态 | 原版没有对应操作；保留 /net-wifi，注册在同一原生 WebServer，使用同一 WifiManager |
| 板载灯与抱闸 | 原版没有对应操作；保留 /board-io，使用原 OnTask 与 OnStepCmd，不修改主控固件 |

本地测试和固件编译不能替代上电、真实串口反馈、并发访问与抱闸接线的实机验证。

## 本地修改审查记录

对照基线为上述原版 193a818，按接入点、协议、页面、存储、配置与独立扩展检查本地差异，结论如下

| 范围 | 处理与依据 |
| --- | --- |
| HTTP 与 TCP 传输 | 删除自定义 WebServer、防护配置与 TCP 分轮处理，直接回到原版接口，保留 80 字节回复缓冲区修复 |
| 页面导航 | 沿用原版在固件端输出页头和选中标签，保留原生链接、表单、路由及状态节点，外观脚本不再重建导航 |
| 静态资源 | 样式链接位于 head，基础样式和网络页样式一并缓存，CSS/JS 使用内核 Flash 发送接口；图标直接随页头输出，不缓存设备状态 |
| 状态与命令 | 保留原版 OnStepCmd 和 State 调度，校时、运动及附件命令不按抱闸状态过滤，保留短写检测与长反馈缓冲修复 |
| 配网 | 保留 AP+STA 流程、有限连接窗口、候选密码隔离及开关提交；切页不扫描，扫描不能中断后台连接或 SDK 关联，仍使用原生 WiFi 接口与原版 NV 键 |
| 抱闸 | 保留独立策略和原版 OnTask 调度，未知反馈保持原电平，停止每轮最多三次；网络活动通知只记录时间，不调用停止逻辑 |
| 存储与设置 | 保留缓存提交及失败重试修复，沿用 NV/KV 接口，不引入另一套存储格式 |
| 布局、翻译与参数 | 保留能力条件、原命令参数及状态标识，继续检查七个页面、轴参数、附件、坐标、时区和语言宏 |

包含路径检查、原版 TCP 回放与本地自动化用于验证上述边界；模拟器不会连接串口。真实无线耗时、串口响应及驱动反馈仍需目标硬件验证，本记录不把静态审查当作全部功能的实机通过证明。
