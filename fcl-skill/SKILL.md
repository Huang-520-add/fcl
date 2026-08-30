---
name: fcl
description: >
  FCL（FoodChain Language，食物链语言）v3.0.1 官方文档蒸馏技能。当用户提到
  FCL、食物链语言、.fc 文件、BIOME / FOODWEB / DECAY 三段式、DEVOURS、ROT、
  INTRODUCE、SEASON、HIBERNATION、MUTATION、CLONE、ASSESS、SYMBIOSIS、
  COMPETITION、MIMICRY、SCENT、LURK、POUNCE、EXTINCTION、营养级、物种命名、
  能量税、生态崩溃、外来物种入侵 等关键词，或需要生成 / 解析 / 解释 / 调试 /
  翻译 .fc 代码时启用本技能。让 AI 快速学会该语言语法并正确生成或解析 FCL 代码。
---

# FCL 技能（FoodChain Language / 食物链语言）

## 这是什么
FCL 是一门生态学主题的 **esolang（深奥编程语言）**，图灵完备，C++17 解释器实现，MIT 许可，作者 by.荒。
- **变量 = 在册物种**，**运算 = 捕食**，**控制流 = 生态事件**。
- 程序强制三段式：`BIOME`(引种) → `FOODWEB`(捕食) → `DECAY`(分解)。
- 维度一维，范式命令式/结构化，**强类型（营养级绑定，5级，无隐式转换）**。
- 文件扩展名 `.fc`；在线 Playground: https://huang-520-add.github.io/fcl/ ；仓库 `Huang-520-add/fcl`。
- 版本：**v3.0.1**。

## 何时用
- 需要**生成、补全、修复、解释**一段 FCL（`.fc`）代码时。
- 用户问到 FCL 语法、营养级、物种命名、指令含义、错误码时。
- 需要把算法/逻辑**翻译成 FCL 三段式结构**时。
- 需要**解析**某段 `.fc` 输出（ASCII / 数值 / 生态事件 / 错误码）时。

> 严格遵守「一致性要求」：所有语法/指令/规则必须与本文档及 reference.md 一字不差地对齐，**不要发明任何不在材料中的语法或指令**。拿不准时查阅 reference.md 与 examples.md。

## 三段式结构（强制顺序，不可嵌套，缺一段 → 🌍生态崩溃终止 FCL-0006）
| 段 | 作用 | 允许内容 |
|----|------|----------|
| `BIOME { }` | 引种 | 仅变量声明与初始能量注入 `INTRODUCE` |
| `FOODWEB { }` | 捕食 | 核心运算，**必须至少一次 DEVOURS/SCENT/POUNCE**，否则🌍终止 |
| `DECAY { }` | 分解 | 输出 `ROT` 与内存回收 |

全局开关（`GMO`/`STORM`/`REAL`/`CODE`/`NUMERIC OUTPUT`）与可选 shebang 写在三段式之外、首行位置。

## 营养级与物种速查（5级，强类型，变量名绑定营养级）
| 级 | 名称 | 类型/范围 | 在册物种（群居/独居） |
|----|------|-----------|----------------------|
| 1 | PRODUCER 生产者 | 浮点 0.0~9999.9，支持分数(3/2→1.5) | Grass(草), Algae(藻) |
| 2 | HERBIVORE 食草 | 整型 0~255，超255归零+2s阻塞(🤢胃溃疡溢出,CODE跳过) | Sheep(羊,群居), Rabbit(兔,独居) |
| 3 | CARNIVORE 食肉 | 整型 0~65535，奇数行加速偶数行减速2ms(CODE跳过) | Wolf(狼,群居), Fox(狐,独居) |
| 4 | APEX 顶级 | 布尔 FULL=1/HUNGRY=0，逻辑依赖"扑咬距离"(地址偏移) | Tiger(虎,独居), Lion(狮,群居) |
| 5 | DECOMPOSER 分解者 | 字符串(数值按ASCII解释)，输出按ASCII渲染 | Fungus(真菌), Bacillus(杆菌) |

