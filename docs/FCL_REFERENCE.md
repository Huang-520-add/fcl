# FCL 官方参考手册（FCL Language Reference）v2.0

> **代码在吞噬中传递，真理在分解中显现。**
> 本手册供**查阅**使用：完整定义 FCL 的语法、类型、指令、控制流与垃圾回收。
> 学习入门请移步 [FCL_TUTORIAL.md](FCL_TUTORIAL.md)；生态圈世界观见 [FCL_ECOLOGY.md](FCL_ECOLOGY.md)。

---

## 1. 语言概述

FCL（FoodChain Language，食物链语言）是一门**深奥编程语言（Esolang）**，同时也是**解释型、命令式、强类型**语言。它把传统编程概念全部重新映射到生态学：

| 传统概念 | FCL 中的形态 |
|---|---|
| 变量 | 生态圈在册物种（10 种，绑定营养级） |
| 赋值/运算 | 捕食（DEVOURS）——单向、破坏性、带能量税 |
| 复制 | 无性繁殖（CLONE） |
| 比较 | 生态位评估（ASSESS） |
| 布尔运算 | 共生/竞争/拟态（SYMBIOSIS/COMPETITION/MIMICRY） |
| if/else | 雨季/旱季（SEASON，湿度驱动） |
| for | 迁徙（MIGRATION，定次+开方衰减） |
| while | 冬眠（HIBERNATION，条件循环+代谢消耗） |
| 输出 | 分解矿化（ROT，仅分解者可执行，按 ASCII） |
| 垃圾回收 | 生态分解（只分解"尸体"） |

**图灵完备**：FCL 具备比较（ASSESS）、条件循环（HIBERNATION）、无限存储（INTRODUCE），理论上可表达一切可计算问题。

**程序形态**：任何合法程序由强制三段式结构组成：`BIOME`（引种）→ `FOODWEB`（捕食）→ `DECAY`（分解）。

### 1.1 生态学依据（设计来源）

FCL 的每条规则都来自真实生态学：

| FCL 特性 | 生态学依据 |
|---|---|
| 20% 能量传递税 | **林德曼定律**（十分之一定律）：能量沿食物链传递效率约 10%~20%，其余经呼吸散失；能量单向流动、逐级递减 |
| 猎物被吃归零 | 能量单向流动：进入捕食者体内的能量不会再回到猎物 |
| 分解者 100% 回收 + 任意级 | 分解者（还原者）把遗体有机物彻底分解为无机物，不浪费残余能量 |
| 只有分解者能输出（ROT） | 物质循环：养分必须经分解者矿化才能回归环境 |
| CLONE（无性繁殖） | 物质循环：养分回到低营养级被重新利用（扦插/克隆） |
| 群居物种有 Alpha 首领 | 狼群/狮群严格等级制度：狼王统一调遣、协同捕猎 |
| 独居物种纯编号 | 猫科多独居（虎）：领域资源有限，独居减少竞争 |
| 食物链 4~5 级 | 能量金字塔逐级递减，无法支撑过长食物链 |
| 分解者只分解"尸体"（能量=0） | 分解者不攻击活体，只分解已死亡有机体 |
| SEASON 湿度分支 / MIGRATION 衰减 / HIBERNATION 代谢 / MUTATION 变异 | 环境因子（季节降水、迁徙消耗、冬眠蛰伏、基因突变）驱动生态 |

---

## 2. 程序结构

```
BIOME   { ... }   // 引种段：仅允许变量声明（INTRODUCE）与能量注入
FOODWEB { ... }   // 捕食段：核心运算，必须包含至少一次 DEVOURS
DECAY   { ... }   // 分解段：输出（ROT）与回收（EXTINCTION）
```

规则：
- 三段**必须齐全、顺序不可颠倒**，缺任一段 → `🌍 生态崩溃，食物链断裂！`
- FOODWEB 必须含 DEVOURS（否则同样报 🌍）
- GMO ENABLED 写在三段式之外（程序首行）
- 段内语句以 `;` 结尾；复合控制块用 `{ }`

---

## 3. 词法与命名

### 3.1 字符集

有效字符：大写拉丁字母（A-Z）、下划线（_）、数字（0-9）、分号（;）、花括号（{}）、引号（"）及换行符。

### 3.2 标识符（生态圈命名制）

变量名 = `物种名_族群标识`，**物种必须在册**（10 种），营养级必须与声明类型一致：

