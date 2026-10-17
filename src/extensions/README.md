# SWS 本地扩展维护说明

独立扩展集中在本目录，按功能分组，继续使用原版的任务调度、页面服务和 OnStep 命令通道，没有新增插件注册框架或运行时配置层。

| 目录 | 职责 |
| --- | --- |
| `ui/` | 公共主题、布局、导航、编辑器和静态资源输出 |
| `network/` | 配网处理、连接与重连时序、网络负载限制、ESP8266 网页服务适配 |
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
| `SmartWebServer.ino` | `boardIo.initPins()`、`boardIo.begin()`、配网轮询、扩展路由及资源登记，保持原有启动顺序 |
| `pages/Pages.h`、`pages/Page.h` | 扩展入口声明，资源及页面外壳接口 |
| `pages/htmlHeaders.h`、页面处理函数 | 公共配色和文字宏，`pageAppearance()`、`servePageShell()` 与分区模板 |
| `pages/network/Network.cpp` | 配网渲染入口，登录、OTA 和其他原版设置仍由本文件处理 |
| `pages/index/StatusTile.cpp`、`AxisTile.cpp` | 活动灯与抱闸设置，读取同一次驱动反馈 |
| `pages/mount/Mount.cpp` | 分派 `coord_action`，坐标校验和命令顺序在扩展中维护 |
| `libApp/status/State.h`、`State.cpp`、`StateController.cpp` | 保护反馈更新声明及调用，驱动文本格式化入口 |
| `libApp/cmd/Cmd.cpp`、Wi-Fi 与以太网命令服务 | 网络活动通知，协议帧和回复语义保持原有约定 |
| `lib/wifi/webServer/WebServer.*` | ESP8266 使用 `SwsWebServer`，ESP32 继续使用内核提供的服务 |

相对于 `src/` 的路径见上表。本地保留 `tools/upstream-audit.cjs` 时，可用它生成差异清单；该工具不随固件仓库跟踪，实际合并按上表审查即可。

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
- 网络检查使用 `network-wifi-backend`、`network-load-guard`、`network-wifi-performance`、`network-wifi-ui`
- 页面检查使用 `coordinate-input`、`network-page`、`page-performance`、`shared-pages-ui`、`navigation-recovery`、`demo-entry` 和 `locales`
- 使用 Arduino 工具链编译 ESP8266 与 ESP32，构建目录和临时文件放在忽略的输出目录

资源长度使用编译期数组大小，避免每次发送先扫描 Flash；版本标签直接从 Flash 分段发送，避免为每页临时构建多个动态字符串。继续按至多 1024 字节分段输出，不缓存设备数据，也不改动运动指令和保护时序。

本地测试和固件编译不能替代上电、真实串口反馈、并发访问与抱闸接线的实机验证。
