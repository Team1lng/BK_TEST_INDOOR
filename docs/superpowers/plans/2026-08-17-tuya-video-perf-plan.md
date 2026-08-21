# Tuya Video Performance Trace Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** Add low-frequency runtime metrics for Tuya ring writes, local decode buffering, and nonblocking decoder feeds.

**Architecture:** A portable accumulator owns interval counters. Existing network, queue, and decoder paths record samples without modifying control flow. The receiver task emits and resets the interval every five seconds.

**Tech Stack:** C99, Anyka SDK, existing `Debug_Lib` logging.

---

### Task 1: Add statistics accumulator and regression test
- [ ] Test operation latency, failure, zero-progress, and slow-count accumulation.
- [ ] Implement an independent C99 statistics module.

### Task 2: Instrument Tuya, queue, and decoder boundaries
- [ ] Record Tuya main/sub append durations and results.
- [ ] Track local decode-ring occupancy and overwrite counters.
- [ ] Record decoder feed results and zero-progress occurrences.

### Task 3: Emit five-second aggregate trace and validate
- [ ] Print a bounded `TUYA_PERF_TRACE` summary from the receiver task.
- [ ] Build unit test and full target.
