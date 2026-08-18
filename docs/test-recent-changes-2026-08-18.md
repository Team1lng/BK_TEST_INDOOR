# `test` 分支近期修改说明

日期：2026-08-18  
仓库：`bk_test_indoor`  
目标分支：`test`

## 修改背景

本次修改主要针对室内机在 Tuya 手机监控、室内机本地监控、待机界面和 Wi‑Fi 异常场景下的稳定性问题，重点解决：

- 室内机退出本地监控后，手机 Tuya 画面停留在最后一帧；
- 室内机退出监控时误关闭 Tuya 共享音频；
- Tuya 后台视频会话导致待机背景变黑；
- Tuya 在线图标闪烁；
- Wi‑Fi 断网后进入网络设置页面卡死、触摸无响应；
- 视频接收和环形缓冲出现异常时缺少定位信息。

## 主要修改

### 1. Tuya 共享视频和音频链路

当 Tuya 云端仍有客户端连接时，室内机退出本地监控页面不再关闭共享的视频接收、视频解码和网络音频任务。这样手机端继续使用同一条门口机媒体链路，不会因为室内机页面退出而停在最后一帧或无故断音频。

相关代码：

- `src/layout/layout_monitor.c`
- `src/layout/tuya_session_guard.c`
- `src/layout/tuya_session_guard.h`
- `src/api/common/leo_monitor.c`
- `src/api/network/network_video_package.c`

### 2. Tuya 流状态保活

在 Tuya 客户端存在期间，网络层周期发送共享流状态 keepalive，降低页面切换或本地显示状态变化导致云端会话失活的风险。关键日志使用 `TUYA_STREAM_TRACE` 标识，便于硬件验证。

相关代码：

- `src/api/network/network_common.c`
- `src/api/network/network_common.h`
- `src/api/network/network_video_package.c`
- `src/include/api/tuya_stream_keepalive_policy.h`

硬件验证时应能持续看到类似：

```text
[TUYA_STREAM_TRACE] active keepalive: device=7 clients=1 order=0x8 arg1=0x9
```

### 3. 待机背景和待机触摸

Tuya 后台媒体会话打开视频路径时不再无条件清空 framebuffer，因此待机界面的背景图片不会被黑色覆盖。Tuya 通话期间，需要占用本地音频的监控、通话记录、消息记录和移动侦测入口继续禁止进入，但现在会给出系统忙提示；锁、门禁等不依赖本地音频的操作不受该限制。

相关代码：

- `src/api/common/leo_monitor.c`
- `src/include/api/monitor_video_mode_guard.h`
- `src/layout/layout_standby.c`
- `src/layout/tuya_session_guard.c`

### 4. Tuya 在线图标状态

在线图标统一读取 MQTT 状态回调缓存，避免一次 UI 刷新同时读取缓存状态和 SDK 实时状态造成 `0 -> 1` 抖动。网络离线/上线回调仍会触发状态区域刷新。

相关代码：

- `src/tuya/app/app_main/src/ty_sdk_status_callback.c`
- `src/layout/layout_define.c`
- `src/include/api/tuya_online_status_policy.h`

### 5. Wi‑Fi 页面异步扫描

Wi‑Fi 列表页面不再在 LVGL 触摸回调中同步等待 `wpa_cli_scan_wifi()`。扫描改由 detached 后台线程执行，并加入 single-flight 保护，避免断网、驱动忙或扫描命令异常时阻塞 UI 线程。首页触发的扫描线程也改为 detached，避免重复进入首页时积累线程。

相关代码：

- `src/api/wlan/wlan.c`
- `src/api/wlan/wlan.h`
- `src/layout/layout_setting_wifi.c`
- `src/layout/layout_home.c`
- `src/include/api/wifi_scan_policy.h`

注意：列表可能先显示上一次缓存结果，后台扫描完成后在下一次打开列表时显示最新结果；这是为了优先保证页面可操作，不在 UI 线程等待扫描。

### 6. 视频接收性能和异常诊断

视频解码队列、Tuya 环形缓冲和网络视频接收增加了限频性能统计、帧类型判断、队列覆盖/失败计数和关键路径日志，用于定位“有数据但画面停住”、环形缓冲写入慢或视频包组帧异常等问题。

相关代码：

- `src/api/network/network_video_package.c`
- `src/api/video/video_decode.c`
- `src/api/queue/ring_buffer.c`
- `src/api/queue/ring_buffer.h`
- `src/include/api/video_perf_trace.h`
- `src/tuya/tuya_video_frame_type.h`

### 7. 日志清理和资源同步

去除高频、低价值的 Wi‑Fi 扫描结果、SSID 列表、图标刷新和 `mqtt is not off-line` 等打印，保留错误信息、关键状态切换和媒体链路诊断日志。同步了本次构建所需的语言资源和提示音资源，并根据当前配置生成固件升级资源。

## 测试与构建

已覆盖的定向测试包括：

- Wi‑Fi 扫描策略；
- Tuya 在线状态策略；
- 待机背景保护；
- Tuya 流保活策略；
- Tuya 共享音频退出策略；
- Tuya 通话期间待机触摸策略；
- Tuya 视频帧类型和视频性能统计。

建议在目标环境执行：

```bash
cmake --build build -j4
```

硬件回归重点：

1. 手机 Tuya 监控、室内机本地监控同时打开；
2. 室内机退出本地监控，确认手机画面继续更新，不停在最后一帧；
3. 确认室内机退出页面后手机 Tuya 音频仍保持；
4. 待机界面背景不变黑，Tuya 在线图标不闪烁；
5. 断网后进入 Wi‑Fi 设置页面，确认页面仍可触摸；
6. Tuya 通话期间点击需要本地音频的待机快捷入口，确认显示系统忙提示而非静默无响应。

## 已知事项

- Wi‑Fi 列表采用后台扫描，首次打开可能显示缓存结果。
- `src/layout/tests/test_tuya_session_guard.c` 的旧测试预期与当前允许 `MEDIA/WIFI` 会话目标的实现不一致；同步前应单独确认是否修正该测试，不能将其误判为本次功能回归失败。
- `SAT_ANYKAOS_*`、`upgrade/app/log_seq_stat`、`.workbuddy/` 等属于构建产物或工作区临时内容，不应作为源码修改提交。
