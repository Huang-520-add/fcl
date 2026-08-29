# FCL 语法规范（FCL Syntax Specification）v2.4

> 本文档以**形式化语法**（EBNF）和**关键词拆解**两种方式，精确定义 FCL 的词法规则与语法结构。适合：编译器实现者、语言研究者、严格对标规范的开发者。
> 学习入门 → [FCL_TUTORIAL.md](FCL_TUTORIAL.md)；语法速查 → [FCL_REFERENCE.md](FCL_REFERENCE.md)。

---

## 1. 词法（Lexicon）

### 1.1 字符集

```
FCL 源码允许的字符：
  - 大写拉丁字母：A-Z（关键字）
  - 小写拉丁字母：a-z（注释、内容）
  - 下划线：_（命名连接符）
  - 数字：0-9（数值、编号）
  - 标点：+ - * / ( ) ; , " { } . _
  - 空格、Tab、换行（分隔符，无语法意义）
  - 大小写敏感：Grass ≠ grass ≠ GRASS
```

### 1.2 记号类型（Token）

| 记号类型 | 示例 | 说明 |
|---|---|---|
| `KEYWORD` | `BIOME`、`INTRODUCE`、`DEVOURS` | 全部大写 |
| `IDENTIFIER` | `Grass_1`、`Wolf_M1`、`Alpha_Wolf` | 物种命名 |
| `NUMBER` | `60`、`3.14`、`2/3`、`60+5` | 整数/浮点/分数/表达式 |
| `OPERATOR` | `+`、`-`、`*`、`/`、`>`、`=` | 算术/比较运算符 |
| `DELIMITER` | `;`、`{`、`}`、`(`、`)`、`,` | 分隔符 |
| `STRING` | `"文本"` | CASE 标签文本 |
| `COMMENT` | 小写字母开头整行、`OBSERVATION:` 注释 | 忽略 |

### 1.3 关键字表（按字母序）

```
Alpha_*, ASSESS, Bacillus_*,
COMPETITION, DECAY, DEVOURS, DRY,
EXTINCTION,
FOODWEB,
GMO,
HIBERNATION,
INTRODUCE,
Lion_*, MATCH,
MIMICRY, MIGRATION, MUTATION,
NUMERIC OUTPUT,
OBSERVATION:,
PRODUCER,
RAIN, REAL MODE, ROT, STORM,
SEASON, STDIN, STDOUT, SUM, SYMBIOSIS,
TO,
USING,
WITH,
```

> 关键字不能用作变量名（小写除外——小写本身就是注释）。

---

## 2. EBNF 语法定义

### 2.1 程序结构（Program）

```
program       ::= shebang? gmoline? stormline? numline? modeDecl?
                  BIOME block
                  FOODWEB block
                  DECAY block

shebang       ::= "#!" ... NEWLINE          // 可选，支持 shebang 行
gmoline       ::= "GMO ENABLED" ";" NEWLINE
stormline     ::= "STORM ENABLED" ";" NEWLINE   // 可选：开启后 RAIN 块 >3 条语句随机乱序
numline       ::= "NUMERIC OUTPUT" ";" NEWLINE
modeDecl      ::= ("REAL MODE" | "CODE MODE") ";" NEWLINE

block         ::= "{" statement* "}" NEWLINE?
statement     ::= (declaration | operation | controlFlow | ioStmt | extinction | comment) ";"
comment       ::= ("OBSERVATION:" [DATE] [COORD] text) | (小写字母开头整行)
```

> `BIOME`、`FOODWEB`、`DECAY` 三段**强制齐全**，顺序固定，不可嵌套。

### 2.2 变量声明（Declaration）

```
declaration   ::= "INTRODUCE" identifier "AS" trophicLevel "WITH" expression

identifier    ::= speciesName ("_" (alphaId | ("M"|"F") digit+))?
               | "Alpha_" speciesName
               | speciesName "_" digit+

speciesName  ::= "Grass" | "Algae" | "Sheep" | "Rabbit"
               | "Wolf" | "Fox" | "Tiger" | "Lion"
               | "Fungus" | "Bacillus"

trophicLevel ::= "PRODUCER" | "HERBIVORE" | "CARNIVORE" | "APEX" | "DECOMPOSER"

alphaId      ::= "1" | "2" | "3" | ...           // 独居编号
```

### 2.3 捕食运算（Devour）

```
operation     ::= devour | clone | assess | symbios | competition | mimicry | sprout

devour        ::= predator "DEVOURS" prey "USING" algorithm
                 // predicate: predator.trophicLevel = prey.trophicLevel + 1
                 //             OR (inside DECAY block AND predator is DECOMPOSER)

predicate     ::= identifier                      // 物种变量名
algorithm     ::= "SUM" | "DIFF" | "PROD" | "QUOT"
                 // SUM/DIFF: 任意合法捕食者
                 // PROD/QUOT: 仅 APEX
```

### 2.4 复制（Clone）

```
clone         ::= "CLONE" targetIdentifier "FROM" sourceIdentifier
                 // predicate: target 已声明（类型已知）；source 不受影响
```

### 2.5 比较（Assess）

```
assess        ::= "ASSESS" exprA "AGAINST" exprB "TO" apexVariable
                 // A.value >= B.value → apexVariable = FULL(1)
                 // otherwise          → apexVariable = HUNGRY(0)
                 // predicate: 第三个参数必须为 APEX
```

### 2.6 布尔逻辑（Boolean Logic）

