# 更新日志（CHANGELOG）

本项目遵循语义化版本号：`主版本.次版本.修订号`。
语言规范的破坏性变更会在主版本升级时说明；新增语言特性随次版本发布。

## [v3.1.0] — 2026-08-31

**新增语言特性（图灵完备 + 架构升级，未引入破坏性变更）**

### 新增
- **图灵完备（核心）**：新增无界存储带与无界循环，FCL 现已可模拟通用图灵机 / Brainfuck：
  - 无界存储带：`TAPE` 为稀疏映射 `std::map<long long,int64_t>`，按需无限增长；带指针 `head_` 可无界左右移动（`FORWARD` / `BACKWARD`）。
  - 带格读写：`BUMP <expr>`（当前格 += 表达式，可为负）、`LOAD <var>`（格子→变量）、`STORE <var>`（变量→格子）；带格为 64 位整数，提供真正无界记忆。
  - 无界循环：`WHILE <物种|TAPE> UNTIL <expr> { ... }`——当物种变量（或 `TAPE` 当前格）不等于目标值时持续循环；运行期设安全上限（属实现保护，非语言限制）。
  - 完备性论证：无界存储 + 算术（DEVOURS / BUMP）+ 条件分支（ASSESS / SEASON / CASE）+ 无界循环 = Brainfuck / 图灵机等价 ⇒ **图灵完备**。示例 `examples/tc_double.fc` 用带 + 循环对任意 N 计算 2N（N=7 输出 14）。
- **独立词法分析器（分词器）**：新增 `src/lexer.h` / `src/lexer.cpp`，将解析器内联的空白切分抽出为类型化 `Token` 流（`KW/IDENT/NUM/STR/...`）；解析器改为词法器驱动，`Stmt` 节点携带 `toks` 词法信息（AST 结构化升级）。与历史 `splitWS` 输出逐字一致，解析 / 单测兼容。
- **AST 结构化**：`Stmt` 除 `args`（执行取参）外携带 `toks`（类型化 token），解析期即给出词法类型，便于未来工具链复用。

### 测试
- 单元测试新增词法器分类（关键字 / 标识符 / 数字 / 字符串）与端到端图灵完备检查（运行 `Interp` 捕获 stdout，断言 2N=14）：断言 140 → 155。
- 黑盒退出码用例 46 → 47（新增 `examples/tc_double.fc`）；白盒输出断言 40 → 41（新增 2N=14 断言）。

### 文档
- README / FCL_REFERENCE / FCL_SPEC / FCL_TUTORIAL（中英）及 ESOLANGS 投稿：将"未证明图灵完备"更正为"v3.1 起图灵完备"，补充完备性论证与存储带原语说明。
- 版本号全仓统一 v3.1.0（main.cpp / web_main.cpp / 文档 / README 徽章 / CI 工作流）。

## [v3.0.2] — 2026-08-31

**缺陷修复（代码评审驱动，未引入语言破坏性变更）**

### 修复
- **A1 DECAY 豁免递归化**：`inDecay`（分解者营养级/能量税豁免）现递归标记到 DECAY 内所有嵌套控制流语句；此前仅标顶层，导致 `DECAY { SEASON { DEVOURS ... } }` 类写法失效。
- **A2 GC 不再误回收活跃变量**：`HIBERNATION ... UNTIL <条件>` 的条件变量现在标记为"活跃"，避免循环期间被"生态分解"误删。
- **A3 物种命名严格校验**：群居物种成员（`Wolf_M1X` 等尾随垃圾）现被正确拒绝。
- **A4 错误行号为真实文件行号**：此前按块片段计数，BIOME 前有内容时错位；现按绝对行号报告。
- **A5 `--seed` 健壮性**：非数字种子不再导致进程 `std::terminate`，改为报错退出。
- **A6 移除 `Virus_Crash` 死代码**：该分支永远不可达，且原实现在库函数内调用 `std::exit(0)`（WASM 端会杀掉宿主页），已删除。
- **A7 `ROT` 越界保护**：编码模式负/超 255 值不再输出乱码（改输出 `?` / 负数码位钳到 0）。
- **A8 小写注释行提醒**：小写首字母且含 FCL 关键字的行（疑似大小写拼写错误）被当注释静默吞掉时给出警告。
- **A9 扑咬距离确定性**：APEX 的 `PROD`/`QUOT` 减半判定改用变量稳定 `id`（引入序号），不再随 GC/EXTINCTION 重排地址表而变。
- **B5 分解者约束**：分解者只能在 `DECAY` 段分解尸体，禁止在 `FOODWEB` 直接捕食活体。
- **C1 块边界字符串感知**：块结束位置计算与 `extractBlock` 一致，避免字符串内 `}` 误导边界。
- **C2 FOODWEB 捕食校验结构化**：改用语句树遍历替代易误判的子串搜索。
- **C3 `POUNCE` 坏输入处理**：读失败时丢弃坏数据，避免 `HIBERNATION` 循环反复读同一段坏输入而卡死。
- **C6 UTF-8 BOM 处理**：读取源文件时剥离 BOM，避免首行解析带乱码。

