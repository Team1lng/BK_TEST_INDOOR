# 涂鸦后台通话保护 Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** 在涂鸦监控或对讲活动期间，保活音频会话并拒绝进入会抢占音频、视频或网络资源的功能。

**Architecture:** 将导航准入策略集中在 `goto_layout()`，使目标页面在当前页面 `quit()` 前被拒绝，避免触发监控页的音频关闭。监控页增加“后台 UI 退出”分支，仅释放页面控件与定时器，不关闭音频、网络收发或涂鸦回调；完整退出仍维持现有清理逻辑。首版仅允许主界面、待机和时间显示进入后台，避免已有监控 UI 回调访问已销毁控件。

**Tech Stack:** C、LVGL、CMake、现有 Anyka 音频/网络 SDK。

---

### Task 1: Add testable navigation policy

**Files:**
- Create: `src/layout/tuya_session_guard.h`
- Create: `src/layout/tuya_session_guard.c`
- Create: `src/layout/tests/test_tuya_session_guard.c`
- Modify: `src/CMakeLists.txt`

- [ ] **Step 1: Write the failing test**

```c
assert(tuya_session_target_allowed(TUYA_SESSION_TARGET_HOME));
assert(tuya_session_target_allowed(TUYA_SESSION_TARGET_STANDBY));
assert(!tuya_session_target_allowed(TUYA_SESSION_TARGET_MEDIA));
assert(!tuya_session_target_allowed(TUYA_SESSION_TARGET_WIFI));
assert(!tuya_session_target_allowed(TUYA_SESSION_TARGET_INTERPHONE));
```

- [ ] **Step 2: Run test to verify it fails**

Run: `gcc -Isrc/layout src/layout/tests/test_tuya_session_guard.c src/layout/tuya_session_guard.c -o /tmp/test_tuya_session_guard && /tmp/test_tuya_session_guard`

Expected: compilation fails because the guard module does not exist.

- [ ] **Step 3: Write minimal implementation**

Define `tuya_session_target` and `tuya_session_target_allowed()`. Return true only for home, standby and time display; return false for media, record/file lists, video/photo playback, Wi-Fi and pairing, upgrade, monitor/CCTV, interphone and transfer targets.

- [ ] **Step 4: Run test to verify it passes**

Run: `gcc -Isrc/layout src/layout/tests/test_tuya_session_guard.c src/layout/tuya_session_guard.c -o /tmp/test_tuya_session_guard && /tmp/test_tuya_session_guard`

Expected: exit code `0`.

### Task 2: Reject unsafe layout changes before teardown

**Files:**
- Modify: `src/layout/layout_define.c:2070`
- Modify: `src/layout/layout_define.h:507`
- Modify: `src/include/api/leo_api.h:162`

- [ ] **Step 1: Write the failing test**

Extend `test_tuya_session_guard.c` to verify each protected target resolves to a denied policy result, including media, Wi-Fi, upgrade, monitor, CCTV, interphone and transfer.

- [ ] **Step 2: Run test to verify it fails**

Run: `gcc -Isrc/layout src/layout/tests/test_tuya_session_guard.c src/layout/tuya_session_guard.c -o /tmp/test_tuya_session_guard && /tmp/test_tuya_session_guard`

Expected: assertion failure for an unclassified target.

- [ ] **Step 3: Write minimal implementation**

Map each `layout *` to a policy target in `layout_define.c`. When `monitor_enter_way_get() == MONITOR_ENTER_TUYA` and the current page is the monitor page, reject disallowed targets before `cur_layout->quit()`. Show a short existing message-box prompt and return `false`; do not clean LVGL objects or invoke the monitor exit callback. Mark allowed monitor-to-background transitions through the new monitor API before calling `quit()`.

- [ ] **Step 4: Run test to verify it passes**

Run: `gcc -Isrc/layout src/layout/tests/test_tuya_session_guard.c src/layout/tuya_session_guard.c -o /tmp/test_tuya_session_guard && /tmp/test_tuya_session_guard`

Expected: exit code `0`.

### Task 3: Preserve audio on allowed background navigation

**Files:**
- Modify: `src/layout/layout_monitor.c:2283`
- Modify: `src/include/api/leo_api.h:162`

- [ ] **Step 1: Write the failing test**

Add a policy test asserting all three background targets are allowed and an ordinary unclassified target is denied.

- [ ] **Step 2: Run test to verify it fails**

Run: `gcc -Isrc/layout src/layout/tests/test_tuya_session_guard.c src/layout/tuya_session_guard.c -o /tmp/test_tuya_session_guard && /tmp/test_tuya_session_guard`

Expected: assertion failure until the default-deny behavior is implemented.

- [ ] **Step 3: Write minimal implementation**

Add a one-shot `monitor_background_ui_exit_request()` flag. At the beginning of `LAYOUT_QUIT_FUNC(monitor)`, consume the flag and only release monitor-page tasks and pointers needed to avoid callbacks targeting removed LVGL objects. It must not call `monitor_video_mode_close()`, `audio_talk_close()`, `send_monitor_talk_cmd(false)`, reset `MONITOR_ENTER_TUYA`, or replace Tuya event callbacks. The existing full cleanup path remains unchanged for hangup and forced teardown.

- [ ] **Step 4: Run test to verify it passes**

Run: `gcc -Isrc/layout src/layout/tests/test_tuya_session_guard.c src/layout/tuya_session_guard.c -o /tmp/test_tuya_session_guard && /tmp/test_tuya_session_guard`

Expected: exit code `0`.

### Task 4: Guard direct destructive actions

**Files:**
- Modify: `src/layout/layout_setting_wifi.c:1201`
- Modify: `src/layout/layout_connect_wifi.c:135`
- Modify: `src/layout/layout_add_wifi.c:470`
- Modify: `src/layout/upgrade_firmware.c`
- Modify: `src/layout/doorbell_upgrade_windows.c`

- [ ] **Step 1: Write the failing test**

Add a `TUYA_SESSION_TARGET_WIFI` and `TUYA_SESSION_TARGET_UPGRADE` policy assertion that requires denial during an active session.

- [ ] **Step 2: Run test to verify it fails**

Run: `gcc -Isrc/layout src/layout/tests/test_tuya_session_guard.c src/layout/tuya_session_guard.c -o /tmp/test_tuya_session_guard && /tmp/test_tuya_session_guard`

Expected: assertion failure if either target is allowed.

- [ ] **Step 3: Write minimal implementation**

Before Wi-Fi connection/disconnection, pairing, firmware update or reboot begins, check the active Tuya session state. If active, show the common prompt and return without changing network or upgrade state.

- [ ] **Step 4: Run test to verify it passes**

Run: `gcc -Isrc/layout src/layout/tests/test_tuya_session_guard.c src/layout/tuya_session_guard.c -o /tmp/test_tuya_session_guard && /tmp/test_tuya_session_guard`

Expected: exit code `0`.

### Task 5: Compile and manually validate target behavior

**Files:**
- Modify: none

- [ ] **Step 1: Compile the changed target**

Run: `cmake -S . -B build && cmake --build build --target ANYKA37E.BIN -j2`

Expected: cross compiler is available and target builds without warnings or errors.

- [ ] **Step 2: Validate on device**

1. Start Tuya monitor and Tuya talk.
2. Navigate to Home, Standby and Time Display; confirm remote audio continues.
3. Try Media, Event playback, Wi-Fi, Tuya registration, upgrade, CCTV, Interphone and Transfer; confirm a busy prompt appears and the current audio remains active.
4. End the Tuya session remotely; confirm full cleanup occurs and all functions are usable again.
