# Vue Web UI 发布与回退

## 发布流程

1. 在 `webui` 目录运行 `npm ci`
2. 运行 `npm run build:embed` 生成压缩资源头文件
3. 编译 ESP-07S 固件并确认镜像适合 OTA 分区
4. 烧录后访问 `/`，并用 `/index.htm` 检查旧界面回退

生产资源完全包含在固件中，不依赖 CDN 或 LittleFS。HTML 使用 `no-cache`，带内容哈希的 CSS 和 JavaScript 使用长期不可变缓存

## 本地与设备调试

- `npm run dev` 启动 MSW 模拟设备
- `npm run dev:device` 通过 Vite 代理访问真实 ESP
- 设备地址从 `.env.local` 的 `VITE_DEVICE_URL` 读取，缺省为 `http://192.168.0.1`
- `npm run preview` 预览最近一次生产构建

代理保持浏览器请求同源，因此不需要修改设备 CORS 配置

## 兼容策略

- OnStep 4.24 保留两轴、高精度坐标和原有 focuser、rotator 行为
- OnStepX 保留 10.15、10.20、10.26、10.28 能力门槛
- Vue 仅使用固件返回的 `capabilities`，不解析控制器版本
- `/api/v1` 不可用时客户端自动回退旧文本状态和 GET 动作端点
- 未识别版本、无效版本和断连状态均锁定动作
- `/index.htm`、原有 `.htm` 和 Ajax 路由至少保留一个稳定版本周期

## 当前验收记录

协议回放覆盖 OnStep 4.24、OnStepX 10.15、10.20、10.26、10.28、无效版本和断连。单元测试覆盖新旧状态等价、能力驱动子系统请求和语义动作适配

ESP-07S 最终固件编译已通过。固件占用 Flash 588,028 / 1,044,464 B，静态 RAM 45,500 / 81,920 B，OTA 空间仍高于 10% 门槛

真实硬件上的连续运行、峰值堆、最大堆块、碎片率、WiFi 中断和 watchdog 指标仍需在发布设备上复核。在这些检查完成前，可通过 `/index.htm` 使用旧界面
