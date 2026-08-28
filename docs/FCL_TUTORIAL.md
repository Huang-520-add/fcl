# FCL 官方教学书（FCL Textbook）v2.4

> **代码在吞噬中传递，真理在分解中显现。**
> 这本教学书是给**完全没学过编程**的你准备的。你不需要任何编程基础——只要会打字、会算数、有好奇心，就能读完它并写出自己的 FCL 程序。
> 每节课都有：学习目标、生态小课堂、逐行讲解、**输出对照表**（把编码输出翻译成真实数值）、练习。
> 语法速查：[FCL_REFERENCE.md](FCL_REFERENCE.md)

---

# 第 0 课：开始之前（零基础预备课）

> 🎯 读完这课你将学会：编程是什么、怎么运行 FCL、**ASCII 编码是什么**（不懂它就看不懂 FCL 的输出）、遇到报错怎么办。

## 0.1 编程是什么？

**程序** = 写给计算机的"说明书"，一步步告诉它做什么。

比如你想让计算机"算 1+1 并告诉我结果"，用最普通的语言（Python）写：

```python
print(1 + 1)    # 结果：2
```

**变量** = 一个"盒子"，里面可以放数字。你可以给盒子起名字、往里面放数、再拿出来用：

```python
x = 5      # 造一个盒子 x，放 5 进去
y = 3      # 造一个盒子 y，放 3 进去
print(x+y) # 拿出两个盒子里的数相加：8
```

FCL 里的"盒子"比较特别——**盒子必须是生态圈里的动物植物**（草、羊、狼、虎、真菌），往盒子里放数叫"引种"，两个盒子相加叫"捕食"。但本质和你学的变量一模一样。

## 0.2 怎么运行 FCL？（三步）

1. **准备解释器**：Windows 下载 `fcl-windows-latest.exe`；Mac/Linux 用 `make build` 编译
2. **写程序**：用记事本/VS Code 写一个 `.fc` 文件（比如 `hello.fc`）
3. **运行**：命令行输入 `fcl hello.fc`

看不懂命令行？没关系，最简单的方式：把 `.fc` 文件和 `fcl.exe` 放同一个文件夹，在文件夹地址栏输入 `cmd` 回车，然后输入：

```
fcl hello.fc
```

### 两种运行模式（v2.4，先记住"默认不用等"）

FCL 有两种运行模式，区别只有一点：**要不要等生态"真的花时间"**：

| 模式 | 怎么开 | 行为 |
|---|---|---|
| **代码模式**（默认） | 什么都不写 | 高速计算，不等待（溢出惩罚/分解耗时全部跳过）|
| **真实模式** | 程序首行写 `REAL MODE ;` 或命令行 `fcl --real` | 模拟真实生态节奏：胃溃疡溢出等 2 秒、分解者工作 0.1~1 秒 |

**初学者用默认（代码模式）就好**——算错了不会罚你等 2 秒。学懂了想体验"生态沉浸感"，再开真实模式。

## 0.3 ASCII 编码是什么？（必读！不看这课你会被 FCL 的输出搞懵）

计算机不认识字母和数字，它只认识**数字**。所以每个字符（字母、数字、符号）都被编了一个号码，这套号码表叫 **ASCII**（读作"阿斯克"）：

| 数字 | 字符 | 说明 |
|---|---|---|
| 48 | `0` | 数字零 |
| 49 | `1` | 数字一 |
| 55 | `7` | 数字七 |
| 65 | `A` | 大写 A |
| 97 | `a` | 小写 a |
| 120 | `x` | 小写 x |
| 13 | （回车） | 换行符（看不见） |
| 8 | （退格） | 退格符（看不见） |

**FCL 默认的输出方式（编码模式）就是"按 ASCII 吐字符"**：一个变量的能量是 65，FCL 吐出来的不是"65"，而是字符 `A`。能量是 55，吐出来的是 `7`。能量是 8，吐出来的是**退格符**（屏幕上显示成乱码！）。

这就是 FCL 的怪脾气：**它是深奥语言，输出故意不直说**。好消息是——FCL 从 v2.2 起支持**数值输出模式**：程序第一行写上 `NUMERIC OUTPUT ;`，能量 55 就直接打印 `55`，不绕弯子。

> 教学建议：**第一遍学的时候，用数值模式**（`NUMERIC OUTPUT ;`）看真实结果；学懂了 FCL 的编码脾气后，再体验编码模式的艺术感。

## 0.4 遇到报错怎么办？

FCL 的报错长这样：

```
[FCL-0001 @第2行] 🌿 变异物种入侵，语法免疫系统失效
```

