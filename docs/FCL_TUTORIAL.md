# FCL 官方教程（FCL Tutorial）v2.1

> **代码在吞噬中传递，真理在分解中显现。**
> 本教程供**学习**使用：从零开始，通过 12 课掌握 FCL 的全部特性与生态学思维。
> 每课都配有一小节 **🌱 生态小课堂**——用最通俗的话解释背后真实的生态学原理。
> 语法查阅：[FCL_REFERENCE.md](FCL_REFERENCE.md) ｜ 生态圈图鉴：[FCL_ECOLOGY.md](FCL_ECOLOGY.md)

---

## 写给读者：FCL 和别的解释型语言有什么不一样？

你已经会 Python？恭喜，你只带着 Python 的思维来学 FCL 是**不够的**——因为 FCL 故意把一切都反着来：

| 你熟悉的 Python | FCL | 为什么这么设计 |
|---|---|---|
| `x = 1` 随便命名 | 变量必须是**生态圈在册物种** | 名字也是生态的一部分 |
| `x + y` 直接算 | **羊吃掉草**才算加法 | 运算 = 生态事件 |
| `print(1+1)` | **分解者**吐出 ASCII 字符 | 只有分解者能"矿化"输出 |
| `if 条件:` | **看天气**（雨季/旱季） | 条件 = 环境变化 |
| `while 条件:` | **冬眠**直到春天 | 循环 = 蛰伏等待 |
| 垃圾回收是隐形的 | 分解者**只分解尸体** | 回收 = 生态分解 |

**FCL 的核心思想**：不把语言当成"表达计算的工具"，而是当成"一个会计算的生态系统"。你写的每一行代码，都是一次生态事件；你写的每个程序，都是一场生态演替。

**学完本教程你将掌握**：
1. 三段式程序结构（BIOME/FOODWEB/DECAY）
2. 营养级类型系统与单向能量流
3. 生态圈命名与族群社会结构
4. 捕食运算与能量传递税
5. 分解者输出模式（ASCII）
6. 生态化控制流（SEASON/MIGRATION/HIBERNATION/MUTATION）
7. 生态化垃圾回收
8. 图灵完备的算法思维（比较/循环/存储三件套）
9. 两个完整数学实战项目（阶乘、三角数）

> 动手准备：`g++ -std=c++17 -O2 -o fcl main.cpp` 编译参考实现，然后用 `./fcl 程序.fc` 运行示例。

---

## 第 1 课：第一个生态程序

### 传统语言的 Hello World

```python
print("Hello World")
```

### FCL 的 Hello World

FCL 里"Hello World"的第一步，是**先理解输出机制**：输出（ROT）只能由**分解者**执行，而分解者输出的是 **ASCII 字符**。所以 FCL 的 Hello World 不是打印字符串，而是**计算一个 ASCII 码**。

```foodchain
GMO ENABLED ;
BIOME {
INTRODUCE Grass_1 AS PRODUCER WITH 60+5 ;   // 能量 65
INTRODUCE Sheep_M1 AS HERBIVORE WITH 0 ;
}
FOODWEB {
Sheep_M1 DEVOURS Grass_1 USING SUM ;        // 羊吃掉草：65
}
DECAY {
INTRODUCE Fungus_1 AS DECOMPOSER WITH 0 ;
Fungus_1 DEVOURS Sheep_M1 USING SUM ;       // 真菌回收羊：65
ROT Fungus_1 TO STDOUT ;                    // 输出字符：'A'
ROT Fungus_1 TO STDOUT ;                    // 再输出：U+0041
}
```

运行：`./fcl ../examples/example3.fc` → 输出 `🧬A🧬U+0041`

### 🌱 生态小课堂：生态系统由谁组成？

真实的生态系统里，所有生物按"分工"分成三类：
- **生产者**（绿色植物、藻类）：把太阳光变成化学能——生态系统的能量源头
- **消费者**（动物）：吃别人获得能量，分食草、食肉、顶级掠食者
- **分解者**（细菌、真菌）：把动植物遗体分解成无机物，**还给土壤**

这三者缺一不可——没有生产者，能量无处来；没有分解者，尸体堆积如山、养分枯竭。FCL 的强制三段式 `BIOME → FOODWEB → DECAY` 就是在模仿这个分工：**引种（生产者）→ 捕食（消费者）→ 分解（分解者）**。