### 发版后勘误（同版本内修正）
- **A7 数值模式回退**：`ROT` 数值输出模式恢复 `static_cast<long long>(v.value)` 截断语义（与 v3.0.1 一致）。初版 v3.0.2 误用 `std::llround`，使 `PRODUCER` 上限 `9999.9` 被舍入成 `10000`，破坏 `edge_producer_max` 断言；编码模式的越界保护（`?` / 负数码位钳 0）不变。

### 文档
- 修正"图灵完备 / 无限存储"的过度宣称：FCL 内存有界、变量集静态声明、运行时不可动态分配，**未证明图灵完备**（README/FCL_REFERENCE/FCL_SPEC/FCL_TUTORIAL/ESOLANGS 投稿及英文版同步更新）。
- README 新增"已知限制"小节；修正过时测试数量描述。

## [v3.0.0] — 2026-08-29

**破坏性变更**：输入原语 SPROUT 移除，分解为三个可组合的原子原语。

### 新增
- **输入原语分解（SPROUT → SCENT / LURK / POUNCE）**：深奥语言不做高级封装——"获取键盘输入并等待"不再是单一原语，而是三个原子行为的组合：
  - `SCENT <嗅探者> TO <APEX> ;` 非阻塞嗅探 STDIN 就绪性（结果存入 APEX 物种：1=嗅到猎物 / 0=无气味）
  - `LURK <物种> FOR <节拍数> ;` 潜伏等待（REAL 100ms/拍，CODE 1ms/拍，上限 600 拍）
  - `POUNCE <捕食者> ;` 非阻塞猛扑读取数值；扑空/猎物腐坏（EOF/非法输入）能量保持不变
  - 组合范式：`SCENT → HIBERNATION { LURK; SCENT } → POUNCE`（示例 `examples/composed_input.fc`）
  - FOODWEB 校验相应放宽：DEVOURS / SCENT / POUNCE 任一即构成有效捕食链
- **`--seed <n>` 确定性开关**：固定生态随机种子（SEASON/STORM/MUTATION），CI 与回归测试可复现随机路径
- **单元测试**：新增 `tests/unit_tests.cpp`（140 断言，直接覆盖 expr / ecology / parser 模块）与 `make unit` / `make coverage` 目标；`.clang-format` 代码风格统一
- **更多错误路径**：括号未闭合 / 表达式意外结束 / 空表达式 / 残留 token 均抛 FCL-0011，不再静默返回 0

### 变更
- **CODE MODE 隐藏 🧬 转基因标识**：`GMO ENABLED` 标识仅 REAL MODE 显示——代码视角下变异静默生效（见 `examples/gmo_real_marker.fc`）
- 表达式求值支持运算符两侧空白（`60 + 5`）；`1/(3-3)` 等除零抛 FCL-0007
- 重复 `INTRODUCE` 同名物种抛 FCL-0003；全局开关（GMO/STORM）精确 token 匹配
- CASE 切分字符串感知（特征串内的 CASE 不再误切分）；块嵌套深度守卫；`ROT ... TO STDOUT` 目的地校验
- 修复 `renameSpeciesToken` Alpha_ 前缀改写偏移错误（`Alpha_Wolf` → `Alpha_Wolv` 此前产出 `Alpha_Wolvf`）

### 文档
- 全部 11 份文档（中英）+ README + Playground 徽章同步 v3.0：SPROUT 章节重写为三原语组合语义、FOODWEB 捕食行为表述对齐实现
- 版本号全仓统一 v3.0.0（main.cpp / web_main.cpp / 文档标题与页脚 / README 徽章 / index.html / CI 工作流）

