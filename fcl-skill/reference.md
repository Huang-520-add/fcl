# FCL 完整参考（v3.1.0）

> 本文件逐条展开官方文档：三段式、营养级矩阵、命名规范、完整指令集与语义、表达式、全局开关、GC 规则、错误码表。所有内容必须与官方文档一字不差地对齐，不发明任何材料中不存在的语法或指令。

---

## 0. 语言概览
- 生态学主题 esolang，图灵完备，C++17 解释器实现，MIT 许可，作者 by.荒。
- 变量 = 在册物种；运算 = 捕食；控制流 = 生态事件。
- 程序强制三段式：`BIOME`(引种) → `FOODWEB`(捕食) → `DECAY`(分解)。
- 维度一维；范式命令式/结构化；强类型（营养级绑定，5级，无隐式转换）。
- 文件扩展名 `.fc`；在线 Playground: https://huang-520-add.github.io/fcl/ ；仓库 `Huang-520-add/fcl`。

---

## 1. 三段式结构（强制顺序，不可嵌套，缺一段 → 🌍生态崩溃终止 FCL-0006）
```
BIOME {      /* 仅变量声明与初始能量注入 INTRODUCE */
}
FOODWEB {     /* 核心运算，必须至少一次 DEVOURS/SCENT/POUNCE，否则🌍终止 */
}
DECAY {       /* 输出 ROT 与内存回收 */
}
```
- 三段顺序固定，不可嵌套。
- 缺任意一段 → 🌍生态崩溃（FCL-0006）。
- FOODWEB 内若无任何 DEVOURS/SCENT/POUNCE → 🌍终止（FCL-0006）。

---

## 2. 营养级矩阵（5级，强类型，变量名绑定营养级）
| 级 | 名称 | 类型 / 范围 | 特殊行为 | 在册物种（群居/独居） |
|----|------|-------------|----------|------------------------|
| 1 | PRODUCER 生产者 | 浮点 0.0~9999.9；支持分数(3/2→1.5) | — | Grass(草), Algae(藻) |
| 2 | HERBIVORE 食草 | 整型 0~255 | 超过255归零+2秒阻塞(🤢胃溃疡溢出,CODE模式跳过阻塞) | Sheep(羊,群居), Rabbit(兔,独居) |
| 3 | CARNIVORE 食肉 | 整型 0~65535 | 奇数行加速、偶数行减速2ms(CODE跳过) | Wolf(狼,群居), Fox(狐,独居) |
| 4 | APEX 顶级 | 布尔 FULL=1 / HUNGRY=0 | 逻辑判断依赖"扑咬距离"(变量内存地址偏移) | Tiger(虎,独居), Lion(狮,群居) |
| 5 | DECOMPOSER 分解者 | 字符串(数值按ASCII解释) | 输出按ASCII渲染 | Fungus(真菌), Bacillus(杆菌) |

---

## 3. 物种命名规范
- 变量名 = `物种_标识`。
- **群居**(狼/狮/羊)：
  - 首领：`Alpha_<物种>`（Alpha_Wolf / Alpha_Lion / Alpha_Sheep）
  - 雄性：`<物种>_M<编号>`（Wolf_M1）
  - 雌性：`<物种>_F<编号>`（Sheep_F2）
- **独居**(虎/狐/兔)：`<物种>_<编号>`（Tiger_1 / Fox_2 / Rabbit_1）
- **生产者/分解者**：`<物种>_<编号>`（Grass_1 / Fungus_1）
- 编号从 1 开始，可跳号。
- 约束：
  - 物种名必须在册，否则 🌿外来物种入侵（FCL-0001）。
  - 营养级须与声明一致，否则 ⚠️分类学混乱（FCL-0004）。
  - 群居须有 Alpha 或 M/F 标签，否则 ⚠️族谱登记混乱（FCL-0005）。

---

## 4. 完整指令集与语义表

### 4.1 INTRODUCE（引种声明，仅 BIOME 内）
- 语法：`INTRODUCE <物种> AS <营养级> WITH <表达式> ;`
- 语义：声明变量（在册物种）并注入初始能量；表达式支持数字/分数/四则/括号。
- 约束：仅出现在 BIOME 段；APEX 初值归一 0/1。
- 示例：`INTRODUCE Grass_1 AS PRODUCER WITH 60+5 ;`

