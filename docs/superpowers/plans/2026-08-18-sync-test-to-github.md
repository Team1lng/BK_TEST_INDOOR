# Sync `test` Recent Changes Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** 将 `bk_test_indoor` 中已确认的近期功能修改、测试和中文说明安全同步到 GitHub 的 `test` 分支。

**Architecture:** 先按功能审查工作树，排除构建临时包和工作区缓存；再将发布说明与相关源码/测试一起暂存。提交前运行定向测试和 CMake 构建，最后核验提交与远端分支一致。

**Tech Stack:** C/C++、CMake、Git、Anyka ARM 交叉编译产物、Tuya IPC/LVGL。

---

### Task 1: 审查同步范围

**Files:**
- Review: `git diff --name-status`
- Review: `src/api/network/`, `src/api/wlan/`, `src/layout/`, `src/tuya/`
- Exclude: `.workbuddy/`, `SAT_ANYKAOS_*`, `upgrade/app/log_seq_stat`

- [ ] **Step 1: Confirm the branch and remote**

Run: `git status --short --branch && git remote -v`

Expected: current branch is `test`, upstream is `origin/test`, and no remote is changed.

- [ ] **Step 2: Review the staged candidate set**

Run: `git diff --stat && git ls-files --others --exclude-standard`

Expected: only source, policy headers, tests, documentation, and intentionally tracked firmware/resource changes are candidates.

### Task 2: Add release documentation

**Files:**
- Create: `docs/test-recent-changes-2026-08-18.md`

- [ ] **Step 1: Document behavior, root causes, files, validation, and known limitations**

The document must cover Tuya shared video/audio continuity, stream keepalive, standby background and touch behavior, online icon stability, non-blocking Wi-Fi scanning, video performance tracing, log cleanup, build commands, hardware verification points, and files intentionally excluded from the commit.

### Task 3: Validate source and tests

**Files:**
- Test: `tests/test_*.c`
- Test: `src/layout/tests/test_*.c`
- Build: `build/src/ANYKA37E.BIN`

- [ ] **Step 1: Run all standalone policy tests**

Run the repository's available `gcc -std=c99` test commands for policy and layout guard tests.

Expected: all relevant tests exit with status 0; the pre-existing `test_tuya_session_guard.c` expectation mismatch is reported separately if still present.

- [ ] **Step 2: Build the firmware**

Run: `cmake --build build -j4`

Expected: exit status 0 and a freshly updated `build/src/ANYKA37E.BIN`.

### Task 4: Commit and synchronize

**Files:**
- Modify: only the reviewed candidate set

- [ ] **Step 1: Review the staged diff**

Run: `git diff --cached --check && git diff --cached --stat`

Expected: no whitespace errors and the summary matches the release document.

- [ ] **Step 2: Create one focused synchronization commit**

Run: `git commit -m "fix: stabilize tuya monitor and wifi flows"`

Expected: a new commit on `test`; no reset, clean, or unrelated deletion is performed.

- [ ] **Step 3: Push the test branch**

Run: `git push origin test`

Expected: `origin/test` advances to the new commit.

- [ ] **Step 4: Verify synchronization**

Run: `git status --short --branch && git rev-parse HEAD && git rev-parse origin/test`

Expected: local `HEAD` and `origin/test` match; any intentionally excluded files remain clearly listed as local-only changes.
