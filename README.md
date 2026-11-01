# SmartWebServer

面向 **OnStep / OnStepX** 的网页控制与网络通信固件，适用于 ESP8266 和 ESP32

本仓库基于 [Howard Dutton 的 SmartWebServer](https://github.com/hjd1964/SmartWebServer) 维护，提供中文界面、统一主题、网页配网、坐标输入和板载功能扩展网页随固件编译并由设备直接提供，使用浏览器即可访问

**快速导航：** [分支说明](#分支说明) · [主要功能](#主要功能) · [快速开始](#快速开始) · [网络设置](#网络设置) · [赤经轴抱闸](#赤经轴抱闸) · [本地演示](#本地演示) · [开发维护](#开发维护) · [仓库动态](#最近代码修改)

**文档入口：** [开发维护指南](docs/DEVELOPMENT.md) · [本地演示说明](demo/README.md) · [修改日志](docs/CHANGELOG.md)

## 分支说明

| 分支 | 用途 |
| --- | --- |
| [SmartWebServer-FeatureRelease](https://github.com/Leo-John233/SmartWebServer/tree/SmartWebServer-FeatureRelease) | 默认发布分支，推荐普通用户使用 |
| [SmartWebServer-Original](https://github.com/Leo-John233/SmartWebServer/tree/SmartWebServer-Original) | 上游 [hjd1964/SmartWebServer](https://github.com/hjd1964/SmartWebServer) 的 `main` 基线，自动同步并保留完整 Git 历史 |

### 其他分支
![其他分支预览](https://leo-john233.github.io/SmartWebServer/branch-preview.svg)

## 主要功能

| 功能 | 说明 |
| --- | --- |
| 网页控制 | 提供控制器、赤道仪、网络、调焦器、旋转器、辅助设备和编码器页面，按主控能力与编译配置显示 |
| 赤道仪操作 | 支持跟踪、方向移动、导星、GOTO、坐标同步、回零、停车、校准、限位、星表和 PEC 等原有功能 |
| 坐标与观测设置 | 提供 JNOW 赤经赤纬输入，以及时间同步、经纬度、时区、位置和限位设置 |
| 网页配网 | 支持搜索 Wi-Fi、连接隐藏网络、DHCP 或静态地址设置，并显示连接进度和结果 |
| 桌面与手机界面 | 采用灰黑与暗红主题，桌面使用侧栏，手机使用底部导航和更多菜单，支持浏览器前进后退 |
| 中文与多语言 | 通过编译配置选择语言，当前默认中文 |
| 状态显示 | 支持 Wi-Fi 信号和内部温度显示；环境数据、编码器及其他可选功能需相应配置和硬件支持 |
| 板载功能 | 提供网络活动指示灯和赤经轴抱闸软件逻辑，需对应引脚、驱动电路与反馈支持 |
| 协议兼容 | 延续原有 OnStep 命令通道及第三方软件连接方式 |

## 快速开始

### 1. 获取源码

```bash
git clone --branch SmartWebServer-FeatureRelease https://github.com/Leo-John233/SmartWebServer.git
cd SmartWebServer
```

也可以在 GitHub 切换到所需分支，通过 **Code → Download ZIP** 下载源码解压后，将包含 `SmartWebServer.ino` 的工程目录命名为 `SmartWebServer`

### 2. 准备开发环境

安装 Arduino IDE，并根据实际硬件安装对应的 ESP8266 或 ESP32 开发板支持包随后打开仓库根目录的 [SmartWebServer.ino](SmartWebServer.ino)，选择正确的开发板和串口

编译环境与板型应和目标设备匹配引脚映射位于 [src/pinmaps/](src/pinmaps/)，请同时核对与 OnStep / OnStepX 主控之间的接线

### 3. 检查配置

主要配置文件为 [Config.h](Config.h) 和 [Extended.config.h](Extended.config.h)下表列出当前源码中的部分默认值：

| 配置项 | 当前默认值 | 用途 |
| --- | --- | --- |
| `OPERATIONAL_MODE` | `WIFI` | 网络工作模式 |
| `SERIAL_BAUD_DEFAULT` | `9600` | 与主控开始通信时使用的串口速率 |
| `SERIAL_BAUD` | `115200` | 目标工作串口速率，需与主控对应设置匹配 |
| `SERIAL_SWAP` | `AUTO` | 按平台配置选择串口引脚 |
| `DISPLAY_LANGUAGE` | `L_cn` | 网页显示语言，修改后需重新编译 |
| `DISPLAY_WIFI_SIGNAL_STRENGTH` | `ON` | 显示 Wi-Fi 信号 |
| `DISPLAY_INTERNAL_TEMPERATURE` | `ON` | 显示设备内部温度 |
| `DISPLAY_WEATHER` | `OFF` | 环境数据展示开关 |
| `AXIS1_ENCODER` / `AXIS2_ENCODER` | `OFF` / `OFF` | 两轴编码器配置 |
| `RA_BRAKE_PROTECTION` | `ON` | 赤经轴抱闸保护，启用条件见下文 |

**当前源码默认启用赤经轴抱闸保护烧录前请阅读 [赤经轴抱闸](#赤经轴抱闸)，根据实际硬件、引脚占用及主控反馈设置此项**

`Config.h` 还提供 W5100 / W5500 以太网模式选项，使用时需要对应硬件和平台配置

### 4. 编译与烧录

在 Arduino IDE 中执行验证和上传

使用 Arduino CLI 时，可参考以下编译命令请先安装对应开发板支持包，并按实际开发板选择目标

```bash
# ESP8266 示例目标：NodeMCU 1.0
arduino-cli compile --clean --jobs 1 --warnings all --fqbn esp8266:esp8266:nodemcuv2 .

# ESP32 示例目标：ESP32 Dev Module
arduino-cli compile --clean --jobs 1 --warnings all --fqbn esp32:esp32:esp32 .
```

升级已有设备前，请记录网络及其他重要设置设备保存的配置可能覆盖源码中的默认值，升级后应检查设置是否保留；存储格式发生变化时可能需要重新设置

### 5. 连接设备

首次使用且尚未保存自定义网络设置时，当前编译默认值如下：

| 项目 | 默认值 |
| --- | --- |
| 设备热点名称 | `ONSTEPX` |
| 热点连接密码 | `password` |
| 热点访问地址 | <http://192.168.0.1/> |
| 网络页面管理密码 | `password` |
| 路由器连接 | 默认关闭，可在网络页面开启并配置 |

连接设备热点后，在浏览器中打开设备地址进入控制器页面，核对与 OnStep / OnStepX 主控的通信状态，再使用相应控制功能

热点连接密码与网络页面管理密码是两个独立配置项实际名称、密码和地址以设备保存的设置为准，部署使用前请修改默认密码

## 网络设置

设备热点称为 **AP**，设备连接路由器的模式称为 **STA**两者可以同时工作，连接路由器时会保留热点作为恢复入口

1. 连接设备热点，打开网络页面并输入管理密码
2. 查看当前连接状态，需要更新网络列表时点击刷新
3. 选择目标网络，输入密码；隐藏网络可通过“其他网络”手动添加
4. 按需要选择自动获取 IP，或填写静态 IP、网关和子网掩码
5. 点击连接，等待页面显示实际连接结果
6. 连接成功后，可使用设备在路由器网络中的 IP 访问网页

配网时请留意以下行为：

- 新网络的参数先用于连接验证，成功后才替换已保存的凭据；连接失败会保留此前的有效凭据
- 取消连接窗口不会提交其中尚未确认的修改
- 已保存并开启的网络会在上电后尝试连接后台连接无需保持网页打开；明确超时后停止尝试，等待手动重试或重启
- 设备热点与路由器应使用不同网段，避免地址冲突
- 连接路由器后，热点信道可能随之改变，手机短暂断开时可重新连接热点
- 修改热点名称或地址后，需要使用新名称或新地址重新访问

启用 STA 时固件会关闭 Wi-Fi 省电模式。冷启动时先关闭 SDK 持久化，再执行无线断开和热点状态重置，避免该重置路径产生不必要的配置 Flash 写入；这项设置不证明热点掉线或页面延迟的根因。

## 赤经轴抱闸

抱闸逻辑运行在 SmartWebServer 中，通过现有命令通道读取主控反馈并控制输出关闭网页后，后台采样和自动逻辑仍会继续运行

### 硬件与配置

当前 ESP8266 配置使用 **IO5** 控制适配的抱闸驱动电路，电平约定为 **LOW 抱闸，HIGH 松闸**启用前需核对引脚映射、驱动电路、电平约定及主控是否提供符合要求的 `:GXU1#` 反馈引脚初始化前的电平由硬件决定，应保留适配电路所需的外部下拉

| `RA_BRAKE_PROTECTION` | 行为 |
| --- | --- |
| `ON` | 启动后先保持抱闸，依据有效反馈及自动保护状态进入后续流程 |
| `OFF` | 隐藏保护开关，停止保护采样与自动停止逻辑；从引脚初始化起输出 HIGH |

### 操作与反馈

- 发生故障、失联或反馈过期等情况时，软件先尝试停止运动并关闭跟踪，再等待停止流程后的有效静止反馈，满足条件后才抱闸
- 停止未完成或反馈无效时，保持当前输出电平；不能将失联理解为立即完成抱闸
- 正常 `:GXU1#` 反馈采样间隔为 1500 ms，BoardIo 采样任务使用 priority 4；命令层可在 1500 ms 内向保护任务提供一次成功回复及其原始采样时间
- 停止命令成功后切换为 250 ms 采样，等待停止之后的两条独立 `ST` 静止反馈；此阶段反馈超过 1000 ms 即过期，正常反馈超过 2000 ms 即过期
- 页面发送或其他高优先级任务若持续运行，使 priority 4 采样任务超过 2 秒得不到执行，可能触发反馈过期保护；没有收到新的两条有效静止反馈时，输出会保持 HIGH 松闸，不会因超时直接切到 LOW
- 本次运行的故障锁定不会随反馈恢复而自动解除
- “手动解锁”会立即松闸并暂停自动逻辑，不检查当前驱动状态，也不会恢复此前的跟踪或移动重新开启自动锁定后才恢复自动流程
- 网页、TCP 和串口命令仍按原通道处理，模块不按抱闸状态拦截运动命令

`ST` 是驱动返回的状态标志，无法直接证明机械轴已经停止或实际保持电流已经建立。该任务由现有协作式调度器运行，不提供硬实时保证；编译和软件回归不能替代接线、上电时序、持续高负载访问与真实机械动作验证。

实现与接入说明见 [扩展维护说明](src/extensions/README.md)，源码位于 [src/extensions/boardIo/](src/extensions/boardIo/)

## 本地演示

[demo/](demo/) 保存电脑上运行的模拟网页，用于查看界面和验证交互，使用模拟设备数据，不连接真实硬件

运行演示需要 Node.js、PATH 中的 g++，以及本地保留的演示与测试脚本

仓库仅跟踪演示说明，服务脚本与测试工具不随源码完整提供，仅检出仓库时不能直接启动演示

在已具备本地工具的环境中，从仓库根目录运行

```powershell
node demo/server.cjs
```

浏览器打开 <http://127.0.0.1:8765/>，停止服务时按 Ctrl+C

文件用途、可模拟功能与检查方法见 [本地演示说明](demo/README.md)

## 项目结构

根目录保留项目入口，`docs/` 集中保存文档，`demo/` 保留演示程序，生成文件统一放在 `tests/output/`

| 路径 | 内容 |
| --- | --- |
| [SmartWebServer.ino](SmartWebServer.ino) | Arduino 工程入口与初始化 |
| [Config.h](Config.h) | 主要编译配置 |
| [Extended.config.h](Extended.config.h) | 网络、管理密码和其他扩展配置 |
| [docs/](docs/) | 文档索引、开发维护指南与修改日志 |
| [docs/DEVELOPMENT.md](docs/DEVELOPMENT.md) | 工程配置、编译、维护与本地验证 |
| [docs/CHANGELOG.md](docs/CHANGELOG.md) | 按日期整理的修改、回归和构建记录 |
| [demo/](demo/) | 本地模拟网页与演示说明，演示脚本不加入版本控制 |
| [src/pages/](src/pages/) | 各功能页面、表单与状态接口 |
| [src/extensions/](src/extensions/) | 主题、配网、板载功能、状态解析及坐标输入扩展 |
| [src/locales/](src/locales/) | 多语言资源 |
| [src/pinmaps/](src/pinmaps/) | 平台引脚映射 |
| [src/lib/](src/lib/) | 基础库与硬件支持 |
| [src/libApp/](src/libApp/) | 应用层状态与命令处理 |
| [.github/scripts/update_activity.py](https://github.com/Leo-John233/SmartWebServer/blob/SmartWebServer-FeatureRelease/.github/scripts/update_activity.py) | 发布分支中的仓库动态与统计页面生成脚本 |
| [.github/workflows/update-activity.yml](https://github.com/Leo-John233/SmartWebServer/blob/SmartWebServer-FeatureRelease/.github/workflows/update-activity.yml) | 发布分支中的统计生成及 GitHub Pages 部署工作流 |

## 开发维护

网页使用原生页面、表单与状态接口，样式、脚本和正文在同一次 HTML 响应中输出界面资源随固件更新，修改后需要重新编译并刷入设备

独立扩展集中在 `src/extensions/`部分配置、翻译、存储与页面接入修改仍位于原模块，合并上游更新时需要逐项核对

参与开发或提交问题时，建议提供开发板型号、主控及 SWS 版本、所用分支和提交编号，并描述复现步骤与实际现象问题可提交到 [Issues](https://github.com/Leo-John233/SmartWebServer/issues)，代码修改可通过 Pull Request 讨论

工程配置、编译与验证方法见 [开发维护指南](docs/DEVELOPMENT.md)，扩展接入和上游合并步骤见 [扩展维护说明](src/extensions/README.md)

历史修改与对应验证结果见 [修改日志](docs/CHANGELOG.md)，逐次提交可查阅 [Git 历史](https://github.com/Leo-John233/SmartWebServer/commits/SmartWebServer-FeatureRelease)

本地测试、对照与演示脚本未随仓库完整提供，维护记录中涉及这些工具的命令，需要具备相应本地文件后才能执行

## 验证范围与已知限制

- 维护记录包含 ESP8266、ESP32 编译，以及页面、配网、语言资源和抱闸逻辑回归这些结果对应记录时的源码快照，后续修改需重新编译与验证
- 2026-11-02 本地序列回归完成 71 项：64 项断言通过、7 项诊断报告生成、0 项失败；源码哈希检查确认测试期间未改动固件源码
- 同轮 ESP8266 nodemcuv2 与 ESP32 `esp32:esp32:esp32` clean build 均通过，均为 jobs 1、warnings all；ESP8266 程序 633800 字节、静态 RAM 50620 字节、可用 31300 字节，ESP32 程序 1184081 字节、静态 RAM 53012 字节、可用 274668 字节。两个构建各有 459 个源码、头文件和配置文件与构建快照一致
- 两份最终构建日志保留一个 `StateMount.cpp` signed/unsigned 比较警告，以及一个 ESP32 SDK `esp32-hal-uart.c` 非 void 函数返回值警告

同轮使用实际 Xtensa 对象和 AF1 编译参数核对了五个页面入口的函数入口帧：

| 页面入口 | AF1（字节） | 当前构建（字节） | 差值 |
| --- | ---: | ---: | ---: |
| Root | 96 | 96 | 0 |
| Mount | 80 | 80 | 0 |
| Focuser | 80 | 80 | 0 |
| Rotator | 96 | 96 | 0 |
| Aux | 272 | 272 | 0 |

这五个入口在 AF1 中均无 240 字节局部临时数组；本轮不据此宣称减少了 240 字节。上述数据只表示函数入口的栈指针调整，不表示调用链峰值或运行时总栈需求。两平台十种语言各检查 46 个页面/扩展翻译单元均通过；ESP32 可选 PCH 只用于 SDK `Arduino.h`，全量矩阵覆盖保持不变，报告记录 20 个有效 `.gch` 命中。双平台 12 个归档文件的 SHA-256 与原构建文件一致。本轮 953 个临时目录清理完成，报告、日志、基线、源码快照和构建证据保留。
- 真实 Wi-Fi 扫描、路由器掉线恢复、热点信道切换、串口响应和抱闸动作，需要在目标设备上验证
- ESP-07S 热点掉线与实际切页延迟仍需实机复核，现有软件优化记录不能据此认定问题已完全解决
- 原生 SDK 默认响应已带 `Connection: close`，固件处理器也显式发送该头以保持策略一致；ESP8266 `handleClient` 仍可能在处理排队的空闲连接时等待，显式响应头不能消除该等待，也不能证明它是切页问题的根因
- `/favicon.ico` 返回空的 HTTP 204 并在 404 回退前处理；原有完整 404 页面保持不变
- 本地 ESP8266 SDK 空闲连接排队探针记录约 5040 ms 和 15120 ms 的等待窗口。这是原生 SDK 的报告数据，不是页面修复指标，也不代表设备实测切页耗时
- 本地保留测试脚本时，可用 `node tests/sta-brake-suite.cjs --list` 查看入口分类，使用 `node tests/sta-brake-suite.cjs` 按顺序运行本轮回归；日志、清单和报告写入 `tests/output/sta-brake-20261102/`
- 新增坐标输入使用 JNOW 赤经赤纬，目前未提供 J2000 转换或方位角、高度角转换
- 显示语言在编译时选择；环境数据显示、编码器及部分附件功能取决于配置和主控能力

## 项目来源与许可

上游项目：[hjd1964/SmartWebServer](https://github.com/hjd1964/SmartWebServer)，原作者 Howard Dutton

本项目采用 GNU General Public License v3.0，具体条款见 [LICENSE](LICENSE)感谢上游作者及各位贡献者


<!-- repository-activity:start -->

<a name="最近代码修改"></a>

![最近代码修改](https://leo-john233.github.io/SmartWebServer/recent-changes.svg)

[查看发布分支完整提交记录](https://github.com/Leo-John233/SmartWebServer/commits/SmartWebServer-FeatureRelease)

![代码活动热力图](https://leo-john233.github.io/SmartWebServer/repository-activity.svg)

![仓库统计](https://leo-john233.github.io/SmartWebServer/repository-statistics.svg)

[查看分支统计与切换页面](https://leo-john233.github.io/SmartWebServer/)

<!-- repository-activity:end -->