### 你学到了什么

1. **三段式结构**：`BIOME`（引种/声明）→ `FOODWEB`（捕食/运算）→ `DECAY`（分解/输出），顺序不可颠倒，缺一不可。
2. **为什么是 65**：`'A'` 的 ASCII 码是 65，而 FCL 输出的是"能量对应的字符"，所以 `60+5`。
3. **GMO ENABLED**：首行声明"转基因"，让能量传递效率从 20% 恢复到 100%（代价是输出带 🧬 标识）。
4. **ROT 两次**：第一次输出 ASCII 字符，第二次输出 Unicode 码位。

> **与传统语言对比**：Python 的 `print` 是"说出来"；FCL 的 ROT 是"分解矿化"。而且 FCL 永远不能直接 `print("A")`——你必须让一株草的能量恰好等于 65，然后让分解者把它"矿化"出来。

---

## 第 2 课：变量是物种

### 生态圈在册物种（只能用它们命名）

| 营养级 | 物种 | 族群 |
|---|---|---|
| 1 生产者 | Grass 草、Algae 藻 | 种群（Grass_1、Algae_2） |
| 2 食草 | Sheep 羊（群居）、Rabbit 兔（独居） | Alpha_Sheep、Sheep_M1、Sheep_F2、Rabbit_1 |
| 3 食肉 | Wolf 狼（群居）、Fox 狐（独居） | Alpha_Wolf、Wolf_M1、Wolf_F1、Fox_1 |
| 4 顶级 | Tiger 虎（独居）、Lion 狮（群居） | Tiger_1、Alpha_Lion、Lion_M1、Lion_F1 |
| 5 分解 | Fungus 真菌、Bacillus 杆菌 | Fungus_1、Bacillus_1 |

### 族群标签规则

- **群居物种**（羊/狼/狮）：首领用 `Alpha_`（狼王 Alpha_Wolf），成员用 `M`（雄性）/`F`（雌性）+ 编号（Wolf_M1 = 1 号雄性狼）
- **独居物种**（兔/狐/虎）：纯编号（Tiger_1 = 1 号虎）
- **生产者/分解者**：纯编号（Grass_1 = 1 号草丛）

```foodchain
INTRODUCE Alpha_Wolf AS CARNIVORE WITH 50 ;   // 狼王，食肉动物
INTRODUCE Wolf_M1 AS CARNIVORE WITH 20 ;      // 1 号雄性狼
INTRODUCE Sheep_F1 AS HERBIVORE WITH 10 ;     // 1 号雌性羊
```

### 常见错误

```foodchain
INTRODUCE Dragon_1 AS PRODUCER WITH 1 ;   // ❌ 龙不在册 → 🌿 外来物种入侵
INTRODUCE Wolf_M1 AS PRODUCER WITH 1 ;    // ❌ 狼是食肉动物 → ⚠️ 分类学混乱
INTRODUCE Wolf_1 AS CARNIVORE WITH 1 ;    // ❌ 狼是群居，必须 M/F 或 Alpha → ⚠️ 族谱登记混乱
```

### 🌱 生态小课堂：营养级与生物分类

生态学把生物按"吃与被吃"分成**营养级（trophic level）**：生产者（第 1 级）→ 食草动物（第 2 级）→ 食肉动物（第 3 级）→ 顶级掠食者（第 4 级）。这就是著名的**食物链金字塔**——越往上，生物数量越少、个体越大、能量越少。

FCL 的 5 个类型正好对应营养级，多出来的是**分解者**（第 5 级，不在金字塔"链"上，而是"兜底"分解一切）。**营养级不是随便分的**——狼是食肉动物（3 级）就是 3 级，你把它声明成生产者（1 级），生态学上就是"分类学混乱"。

> **与传统语言对比**：Python 的变量名只是"标签"，随便起；FCL 的变量名是**身份**——它决定了这个变量属于哪个营养级、能吃什么、会被谁吃。**名字即生态位。**

---

## 第 3 课：捕食即运算

### 算术的生态表达

```foodchain
Sheep_M1 DEVOURS Grass_1 USING SUM ;    // 加法：羊 += 草
Sheep_M1 DEVOURS Grass_1 USING DIFF ;   // 减法：羊 −= 草（大减小）
Alpha_Wolf DEVOURS Wolf_M1 USING PROD ; // 乘法：仅 APEX（顶级掠食者才能乘）
Alpha_Lion DEVOURS Wolf_M1 USING QUOT ; // 除法：仅 APEX，余数丢弃
```

