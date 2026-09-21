# 博科两线室内机（STECH）

安凯 AK37E 平台 · C + LVGL · 涂鸦 IPC SDK 6.2.20

## 编译

编译打包（在当前目录生成 `SAT_ANYKAOS_*` 升级包与 `五棵树-*.ZIP` 压缩包）

    ./make.sh -a

清除编译缓存

    ./make.sh -c

注：编译前请将工具链安装到 `/opt` 目录下，并添加到环境变量。

## 目录结构

```
├── build           // 编译缓存（make.sh -c 清除，不入版本库）
├── docs            // 文档
│   ├── hardware    // 硬件资料（原理图 PCB、规格书）
│   ├── superpowers // 开发方案与设计文档（plans / specs）
│   └── test-recent-changes-*.md
├── src             // 源码
│   ├── api         // 接口层：audio / video / network / tcp_network / gpio / queue / wlan / filelist ...
│   ├── include     // 头文件（api/ 下为可独立测试的策略头）
│   ├── layout      // LVGL 界面层（layout_*.c）与 xls 语言表解析
│   ├── lib         // 预编译库（freetype 等）
│   ├── tuya        // 涂鸦 SDK
│   ├── lvgl / lv_drivers / lv_qrcode
│   └── iniparser / onvif
├── tests           // 独立策略测试（对应 src/include/api 下的策略头）
├── upgrade         // 升级打包：app 分区内容 / dtb / platform / 打包脚本
├── CMakeLists.txt  // 编译配置（屏幕尺寸、PID、功能开关）
├── make.sh         // 编译脚本
├── README.md       // 工程说明
└── 修改日志.txt    // 版本变更记录
```

## 常用配置（CMakeLists.txt）

| 开关 | 默认值 | 说明 |
|---|---|---|
| `SET(IPS_SCREEN ...)` | `10` | 屏幕尺寸，10 / 8 / 7 |
| `SET(CMAKE_APP_OID ...)` | `OFF` | 定制 OID 版本开关 |
| `SET(CMAKE_FRENCH_ENABLE ...)` | `OFF` | 使能法语，默认语言为法语 |

屏幕尺寸决定编译宏与涂鸦 PID：

| 尺寸 | 宏 | PID |
|---|---|---|
| 10 寸 | `_10_IPS_SCREEN` | `0htjckdwyflg65oi` |
| 8 寸 | `_8_IPS_SCREEN` | `wlmukdvb76nx93ul` |
| 7 寸 | — | `ggdi7gxpghq6fbxf` |
| 定制 OID | — | `8hbui777kxqebtpk` |

## 语言表

**唯一真源：`src/layout/resource/language/language.xls`**

`make.sh` 在编译时会自动将 `resource/language/*` 拷贝到 `upgrade/app/`，
因此**只需修改这一处**，不要手工维护两份拷贝。

设备端运行时路径由宏 `TTF_XLS_PATH` 指定，默认为 `/app/app/language.xls`。

## 测试

`tests/` 与 `src/layout/tests/` 下的 `test_*.c` 是独立的断言型策略测试，
**不参与 CMake 构建**，需在主机上手动编译运行（`-I` 指向对应策略头文件所在目录）。
两个目录按被测层次划分，请勿合并：

- `tests/` — 对应 `src/include/api/` 下的策略（online status、stream keepalive、wifi scan、video perf、video mode guard）
- `src/layout/tests/` — 对应 layout 层守卫（tuya_session、cctv_stream、outdoor_talk_state、lang_xls）