| 营养级 | 在册物种 | 族群结构 | 命名格式 | 示例 |
|---|---|---|---|---|
| 1 生产者 | Grass（草）、Algae（藻） | 种群 | `物种_编号` | `Grass_1`、`Algae_2` |
| 2 食草 | Sheep（羊） | **群居** | `Alpha_Sheep` / `Sheep_M1` / `Sheep_F2` | 头羊/雄性1号/雌性2号 |
| 2 食草 | Rabbit（兔） | 独居 | `物种_编号` | `Rabbit_1` |
| 3 食肉 | Wolf（狼） | **群居** | `Alpha_Wolf` / `Wolf_M1` / `Wolf_F1` | 狼王/成员 |
| 3 食肉 | Fox（狐） | 独居 | `物种_编号` | `Fox_1` |
| 4 顶级 | Tiger（虎） | 独居 | `物种_编号` | `Tiger_1` |
| 4 顶级 | Lion（狮） | **群居** | `Alpha_Lion` / `Lion_M1` / `Lion_F1` | 狮王/成员 |
| 5 分解 | Fungus（真菌）、Bacillus（杆菌） | 菌落 | `物种_编号` | `Fungus_1`、`Bacillus_1` |

违规报错：
- 物种不在册 → `🌿 外来物种入侵，生态圈不予接纳！`
- 物种营养级与声明类型不符 → `⚠️ 分类学混乱！<物种> 是<实际营养级>，不是<声明营养级>`
- 族群格式错误（群居无首领/性别标签、独居带非数字标识）→ `⚠️ 族谱登记混乱！`

### 3.3 大小写

指令关键字全部大写；**小写内容视为注释**（整行忽略）。

---

## 4. 类型系统

### 4.1 五种营养级

| 类型关键字 | 营养级 | 存储范围 | 特性 |
|---|---|---|---|
| `PRODUCER` | 1 生产者 | 浮点 0.0~9999.9 | 支持分数输入（3/2 → 1.5） |
| `HERBIVORE` | 2 食草 | 整型 0~255 | **胃溃疡溢出**：>255 归零并阻塞 2 秒 |
| `CARNIVORE` | 3 食肉 | 整型 0~65535 | 运算速度与行号奇偶挂钩 |
| `APEX` | 4 顶级 | 布尔 FULL=1 / HUNGRY=0 | 唯一能承载判定结果（裁决权） |
| `DECOMPOSER` | 5 分解 | 数值按 ASCII 解释 | 唯一能执行 ROT 输出 |

内部存储：所有变量统一 `double` 数值 + 营养级标签。HERBIVORE/CARNIVORE 运算后四舍五入取整。

### 4.2 营养级铁律（捕食优先级）

```
PRODUCER(1) → HERBIVORE(2) → CARNIVORE(3) → APEX(4)
                          ↕（分解者回收一切，5 级）
```

- 捕食者营养级必须**恰好**比猎物高 1（例外：DECAY 块内分解者豁免，可吃任意级）
- 同级互吃、跨级吃 → `🦴 食性冲突，捕食者拒绝进食`
- 能量流严格单向：猎物被吃即归零，值只能流向更高营养级

---

## 5. 表达式

用于 `INTRODUCE ... WITH <表达式>` 与 `MATCH()`：

```
expr   := term (('+' | '-') term)*
term   := factor (('*' | '/') factor)*
factor := 数字 | 分数(a/b) | '(' expr ')' | MATCH(变量名)
```

- 除法除零 → `🔥 干旱导致食物链断裂`
- `MATCH(name)`：变量是否发生过 MUTATION 变异 → 返回 1/0
- 分数按除法计算（1/3 → 0.333…）

---

## 6. 指令全集

### 6.1 引种：INTRODUCE

```ebnf
INTRODUCE <物种名> AS <营养级> WITH <表达式> ;
```

声明变量并注入初始能量。示例：
```foodchain
INTRODUCE Grass_1 AS PRODUCER WITH 30+35 ;
INTRODUCE Sheep_M1 AS HERBIVORE WITH 0 ;
```
约束：命名必须通过生态圈校验（见 3.2）；APEX 初始值自动归一为 0/1。

### 6.2 捕食：DEVOURS

```ebnf
<捕食者> DEVOURS <猎物> USING <算法> ;
```