### 三条铁律

1. **营养级恰好差 1**：羊(2) 吃草(1) ✓；狼(3) 吃羊(2) ✓；狼(3) 吃草(1) ✗（跨级）→ `🦴 食性冲突`
2. **单向流动**：猎物被吃后能量归零（被消化了）
3. **能量传递税**：捕食者实际获得 = 理论结果 × 0.2

```foodchain
// 羊吃草：65 × 0.2 = 13（只有 20% 传递到下一营养级！）
Sheep_M1 DEVOURS Grass_1 USING SUM ;
// 如果 Sheep_M1 的能量是 13，说明它吃掉了 65 能量的草
```

### 🌱 生态小课堂：林德曼定律（十分之一定律）

生态学家**林德曼**发现：能量沿着食物链流动时，**每经过一个营养级，只有大约 10%~20% 能传递到下一级**，其余 80%~90% 都通过呼吸作用散失、或成为尸体被分解者利用。这就是著名的**林德曼定律/十分之一定律**。

两个推论：
- **能量单向流动**：被呼吸散失的热能不能再被生物利用——所以能量流是单向的、不可逆的
- **能量逐级递减**：所以食物链通常只有 4~5 级，再往上能量不够维持了

FCL 的 20% 能量税就是林德曼定律的化身（取的是 10%~20% 范围的上限，这样 FCL 程序还能算得动 😄）。**你每次 DEVOURS 扣的税，就是"呼吸散失"**。想模拟真实生态？去掉 GMO，让能量严格按 20% 递减。

> **与传统语言对比**：Python 的 `x += y` 后 `y` 还在；FCL 的捕食后**猎物归零**。Python 的算术无损耗；FCL 每次运算损耗 80%（热力学第二定律）。这不是 bug，是特性——**在 FCL 里，"效率"是一种生态代价**。想让效率 100%，请开 GMO（但输出要打 🧬 标签）。

---

## 第 4 课：分解者与输出

### 为什么只有分解者能输出

生态学：能量最终要**矿化回归土壤**，而干这活的是分解者。FCL 的哲学一致——**只有 DECOMPOSER 能 ROT**。

```foodchain
DECAY {
INTRODUCE Fungus_1 AS DECOMPOSER WITH 0 ;
Fungus_1 DEVOURS Sheep_M1 USING SUM ;   // 分解者豁免：吃任意营养级，100% 回收
ROT Fungus_1 TO STDOUT ;                // 第一次：ASCII 字符
ROT Fungus_1 TO STDOUT ;                // 第二次：U+XXXX
}
```

### ASCII 是 FCL 的"字符串"

FCL 没有字符串！输出 = 数值 → ASCII 字符。这带来一个经典笑话：

```foodchain
INTRODUCE Grass_1 AS PRODUCER WITH 1 ;  // 算 1+1
INTRODUCE Grass_2 AS PRODUCER WITH 1 ;
// 羊吃掉两株草 → 能量 2 → ROT 输出 ASCII 2 = 控制字符（终端显示 ^B！）
// 想要可见的 '2'？ASCII '2' = 50 → 得算 25+25
```

### 分解者豁免条款

在 DECAY 块内，分解者可以吃**任意营养级**（差几级都行），且 **100% 回收**（不扣税）——分解者分解一切。

### 输入：光合捕获（SPROUT）

有输出就有输入。FCL 的输入叫 **SPROUT（发芽/光合捕获）**，只能注入生产者：

```foodchain
SPROUT Grass_1 FROM STDIN ;   // 草从标准输入吸收能量
```

控制台会播放 2 秒"摩斯电码"提示音，你要在声音结束前输入数值；超时则输入为 0（捕捉失败）。生态对应：**植物捕捉阳光/养分是"输入"的源头**——整个食物链的能量都从生产者的光合作用开始。测试时可以用管道喂数：`echo 65 | ./fcl 程序.fc`。

### 🌱 生态小课堂：分解者 = 生态系统的"还原者"

