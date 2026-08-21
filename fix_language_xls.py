#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
修复 language.xls 中 camera 设置页文本错位。

问题：STR 枚举(layout_define.h 252-260)顺序为
  Camera model / Dahua / Hikvision / Channel switch / Main stream / Sub stream
  / Camera IP address / Account number / Password
而 xls 里 "Camera IP address" 被插到了 "Channel switch/Main stream/Sub stream" 之前，
导致 Camera model 下面显示的是 IP 地址、通道切换标签错位。

本脚本把 "Camera IP address" 这一行(原 row107) 移到 "Sub stream"(原 row110) 之后。
"""
import os
import sys
import shutil

try:
    import xlrd
    import xlwt
except ImportError:
    print("缺少 xlrd/xlwt，请先安装： pip install xlrd xlwt")
    sys.exit(1)

FILES = [
    r"Z:\BK\APP\Indoor\bk_test_indoor\src\layout\resource\language\language.xls",
    r"Z:\BK\APP\Indoor\bk_test_indoor\upgrade\app\language.xls",
]


def reorder(path):
    wb = xlrd.open_workbook(path)
    sh = wb.sheet_by_index(0)

    data = []
    for r in range(sh.nrows):
        row = []
        for c in range(sh.ncols):
            if sh.cell_type(r, c) == xlrd.XL_CELL_EMPTY:
                row.append(None)
            else:
                row.append(str(sh.cell_value(r, c)))
        data.append(row)

    # 幂等保护：若已正确，则跳过
    if data[107][0] == "Channel switch" and data[110][0] == "Camera IP address":
        print("already fixed, skip:", path)
        return

    # 当前 107..110 = [Camera IP address, Channel switch, Main stream, Sub stream]
    # 目标 107..110 = [Channel switch, Main stream, Sub stream, Camera IP address]
    block = data[107:111]
    data[107:111] = block[1:] + [block[0]]

    out = xlwt.Workbook(encoding="utf-8")
    ws = out.add_sheet("Sheet1")
    for r, row in enumerate(data):
        for c, v in enumerate(row):
            if v is not None:
                ws.write(r, c, v)

    bak = path + ".bak"
    if os.path.exists(path):
        shutil.copy2(path, bak)
    out.save(path)
    print("updated:", path, "(backup -> %s)" % bak)


def verify(path):
    sh = xlrd.open_workbook(path).sheet_by_index(0)
    print("== %s ==" % path)
    for r in range(104, 113):
        print("  row%3d  EN=%-24r ZH=%r" % (r, sh.cell_value(r, 0), sh.cell_value(r, 1)))


if __name__ == "__main__":
    for f in FILES:
        if os.path.exists(f):
            reorder(f)
        else:
            print("NOT FOUND:", f)
    print("\n=== 验证 ===")
    for f in FILES:
        if os.path.exists(f):
            verify(f)