```
symbios       ::= "SYMBIOSIS" apexA "WITH" apexB "TO" apexC
                 // C = A AND B
competition   ::= "COMPETITION" apexA "OR" apexB "TO" apexC
                 // C = A OR B
mimicry       ::= "MIMICRY" apexA "TO" apexB
                 // B = NOT A
```

### 2.7 输入（Input）

```
sprout        ::= "SPROUT" producerIdentifier "FROM" "STDIN"
                 // predicate: 只能注入 PRODUCER
                 // timeout: 2 秒
```

### 2.8 输出（Output）

```
ioStmt        ::= "ROT" decomposerIdentifier "TO" "STDOUT"
                 // predicate: 只有 DECOMPOSER 可执行
                 // 编码模式（默认）：首次输出 char(int(energy))；第二次输出 U+XXXX
                 // 数值模式（NUMERIC OUTPUT）：直接输出数值
```

### 2.9 控制流（Control Flow）

```
controlFlow   ::= season | migration | hibernation | mutation

season        ::= "SEASON" "RAIN" block "DRY" block
                 // 湿度 = 最近 3 条 DEVOURS 次数；>=2 → RAIN，否则 DRY

migration    ::= "MIGRATION" identifier "OVER" integer block
                 // 定次循环；每轮后 identifier 的能量开平方根

hibernation ::= "HIBERNATION" identifier "UNTIL" apexVariable block
                 // 条件循环；直到 apex = FULL(1)；每轮 identifier *= 0.9
                 // 防死循环：>10000 轮强制唤醒

mutation     ::= "MUTATION" identifier "{" caseClause+ "}"
caseClause   ::= "CASE" stringLiteral ":" statement+
                 // 运行时 identifier 有 1/3 概率变异（改名）
                 // 可用 MATCH(identifier) 检测
```

### 2.10 灭绝（Extinction）

```
extinction   ::= "EXTINCTION" identifier
                 // 立即回收变量并打印 16 行十六进制遗照
                 // EXTINCTION Virus_Crash → 强制终止进程
```

### 2.11 表达式（Expression）

```
expression   ::= term (('+' | '-') term)*
term         ::= factor (('*' | '/') factor)*
factor       ::= number | fraction | '(' expression ')' | "MATCH(" identifier ")"

number       ::= integer | float
fraction     ::= integer '/' integer          // 1/3 → 0.333...
MATCH()      ::= "MATCH(" identifier ")"      // 变异检测：变异→1，未变→0
```

---

## 3. 营养级约束矩阵（Trophic Constraint Matrix）

```
生产者 PRODUCER (1)：
  可被：HERBIVORE、DECOMPOSER（在 DECAY 块内）
  可吃：无（顶级来源）

食草 HERBIVORE (2)：
  可被：CARNIVORE、DECOMPOSER（在 DECAY 块内）
  可吃：PRODUCER

食肉 CARNIVORE (3)：
  可被：APEX、DECOMPOSER（在 DECAY 块内）
  可吃：HERBIVORE

顶级 APEX (4)：
  可被：DECOMPOSER（在 DECAY 块内）
  可吃：CARNIVORE
  特殊：可使用 PROD/QUOT，可承载 ASSESS 结果

分解 DECOMPOSER (5)：
  可被：无（末端）
  可吃：任意（仅在 DECAY 块内）
```

---

## 4. 优先级与结合性（Precedence & Associativity）

| 优先级 | 运算符 | 说明 | 结合性 |
|---|---|---|---|
| 1（最高）| `()` | 括号 | — |
| 2 | `*` `/` | 乘除 | 左 |
| 3 | `+` `-` | 加减 | 左 |
| 4 | `MATCH()` | 变异检测 | — |
| 5 | `DEVOURS` 等指令 | 语句级（非表达式） | — |

---

## 5. 完整关键字与报错对应表

| 关键字 | 报错场景 | 错误码 |
|---|---|---|
| `INTRODUCE` + 不在册物种 | 🌿 外来物种入侵 | FCL-0003 |
| `INTRODUCE` + 类型/物种不匹配 | ⚠️ 分类学混乱 | FCL-0004 |
| `INTRODUCE` + 命名格式错误 | ⚠️ 族谱登记混乱 | FCL-0005 |
| `DEVOURS` + 营养级差不为 1 | 🦴 食性冲突 | FCL-0002 |
| `DEVOURS` + 非 APEX 使用 PROD/QUOT | 🦴 食性冲突 | FCL-0002 |
| `ROT` + 非 DECOMPOSER | 🦴 食性冲突 | FCL-0002 |
| `SPROUT` + 非 PRODUCER | 🦴 食性冲突 | FCL-0002 |
| `ASSESS` + 第三个参数非 APEX | 🦴 食性冲突 | FCL-0002 |
| `DEVOURS` + DIFF 时捕食者能量不足 | 🥀 捕食者饿死 | FCL-0008 |
| HERBIVORE 能量 > 255 | 🤢 胃溃疡溢出 | FCL-0009 |
| 除零 | 🔥 干旱导致食物链断裂 | FCL-0007 |
| HIBERNATION 超过 10000 轮 | ⏰ 冬眠过久 | FCL-0010 |
| 缺少 BIOME / FOODWEB / DECAY | 🌍 生态崩溃 | FCL-0006 |
| FOODWEB 内无 DEVOURS | 🌍 生态崩溃 | FCL-0006 |
| 表达式解析失败 | 🌿 变异物种入侵 | FCL-0001 |
| 未知语句 | 🌿 变异物种入侵 | FCL-0001 |

---

> 语法规范版本：v2.4
> 参考实现：[Huang-520-add/fcl](https://github.com/Huang-520-add/fcl)