分解者（细菌、真菌、蚯蚓等）把动植物的**遗体、粪便中的复杂有机物**，分解成简单的无机物（二氧化碳、水、无机盐），归还给土壤——所以它们又叫**还原者**。

为什么分解者不可或缺？如果没有它们：死掉的动植物不会腐烂，土壤养分被"锁"在尸体里，生产者（植物）得不到养分，整个生态系统会崩溃。**分解者是物质循环的关键一环**：生产者固定养分 → 消费者传递 → 分解者归还 → 生产者再用。循环往复。

FCL 的哲学一模一样：**能量（数值）最终必须交给分解者"矿化"（输出）**——这就是为什么 ROT 只能由 DECOMPOSER 执行。而且分解者在 DECAY 块内 100% 回收（不扣税），就像真实生态中分解者不浪费任何残余能量。

> **与传统语言对比**：Python 的 `print` 想打什么打什么；FCL 的输出权在生态圈里是**分工**的——生产者和消费者（草、羊、狼）都没有输出权。想输出？把能量交给真菌。

---

## 第 5 课：复制与物质循环（CLONE）

### 为什么需要复制

还记得"猎物归零"吗？捕食是破坏性的，值只能单向流向更高营养级。但很多算法需要"回写"（把值复制回去）。生态学上，这叫**物质循环**——分解者把养分送回土壤，生产者重新利用。

```foodchain
CLONE Algae_1 FROM Grass_2 ;   // 藻类"克隆"了草的能量：Algae_1 = Grass_2
```

源不受影响，目标获得副本（无性繁殖/扦插）。

### 实战：斐波那契（FCL 的第一个算法）

```foodchain
GMO ENABLED ;
BIOME {
INTRODUCE Grass_1 AS PRODUCER WITH 1 ;     // F(n-1)
INTRODUCE Grass_2 AS PRODUCER WITH 1 ;     // F(n)
INTRODUCE Sheep_M1 AS HERBIVORE WITH 0 ;   // 累加器
INTRODUCE Algae_1 AS PRODUCER WITH 0 ;     // 备份
INTRODUCE Fungus_1 AS DECOMPOSER WITH 0 ;
}
FOODWEB {
MIGRATION Sheep_M1 OVER 5 {
CLONE Algae_1 FROM Grass_2 ;          // ① 备份 F(n)
Sheep_M1 DEVOURS Grass_1 USING SUM ;  // ② 累加 F(n-1)
Sheep_M1 DEVOURS Grass_2 USING SUM ;  // ③ 累加 F(n) → F(n+1)
CLONE Grass_1 FROM Algae_1 ;          // ④ A = 旧 B
CLONE Grass_2 FROM Sheep_M1 ;         // ⑤ B = 新和
INTRODUCE Sheep_M1 AS HERBIVORE WITH 0 ;   // 重置累加器
INTRODUCE Algae_1 AS PRODUCER WITH 0 ;     // 重置备份
}
}
DECAY {
Fungus_1 DEVOURS Grass_2 USING SUM ;
ROT Fungus_1 TO STDOUT ;
ROT Fungus_1 TO STDOUT ;
}
```

运行 `./fcl ../examples/fib.fc` → `🧬\x03🧬U+0003`（F(7)=13，ASCII 13 = 回车符）。

### 🌱 生态小课堂：能量流动 vs 物质循环

生态学有两个容易混淆的概念：
- **能量流动**：单向的，逐级递减（林德曼定律），散失的热能无法回收
- **物质循环**：循环的，元素（碳、氮、磷…）在生物与环境之间反复循环

打个比方：能量像"汽油"，烧了就没了；物质像"钱"，在生态圈里流通、反复使用。**能量必须单向，物质必须循环**——这是健康生态系统的两条铁律。

FCL 里：`DEVOURS` 是能量流动（单向、扣税）；`CLONE`（无性繁殖）+ 分解者豁免是物质循环（数值回到低营养级再利用）。**斐波那契能跑通，靠的正是物质循环**——没有 CLONE 回写，值被吃进高营养级就永远回不来。

> **传统语言对比**：Python 里 `a, b = b, a+b` 一行搞定；FCL 里你要走完"备份 → 捕食 → 克隆回写 → 重置"一整套生态流程。**复杂不是 FCL 的缺点，是它的风格**——就像自然纪录片永远比算式啰嗦。

---

## 第 6 课：比较与布尔逻辑

