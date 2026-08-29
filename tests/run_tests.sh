#!/usr/bin/env bash
# FCL 全量测试脚本
# 预期：正常用例退出码 0；err_* 与 control_season_dry 退出码 1
set -u
cd "$(dirname "$0")/.."

BIN=./fcl
[ ! -x "$BIN" ] && [ -f ./fcl.exe ] && BIN=./fcl.exe   # Windows 构建产物
if [ ! -x "$BIN" ] && [ ! -f "$BIN" ]; then
    echo "未找到解释器，请先执行: make build"
    exit 1
fi

pass=0
fail=0
for f in examples/*.fc; do
    if "$BIN" "$f" >/dev/null 2>&1; then ec=0; else ec=1; fi
    case "$f" in
        *err_*|*control_season_dry*) want=1 ;;
        *) want=0 ;;
    esac
    if [ "$ec" -eq "$want" ]; then
        pass=$((pass + 1))
    else
        fail=$((fail + 1))
        echo "FAIL: $f"
    fi
done

echo "测试通过: $pass / 失败: $fail"
[ "$fail" -eq 0 ]