| 算法 | 运算 | 约束 |
|---|---|---|
| `SUM` | 捕食者 = 捕食者 + 猎物 | 无 |
| `DIFF` | 捕食者 = 捕食者 − 猎物（大减小） | 捕食者能量 < 猎物 → `🥀 捕食者饿死` |
| `PROD` | 捕食者 = 捕食者 × 猎物 | 仅 APEX |
| `QUOT` | 捕食者 = 捕食者 ÷ 猎物（余数丢弃） | 仅 APEX |

通用规则：
- **能量传递税**：非豁免场景下 `实际结果 = 理论结果 × 0.2`（热力学第二定律）
- 豁免：DECAY 块内分解者捕食（100% 回收）；GMO ENABLED 后全部捕食免税
- **猎物被消化归零**（值单向流动）
- APEX 的 PROD/QUOT 受**扑咬距离**影响：捕食者与猎物存储地址差为偶数 → 扑咬落空，结果减半（`🐾`）
- HERBIVORE 溢出（>255）→ 归零 + 2 秒阻塞（`🤢 胃溃疡溢出`）
- CARNIVORE：语句行号奇数加速、偶数减速 2ms

### 6.3 无性繁殖：CLONE

```ebnf
CLONE <目标> FROM <源> ;
```

目标 = 源的数值副本，源不受影响。目标必须先 INTRODUCE（类型已知）。
生态依据：营养繁殖/克隆。**这是 FCL 唯一的"回写"手段（物质循环）。**

### 6.4 生态位评估：ASSESS

```ebnf
ASSESS <A> AGAINST <B> TO <C> ;
```

`A.value ≥ B.value → C = FULL(1)`，否则 `C = HUNGRY(0)`。A、B 任意类型，**C 必须 APEX**。
非 APEX 承载判定 → `🦴 食性冲突，只有顶级掠食者（APEX）能承载判定`。
执行时打印生态位评估日志（🧭）。

### 6.5 逻辑运算（布尔）

```ebnf
SYMBIOSIS  <A> WITH <B> TO <C> ;   (* C = A AND B，互利共生 *)
COMPETITION <A> OR <B> TO <C> ;    (* C = A OR B，替代觅食路径 *)
MIMICRY    <A> TO <B> ;            (* B = NOT A，拟态伪装 *)
```

三个操作数（MIMICRY 为两个）必须全部为 APEX。

### 6.6 输入：SPROUT

```ebnf
SPROUT <生产者名> FROM STDIN ;
```

只能注入 PRODUCER（否则 🦴）。控制台播放 2 秒音调，需在时限内输入数值；超时 → 0。

### 6.7 输出：ROT

```ebnf
ROT <分解者名> TO STDOUT ;
```

只能由 DECOMPOSER 执行（否则 🦴）。
- 第一次 ROT：输出 `char(int(能量))`（ASCII 字符）
- 连续第二次 ROT：输出 `U+XXXX`（Unicode 码位）
- GMO 模式下输出前加 🧬 标识

### 6.8 环境分支：SEASON

```ebnf
SEASON RAIN { ... } DRY { ... } ;
```

- **湿度** = 最近 3 条指令中 DEVOURS 次数；≥2 为湿润 → 执行 RAIN，否则执行 DRY（**二选一**）
- RAIN 块内语句数 > 3 → 降级为 STORM，块内语句随机乱序执行一次（🌩️）

### 6.9 迁徙循环：MIGRATION

```ebnf
MIGRATION <物种> OVER <次数> { ... } ;
```

定次循环。每轮迭代后该物种能量**开平方根**（迁徙消耗）。

### 6.10 冬眠循环：HIBERNATION

```ebnf
HIBERNATION <物种> UNTIL <APEX变量> { ... } ;
```

**条件循环（while）**：循环执行直到指定 APEX 变量为 FULL（1）。每轮该物种能量 ×0.9（冬眠代谢 10%）。超过 10000 轮强制唤醒（⏰）。

### 6.11 变异：MUTATION

```ebnf
MUTATION <物种> { CASE "特征": <语句> ... } ;
```

运行时物种名可能被随机替换为同音异形词（Wolf→Wolv、Sheep→Sheepe、Tiger→Tygre、Bacillus→Bacilluz，概率 1/3），块内引用该物种的语句随之改名。可用 `MATCH(name)` 在表达式中检测变异。

### 6.12 灭绝：EXTINCTION

```ebnf
EXTINCTION <物种> ;
```

立即回收变量，并打印 16 行十六进制内存"遗照"。`EXTINCTION Virus_Crash ;` 强制终止进程（生态圈外病毒，只能被灭绝）。