### 4.2 DEVOURS（捕食，核心运算）
- 语法：`<捕食者 p> DEVOURS <猎物 y> USING SUM|DIFF|PROD|QUOT ;`
- 运算：
  - `SUM`：加
  - `DIFF`：大减小（结果为负 → 🥀饿死，FCL-0008）
  - `PROD`：乘（**仅 APEX 可用**）
  - `QUOT`：除（**仅 APEX 可用**，余数丢弃；除零 → 🔥，FCL-0007）
- 营养级限制：捕食者营养级必须**恰好比猎物高 1 且捕食者 > 猎物**。
  - 豁免：在 DECAY 段内，DECOMPOSER 可吃任意级、100% 吸收。
- 副作用：猎物能量归零。
- 能量税：实际能量 = 理论值 × 0.2（20%）；`GMO ENABLED` 时免税（0%）。
- APEX 的 PROD/QUOT 受**扑咬距离**影响：变量内存地址差为偶数 → 结果减半🐾。

### 4.3 CLONE（克隆）
- 语法：`CLONE <目标> FROM <源> ;`
- 语义：目标获得源的副本，源不变。
- 约束：目标须已 INTRODUCE。

### 4.4 ASSESS（评估）
- 语法：`ASSESS <A> AGAINST <B> TO <C> ;`
- 语义：若 A ≥ B → C = FULL(1)，否则 C = HUNGRY(0)。
- 约束：C 必须为 APEX。

### 4.5 SYMBIOSIS（共生）
- 语法：`SYMBIOSIS <A> WITH <B> TO <C> ;`
- 语义：C = A AND B。
- 约束：A/B/C 全为 APEX。

### 4.6 COMPETITION（竞争）
- 语法：`COMPETITION <A> OR <B> TO <C> ;`
- 语义：C = A OR B。
- 约束：A/B/C 全为 APEX。

### 4.7 MIMICRY（拟态）
- 语法：`MIMICRY <A> TO <B> ;`
- 语义：B = NOT A。
- 约束：A/B 全为 APEX。

### 4.8 SCENT（嗅探，仅 FOODWEB 内）
- 语法：`SCENT <嗅探者> TO <APEX变量> ;`
- 语义：非阻塞嗅探 STDIN 是否就绪 → 就绪则 APEX 存 1.0，否则 0.0；输出👃。
- 约束：结果变量须为 APEX（Tiger/Lion）。

### 4.9 LURK（潜伏，仅 FOODWEB 内）
- 语法：`LURK <物种> FOR <节拍数> ;`
- 语义：等待 N 拍（REAL 100ms/拍，CODE 1ms/拍，钳制 0-600）；该物种须已注册；输出🕳️。

### 4.10 POUNCE（猛扑，仅 FOODWEB 内）
- 语法：`POUNCE <捕食者> ;`
- 语义：非阻塞猛扑 —— STDIN 就绪则读入存物种(🦅命中)，否则扑空能量保持(🐾)；EOF/非法 → 🦠腐坏。
- 约束：WASM 环境下永远扑空。

### 4.11 ROT（分解输出，仅 DECAY 段，仅 DECOMPOSER）
- 语法：`ROT <分解者> TO STDOUT ;`
- 语义：仅 DECOMPOSER 可用。
  - 首次：输出 ASCII 字符 `char(int(能量))`。
  - 第二次：输出 `U+XXXX`。
  - `NUMERIC OUTPUT` 模式：直出数值（不转 ASCII）。

### 4.12 SEASON（季节湿度分支，FOODWEB 内）
- 语法：`SEASON RAIN { ... } DRY { ... } ;`
- 语义：湿度 = 最近 3 条 DEVOURS 次数；≥2 → 执行 RAIN 块，否则执行 DRY 块。
- 乱序：`STORM ENABLED` 后，仅当 RAIN 块语句 > 3 才随机乱序一次(🌩️)，默认不乱序。

### 4.13 MIGRATION（定次迁移循环，FOODWEB 内）
- 语法：`MIGRATION <物种> OVER <次数> { ... } ;`
- 语义：定次循环；每轮结束后该物种能量开平方根衰减。