### 生态位评估（比较运算）

```foodchain
ASSESS Wolf_M1 AGAINST Sheep_M1 TO Lion_M1 ;   // 狼的能量 ≥ 羊？→ 狮判定
```

- A、B 可以是任何物种
- **C 必须是 APEX**（只有顶级掠食者有"裁决权"）
- 结果：FULL(1) / HUNGRY(0)

### 布尔三件套

```foodchain
SYMBIOSIS  Lion_M1 WITH Lion_F1 TO Tiger_1 ;   // AND：两者都 FULL 才 FULL（互利共生）
COMPETITION Lion_M1 OR  Lion_F1 TO Tiger_1 ;   // OR：任一 FULL 即 FULL（替代路径）
MIMICRY    Lion_F1 TO Tiger_1 ;                // NOT：颠倒（拟态伪装）
```

### 🌱 生态小课堂：生态位与竞争

**生态位（niche）**：一个物种在生态系统中的"职业"——吃什么、住哪、何时活动。两个物种生态位越接近，竞争越激烈（**竞争排斥原理**：完全同生态位的物种无法长期共存）。

**优势种（dominant species）**：在群落中数量、影响力最大的物种。判定"谁占优势"，就是比较谁的生态位更强势——这就是 ASSESS 的生态学含义：**评估 A 的生存竞争力是否压过 B**。

至于为什么只有顶级掠食者能"裁决"：食物链顶端的物种（虎、狮）没有天敌，是生态系统的"最高法官"。FCL 里布尔值只能存进 APEX——**判定权是生态地位，不是语法**。

> **传统语言对比**：Python 的 `>` 直接返回 True/False；FCL 的比较要"请一位顶级掠食者来裁决"，结果存在它的 FULL/HUNGRY 状态里。

---

## 第 7 课：生态化控制流

### 没有 if，只有天气（SEASON）

```foodchain
SEASON RAIN { ... } DRY { ... }
```

- **湿度** = 最近 3 条指令里 DEVOURS 的次数，≥2 湿润 → 执行 RAIN；否则 DRY
- **STORM 是显式开关**（v2.2 起）：默认 RAIN 块超过 3 条语句**不会**乱序（保证程序确定性）；只有程序首行声明 `STORM ENABLED ;` 后，RAIN 块超过 3 条语句才会**随机乱序**执行（🌩️ 天有不测风云——但你需要主动邀请风暴）

### 没有 for，只有迁徙（MIGRATION）

```foodchain
MIGRATION Sheep_M1 OVER 5 { ... }
```

固定循环 5 次，每轮后 Sheep_M1 能量**开平方根**（迁徙消耗：100 → 10 → 3.16 → 1.77…）

### 没有 while，只有冬眠（HIBERNATION）

```foodchain
HIBERNATION Fungus_1 UNTIL Tiger_1 { ... }
```

循环执行直到 Tiger_1 为 FULL，每轮 Fungus_1 能量 ×0.9（冬眠代谢 10%）。超过 10000 轮强制唤醒。

```foodchain
// 示例：狮群状态翻转，2 轮后醒（eco_hibernate.fc）
HIBERNATION Fungus_1 UNTIL Tiger_1 {
MIMICRY Lion_F1 TO Tiger_1 ;
SYMBIOSIS Lion_F1 WITH Tiger_1 TO Lion_F1 ;
}
// 轮1: Tiger=NOT Lion(1)=0 → 继续；轮2: Tiger=NOT Lion(0)=1 → 醒！
// Fungus: 100 → 90 → 81 → 输出 'Q'（ASCII 81）
```

### 没有 switch，只有变异（MUTATION）

```foodchain
MUTATION Wolf_M1 { CASE "长毛": INTRODUCE Sheep_F1 AS HERBIVORE WITH 7 ; }
```

Wolf_M1 可能随机变异成 Wolv_M1（同音异形），块内语句中的变量名随之改变。用 `MATCH(Wolf_M1)` 检测是否变异。

### 🌱 生态小课堂：环境是生态的"遥控器"