## 物种命名规范
- 变量名 = `物种_标识`。
- **群居**(狼/狮/羊)：首领 `Alpha_<物种>`（Alpha_Wolf/Alpha_Lion/Alpha_Sheep），雄性 `<物种>_M<编号>`（Wolf_M1），雌性 `<物种>_F<编号>`（Sheep_F2）。
- **独居**(虎/狐/兔)：`<物种>_<编号>`（Tiger_1/Fox_2/Rabbit_1）。
- **生产者/分解者**：`<物种>_<编号>`（Grass_1/Fungus_1）。
- 编号从 1 开始，可跳号。
- 物种名必须在册，否则 🌿外来物种入侵(FCL-0001)；营养级须与声明一致否则 ⚠️分类学混乱(FCL-0004)；群居须有 Alpha 或 M/F 标签否则 ⚠️族谱登记混乱(FCL-0005)。

## 指令速查表
| 指令 | 位置 | 语义要点 |
|------|------|----------|
| `INTRODUCE <物种> AS <营养级> WITH <表达式> ;` | 仅 BIOME | 引种声明；表达式支持数字/分数/四则/括号；APEX 初值归一 0/1 |
| `<p> DEVOURS <y> USING SUM\|DIFF\|PROD\|QUOT ;` | FOODWEB（DECAY 内 DECOMPOSER 豁免） | 捕食：SUM加/DIFF大减小(负→🥀)/PROD乘(仅APEX)/QUOT除(仅APEX,余丢)；级差恰1且捕食者>猎物；猎物归零；能量税20%(GMO免)；APEX的PROD/QUOT受扑咬距离影响(地址差偶数→结果减半🐾) |
| `CLONE <目标> FROM <源> ;` | FOODWEB | 目标获源副本，源不变；目标须已 INTRODUCE |
| `ASSESS <A> AGAINST <B> TO <C> ;` | FOODWEB | A≥B→C=FULL(1) 否则 HUNGRY(0)；C 必须 APEX |
| `SYMBIOSIS <A> WITH <B> TO <C> ;` | FOODWEB | C=A AND B；A/B/C 全 APEX |
| `COMPETITION <A> OR <B> TO <C> ;` | FOODWEB | C=A OR B；全 APEX |
| `MIMICRY <A> TO <B> ;` | FOODWEB | B=NOT A；A/B 全 APEX |
| `SCENT <嗅探者> TO <APEX变量> ;` | 仅 FOODWEB | 非阻塞嗅探 STDIN 就绪→APEX 存 1.0 否则 0.0；结果变量须 APEX(Tiger/Lion)；输出👃 |
| `LURK <物种> FOR <节拍数> ;` | 仅 FOODWEB | 等待 N 拍(REAL 100ms/拍,CODE 1ms/拍,钳制0-600)；物种须已注册；输出🕳️ |
| `POUNCE <捕食者> ;` | 仅 FOODWEB | 非阻塞猛扑：STDIN 就绪读入存物种(🦅)否则扑空能量保持(🐾)，EOF/非法→🦠腐坏；WASM 永远扑空 |
| `ROT <分解者> TO STDOUT ;` | 仅 DECAY | 仅 DECOMPOSER；首次 ASCII 字符(char(int(能量)))，第二次 U+XXXX；NUMERIC OUTPUT 模式直出数值 |
| `SEASON RAIN {…} DRY {…} ;` | FOODWEB | 湿度=最近3条 DEVOURS 次数≥2→RAIN 否则 DRY；STORM ENABLED 后 RAIN 块>3语句才随机乱序一次(🌩️)，默认不乱序 |
| `MIGRATION <物种> OVER <次数> {…} ;` | FOODWEB | 定次循环；每轮后该物种能量开平方根衰减 |
| `HIBERNATION <物种> UNTIL <APEX变量> {…} ;` | FOODWEB | while 循环直到 APEX=FULL；每轮×0.9；超10000轮强制唤醒(⏰) |
| `MUTATION <物种> { CASE "特征": <语句> … } ;` | FOODWEB | 1/3概率变异：根名替换变异名(如Wolf→Wolv)，块内引用随之改写，触发时随机表达一个 CASE，未触发空转；MATCH(标识符)可检测(变异→1未变→0) |
| `EXTINCTION <变量名> ;` | FOODWEB/DECAY | 立即回收并打印16行十六进制遗照；`EXTINCTION Virus_Crash ;` 强制终止进程 |

