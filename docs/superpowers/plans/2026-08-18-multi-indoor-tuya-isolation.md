# Multi-Indoor Tuya Isolation Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** A 室内机被手机 Tuya 监控时，B 室内机保持可操作并可进入相同门口机视频，但 B 不允许开启通话。

**Architecture:** 将总线 busy 拆成“是否中断本地 UI”和“是否允许本地音频”两类纯策略。UI busy 事件永不强制其他室内机跳回待机；视频入口只受本机 Tuya 通话限制；通话按钮仍受本机 Tuya 状态和门口机 `talk_busy` 限制。

**Tech Stack:** C99、LVGL、Anyka 网络状态、独立断言测试、CMake。

---

### Task 1: 为跨室内机策略增加失败测试

**Files:**
- Modify: `src/layout/tests/test_tuya_session_audio_guard.c`
- Modify: `src/layout/tests/test_tuya_session_guard.c`
- Test: `src/layout/tuya_session_guard.c`

- [ ] **Step 1: Update video-entry assertions**

将 `outdoor_busy=true`、本机无 Tuya 通话的断言改为允许视频监控，并保留本机 Tuya 通话时禁止进入的断言。

- [ ] **Step 2: Add local-talk policy assertions**

为 `tuya_session_local_talk_allowed(local_client, tuya_monitor, outdoor_busy)` 增加以下断言：三个占用条件任一为真时返回 `false`，全部为假时返回 `true`。

- [ ] **Step 3: Add UI interruption assertions**

为 `tuya_session_remote_busy_should_interrupt_ui()` 增加返回 `false` 的断言，并把当前 target 允许列表的旧测试预期调整为生产策略。

- [ ] **Step 4: Run tests and verify RED**

Run:

```bash
gcc -std=c99 -I src/layout src/layout/tests/test_tuya_session_audio_guard.c src/layout/tuya_session_guard.c -o /tmp/test_tuya_session_audio_guard && /tmp/test_tuya_session_audio_guard
gcc -std=c99 -I src/layout src/layout/tests/test_tuya_session_guard.c src/layout/tuya_session_guard.c -o /tmp/test_tuya_session_guard && /tmp/test_tuya_session_guard
```

Expected: compilation or assertion failure because the new policy API/behavior is not implemented.

### Task 2: 实现最小策略修改

**Files:**
- Modify: `src/layout/tuya_session_guard.h`
- Modify: `src/layout/tuya_session_guard.c`

- [ ] **Step 1: Allow video while remote outdoor is busy**

`tuya_session_local_monitor_allowed()` 仅在 `tuya_talk_active=true` 时返回 `false`；保留现有参数以避免扩大调用点修改。

- [ ] **Step 2: Add local-talk guard**

实现：

```c
bool tuya_session_local_talk_allowed(bool local_video_client_active,
                                     bool background_tuya_monitor_active,
                                     bool outdoor_busy)
{
    return !local_video_client_active &&
           !background_tuya_monitor_active &&
           !outdoor_busy;
}
```

- [ ] **Step 3: Add remote-busy UI policy**

实现：

```c
bool tuya_session_remote_busy_should_interrupt_ui(void)
{
    return false;
}
```

- [ ] **Step 4: Run policy tests and verify GREEN**

运行 Task 1 的两个命令。

Expected: both tests exit 0.

### Task 3: 接入 UI 和通话按钮

**Files:**
- Modify: `src/main.c`
- Modify: `src/layout/layout_monitor.c`
- Preserve: `src/layout/layout_standby.c`

- [ ] **Step 1: Stop remote busy from switching layouts**

`main_device_monitor_busy_func()` 和 `device_monitor_busy_func()` 使用 `tuya_session_remote_busy_should_interrupt_ui()`；策略返回 false 时记录限频/关键日志并直接返回，不调用 `goto_layout()`。

- [ ] **Step 2: Guard the monitor talk button**

`monitor_talk_btn_up()` 读取本机 Tuya 客户端、Tuya 监控状态和当前门口机 `talk_busy`，调用 `tuya_session_local_talk_allowed()`。返回 false 时复用 `monitor_tuya_busy_msgbox_create()` 并退出，不创建音频任务。

- [ ] **Step 3: Preserve standby touch changes**

不覆盖当前工作树中 `src/layout/layout_standby.c` 已有的本机状态判断修改。

### Task 4: 验证工程

**Files:**
- Test: `src/layout/tests/test_tuya_session_audio_guard.c`
- Test: `src/layout/tests/test_tuya_session_guard.c`
- Build: `build/src/ANYKA37E.BIN`

- [ ] **Step 1: Run all Tuya session guard tests**

Run all `src/layout/tests/test_tuya_session_*_guard.c` programs against `src/layout/tuya_session_guard.c`.

Expected: all exit 0.

- [ ] **Step 2: Build firmware**

Run: `cmake --build build -j4`

Expected: exit 0 and `ANYKA37E.BIN` is generated.

- [ ] **Step 3: Review diff boundaries**

Run: `git diff -- src/main.c src/layout/layout_monitor.c src/layout/layout_standby.c src/layout/tuya_session_guard.c src/layout/tuya_session_guard.h src/layout/tests docs/superpowers`

Expected: no unrelated local changes are reverted or overwritten.
