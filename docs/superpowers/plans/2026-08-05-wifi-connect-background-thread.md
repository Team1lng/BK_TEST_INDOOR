# Wi-Fi Connection Background Thread Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** Prevent the Wi-Fi connection dialog from blocking LVGL while waiting for `wpa_supplicant` and DHCP to become ready.

**Architecture:** Keep process startup and `wpa_cli` polling in a detached worker owned by `wlan.c`. Expose only a small connection-state API to `layout_setting_wifi.c`; the LVGL task polls that state and only updates the dialog, user data, and layout.

**Tech Stack:** C, Anyka thread API, LVGL task API, existing `wpa_supplicant`/`wpa_cli` flow.

---

### Task 1: Add background connection state API

**Files:**
- Modify: `src/api/wlan/wlan.h:37-60`
- Modify: `src/api/wlan/wlan.c:401-445`

- [ ] Add start, poll, and cancel declarations for the connection worker.
- [ ] Make `wifi_connection_status_sucess()` execute `wpa_cli` synchronously only inside the worker, without shell backgrounding or a UI-thread `select()` timeout.
- [ ] Add a generation token so cancelling one attempt cannot be confused with a later attempt.
- [ ] Let the worker wait for the control socket, poll connection state, persist the temporary config on success, and restore the saved configuration on timeout.

### Task 2: Make the LVGL task UI-only

**Files:**
- Modify: `src/layout/layout_setting_wifi.c:607-705`
- Modify: `src/layout/layout_add_wifi.c:153-498`
- Modify: `src/layout/layout_connect_wifi.c:113-306`

- [ ] Replace the direct `wifi_connection_status_sucess()` call with the new state getter.
- [ ] Remove process-kill, file-copy, `sync`, and process-restart commands from `msg_task()`.
- [ ] Keep layout transitions and `set_msg_text()` in the LVGL task.
- [ ] Cancel the worker when leaving/cancelling an active connection attempt.

### Task 3: Verify source integrity and build

**Files:**
- No additional source files.

- [ ] Run `git diff --check` for the two source files.
- [ ] Run the project compile command if the local toolchain is available.
- [ ] Confirm unrelated existing modifications remain untouched.