### 4.14 HIBERNATION（冬眠 while 循环，FOODWEB 内）
- 语法：`HIBERNATION <物种> UNTIL <APEX变量> { ... } ;`
- 语义：while 循环，直到 APEX 变量 = FULL 时停止；每轮物种能量 ×0.9。
- 边界：超过 10000 轮强制唤醒(⏰，FCL-0010)。

### 4.15 MUTATION（变异，FOODWEB 内）
- 语法：`MUTATION <物种> { CASE "特征": <语句> ... } ;`
- 语义：
  - 1/3 概率变异：根名替换为变异名（如 Wolf→Wolv），块内对该物种的引用随之改写。
  - 触发时随机表达（执行）其中一个 CASE；未触发则空转。
- 检测：`MATCH(标识符)` 可检测是否变异（变异→1，未变→0）。MATCH 可在表达式中作为 factor 使用。

### 4.16 EXTINCTION（灭绝回收）
- 语法：`EXTINCTION <变量名> ;`
- 语义：立即回收并打印 16 行十六进制遗照。
- 特殊：`EXTINCTION Virus_Crash ;` 强制终止进程。

---

## 5. 表达式与全局开关

### 5.1 表达式文法
```
expr   = term (('+' | '-') term)*
term   = factor (('*' | '/') factor)*
factor = number | fraction | '(' expr ')' | MATCH(id)
fraction = integer '/' integer
```
- 优先级：`()` > `*` `/` > `+` `-` > `MATCH()` > 指令语句。
- 分数：`integer/integer`（如 3/2 → 1.5，仅 PRODUCER 支持分数语义）。

### 5.2 全局开关（首行，三段式之外）
- `GMO ENABLED ;` —— 能量税降为 0%（免税）；REAL MODE 下每次 ROT 前打印🧬。
- `STORM ENABLED ;` —— 开启 RAIN 块 >3 语句随机乱序。
- `REAL MODE ;` / `CODE MODE ;`（默认）—— 真实模式生态等待生效；CODE 模式跳过等待高速（胃溃疡溢出即时归零 / 食肉奇偶延迟跳过 / GC 即时）。
- `NUMERIC OUTPUT ;` —— ROT 直出数值而非 ASCII。
- 可选 shebang 行。
- 注释：小写字母开头的整行，或 `OBSERVATION:` 开头。

---

## 6. GC 与错误

### 6.1 GC（垃圾回收）规则
- 能量 = 0（value==0）且**连续 3 条指令未引用** → 被分解者回收（DECOMPOSE）。
- 活体（>0）豁免。
- APEX **永免**回收。
- BIOME 引种段与 MIGRATION/SEASON/MUTATION/HIBERNATION 复合块内**暂停分解**。
- REAL 模式回收时随机阻塞 100-1000ms。

### 6.2 错误码表
| 错误 | 含义 | 码 |
|------|------|----|
| 🌿 外来物种入侵 / 未知语句 | 物种名不在册或未知语句 | FCL-0001 |
| 🦴 食性冲突 / 类型不符 | 食性冲突或类型不匹配 | FCL-0002 |
| ⚠️ 分类学混乱 | 营养级与声明不一致 | FCL-0004 |
| ⚠️ 族谱登记混乱 | 群居缺少 Alpha 或 M/F 标签 | FCL-0005 |
| 🌍 生态崩溃 | 缺块 / FOODWEB 无捕食 | FCL-0006 |
| 🔥 除零 | QUOT 除零 | FCL-0007 |
| 🥀 捕食者饿死 | DIFF 结果为负 | FCL-0008 |
| 🤢 胃溃疡溢出 | HERBIVORE 超过 255 | FCL-0009 |
| ⏰ 冬眠过久 | HIBERNATION 超过 10000 轮 | FCL-0010 |

---

## 7. 参考链接
- 在线 Playground: https://huang-520-add.github.io/fcl/
- 仓库与文档: `Huang-520-add/fcl`
- 技能总览与触发词: `SKILL.md`
- 示例集（含 Hello World / 斐波那契 / SEASON / HIBERNATION / MUTATION）: `examples.md`