生态学告诉我们：生物的命运由**环境因子**驱动——
- **季节与降水**：旱季雨季决定植被、决定食草动物、进而决定整个食物网（SEASON）
- **迁徙**：角马大迁徙、候鸟南飞——为了食物和繁殖，动物用生命长途跋涉，消耗巨大（MIGRATION 的开方衰减）
- **冬眠/蛰伏**：熊、蛇、蛙在冬天降低代谢，躲过食物匮乏期（HIBERNATION 的 ×0.9 代谢消耗）
- **基因突变**：变异是进化的原材料，随机、不可预测（MUTATION 的随机改名）

**FCL 的控制流全部来自这些真实环境机制**——没有人工的 `if/for`，因为大自然本来就不用它们。

> **与传统语言对比**：Python 的条件是确定的、循环是精确的；FCL 的控制流**看天吃饭**——湿度、迁徙损耗、冬眠代谢、随机变异。**不确定性是生态的一部分**，写 FCL 要学会"与自然共处"。

---

## 第 8 课：生态圈与社会结构

### 独居 vs 群居

- **独居**：虎、狐、兔——各自为战，编号即可（Tiger_1）
- **群居**：狼、狮、羊——有首领（Alpha）+ 性别成员（M/F + 编号）

```foodchain
INTRODUCE Alpha_Wolf AS CARNIVORE WITH 50 ;   // 狼王
INTRODUCE Wolf_M1  AS CARNIVORE WITH 20 ;     // 1 号雄性
INTRODUCE Wolf_F1  AS CARNIVORE WITH 20 ;     // 1 号雌性
INTRODUCE Wolf_M2  AS CARNIVORE WITH 15 ;     // 2 号雄性
```

### 🌱 生态小课堂：为什么有的独居、有的群居？

- **独居的理由**：领域资源有限，独居减少竞争。老虎是典型的独居猫科——领地意识极强，一只虎的领地可达几十平方公里
- **群居的理由**：团队协作捕猎大型猎物。狼群有**严格的等级制度**：狼王（Alpha）统一调遣，像军队一样协同作战，能猎杀野牛、驼鹿这种单只狼搞不定的猎物；狮群（pride）也群居，母狮集体捕猎
- **有趣的事实**：猫科大多独居（虎、豹、猞猁），**狮子是唯一群居的猫科**；犬科大多群居（狼、豺），但狐却独居

FCL 把"群居"编码进命名规则：`Alpha_` 前缀代表首领，`M/F` 代表性别，编号代表个体。**一个变量名，就是一份完整的族群档案。**

> **传统语言对比**：Python 的列表 `[50, 20, 20, 15]` 没有身份；FCL 的 `Alpha_Wolf` 一出现，就知道这是狼群的首领，能量分配语义自然浮现。

---

## 第 9 课：垃圾回收（生态分解）

### 分解者只分解"尸体"

```foodchain
// Sheep_M1 被狼吃掉后能量归零（尸体）
// 连续 3 条指令没人碰它 → 真菌来分解：
🍄 Sheep_M1 被分解者回收
```

### 规则一览

1. **活体豁免**：能量 >0 的变量（活着的生物）永远不会被分解
2. **尸体分解**：能量 =0 且闲置 3 条指令 → 回收，随机阻塞 100~1000ms（模拟微生物速度）
3. **活跃期暂停**：BIOME（引种）、MIGRATION/SEASON/HIBERNATION/MUTATION（活跃期）不分解
4. **顶级豁免**：APEX 永不自动回收
5. **手动灭绝**：EXTINCTION 立即回收 + 打印 16 行内存遗照

```foodchain
EXTINCTION Wolf_M1 ;   // 打印遗照后立即消失
```

### 🌱 生态小课堂：分解者平时在干什么？

你可能觉得细菌、真菌不起眼，但它们是生态系统的**清洁工兼回收站**：
- 清除**尸体和粪便**（否则生态系统会被垃圾淹没）
- 把有机物**还原成无机物**（CO2、水、无机盐），让养分回到土壤
- 分解速度受环境影响——温度、湿度合适时分解快，干燥寒冷时几乎停滞（FCL 里回收时随机阻塞 100~1000ms 就是在模拟这种"微生物工作速度"）

**关键区别**：分解者不攻击活物！它们只分解**已经死亡**的有机体。所以 FCL 的 GC 只回收"尸体"（能量归零的变量），活体（能量>0）安然无恙——**分解者是清道夫，不是杀手**。