## [v3.0.1] — 2026-08-30

### 新增
- fcl-skill/ AI 技能目录：将官方文档（SPEC/SYNTAX/REFERENCE/TUTORIAL/ECOLOGY）蒸馏成的 AI 技能包（SKILL.md + reference.md + examples.md）。目的：让 AI（如 ima.copilot）装上后能快速学会 FCL 语法，直接生成/解析 .fc 代码，无需逐篇翻文档。与 v3.0.1 严格对齐。

### 修复
- **嵌套 CASE 解析**：`splitCases` 的 CASE 查找升级为花括号深度感知——CASE 段内再嵌套 `MUTATION { CASE ... }` 时，内层 CASE 归属内层作用域，不再被外层切分撕碎（此前解析必然 FCL-0001 报错，EBNF 允许 `statement+` 包含 `mutation` 但解析器不支持）；字符串内的花括号与 CASE 依旧正确跳过
- **Windows SCENT 嗅探 EOF**：设备重定向（如 CI 的 `</dev/null` → NUL）与断开管道（EOF）此前嗅不到气味，`SCENT → HIBERNATION { LURK }` 组合在 EOF 场景下无限潜伏；现非控制台非管道句柄与 `ERROR_BROKEN_PIPE` 均判"可读"，POUNCE 统一走"猎物腐坏"路径，与 POSIX 的 select EOF 语义对齐（v3.0.0 起三平台 CI 的 Windows 断言失败即源于此）
- **SPEC_EN 尾部损坏**：删除重复的页脚块与夹在中间的乱码孤立行（`�️ RAIN downgraded...`），恢复与中文版一致的附录 C 结构

### 文档（v2.x 旧版语义残留清理，中英同步）
- **GMO/🧬 标识行为对齐 v3.0 实现**：v3.0 起 🧬 仅 REAL MODE 显示、CODE 模式静默免税——教程第 1/5 课的输出示例（`🧬A`/`🧬65`）、输出对照表、练习答案、SPEC 豁免条款、REFERENCE 6.7/6.13、README 快速开始与语言概述共 10 处修正为"CODE 模式无标识、--real 才显示"
- **版本号全库统一 v3.0.1**：10 份文档标题与页脚、README 徽章与结构树、`main.cpp`/`web_main.cpp`（`--version` 与 `fcl_version()`）、CI 工作流步骤名

### 变更
- **仓库瘦身**：移除误提交的 220KB Linux 二进制 `fcl-bin`（三平台产物已由 Releases 提供），`.gitignore` 增加 `/fcl-bin`、`/fcl.exe`、`*.wasm` 等防复发条目

### 新增
- **测试**：新增 `examples/edge_nested_case.fc` 嵌套变异回归用例（CASE 段内套 MUTATION），黑盒用例 45 → 46；输出断言新增 `--seed 8`（双层变异全触发，内层 DEVOURS 含能量税 20×0.2=4）与解析不再报错断言，断言总数 38 → 40

## [未发布]（已随 v3.0.1 发布）

### 修复
- **MUTATION 语义修复（规范-实现对齐）**：变异判定改按物种根名匹配——此前 `MUTATION Wolf_M1` 因变异表以根名为键而**永不触发**（官方示例 20 次运行 0 次变异）；块内引用改名升级为物种级改写（`Wolf_M1`/`Alpha_Wolf` → `Wolv_M1`/`Alpha_Wolv`），块执行期间在册成员临时改名、块结束后恢复原名（块外旧名始终有效）；`MATCH()` 改按物种根名判定（根名/成员名/变异后名称均可）
- **MUTATION CASE 选择性**：此前所有 CASE 分支无条件执行；现与规范对齐——变异触发时等概率随机表达一个分支，未触发则块空转（教程第 12 课"switch + 随机 case"）
- **缺参防御**：`MUTATION`/`HIBERNATION` 语句缺操作数时抛 FCL-0001（此前为 vector 越界未定义行为）
- **文档**：能量传递表述全文修正——实现为"仅 20% 能量流向捕食者"（×0.2），原文"扣 20% 能量传递税 / 20% loss"与代码矛盾（README/教程/图鉴/参考/规范/投稿素材，中英同步）；教程 12.2 变异名表与代码对齐（Algae→Algee 等 5 处）；教程 12.3 示例补上缺失的 `Sheep_M1` 引种

