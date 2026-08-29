#!/usr/bin/env bash
# FCL 输出断言测试（白盒：验证实际输出内容，不只是退出码）
# 与 run_tests.sh（黑盒）互补。用法: make test 或 bash tests/output_tests.sh
set -u
cd "$(dirname "$0")/.."

BIN=./fcl
if [ ! -x "$BIN" ]; then
    echo "未找到解释器，请先执行: make build"
    exit 1
fi

pass=0
fail=0

# assert_output <文件> <必须包含的片段>...
assert_output() {
    local f="$1"; shift
    local out
    out=$("$BIN" "examples/$f" 2>&1)
    local ok=1
    for want in "$@"; do
        if ! printf '%s' "$out" | grep -q -- "$want"; then
            ok=0
            echo "FAIL: $f — 输出缺少: $want"
            echo "------ 实际输出 ------"
            printf '%s\n' "$out" | head -8
            echo "----------------------"
        fi
    done
    if [ "$ok" -eq 1 ]; then pass=$((pass+1)); else fail=$((fail+1)); fi
}

# assert_exit <文件> <期望退出码>
assert_exit() {
    local f="$1" want="$2"
    "$BIN" "examples/$f" >/dev/null 2>&1
    local ec=$?
    if [ "$ec" -eq "$want" ]; then pass=$((pass+1)); else
        fail=$((fail+1)); echo "FAIL: $f — 退出码 $ec（期望 $want）"
    fi
}

# ---------- 基础示例 ----------
assert_output "example3.fc" "U+0041" "🧬A"
assert_output "plus1_visible.fc" "U+0032"
assert_output "math_factorial.fc" "U+0078"
assert_output "math_triangular.fc" "U+0037"
assert_output "math_triangular_num.fc" "55"

# ---------- 数值边界 ----------
assert_output "edge_herb_255.fc" "🧬255"            # HERBIVORE 上限 255 不溢出
assert_output "edge_herb_overflow.fc" "🤢" "🧬0"     # 256 触发胃溃疡归零
assert_output "edge_producer_max.fc" "🧬9999"        # PRODUCER 上限 9999.9
assert_exit   "err_producer_range.fc" 1              # PRODUCER 10000 越界报错
assert_output "edge_apex_normalize.fc" "🧬1" "🧬0"   # APEX 布尔归一（5→1, -3→0）

# ---------- 运算边界 ----------
assert_output "edge_quot_divzero.fc" "🧬0"           # APEX QUOT 除以 0 返回 0 不崩溃
assert_output "edge_diff_zero.fc" "🧬0"              # DIFF 恰好归零不报饿死
assert_output "edge_expr_arith.fc" "🧬30"            # 表达式 (10+5)*2

# ---------- 命名与族群 ----------
assert_output "edge_multi_digit.fc" "🧬109"          # 多位数编号（Grass_10/Fungus_100）
assert_exit   "err_genealogy_pack.fc" 1              # 群居物种缺 M/F 标签
assert_exit   "err_genealogy_alpha.fc" 1             # 独居物种伪造 Alpha 首领
assert_exit   "err_invasive.fc" 1                    # 外来物种
assert_exit   "err_taxonomy.fc" 1                    # 营养级错配

# ---------- 表达式错误 ----------
assert_exit   "err_expr_paren.fc" 1                  # 未闭合括号
assert_exit   "err_div0.fc" 1                        # 除零

# ---------- 控制流 ----------
assert_output "edge_nested_migration.fc" "🧬1"       # 嵌套 MIGRATION（双层 sqrt 衰减）
assert_output "edge_assess_equal.fc" "FULL" "🧬1"    # ASSESS 相等判 FULL（>=）
assert_output "edge_clone_chain.fc" "🧬42"           # CLONE 跨营养级链式复制
assert_output "edge_season_wet.fc" "🧬7"             # 湿度≥2 走 RAIN 分支

echo "输出断言测试通过: $pass / 失败: $fail"
[ "$fail" -eq 0 ]