> **传统语言对比**：Python 的垃圾回收是隐形的（引用计数）；FCL 的回收是**可见的生态事件**——你会在控制台看到 `🍄 某某 被分解者回收`。内存管理不再是玄学，而是自然纪录片。

---

## 第 10 课：图灵完备——FCL 能算一切吗？

### 图灵完备三件套

| 能力 | FCL 对应 | Python 对应 |
|---|---|---|
| 比较 | ASSESS | `>`、`==` |
| 条件循环 | HIBERNATION | `while` |
| 无限存储 | INTRODUCE（无限变量） | 变量/列表 |

三件套齐了，FCL 理论上**图灵完备**——任何可计算问题都能写，包括素数、公约数、任何算法。

### 对比终局：1+1

```python
print(1 + 1)    # 2
```

```foodchain
GMO ENABLED ;
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
ROT Fungus_1 TO STDOUT ;
}
```

Python 用 13 个字符；FCL 用一场完整的生态剧。而且 FCL 输出的 2 是 **ASCII 控制字符**（终端显示 ^B），想看到可见的 '2' 得算 25+25（ASCII '2'=50）。

### 🌱 生态小课堂：能量金字塔

生态学经典的**能量金字塔**：每一营养级的能量大约是下一级的 10%~20%，所以金字塔越往上越窄——**生产者最多，顶级掠食者最少**。这也是为什么食物链一般只有 4~5 级：能量不够支撑更长的链。

FCL 的"金字塔"藏在类型系统里：PRODUCER 的数值范围最大（9999.9），到 APEX 只剩 0/1（布尔）——**越顶级的物种，能承载的"能量"越少，但"裁决权"越大**。这也提醒你：大数别塞给 APEX，它只会回答 FULL 或 HUNGRY。

### 学完 FCL 你学会了什么（概念清单）

1. **营养级与类型**：数据不是"类型"，是生态位；类型决定你能吃谁、被谁吃
2. **单向能量流**：赋值可以是破坏性的；有些值一旦传递就回不来（除非 CLONE）
3. **效率是生态代价**：20% 传递税不是误差，是林德曼定律
4. **输出是分工**：不是谁都能打印，输出权属于分解者
5. **名字即生态位**：变量命名承载族群结构、性别、社会地位
6. **不确定性**：天气、迁徙损耗、变异——算法要能容忍随机
7. **显式回收**：内存管理是看得见的生态过程
8. **图灵完备**：约束再奇怪，计算能力不打折

---

## 第 11 课：实战项目一——阶乘（5! = 120）

> 项目文件：`../examples/math_factorial.fc`

### 数学背景

阶乘：`5! = 5 × 4 × 3 × 2 × 1 = 120`。在 FCL 里，乘法 = 顶级掠食者的 PROD 捕食。所以我们要：**让一只虎把因子 2、3、4、5 依次吃进肚子里（相乘），最后交给真菌输出**。

### 生态设计思路

| 角色 | 物种 | 说明 |
|---|---|---|
| 累乘器 | `Tiger_1`（APEX） | 存阶乘结果，初始 1，用 PROD 连乘 |
| 因子搬运工 | `Fox_1`（CARNIVORE） | 每轮从狼身上克隆当前因子，被虎吃掉 |
| 因子递增源 | `Wolf_M1`（CARNIVORE） | 从 2 开始，每轮吃羊 +1 |
| 递增饲料 | `Sheep_M1`（HERBIVORE） | 能量 1，喂给狼实现 +1（每轮重新种植） |
| 输出者 | `Fungus_1`（DECOMPOSER） | 回收虎的能量，ROT 输出 |

关键技巧：
- **因子递增**：狼每轮吃一只能量 1 的羊（`Wolf_M1 DEVOURS Sheep_M1 USING SUM`）→ 2→3→4→5
- **克隆搬运**：虎不能直接吃狼（虎吃狼 = 4 级吃 3 级 ✓ 可以！）——等等，为什么用狐狸搬运？因为虎每轮 PROD 会把猎物吃光，如果直接吃狼，因子源就没了。所以先 `CLONE Fox_1 FROM Wolf_M1` 备份，虎吃狐狸，狼安然无恙
- **扑咬距离**：虎(地址0) 吃狐(地址1)，0+1=1 为奇数 → 扑咬成功不落空
- **循环**：MIGRATION 选 Sheep_M1（每轮重置，sqrt 无影响）

