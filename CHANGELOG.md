# 更新日志（CHANGELOG）

本项目遵循语义化版本号：`主版本.次版本.修订号`。
语言规范的破坏性变更会在主版本升级时说明；新增语言特性随次版本发布。

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

## [未发布]

### 修复
- **MUTATION 语义修复（规范-实现对齐）**：变异判定改按物种根名匹配——此前 `MUTATION Wolf_M1` 因变异表以根名为键而**永不触发**（官方示例 20 次运行 0 次变异）；块内引用改名升级为物种级改写（`Wolf_M1`/`Alpha_Wolf` → `Wolv_M1`/`Alpha_Wolv`），块执行期间在册成员临时改名、块结束后恢复原名（块外旧名始终有效）；`MATCH()` 改按物种根名判定（根名/成员名/变异后名称均可）
- **MUTATION CASE 选择性**：此前所有 CASE 分支无条件执行；现与规范对齐——变异触发时等概率随机表达一个分支，未触发则块空转（教程第 12 课"switch + 随机 case"）
- **缺参防御**：`MUTATION`/`HIBERNATION` 语句缺操作数时抛 FCL-0001（此前为 vector 越界未定义行为）
- **SPROUT WASM 守卫**：摩斯电码播放循环（10×180ms）加 `FCL_WASM` 宏保护，浏览器版不再阻塞约 1.8 秒
- **文档**：能量传递表述全文修正——实现为"仅 20% 能量流向捕食者"（×0.2），原文"扣 20% 能量传递税 / 20% loss"与代码矛盾（README/教程/图鉴/参考/规范/投稿素材，中英同步）；教程 12.2 变异名表与代码对齐（Algae→Algee 等 5 处）；教程 12.3 示例补上缺失的 `Sheep_M1` 引种

### 变更
- 变异表移入 ecology 模块作为单一事实来源（解释器与表达式求值共享）；`ExprEval::eval` 的 MATCH 上下文参数改为变异物种根名集合

### 新增
- **测试**：新增 `examples/edge_mutation_rename.fc` 回归用例（MATCH 不变量补偿设计：变异触发与否输出恒为 44），用例总数 42 → 43，输出断言 24 → 25
- **测试**：新增 16 个边界/错误用例（12 个 `edge_*.fc` + 4 个 `err_*.fc`），覆盖数值边界、命名族谱、嵌套控制流等此前未覆盖路径，用例总数 26 → 42
- **测试**：新增 `tests/output_tests.sh` 输出断言测试（白盒），验证关键用例的实际输出内容，不再只检查退出码；`make test` 同时运行两套测试
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

[未发布]: https://github.com/Huang-520-add/fcl/compare/v3.0.0...HEAD
[v3.0.0]: https://github.com/Huang-520-add/fcl/compare/v2.4.0...v3.0.0
[v2.4.0]: https://github.com/Huang-520-add/fcl/compare/v2.3.0...v2.4.0
[v2.3.0]: https://github.com/Huang-520-add/fcl/compare/v2.2.0...v2.3.0
[v2.2.0]: https://github.com/Huang-520-add/fcl/compare/v2.1.0...v2.2.0
[v2.1.0]: https://github.com/Huang-520-add/fcl/releases/tag/v2.1.0