`FCL-0001` 是错误码，`@第2行` 告诉你出错的行号，后面的中文是"生态化"的错误描述。完整错误码表见[附录 B](#附录-b-错误码表)。

最常见的初学者报错：
- **🌿 外来物种入侵**：变量名用了生态圈外的物种（比如 `Dragon_1`）→ 去[第 2 课](#第-2-课变量是物种)查在册名单
- **🦴 食性冲突**：营养级不对（狼直接吃草）→ 去[第 3 课](#第-3-课捕食即运算)
- **🌍 生态崩溃**：三段式结构不完整 → 去[第 1 课](#第-1-课第一个生态程序)

---

# 第 1 课：第一个生态程序

> 🎯 学习目标：理解三段式结构、运行你的第一个 FCL 程序、学会看输出对照表。
> 🌱 生态小课堂：真实生态系统中，生产者（植物）固定太阳能 → 消费者（动物）传递能量 → 分解者（真菌细菌）把遗体还原为无机物。三者分工明确，缺一不可。

## 1.1 代码与逐行讲解

```foodchain
GMO ENABLED ;                                    # ← 第 1 行：启动"转基因"模式
BIOME {                                          # ← 第 2 行：BIOME 段开始（引种）
INTRODUCE Grass_1 AS PRODUCER WITH 60+5 ;        # ← 引种一株草，能量 60+5=65
INTRODUCE Sheep_M1 AS HERBIVORE WITH 0 ;         # ← 引种一只雄性羊，初始能量 0
}                                                # ← BIOME 段结束
FOODWEB {                                        # ← 第 5 行：FOODWEB 段开始（捕食）
Sheep_M1 DEVOURS Grass_1 USING SUM ;             # ← 羊吃掉草，能量 = 0+65 = 65
}                                                # ← FOODWEB 段结束
DECAY {                                          # ← DECAY 段开始（分解）
INTRODUCE Fungus_1 AS DECOMPOSER WITH 0 ;        # ← 引种一丛真菌
Fungus_1 DEVOURS Sheep_M1 USING SUM ;            # ← 真菌回收羊的尸体：65
ROT Fungus_1 TO STDOUT ;                         # ← 第一次输出
ROT Fungus_1 TO STDOUT ;                         # ← 第二次输出
}                                                # ← DECAY 段结束
```

**逐行翻译成人话**：
- `GMO ENABLED ;`：正常捕食会损耗 80% 能量（生态税），开 GMO 后 100% 传递（代价是输出带 🧬 标识）
- `INTRODUCE Grass_1 AS PRODUCER WITH 60+5 ;`：造一个变量叫 `Grass_1`，类型是生产者（草），初始值 60+5=65
- `Sheep_M1 DEVOURS Grass_1 USING SUM ;`：羊把草吃了，羊的能量变成 0+65=65，草的能量变成 0（被消化）
- `ROT Fungus_1 TO STDOUT ;`：真菌"矿化"能量——把 65 交给屏幕

## 1.2 输出对照表

运行 `fcl examples/example3.fc`：

| 屏幕上显示的 | 真实含义 |
|---|---|
| `🧬` | GMO 转基因标识（可忽略） |
| `A` | 能量 65 对应的 ASCII 字符（**65 = 'A'**） |
| `U+0041` | 65 的十六进制写法（0x41 = 65） |

**关键点**：FCL 输出的是"字符"，不是数字。要显示 `A` 就得让能量等于 65；想显示数字 `2` 就得让能量等于 50（因为 ASCII '2'=50）。如果加一行 `NUMERIC OUTPUT ;`，能量 65 会直接打印 `65`。

## 1.3 三段式规则（背下来）

```
BIOME   { ... }   ← 引种段：声明变量（只能放声明）
FOODWEB { ... }   ← 捕食段：核心运算（必须至少有 1 次 DEVOURS）
DECAY   { ... }   ← 分解段：输出与回收
```

三段**必须齐全、顺序不能颠倒**，违反 → `🌍 生态崩溃，食物链断裂！`

## ✏️ 练习

1. 把 `60+5` 改成 `61+5`，运行看输出变成了什么字符？（提示：66 = 'B'）
2. 在程序第一行加 `NUMERIC OUTPUT ;`，再看输出——是不是直接显示数字了？

---

# 第 2 课：变量是物种

> 🎯 学习目标：掌握生态圈 10 个在册物种和命名规则（这是 FCL 最容易报错的地方，请仔细看）。
> 🌱 生态小课堂：生态学把生物按"吃与被吃"分成**营养级**：生产者（第 1 级）→ 食草动物（第 2 级）→ 食肉动物（第 3 级）→ 顶级掠食者（第 4 级）。越往上数量越少、能量越少，这就是**能量金字塔**。分解者（第 5 级）不在这条链上，而是"兜底"分解一切。

## 2.1 在册物种名单（只能用它命名变量！）

| 营养级 | 类型关键字 | 物种 | 族群结构 |
|---|---|---|---|
| 1 生产者 | PRODUCER | Grass 草、Algae 藻 | 种群（Grass_1、Algae_2） |
| 2 食草 | HERBIVORE | Sheep 羊、Rabbit 兔 | 羊群居 / 兔独居 |
| 3 食肉 | CARNIVORE | Wolf 狼、Fox 狐 | 狼群居 / 狐独居 |
| 4 顶级 | APEX | Tiger 虎、Lion 狮 | 虎独居 / 狮群居 |
| 5 分解 | DECOMPOSER | Fungus 真菌、Bacillus 杆菌 | 菌落 |

## 2.2 命名格式（三种）

```
群居物种（羊/狼/狮）：
  首领：  Alpha_<物种>       例：Alpha_Wolf（狼王）
  成员：  <物种>_<M|F><编号>  例：Wolf_M1（1号雄性狼）、Sheep_F2（2号雌性羊）

独居物种（兔/狐/虎）：
  <物种>_<编号>              例：Tiger_1（1号虎）

生产者/分解者（草/藻/真菌/杆菌）：
  <物种>_<编号>              例：Grass_1（1号草丛）、Fungus_1（1号菌落）
```

## 2.3 三个经典错误（考试重点！）

```foodchain
INTRODUCE Dragon_1 AS PRODUCER WITH 1 ;   // ❌ 龙不在册 → 🌿 外来物种入侵
INTRODUCE Wolf_M1 AS PRODUCER WITH 1 ;    // ❌ 狼是食肉动物，不是生产者 → ⚠️ 分类学混乱
INTRODUCE Wolf_1 AS CARNIVORE WITH 1 ;    // ❌ 狼是群居，编号要带 M/F → ⚠️ 族谱登记混乱
```

记忆口诀：**名字 = 物种 + 身份**。群居要有首领或性别，独居只带编号，营养级必须匹配。

## ✏️ 练习

1. 下面哪些名字合法？`Rabbit_1`、`Alpha_Tiger`、`Fox_F1`、`Sheep_5`、`Bacillus_2`
2. 写出：1 号雌性狮、狼王、2 号草丛的正确名字。

（答案：`Rabbit_1`✓、`Alpha_Tiger`✗（虎独居没首领）、`Fox_F1`✗（狐独居不带性别）、`Sheep_5`✗（群居要 M/F）、`Bacillus_2`✓；`Lion_F1`、`Alpha_Wolf`、`Grass_2`）

---

# 第 3 课：捕食即运算

> 🎯 学习目标：理解 FCL 的加减乘除（全是捕食）、能量税、单向流动。
> 🌱 生态小课堂：生态学家**林德曼**发现，能量沿食物链每传递一级，只有 10%~20% 能到下一级，其余 80%~90% 通过呼吸散失、变成尸体被分解。这就是**林德曼定律**，也叫十分之一定律。推论一：能量**单向流动**（散失的热收不回）；推论二：能量**逐级递减**（所以食物链一般只有 4~5 级）。

## 3.1 四种"算法"都是捕食

| FCL 写法 | 含义 | 营养级要求 |
|---|---|---|
| `A DEVOURS B USING SUM` | A = A + B | A 比 B 高 1 级 |
| `A DEVOURS B USING DIFF` | A = A − B（大减小） | 同上 |
| `A DEVOURS B USING PROD` | A = A × B | 仅 APEX 可用 |
| `A DEVOURS B USING QUOT` | A = A ÷ B（余数丢弃） | 仅 APEX 可用 |

## 3.2 能量税（默认 20%）

```foodchain
// 羊吃草：草的能量 65
Sheep_M1 DEVOURS Grass_1 USING SUM ;
// 实际羊只获得 65 × 20% = 13（其余 52 点"呼吸散失"了）
```

不想扣税？程序首行加 `GMO ENABLED ;`（转基因），但输出会带 🧬 标识。

## 3.3 猎物被吃光（单向流动）

捕食后**猎物的能量归零**——它被消化了。这是 FCL 和普通语言最大的不同：

```python
x = 5; y = 3; x = x + y   # Python：y 还是 3
```
```foodchain
# FCL：羊吃掉草后，草的能量变成 0（回不来了！）
# 除非用 CLONE 复制（第 5 课）
```

## 3.4 输出对照

算 3+5=8（`examples/example1.fc`）：

| 屏幕上 | 真实含义 |
|---|---|
| `🧬` + 退格符 | ASCII 8 = 退格（**屏幕上显示成乱码**） |
| `U+0008` | 8 的十六进制 |

**这就是为什么教学书建议你开 `NUMERIC OUTPUT ;`**——否则算个 8 屏幕显示乱码，初学者会一脸懵 😅

## ✏️ 练习

1. 羊能量 100，草能量 50，SUM 捕食后（无 GMO）羊变成多少？（答案：30，因为 (100+50)×0.2=30）
2. 为什么 FCL 的加法会"损耗"？用林德曼定律解释。

---

# 第 4 课：分解者与输出

> 🎯 学习目标：理解"只有分解者能输出"、编码模式 vs 数值模式、ROT 两次的含义。
> 🌱 生态小课堂：分解者（细菌、真菌、蚯蚓）把动植物遗体、粪便中的有机物分解成无机物（CO₂、水、无机盐），归还给土壤——所以又叫**还原者**。没有它们，养分被锁在尸体里，生产者会饿死。分解者是物质循环的关键。

## 4.1 为什么只有分解者能输出？

生态学逻辑：能量最终要"矿化回归环境"，干这活的是分解者。所以 FCL 里**只有 DECOMPOSER 能执行 ROT**。草、羊、狼想输出？把能量交给真菌。

```foodchain
DECAY {
INTRODUCE Fungus_1 AS DECOMPOSER WITH 0 ;
Fungus_1 DEVOURS Sheep_M1 USING SUM ;   // 分解者豁免：吃任意营养级，100% 回收
ROT Fungus_1 TO STDOUT ;                // 输出
}
```

**分解者豁免条款**：在 DECAY 块内，分解者可以吃**任意营养级**（差几级都行）且**不扣税**。

## 4.2 两种输出模式（v2.2 新特性）

| 模式 | 怎么开 | ROT 输出 | 例子（能量 55） |
|---|---|---|---|
| 编码模式（默认） | 什么都不写 | 第一次：ASCII 字符；第二次：U+XXXX | `7` 和 `U+0037` |
| **数值模式** | 首行加 `NUMERIC OUTPUT ;` | 直接打印数字 | `55` |

**初学者强烈建议开数值模式**：

```foodchain
GMO ENABLED ;
NUMERIC OUTPUT ;          # ← 就是这一行
BIOME { ... }
```

## 4.3 输出对照表（重点记忆）

| 能量 | 编码模式显示 | 数值模式显示 |
|---|---|---|
| 8 | 退格符（乱码！）+ U+0008 | `8` |
| 13 | 回车符（看不见）+ U+000D | `13` |
| 48 | `0` + U+0030 | `48` |
| 55 | `7` + U+0037 | `55` |
| 65 | `A` + U+0041 | `65` |
| 120 | `x` + U+0078 | `120` |

## ✏️ 练习

1. 能量 72 在编码模式显示什么字符？（提示：ASCII 72 = 'H'）
2. 把 `examples/math_triangular.fc` 加上 `NUMERIC OUTPUT ;`，看输出变成什么。

---

# 第 5 课：复制与物质循环（CLONE）

> 🎯 学习目标：理解 CLONE（无性繁殖）——FCL 里唯一的"复制"手段。
> 🌱 生态小课堂：生态学区分**能量流动**（单向、逐级递减、像汽油烧了就没）和**物质循环**（元素在生物与环境间反复循环、像钱流通）。健康生态系统必须同时满足：能量单向 + 物质循环。FCL 的 DEVOURS 是能量流动（单向扣税），CLONE 是物质循环（复制回低营养级）。

## 5.1 CLONE 语法

```foodchain
CLONE Algae_1 FROM Grass_2 ;   // 藻类"克隆"草的能量：Algae_1 = Grass_2
```

- 目标获得源的**副本**，源不受影响
- 目标必须已经 INTRODUCE 过（不然不知道它是什么类型）
- 生态依据：植物扦插、克隆技术

## 5.2 实战：斐波那契（第一个算法）

斐波那契数列：1, 1, 2, 3, 5, 8, 13... 每一项 = 前两项之和。

```foodchain
GMO ENABLED ;
BIOME {
INTRODUCE Grass_1 AS PRODUCER WITH 1 ;     // 前前项 F(n-1)
INTRODUCE Grass_2 AS PRODUCER WITH 1 ;     // 前项 F(n)
INTRODUCE Sheep_M1 AS HERBIVORE WITH 0 ;   // 累加器
INTRODUCE Algae_1 AS PRODUCER WITH 0 ;     // 备份
INTRODUCE Fungus_1 AS DECOMPOSER WITH 0 ;
}
FOODWEB {
MIGRATION Sheep_M1 OVER 5 {                // 循环 5 轮
CLONE Algae_1 FROM Grass_2 ;          // ① 备份 F(n) 到藻（否则草2被吃就丢了）
Sheep_M1 DEVOURS Grass_1 USING SUM ;  // ② 累加 F(n-1)
Sheep_M1 DEVOURS Grass_2 USING SUM ;  // ③ 累加 F(n) → 得到 F(n+1)
CLONE Grass_1 FROM Algae_1 ;          // ④ 草1 = 旧草2（前项前移）
CLONE Grass_2 FROM Sheep_M1 ;         // ⑤ 草2 = 新和（下一轮的后项）
INTRODUCE Sheep_M1 AS HERBIVORE WITH 0 ;  // 重置累加器
INTRODUCE Algae_1 AS PRODUCER WITH 0 ;    // 重置备份
}
}
DECAY {
Fungus_1 DEVOURS Grass_2 USING SUM ;
ROT Fungus_1 TO STDOUT ;
ROT Fungus_1 TO STDOUT ;
}
```

**逐轮推演**（每轮结束时的状态）：

| 轮次 | Grass_1 | Grass_2 | 含义 |
|---|---|---|---|
| 初始 | 1 | 1 | F(1), F(2) |
| 轮 1 | 1 | 2 | F(2), F(3) |
| 轮 2 | 2 | 3 | F(3), F(4) |
| 轮 3 | 3 | 5 | F(4), F(5) |
| 轮 4 | 5 | 8 | F(5), F(6) |
| 轮 5 | 8 | **13** | F(6), F(7) |

## 5.3 输出对照

| 模式 | 屏幕 | 含义 |
|---|---|---|
| 编码模式 | `🧬` + 回车符 + `U+000D` | ASCII 13 = 回车（看不见）|
| 数值模式 | `🧬13` | **F(7) = 13**，一目了然 |

## ✏️ 练习

1. 把 `MIGRATION ... OVER 5` 改成 OVER 8，用数值模式看结果（提示：F(10)=55）。
2. 为什么第 ① 步必须 CLONE？不备份会怎样？（提示：羊会把草2吃光）

---

# 第 6 课：比较与布尔逻辑

> 🎯 学习目标：ASSESS（比较）+ 三种布尔运算（AND/OR/NOT）。
> 🌱 生态小课堂：**生态位**是一个物种的"职业"（吃什么、住哪、何时活动）。两个物种生态位越接近竞争越激烈。**优势种**是群落中占主导的物种——判定谁占优势，就是比较。而"裁决权"在真实生态里属于顶级掠食者：它们没有天敌，是食物链的最高层。

## 6.1 比较：ASSESS（生态位评估）

```foodchain
ASSESS Wolf_M1 AGAINST Sheep_M1 TO Lion_M1 ;
```

- 比较 A（狼）和 B（羊）的能量：A ≥ B？
- 结果存进 C（**必须是 APEX**——只有顶级掠食者有裁决权）
- FULL = 1（赢），HUNGRY = 0（输）

## 6.2 布尔三件套

```foodchain
SYMBIOSIS  Lion_M1 WITH Lion_F1 TO Tiger_1 ;   // AND：两个都 FULL 才 FULL
COMPETITION Lion_M1 OR  Lion_F1 TO Tiger_1 ;   // OR：任一 FULL 即 FULL
MIMICRY    Lion_F1 TO Tiger_1 ;                // NOT：颠倒（FULL↔HUNGRY）
```

**真值表**（布尔运算的标准规则）：

| A | B | AND | OR |
|---|---|---|---|
| 0 | 0 | 0 | 0 |
| 0 | 1 | 0 | 1 |
| 1 | 0 | 0 | 1 |
| 1 | 1 | 1 | 1 |

| A | NOT A |
|---|---|
| 0 | 1 |
| 1 | 0 |

## 6.3 输出对照

运行 `examples/eco_logic.fc`：

| 屏幕 | 含义 |
|---|---|
| `🧭 生态位评估：Wolf_M1(65) vs Sheep_M1(0) → Lion_M1 FULL` | 狼 65 ≥ 羊 0，判定成功 |
| `\x01` + `U+0001` | 最终布尔值 1（FULL）|

## ✏️ 练习

1. ASSESS 的结果必须存进什么类型的变量？为什么？
2. 用 SYMBIOSIS/COMPETITION/MIMICRY 各写一行，算 `1 AND 0`、`1 OR 0`、`NOT 1`。

---

# 第 7 课：生态化控制流

> 🎯 学习目标：FCL 没有 if/for/while——用天气、迁徙、冬眠、变异实现控制流。
> 🌱 生态小课堂：生物的命运由**环境因子**驱动：季节降水决定植被（SEASON）、角马为食物长途迁徙消耗巨大（MIGRATION）、熊冬天降低代谢躲过饥荒（HIBERNATION）、基因突变随机不可预测（MUTATION）。大自然没有 if/for，但它有这些机制。

## 7.1 条件分支：SEASON（看天气）

```foodchain
SEASON RAIN { ... } DRY { ... }
```

- **湿度** = 最近 3 条指令里 DEVOURS 的次数，≥2 → RAIN，否则 DRY（二选一）
- **STORM 是显式开关**（v2.2）：默认 RAIN 超过 3 条语句**不乱序**（保证确定性）；首行 `STORM ENABLED ;` 才启用乱序

## 7.2 定次循环：MIGRATION（迁徙）

```foodchain
MIGRATION Sheep_M1 OVER 5 { ... }
```

循环 5 次。**副作用**：每轮后该物种能量开平方根（迁徙消耗）——100→10→3.16→1.78。想循环又不想能量被开方？选一个每轮会被重置的变量当"迁徙者"（像斐波那契里用 Sheep_M1）。

## 7.3 条件循环：HIBERNATION（冬眠）

```foodchain
HIBERNATION Fungus_1 UNTIL Tiger_1 { ... }
```

循环直到 Tiger_1 为 FULL（1）。**副作用**：每轮该物种能量 ×0.9（冬眠代谢）。超过 10000 轮强制唤醒（防死循环）。

## 7.4 多路选择：MUTATION（变异）

```foodchain
MUTATION Wolf_M1 { CASE "长毛": ... }
```

运行时 Wolf_M1 有 1/3 概率变异成 Wolv_M1（10 个物种都有变异名）。可用 `MATCH(Wolf_M1)` 在表达式中检测是否变异（变异过返回 1）。

## ✏️ 练习

1. 湿度队列是 `[吃, 吃, 声明]`，会执行 RAIN 还是 DRY？（答案：RAIN，2 次捕食 ≥2）
2. 为什么 MIGRATION 的循环变量要选"会被重置"的？（提示：sqrt 副作用）

---

# 第 8 课：生态圈与社会结构

> 🎯 学习目标：独居 vs 群居的命名规则与生态学原因。
> 🌱 生态小课堂：**独居**（虎、狐、兔）：领域资源有限，独居减少竞争——老虎领地可达几十平方公里。**群居**（狼、狮、羊）：团队协作捕猎大型猎物——狼群有严格等级制度，狼王（Alpha）统一调遣，像军队一样协同作战。有趣：猫科大多独居，**狮子是唯一群居的猫科**。

## 8.1 命名对照

```foodchain
// 狼群（群居）：
INTRODUCE Alpha_Wolf AS CARNIVORE WITH 50 ;   // 狼王
INTRODUCE Wolf_M1  AS CARNIVORE WITH 20 ;     // 1号雄性
INTRODUCE Wolf_F1  AS CARNIVORE WITH 20 ;     // 1号雌性

// 虎（独居）：
INTRODUCE Tiger_1  AS APEX WITH 80 ;          // 1号虎
```

**一个变量名 = 一份完整的族群档案**：物种、等级、性别、编号全在里面。

## ✏️ 练习

写出一个完整狮群：狮王 + 2 号雄狮 + 3 号雌狮。

（答案：`Alpha_Lion`、`Lion_M2`、`Lion_F3`）

---

# 第 9 课：垃圾回收（生态分解）

> 🎯 学习目标：FCL 的内存管理是"看得见的生态事件"。
> 🌱 生态小课堂：分解者是生态系统的**清洁工兼回收站**：清除尸体粪便、把有机物还原成无机物。关键区别：**分解者不攻击活物**——只分解已经死亡的有机体。

## 9.1 规则（背下来）

```
1. 活体豁免：能量 >0 的变量永远不会被回收（活着的生物不被分解）
2. 尸体分解：能量 =0 且连续 3 条指令没人碰 → 🍄 回收
3. 活跃期暂停：BIOME 引种、复合块执行期间不回收
4. 顶级豁免：APEX 永不自动回收
5. 手动灭绝：EXTINCTION 立即回收 + 打印内存遗照
```

运行时会看到 `🍄 Grass_1 被分解者回收`——这就是 GC 在工作。

## 9.2 手动回收

```foodchain
EXTINCTION Wolf_M1 ;   // 立即回收，打印 16 行十六进制"遗照"
```

## ✏️ 练习

一个变量能量 100（活体）闲置 10 条指令，会被回收吗？（答案：不会，活体豁免）

---

# 第 10 课：图灵完备——FCL 能算一切吗？

> 🎯 学习目标：理解"图灵完备"概念，知道 FCL 的极限。
> 🌱 生态小课堂：能量金字塔——每级能量约为下一级的 10%~20%，所以金字塔越往上越窄：生产者最多，顶级掠食者最少。FCL 的类型系统也在模仿它：PRODUCER 数值范围最大（9999.9），APEX 只有 0/1。

## 10.1 图灵完备三件套

| 能力 | FCL 对应 | 普通语言对应 |
|---|---|---|
| 比较 | ASSESS | `>`、`==` |
| 条件循环 | HIBERNATION | `while` |
| 无限存储 | INTRODUCE（无限变量） | 变量 |

**三件套齐了 = 图灵完备 = 理论上能算任何可计算问题**（素数、公约数、任何算法都能写）。

## 10.2 1+1 终局对比

Python：`print(1 + 1)` → `2`（13 个字符）
FCL：一场完整的生态剧（引种两株草 → 羊吃掉 → 真菌矿化），而且默认输出是 ASCII 控制字符（^B）！

```foodchain
// 数值模式下的 FCL 1+1：
GMO ENABLED ;
NUMERIC OUTPUT ;
BIOME {
INTRODUCE Grass_1 AS PRODUCER WITH 1 ;
INTRODUCE Grass_2 AS PRODUCER WITH 1 ;
INTRODUCE Sheep_M1 AS HERBIVORE WITH 0 ;
INTRODUCE Fungus_1 AS DECOMPOSER WITH 0 ;
}
FOODWEB {
Sheep_M1 DEVOURS Grass_1 USING SUM ;
Sheep_M1 DEVOURS Grass_2 USING SUM ;
}
DECAY {
Fungus_1 DEVOURS Sheep_M1 USING SUM ;
ROT Fungus_1 TO STDOUT ;
}
```

| 模式 | 输出 |
|---|---|
| 数值模式 | `🧬2` |
| 编码模式 | `🧬^B`（ASCII 2 = 控制字符）|

## 10.3 学完这 10 课，你掌握了什么？

1. 营养级类型系统（数据是生态位，决定能吃什么、被谁吃）
2. 单向能量流（赋值可能是破坏性的，除非 CLONE）
3. 效率是生态代价（20% 税 = 林德曼定律）
4. 输出分工制（只有分解者能打印；编码/数值两种模式）
5. 名字即生态位（命名承载族群、性别、地位）
6. 不确定性（天气、变异——算法要容忍随机）
7. 显式回收（GC 是看得见的生态过程）
8. 图灵完备（约束再怪，计算能力不打折）

---

# 第 11 课：实战项目一——阶乘（5! = 120）

> 🎯 用 FCL 算 5! = 5×4×3×2×1 = 120。项目文件：`examples/math_factorial.fc`

## 11.1 思路

乘法 = APEX 的 PROD 捕食。让一只虎把因子 2、3、4、5 依次"吞进肚子"（相乘），最后交给真菌输出。

| 角色 | 物种 | 说明 |
|---|---|---|
| 累乘器 | Tiger_1（APEX） | 初始 1，PROD 连乘 |
| 因子搬运工 | Fox_1（CARNIVORE） | 从狼身上克隆因子，被虎吃掉 |
| 因子递增源 | Wolf_M1（CARNIVORE） | 从 2 开始每轮 +1 |
| 递增饲料 | Sheep_M1（HERBIVORE） | 能量 1，喂狼实现 +1 |
| 输出者 | Fungus_1 | 回收并输出 |

**为什么需要狐狸搬运？** 虎吃猎物会把它吃光（归零）。如果虎直接吃狼，因子源就没了。所以先 `CLONE Fox_1 FROM Wolf_M1`，虎吃狐狸，狼安然无恙——这是 FCL 版的"不要把下蛋的鸡吃掉"。

## 11.2 代码与推演

```foodchain
GMO ENABLED ;
NUMERIC OUTPUT ;
BIOME {
INTRODUCE Tiger_1 AS APEX WITH 1 ;
INTRODUCE Fox_1 AS CARNIVORE WITH 0 ;
INTRODUCE Wolf_M1 AS CARNIVORE WITH 2 ;
INTRODUCE Sheep_M1 AS HERBIVORE WITH 1 ;
INTRODUCE Fungus_1 AS DECOMPOSER WITH 0 ;
}
FOODWEB {
MIGRATION Sheep_M1 OVER 4 {
CLONE Fox_1 FROM Wolf_M1 ;
Tiger_1 DEVOURS Fox_1 USING PROD ;
INTRODUCE Sheep_M1 AS HERBIVORE WITH 1 ;
Wolf_M1 DEVOURS Sheep_M1 USING SUM ;
}
}
DECAY {
Fungus_1 DEVOURS Tiger_1 USING SUM ;
ROT Fungus_1 TO STDOUT ;
}
```

| 轮次 | 虎（阶乘） | 狼（因子） |
|---|---|---|
| 初始 | 1 | 2 |
| 轮 1 | 1×2=2 | 3 |
| 轮 2 | 2×3=6 | 4 |
| 轮 3 | 6×4=24 | 5 |
| 轮 4 | 24×5=**120** | 6 |

## 11.3 输出对照

| 模式 | 输出 | 含义 |
|---|---|---|
| 数值模式 | `🧬120` | 5! = 120，直白 |
| 编码模式 | `🧬x` + `U+0078` | ASCII 120 = 'x' |

---

# 第 12 课：实战项目二——三角数（1+2+…+10 = 55）

> 🎯 用 FCL 算 1+2+3+…+10 = 55。项目文件：`examples/math_triangular.fc`（编码）与 `math_triangular_num.fc`（数值）。

## 12.1 思路

加法 = SUM 捕食。让一只羊把 10 株能量分别为 1~10 的草全部吃掉，最后交给真菌输出。

**为什么用 10 株显式的草而不是循环？** 因为草（生产者）是营养级最低端，**无法自己增长**——它的能量只能来自 INTRODUCE 常量。这恰好符合现实：植物不能吃别的生物。所以 FCL 里"数列求和"最直白的写法，就是**把 10 株草喂给一只羊**。

## 12.2 代码

```foodchain
GMO ENABLED ;
NUMERIC OUTPUT ;
BIOME {
INTRODUCE Sheep_M1 AS HERBIVORE WITH 0 ;
INTRODUCE Grass_1 AS PRODUCER WITH 1 ;
INTRODUCE Grass_2 AS PRODUCER WITH 2 ;
INTRODUCE Grass_3 AS PRODUCER WITH 3 ;
INTRODUCE Grass_4 AS PRODUCER WITH 4 ;
INTRODUCE Grass_5 AS PRODUCER WITH 5 ;
INTRODUCE Grass_6 AS PRODUCER WITH 6 ;
INTRODUCE Grass_7 AS PRODUCER WITH 7 ;
INTRODUCE Grass_8 AS PRODUCER WITH 8 ;
INTRODUCE Grass_9 AS PRODUCER WITH 9 ;
INTRODUCE Grass_10 AS PRODUCER WITH 10 ;
}
FOODWEB {
Sheep_M1 DEVOURS Grass_1 USING SUM ;
Sheep_M1 DEVOURS Grass_2 USING SUM ;
Sheep_M1 DEVOURS Grass_3 USING SUM ;
Sheep_M1 DEVOURS Grass_4 USING SUM ;
Sheep_M1 DEVOURS Grass_5 USING SUM ;
Sheep_M1 DEVOURS Grass_6 USING SUM ;
Sheep_M1 DEVOURS Grass_7 USING SUM ;
Sheep_M1 DEVOURS Grass_8 USING SUM ;
Sheep_M1 DEVOURS Grass_9 USING SUM ;
Sheep_M1 DEVOURS Grass_10 USING SUM ;
}
DECAY {
INTRODUCE Fungus_1 AS DECOMPOSER WITH 0 ;
Fungus_1 DEVOURS Sheep_M1 USING SUM ;
ROT Fungus_1 TO STDOUT ;
}
```

## 12.3 输出对照（天选之数！）

| 模式 | 输出 | 含义 |
|---|---|---|
| 数值模式 | `🧬55` | 1+2+…+10 = 55 |
| 编码模式 | `🧬7` + `U+0037` | **55 恰好是字符 '7' 的 ASCII**——屏幕上真的显示 7！|

> 这就是 FCL 的浪漫：编码模式下，数学算出来的数字直接变成你看到的字符。想显示数字字符 '7'？你得算出 55 来。

---

# 附录 A：ASCII 速查表（常用）

| 数字 | 字符 | 数字 | 字符 |
|---|---|---|---|
| 32 | 空格 | 65 | A |
| 48 | 0 | 66 | B |
| 49 | 1 | 72 | H |
| 50 | 2 | 97 | a |
| 55 | 7 | 120 | x |
| 13 | 回车（不可见） | 8 | 退格（不可见） |

# 附录 B：错误码表

| 错误码 | 报错 | 含义 |
|---|---|---|
| FCL-0001 | 🌿 变异物种入侵 | 语法错误 |
| FCL-0002 | 🦴 食性冲突 | 营养级不对 |
| FCL-0003 | 🌿 外来物种入侵 | 物种不在册 |
| FCL-0004 | ⚠️ 分类学混乱 | 物种类型不符 |
| FCL-0005 | ⚠️ 族谱登记混乱 | 命名格式错 |
| FCL-0006 | 🌍 生态崩溃 | 结构不完整 |
| FCL-0007 | 🔥 干旱 | 除零 |
| FCL-0011 | 🌿 表达式无法解析 | WITH 写错 |

# 附录 C：词汇表

| 术语 | 含义 |
|---|---|
| 营养级 | 生物在食物链中的层级（1 生产者 ~ 4 顶级） |
| 林德曼定律 | 能量每传递一级损耗约 80%~90% |
| 能量税 | FCL 每次捕食扣 20% 能量（模拟林德曼定律） |
| 分解者豁免 | DECAY 块内分解者吃任意级、不扣税 |
| GMO | 转基因模式：免能量税，输出带 🧬 |
| NUMERIC OUTPUT | 数值输出模式：直接打印数字 |
| 编码模式 | 默认输出：ASCII 字符 + U+XXXX |
| 图灵完备 | 能表达一切可计算问题 |

---

> 教学书 v2.4 ｜ 参考实现：[Huang-520-add/fcl](https://github.com/Huang-520-add/fcl)
> 语法速查：[FCL_REFERENCE.md](FCL_REFERENCE.md) ｜ 生态圈图鉴：[FCL_ECOLOGY.md](FCL_ECOLOGY.md)