```foodchain
GMO ENABLED ;
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
ROT Fungus_1 TO STDOUT ;
}
```

### 逐轮推演

| 轮次 | 操作 | Tiger（阶乘） | Wolf（因子） |
|---|---|---|---|
| 初始 | — | 1 | 2 |
| 轮 1 | 虎吃狐(2) → 狼吃羊变 3 | 1×2 = **2** | 3 |
| 轮 2 | 虎吃狐(3) → 狼变 4 | 2×3 = **6** | 4 |
| 轮 3 | 虎吃狐(4) → 狼变 5 | 6×4 = **24** | 5 |
| 轮 4 | 虎吃狐(5) | 24×5 = **120** | 6 |

**结果**：`Fungus = 120` → ROT 输出 `'x'`（ASCII 120）+ `U+0078`

```
$ ./fcl ../examples/math_factorial.fc
🧬x🧬U+0078
```

> 为什么是 'x'？因为 120 恰好是字母 x 的 ASCII 码。FCL 的每个数学结果都是"字符"，这是它的怪脾气。

---

## 第 12 课：实战项目二——三角数（1+2+…+10 = 55）

> 项目文件：`../examples/math_triangular.fc`

### 数学背景

三角数：`T(10) = 1 + 2 + 3 + … + 10 = 55`。它叫"三角数"是因为可以摆成三角形：
```
1
2 2
3 3 3
...
```
FCL 里加法 = SUM 捕食。所以我们要：**让一只羊把 10 株能量分别为 1~10 的草全部吃掉（累加），最后交给真菌输出**。

### 生态设计思路

| 角色 | 物种 | 说明 |
|---|---|---|
| 累加器 | `Sheep_M1`（HERBIVORE） | 初始 0，依次吃掉 10 株草 |
| 加数 | `Grass_1` ~ `Grass_10`（PRODUCER） | 能量分别是 1、2、3…10 |
| 输出者 | `Fungus_1`（DECOMPOSER） | 回收羊的能量，ROT 输出 |

为什么用 10 株显式的草而不是循环？因为**生产者（草）是营养级最低端，无法自己增长**——草的能量只能来自 INTRODUCE 常量（这恰好符合现实：植物不能吃别的生物）。所以 FCL 里"数列求和"最直白的写法，就是**把 10 株草喂给一只羊**——能量金字塔从底部聚集。

```foodchain
GMO ENABLED ;
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
ROT Fungus_1 TO STDOUT ;
}
```

### 推演

GMO 免税下：`Sheep = 1+2+3+4+5+6+7+8+9+10 = 55` → `Fungus = 55` → ROT 输出 `'7'`（ASCII 55）+ `U+0037`

```
$ ./fcl ../examples/math_triangular.fc
🧬7🧬U+0037
```

**天选之数**：55 恰好是字符 `'7'` 的 ASCII 码——所以屏幕上真的显示出一个 7！这是 FCL 独有的浪漫：**数学算出来的数字，直接变成屏幕上你看到的字符**。想要输出 `'7'` 这个数字字符，你就得算出 55——在别的语言里 `print(7)` 就行，在 FCL 里你得先让一只羊吃掉能量为 1~10 的十株草。

---

## 练习与挑战

1. **入门**：修改 example3.fc 输出 `'B'`（提示：66 = 61+5 或 30+36）
2. **进阶**：写一个程序让狼王 Alpha_Wolf 捕食 3 只羊，输出捕食后狼群的剩余能量
3. **挑战**：把阶乘项目改成 4!（提示：MIGRATION OVER 3，初始因子不变）
4. **挑战**：把三角数项目改成 1+2+…+7 = 28（提示：删掉 3 株草）——看看 28 是什么字符
5. **高手**：用 ASSESS + HIBERNATION 实现"直到羊的能量超过狼"才停止的循环
6. **大师**：尝试实现 2 的幂（2ⁿ）：参考阶乘，但每轮因子固定为 2——想想怎么让狼每轮"复活"

> 参考实现：`/sandbox/workspace/fcl/`（main.cpp + ../examples/）
> 语法查阅：[FCL_REFERENCE.md](FCL_REFERENCE.md) ｜ 生态圈图鉴：[FCL_ECOLOGY.md](FCL_ECOLOGY.md)

文档版本：v2.1（官方教程）
最后更新：2026-08-27