## 关键规则与陷阱（逐条对齐官方文档）
1. 三段式顺序强制 BIOME→FOODWEB→DECAY，**不可嵌套**；缺任意一段 → 🌍生态崩溃(FCL-0006)。
2. FOODWEB **必须至少一次 DEVOURS/SCENT/POUNCE**，否则 🌍终止(FCL-0006)。
3. **强类型**，营养级绑定，**无隐式转换**；变量名绑定营养级。
4. 物种名必须在册，否则 🌿外来物种入侵/未知语句(FCL-0001)。
5. 营养级须与声明一致，否则 ⚠️分类学混乱(FCL-0004)。
6. 群居须有 Alpha 或 M/F 标签，否则 ⚠️族谱登记混乱(FCL-0005)。
7. DEVOURS 营养级必须**恰好差1且捕食者>猎物**；DECAY 内 DECOMPOSER **豁免**可吃任意级 100%；猎物归零。
8. DEVOURS **能量税20%**（实际=理论×0.2）；`GMO ENABLED` 免税(0%)；REAL MODE 每次 ROT 前打印🧬。
9. PROD/QUOT **仅 APEX 可用**；DIFF 为负 → 🥀捕食者饿死(FCL-0008)；QUOT 除零 → 🔥(FCL-0007)；余数丢弃。
10. APEX 的 PROD/QUOT 受**扑咬距离**影响：地址差偶数 → 结果减半🐾。
11. HERBIVORE 超 255 → 归零+2秒阻塞(🤢胃溃疡溢出 FCL-0009)，CODE 模式跳过阻塞即时归零。
12. CARNIVORE 奇数行加速、偶数行减速2ms，CODE 模式跳过。
13. ROT **仅 DECOMPOSER 可用，仅 DECAY 段**；首次输出 ASCII 字符(char(int(能量)))，第二次输出 U+XXXX；NUMERIC OUTPUT 模式直出数值。
14. SCENT/LURK/POUNCE **三者仅 FOODWEB 内**；SCENT 结果变量须 APEX(Tiger/Lion)；WASM 下 POUNCE 永远扑空。
15. SEASON 湿度由**最近3条 DEVOURS 次数**决定：≥2→RAIN，否则 DRY；STORM ENABLED 后仅 RAIN 块语句>3 才随机乱序一次(🌩️)。
16. MIGRATION 定次循环，每轮后该物种能量**开平方根衰减**。
17. HIBERNATION while 直到 APEX=FULL，每轮×0.9，超 10000 轮强制唤醒(⏰ FCL-0010)。
18. MUTATION 1/3概率变异（根名替换，块内引用改写），触发随机表达一个 CASE，未触发空转；`MATCH(标识符)` 检测变异→1/未变→0。
19. EXTINCTION 立即回收并打印 16 行十六进制遗照；`EXTINCTION Virus_Crash` 强制终止进程。
20. **GC**：能量=0(value==0)且**连续3条指令未引用**才被分解者回收(DECOMPOSE)；活体(>0)豁免；APEX 永免；BIOME 引种段与 MIGRATION/SEASON/MUTATION/HIBERNATION 复合块内**暂停分解**；REAL 模式回收随机阻塞 100-1000ms。
21. 全局开关写在**首行、三段式之外**；可选 shebang 行。
22. 注释：小写字母开头整行，或 `OBSERVATION:` 开头。
23. **表达式优先级**：`()` > `*` `/` > `+` `-` > `MATCH()` > 指令语句；factor 可为 `number`|`fraction`|`(expr)`|`MATCH(id)`；分数 `integer/integer`。

## 示例索引
见 **examples.md**：
- 示例1 Hello World（三段式 + INTRODUCE + DEVOURS + ROT + GMO）
- 示例2 斐波那契（CLONE + MIGRATION）
- 示例3 SEASON 湿度分支
- 示例4 HIBERNATION 短例（自写，严格符合规则）
- 示例5 MUTATION 短例（自写，严格符合规则）

## 参考链接
- 在线 Playground: https://huang-520-add.github.io/fcl/
- 仓库与文档: `Huang-520-add/fcl`
- 完整指令集 / 语义 / 营养级矩阵 / 命名规范 / 表达式优先级 / 全局开关 / GC 规则 / 错误码表：见本技能 **reference.md**
