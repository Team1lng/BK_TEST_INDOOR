# CCTV Stream Selection Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** Add persistent main/sub RTSP stream selection for both CCTV cameras, defaulting newly configured cameras to the main stream.

**Architecture:** Add a small header-only URL policy that determines stream type from a saved URL and generates brand-specific RTSP URLs. The active camera settings page stores the selection through `camera_info.url`, while monitor opening uses the saved URL directly instead of rebuilding a brand-only URL. No persisted structure changes are required.

**Tech Stack:** C11, LVGL, existing user-data persistence, standalone assert-based C tests.

---

### Task 1: RTSP stream URL policy

**Files:**
- Create: `src/api/common/cctv_stream_policy.h`
- Create: `src/layout/tests/test_cctv_stream_policy.c`

- [ ] **Step 1: Write the failing test**

Test default stream detection and all four brand/stream URL combinations.

- [ ] **Step 2: Run test to verify it fails**

Run: `gcc -std=c11 -Wall -Werror src/layout/tests/test_cctv_stream_policy.c -o /tmp/test_cctv_stream_policy`
Expected: FAIL because `cctv_stream_policy.h` does not exist.

- [ ] **Step 3: Write minimal implementation**

Add `CCTV_STREAM_MAIN`, `CCTV_STREAM_SUB`, URL stream detection, and bounded URL generation helpers.

- [ ] **Step 4: Run test to verify it passes**

Run: `gcc -std=c11 -Wall -Werror src/layout/tests/test_cctv_stream_policy.c -o /tmp/test_cctv_stream_policy && /tmp/test_cctv_stream_policy`
Expected: PASS with exit code 0.

### Task 2: Active CCTV settings page

**Files:**
- Modify: `src/layout/layout_setting_camera.c`
- Modify: `src/layout/layout_define.h`
- Modify: `src/layout/layout_define.c`

- [ ] **Step 1: Add localized stream labels**

Add enum/string entries for stream type, main stream, and sub stream in all supported languages.

- [ ] **Step 2: Add stream selection row**

Insert a stream module below camera model, move IP/account/password rows down, and use the existing left/right selector behavior.

- [ ] **Step 3: Persist selection in URL**

Initialize display selection by parsing the camera's saved URL. On page exit generate and save the selected brand/stream URL, defaulting an empty/new configuration to main stream.

### Task 3: Monitor saved stream usage

**Files:**
- Modify: `src/api/common/leo_monitor.c`

- [ ] **Step 1: Stop rebuilding brand-only URLs**

For CCTV1 and CCTV2, open the saved `camera_info.url` when valid so the persisted stream selection is honored after reboot and channel switches.

- [ ] **Step 2: Keep safe fallback behavior**

If an older camera configuration has no URL, generate the default main-stream URL from its brand and credentials.

### Task 4: Verification

**Files:**
- Verify: all modified files

- [ ] **Step 1: Run policy test**

Run the standalone test and confirm all URL combinations pass.

- [ ] **Step 2: Build firmware**

Run: `cmake --build build -j4`
Expected: `[100%] Built target ANYKA37E.BIN`.

- [ ] **Step 3: Inspect focused diff**

Confirm no `camera_info`/`user_data_info` layout changes and no unrelated files were modified.
