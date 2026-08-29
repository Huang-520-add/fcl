#!/usr/bin/env bash
# FCL 输出断言测试（白盒：验证实际输出内容，不只是退出码）
# 与 run_tests.sh（黑盒）+ unit_tests（单元级）互补。用法: make test
# v3.0：🧬 标识仅 REAL MODE 显示（CODE 模式断言不再包含 🧬）
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

# assert_output <文件> <必须包含的片段>...（stdin 接 /dev/null，输出断言确定性）
assert_output() {
    local f="$1"; shift
    local out
    out=$("$BIN" "examples/$f" </dev/null 2>&1)
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

# assert_not_output <文件> <不得包含的片段>（负向断言）
assert_not_output() {
    local f="$1" want="$2"
    local out
    out=$("$BIN" "examples/$f" </dev/null 2>&1)
    if printf '%s' "$out" | grep -q -- "$want"; then
        fail=$((fail+1))
        echo "FAIL: $f — 输出不应包含: $want"
    else
        pass=$((pass+1))
    fi
}

# assert_exit <文件> <期望退出码>
assert_exit() {
    local f="$1" want="$2"
    "$BIN" "examples/$f" </dev/null >/dev/null 2>&1
    local ec=$?
    if [ "$ec" -eq "$want" ]; then pass=$((pass+1)); else
        fail=$((fail+1)); echo "FAIL: $f — 退出码 $ec（期望 $want）"
    fi
}

# assert_flags <旗标> <文件> <必须包含的片段>...（带 CLI 旗标运行，如 --real / --seed）
assert_flags() {
    local flags="$1" f="$2"; shift 2
    local out
    out=$($BIN $flags "examples/$f" </dev/null 2>&1)
    local ok=1
    for want in "$@"; do
        if ! printf '%s' "$out" | grep -q -- "$want"; then
            ok=0
            echo "FAIL: $flags $f — 输出缺少: $want"
        fi
    done
    if [ "$ok" -eq 1 ]; then pass=$((pass+1)); else fail=$((fail+1)); fi
}

# assert_stdin <输入内容> <文件> <必须包含的片段>...（管道喂输入）
assert_stdin() {
    local input="$1" f="$2"; shift 2
    local out
    out=$(printf '%s\n' "$input" | "$BIN" "examples/$f" 2>&1)
    local ok=1
    for want in "$@"; do
        if ! printf '%s' "$out" | grep -q -- "$want"; then
            ok=0
            echo "FAIL: (echo $input |) $f — 输出缺少: $want"
        fi
    done
    if [ "$ok" -eq 1 ]; then pass=$((pass+1)); else fail=$((fail+1)); fi
}

# ---------- 基础示例（v3.0：CODE 模式不显示 🧬） ----------
assert_output "example3.fc" "U+0041" "A"
assert_not_output "example3.fc" "🧬"          # 🧬 标识仅 REAL MODE 显示
assert_output "plus1_visible.fc" "U+0032"
assert_output "math_factorial.fc" "U+0078"
assert_output "math_triangular.fc" "U+0037"
assert_output "math_triangular_num.fc" "55"

# ---------- 数值边界 ----------
assert_output "edge_herb_255.fc" "255"              # HERBIVORE 上限 255 不溢出
assert_output "edge_herb_overflow.fc" "🤢" "0"      # 256 触发胃溃疡归零
assert_output "edge_producer_max.fc" "9999"         # PRODUCER 上限 9999.9
assert_exit   "err_producer_range.fc" 1             # PRODUCER 10000 越界报错
assert_output "edge_apex_normalize.fc" "1" "0"      # APEX 布尔归一（5→1, -3→0）

# ---------- 运算边界 ----------
assert_exit   "err_quot_divzero.fc" 1               # v3.0：QUOT 除零改报 FCL-0007（与表达式除零一致）
assert_flags  "" "err_quot_divzero.fc" "FCL-0007"
assert_output "edge_diff_zero.fc" "0"               # DIFF 恰好归零不报饿死
assert_output "edge_expr_arith.fc" "30"             # 表达式 (10+5)*2

# ---------- 命名与族群 ----------
assert_output "edge_multi_digit.fc" "109"           # 多位数编号（Grass_10/Fungus_100）
assert_exit   "err_genealogy_pack.fc" 1             # 群居物种缺 M/F 标签
assert_exit   "err_genealogy_alpha.fc" 1            # 独居物种伪造 Alpha 首领
assert_exit   "err_invasive.fc" 1                   # 外来物种
assert_exit   "err_taxonomy.fc" 1                   # 营养级错配
assert_exit   "err_dup_introduce.fc" 1              # v3.0：BIOME 段重复引种报 FCL-0005
assert_flags  "" "err_dup_introduce.fc" "FCL-0005" "第3行"

# ---------- 表达式错误 ----------
assert_exit   "err_expr_paren.fc" 1                 # 未闭合括号（v3.0：factor 精确报错）
assert_exit   "err_div0.fc" 1                       # 除零

# ---------- 控制流 ----------
assert_output "edge_nested_migration.fc" "1"        # 嵌套 MIGRATION（双层 sqrt 衰减）
assert_output "edge_assess_equal.fc" "FULL" "1"     # ASSESS 相等判 FULL（>=）
assert_output "edge_clone_chain.fc" "42"            # CLONE 跨营养级链式复制
assert_output "edge_season_wet.fc" "7"              # 湿度≥2 走 RAIN 分支
assert_output "edge_mutation_rename.fc" "44"        # MUTATION 物种级改名 + MATCH 不变量（触发与否输出恒 44）
assert_flags  "--seed 1" "edge_mutation_rename.fc" "44"   # 固定种子：确定性复现（seed=1 不触发变异）
assert_flags  "--seed 2" "edge_mutation_rename.fc" "44"   # seed=2 触发变异，不变量仍为 44
assert_flags  "--seed 2" "edge_mutation_rename.fc" "🧬 变异"

# ---------- v3.0 输入原语（SCENT/LURK/POUNCE 组合） ----------
assert_stdin "65" "composed_input.fc" "嗅探" "猛扑命中" "65"   # 管道输入被组合捕获
assert_output "composed_input.fc" "嗅探" "腐坏"               # /dev/null stdin（EOF）：扑空不崩溃

# 延迟到达的输入：先嗅不到气味 → LURK 潜伏等待 → 猛扑命中（组合的完整时序）
out_slow=$( (sleep 0.3; printf '65\n') | "$BIN" "examples/composed_input.fc" 2>&1 )
if printf '%s' "$out_slow" | grep -q "潜伏" && printf '%s' "$out_slow" | grep -q "猛扑命中"; then
    pass=$((pass+1))
else
    fail=$((fail+1)); echo "FAIL: composed_input.fc — 延迟输入场景未走 LURK+POUNCE 组合"
fi

# ---------- v3.0 🧬 标识（仅 REAL MODE） ----------
assert_output "gmo_real_marker.fc" "60"                     # CODE 模式：纯数值输出
assert_not_output "gmo_real_marker.fc" "🧬"                 # CODE 模式无 🧬
assert_flags "--real" "gmo_real_marker.fc" "🧬60"           # REAL 模式显示 🧬

echo "输出断言测试通过: $pass / 失败: $fail"
[ "$fail" -eq 0 ]
