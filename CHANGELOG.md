# 更新日志（CHANGELOG）

本项目遵循语义化版本号：`主版本.次版本.修订号`。
语言规范的破坏性变更会在主版本升级时说明；新增语言特性随次版本发布。

## [未发布]

### 新增
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

[未发布]: https://github.com/Huang-520-add/fcl/compare/v2.4.0...HEAD
[v2.4.0]: https://github.com/Huang-520-add/fcl/compare/v2.3.0...v2.4.0
[v2.3.0]: https://github.com/Huang-520-add/fcl/compare/v2.2.0...v2.3.0
[v2.2.0]: https://github.com/Huang-520-add/fcl/compare/v2.1.0...v2.2.0
[v2.1.0]: https://github.com/Huang-520-add/fcl/releases/tag/v2.1.0