### 6.13 转基因：GMO

```ebnf
GMO ENABLED ;   // 必须位于程序首行（三段式之外）
```

- 能量税降为 0%（100% 传递效率）
- 代价：每次 ROT 输出前打印 🧬 转基因产品标识

---

## 7. 控制流总结

| 需求 | 传统语言 | FCL |
|---|---|---|
| 顺序执行 | 逐行 | 逐语句（段内顺序） |
| 条件分支 | if/else | SEASON RAIN/DRY（湿度≥2 → RAIN） |
| 定次循环 | for i in range(n) | MIGRATION（每轮 sqrt 衰减） |
| 条件循环 | while | HIBERNATION UNTIL（每轮 ×0.9） |
| 多路选择 | switch | MUTATION CASE（随机变异） |
| 跳转/退出 | break/return | EXTINCTION Virus_Crash |

---

## 8. 垃圾回收（生态分解）

1. **活体豁免**：能量 >0（value≠0）的变量不受分解威胁
2. **尸体分解**：能量 =0 且连续 3 条指令未被引用 → DECOMPOSE，随机阻塞 100~1000ms
3. **活跃期暂停**：BIOME 引种段、复合控制块（MIGRATION/SEASON/MUTATION/HIBERNATION）执行期间暂停分解
4. **顶级豁免**：APEX 永不自动回收
5. 手动回收：EXTINCTION（含遗照）

---

## 9. 注释

```ebnf
OBSERVATION: YYYY-MM-DD, Lat:<纬度>, Lon:<经度>, <内容>
```

缺少日期或坐标 → 忽略并警告 `🔭 缺乏科考精神！`。小写内容行同样视为注释。

---

## 10. 错误信息对照表

| 传统报错 | FCL 报错 | 触发场景 |
|---|---|---|
| Syntax Error | 🌿 变异物种入侵，语法免疫系统失效 | 未知语句/表达式错误 |
| Type Mismatch | 🦴 食性冲突，捕食者拒绝进食 | 营养级差 ≠1 / 类型误用 |
| 非法标识符 | 🌿 外来物种入侵，生态圈不予接纳！ | 物种不在册 |
| 类型错配（命名） | ⚠️ 分类学混乱！ | 物种营养级与声明不符 |
| 命名格式错误 | ⚠️ 族谱登记混乱！ | 族群标识格式不符 |
| 结构错误 | 🌍 生态崩溃，食物链断裂！ | 缺段 / 无 DEVOURS |
| Division by Zero | 🔥 干旱导致食物链断裂 | 表达式除零 |
| 负值 | 🥀 捕食者饿死，能量为负 | DIFF 时捕食者能量不足 |
| 溢出 | 🤢 胃溃疡溢出，能量归零 | HERBIVORE >255 |
| 超时/死循环 | ⏰ 冬眠过久（>10000 轮），强制唤醒 | HIBERNATION 不醒 |
| 其他 | 🧬 转基因污染 | GMO 相关违规（输出标识） |

---

## 11. 附录

### 11.1 标准库（预置物种）

- `Bacillus_Stdio`：标准输入输出分解者（自动处理摩斯电码）
- `Rhizobium_Math`：高精度乘法共生菌（可被 APEX 调用）
- `Virus_Crash`：强制退出（EXTINCTION Virus_Crash ;）

### 11.2 完整示例索引

| 文件 | 演示 |
|---|---|
| ../examples/example1.fc | 3+5=8（输出退格符 + U+0008） |
| ../examples/example2.fc | 20% 能量税（65×0.2=13） |
| ../examples/example3.fc | GMO 免税输出 'A' + U+0041 |
| ../examples/fib.fc | 斐波那契递推（CLONE 物质循环） |
| ../examples/plus1.fc | 1+1（ASCII 控制字符版） |
| ../examples/plus1_visible.fc | 25+25=50='2'（可见字符版） |
| ../examples/eco_logic.fc | ASSESS/SYMBIOSIS/COMPETITION/MIMICRY |
| ../examples/eco_hibernate.fc | HIBERNATION 冬眠循环 |
| ../examples/control_*.fc | 控制流与输入输出系列 |
| ../examples/err_*.fc | 错误用例系列 |

### 11.3 编译与运行

```bash
g++ -std=c++17 -O2 -o fcl main.cpp
./fcl 程序文件.fc
```

文档版本：v2.0（参考手册）
最后更新：2026-08-27