### 变更
- 变异表移入 ecology 模块作为单一事实来源（解释器与表达式求值共享）；`ExprEval::eval` 的 MATCH 上下文参数改为变异物种根名集合

### 新增
- **测试**：新增 `examples/edge_mutation_rename.fc` 回归用例（MATCH 不变量补偿设计：变异触发与否输出恒为 44）
- **测试**：新增 `tests/output_tests.sh` 输出断言测试（白盒），验证关键用例的实际输出内容，不再只检查退出码；`make test` 同时运行多套测试
- **文档**：新增 `docs/ESOLANGS_SUBMISSION.md`（esolangs.org 条目投稿素材，英文）
- **网页版**：Playground 支持代码分享链接（编码进 URL，打开即还原）
- **仓库**：新增本更新日志（CHANGELOG）

## [v2.4.0] — 2026-08-28

### 新增
- **REAL MODE / CODE MODE**：双运行时模式。REAL MODE 启用全部生态等待（进食阻塞、分解延时、胃溃疡惩罚）；CODE MODE（默认）跳过等待快速执行，WASM 网页版不再卡死
- **字符串感知的花括号解析**：`extractBlock` 忽略字符串内的 `{ }`，CASE 特征含花括号不再截断块

### 变更
- 所有 `sleep` 调用受运行模式与 `FCL_WASM` 宏双重保护

## [v2.3.0] — 2026-08-27

### 新增
- **NUMERIC OUTPUT**：ROT 直接输出数值（如 `55`），不再强制转 ASCII 字符，解决"算出 8 输出退格符"的可读性痛点
- **教科书级教程**：`FCL_TUTORIAL.md` 扩充至 1600+ 行，零基础友好，新增输出对照表

## [v2.2.0] — 2026-08-27

### 新增
- **模块化架构**：单文件 779 行拆分为 8 个文件（`ecology` / `expr` / `parser` / `interpreter` / `fcl_error`），引入 `fcl` 命名空间
- **错误码体系**：FCL-0001 ~ FCL-0011，错误信息携带行号
- **STORM ENABLED**：RAIN 块语句乱序改为显式开关（默认关闭），消除隐性随机行为
- **MUTATION 全物种覆盖**：变异表从 4 种扩展到全部 10 个在册物种
- **表达式残留检测**：解析后校验是否到达末尾，非法 token 不再被静默吞掉

### 修复
- macOS 编译：`Err`/`Error` 更名 `ErrCode`/`FclError`，`OVERFLOW` 枚举成员更名（与 macOS SDK 宏冲突）

## [v2.1.0] — 2026-08-27

### 新增
- **首个公开发布**：图灵完备的食物链深奥编程语言
- 三段式结构（BIOME → FOODWEB → DECAY）、10 物种 5 营养级、20% 能量传递税
- 控制流：SEASON / MIGRATION / HIBERNATION / MUTATION
- v2.0 逻辑套件：ASSESS / SYMBIOSIS / COMPETITION / MIMICRY
- 跨平台支持：Windows 非阻塞输入超时、去除 `bits/stdc++.h` 依赖
- GitHub Actions：三平台 CI 矩阵 + Release 自动构建（Windows 静态链接）

---

[未发布]: https://github.com/Huang-520-add/fcl/compare/v3.1.0...HEAD
[v3.1.0]: https://github.com/Huang-520-add/fcl/compare/v3.0.2...v3.1.0
[v3.0.2]: https://github.com/Huang-520-add/fcl/compare/v3.0.1...v3.0.2
[v3.0.1]: https://github.com/Huang-520-add/fcl/compare/v3.0.0...v3.0.1
[v3.0.0]: https://github.com/Huang-520-add/fcl/compare/v2.4.0...v3.0.0
[v2.4.0]: https://github.com/Huang-520-add/fcl/compare/v2.3.0...v2.4.0
[v2.3.0]: https://github.com/Huang-520-add/fcl/compare/v2.2.0...v2.3.0
[v2.2.0]: https://github.com/Huang-520-add/fcl/compare/v2.1.0...v2.2.0
[v2.1.0]: https://github.com/Huang-520-add/fcl/releases/tag/v2.1.0
