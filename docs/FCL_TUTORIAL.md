# FCL 超详细中文教学书（FCL Textbook — Chinese Edition）v3.0

> **给"零基础"学习者的大型保姆式教程。**
>
> 每一个词、每一个符号、每一步推理，我都给你掰开揉碎讲清楚。
> 你不需要任何编程基础——只要会打字、会算数、有好奇心。
>
> 本书结构：每课 = 学习目标 + 关键词拆解 + 生态类比 + 程序员视角 + 逐行代码 + 输出对照 + 练习。

---

# 🎓 第 0 课：什么是编程？什么是 FCL？

> **读完这课你将学会：**
> 1. 理解"编程"是什么（不需要记，先感受）
> 2. 理解 FCL 在所有编程语言里的特殊位置
> 3. 理解 ASCII 编码（这是 FCL 输出的核心，不懂它会被输出搞懵）
> 4. 知道如何运行 FCL 程序

---

## 0.1 什么是"编程"？

**编程 = 写一份"做事的步骤清单"，让计算机帮你执行。**

打个比方：你想让计算机帮你算"1 + 1 = ?"

```
你（用中文）：请帮我算 1 + 1
计算机：答案是 2
```

但计算机不懂中文，它只懂一种特殊的"机器语言"。所以你需要把步骤翻译成计算机能懂的形式：

```
Python 写法：    print(1 + 1)
FCL 写法：      （一套生态学写法，稍后你会看到）
```

> **关键理解**：计算机是工具，编程语言是人和计算机之间的"翻译官"。不同的翻译官有不同的风格——FCL 的风格是"生态学"。

---

## 0.2 什么是"变量"？

**变量 = 给数据起的名字，本质是一个存放数值的"盒子"。**

| 现实类比 | 编程语言对应 |
|---|---|
| 盒子上贴标签"x"，里面放 5 颗糖 | `x = 5`（变量 x 的值是 5） |
| 把盒子里的糖拿走 2 颗，剩下 3 颗 | `x = x - 2`（减法）|
| 再拿另一个盒子"y"，放 3 颗糖 | `y = 3` |

普通编程语言（Python）里，你可以给盒子起**任意名字**：
```python
x = 5
apple = 5        # 叫 apple 也可以，叫 zhangsan 也可以
```

**FCL 里的变量 = 生态盒子。** 你不能给盒子随便起名——盒子必须是生态圈里的"物种"（草、羊、狼、真菌等）。这听起来很奇怪，但等你熟悉了就发现：**它只是换了一套命名系统，本质还是变量**。

---

## 0.3 什么是 ASCII 编码？（★ 核心，必须理解）

> **这是 FCL 里最重要的前置概念。不理解 ASCII，你会觉得 FCL 的输出全是乱码。**

**计算机只认识数字，不认识字母。** 所以给每个字符编了一个数字号码，这套号码表叫 **ASCII**（读"阿-sí-kè-ī"，American Standard Code for Information Interchange，美国信息交换标准码）。

关键对应关系（必须记住）：

| 数字 | 对应的字符 | 生活中对应的意思 |
|---|---|---|
| 48 | `0` | 数字"零" |
| 49 | `1` | 数字"一" |
| 55 | `7` | 数字"七" |
| 65 | `A` | 英文字母 A |
| 66 | `B` | 英文字母 B |
| 90 | `Z` | 英文字母 Z |
| 97 | `a` | 小写字母 a |
| 120 | `x` | 小写字母 x |
| 13 | （回车符） | 换行，看不见 |
| 8 | （退格符） | 删除前一个字符，看不见 |

**FCL 的默认行为：输出的不是数字，而是数字对应的字符。**

例如：
- 能量 65 → FCL 默认输出 `A`（而不是 `65`）
- 能量 55 → FCL 默认输出 `7`（而不是 `55`）
- 能量 8 → FCL 默认输出**退格符**（屏幕上什么都不显示，或者显示成乱码！）

> 这就是为什么初学者经常被 FCL 的输出吓到——不是程序写错了，是 ASCII 的问题。

**解决方案（从 v2.2 起）：** 在程序第一行写 `NUMERIC OUTPUT ;`，FCL 就会直接打印数字，不走 ASCII 转换。

```
📌 记忆口诀：FCL 默认是"编码模式"（按字符吐），加 NUMERIC OUTPUT 变"数值模式"（直说数字）。
```

---

## 0.4 FCL 的独特之处

FCL 和其他编程语言最大的区别：

| 普通语言 | FCL |
|---|---|
| 变量名随便起（`x`、`count`、`name`） | 变量名 = 生态物种（`Grass_1`、`Wolf_M1`） |
| 加法 `x + y` | 加法 = 捕食：`Sheep_M1 DEVOURS Grass_1 USING SUM ;` |
| 乘法 `x * y` | 乘法 = 顶级捕食：`Tiger_1 DEVOURS Fox_1 USING PROD ;` |
| 输出 `print(x)` | 输出 = 分解矿化：`ROT Fungus_1 TO STDOUT ;` |
| 没有 if/for/while | 用季节（SEASON）、迁徙（MIGRATION）、冬眠（HIBERNATION）代替 |
| 内存自动回收 | 内存回收 = 生态分解（真菌吃掉尸体） |

**一句话概括 FCL**：把整个编程世界重新用生态学语言描述了一遍。它是深奥语言（Esolang），但不是玩具——它是**图灵完备**的，理论上能算任何问题。

---

## 0.5 如何运行 FCL（三种方式）

### 方式一：下载预编译版本（最简单）

去 GitHub Releases 下载对应系统的可执行文件：
- Windows → `fcl-windows-latest.exe`
- macOS → `fcl-macos-latest`
- Linux → `fcl-linux-latest`

下载后，把 `.exe` 和你的 `.fc` 程序文件放在**同一个文件夹**里，在文件夹地址栏输入 `cmd` 回车，打开黑窗口，输入：

```bash
fcl 程序名.fc
```

### 方式二：用 Web Playground（最方便，无需安装）

访问 `fcl.pages.dev`（GitHub Pages 在线版），直接在浏览器里写代码、点运行，输出结果。

### 方式三：自己编译（适合开发者）

```bash
git clone https://github.com/Huang-520-add/fcl.git
cd fcl
make build
./fcl examples/example1.fc
```

---

## 0.6 两种运行模式（先记住"默认不等待"）

| 模式 | 怎么开 | 什么时候用 |
|---|---|---|
| **代码模式**（默认） | 什么都不写 | 学语言本身，用这个——算错了不会让你等 |
| **真实模式** | 程序首行写 `REAL MODE ;` | 想体验"生态沉浸感"——模拟真实的吃、分解需要花时间 |

```
初学者 → 用代码模式（默认）
学完了想玩 → 开真实模式体验生态节奏
```

---

## 0.7 遇到报错怎么办？

FCL 的报错格式：

```
[FCL-0003 @第2行] 🌿 外来物种入侵，生态圈不予接纳！
```

拆解一下：
- `FCL-0003` = 错误码（对照查表）
- `@第2行` = 出错的代码行号（方便定位）
- `🌿 外来物种入侵...` = 生态化的错误描述（告诉你是什么错）

最常见的 3 个初学者报错：

| 报错 | 原因 | 解决 |
|---|---|---|
| 🌿 外来物种入侵 | 用了 FCL 不认识的物种名（比如 `Dragon_1`） | 去[第 2 课](#第-2-课物种图鉴)看合法名单 |
| 🦴 食性冲突 | 捕食者和猎物"吃"的关系不对（狼直接吃草，不对） | 去[第 3 课](#第-3-课捕食即运算)理解营养级 |
| 🌍 生态崩溃 | 程序缺少 BIOME/FOODWEB/DECAY 三段式 | 去[第 1 课](#第-1-课三段式结构)看正确结构 |

---

## ✏️ 第 0 课练习

1. ASCII 码 97 对应什么字符？（答案：`a`）
2. ASCII 码 50 对应什么字符？（答案：`2`）
3. FCL 默认输出数字 65 时，屏幕上显示什么？（答案：字符 `A`）
4. 想让 FCL 直接输出数字而不是字符，应该怎么做？（答案：在程序首行加 `NUMERIC OUTPUT ;`）
5. FCL 有哪三段式结构？顺序能换吗？（答案：BIOME → FOODWEB → DECAY，不能换顺序）

---

# 📗 第 1 课：三段式结构——FCL 程序的骨架

> **读完这课你将学会：**
> 1. 理解 BIOME / FOODWEB / DECAY 三段式的含义和作用
> 2. 运行你的第一个完整 FCL 程序
> 3. 理解输出对照表

---

## 1.1 三段式 = FCL 程序的"一生"

FCL 程序模拟了一个完整的**生态系统运转过程**：

```
BIOME   { ... }   ← 段1：引种。造出所有"物种"（变量），注入初始能量。
FOODWEB { ... }   ← 段2：捕食。让物种之间互相作用（运算）。
DECAY   { ... }   ← 段3：分解。分解者输出结果，回收内存。
```

**为什么要分成三段？**

因为 FCL 是"生态模拟器"，它要求你按照真实生态的逻辑组织程序：

- **BIOME（生物群系）**：生态系统开始运转前，先把所有物种放进来。相当于"开园"
- **FOODWEB（食物网）**：物种之间开始捕食、能量流动。相当于"生态系统运作"
- **DECAY（分解）**：所有生物死后，分解者把它们矿化，能量回归环境。相当于"落幕"

> **类比理解**：就像一出戏——第一幕介绍演员（BIOME），第二幕演员演戏（FOODWEB），第三幕谢幕分解（DECAY）。顺序固定，缺一不可。

---

## 1.2 第一个完整程序（逐行讲解）

先完整看一遍，然后我会一行行解释：

```foodchain
GMO ENABLED ;                              ← 【1】开头声明（可选）
BIOME {                                    ← 【2】BIOME 段开始
    INTRODUCE Grass_1 AS PRODUCER WITH 60+5 ;   ← 【3】造一株草，能量 60+5=65
    INTRODUCE Sheep_M1 AS HERBIVORE WITH 0 ;    ← 【4】造一只雄性1号羊，初始能量 0
}                                          ← 【5】BIOME 段结束
FOODWEB {                                  ← 【6】FOODWEB 段开始
    Sheep_M1 DEVOURS Grass_1 USING SUM ;    ← 【7】羊吃掉草，羊的能量 = 0+65 = 65
}                                          ← 【8】FOODWEB 段结束
DECAY {                                    ← 【9】DECAY 段开始
    INTRODUCE Fungus_1 AS DECOMPOSER WITH 0 ;    ← 【10】造一丛真菌，初始能量 0
    Fungus_1 DEVOURS Sheep_M1 USING SUM ;        ← 【11】真菌吃掉羊，能量 = 0+65 = 65
    ROT Fungus_1 TO STDOUT ;                     ← 【12】真菌输出能量
    ROT Fungus_1 TO STDOUT ;                     ← 【13】真菌再输出一次
}                                          ← 【14】DECAY 段结束
```

### 逐行翻译成人话：

**第 1 行 `GMO ENABLED ;`**
- `GMO` = 转基因（Genetically Modified Organism）
- `ENABLED` = 启用
- 意思是：启用转基因模式
- 为什么要转基因？因为正常情况下，羊吃草只能拿到 20% 的能量（林德曼定律的传递效率）。开转基因后，100% 传递，免税。
- 代价是输出时前面会多一个 🧬 标识（表示"这是转基因产品"）

**第 3 行 `INTRODUCE Grass_1 AS PRODUCER WITH 60+5 ;`**
- `INTRODUCE` = 引入（把一个新物种放进生态系统）
- `Grass_1` = 草，1 号（生态学里叫"种群编号"）
- `AS PRODUCER` = 作为生产者（营养级 1，植物）
- `WITH 60+5` = 初始能量是 60+5=65
- 整句翻译：引入一株草（Grass_1），它是生产者，初始能量 65

**第 4 行 `INTRODUCE Sheep_M1 AS HERBIVORE WITH 0 ;`**
- `Sheep_M1` = 羊，雄性 1 号（M=Male，F=Female）
- `AS HERBIVORE` = 作为食草动物（营养级 2）
- `WITH 0` = 初始能量 0（羊刚出生还没有能量）
- 整句翻译：引入一只雄性 1 号羊（Sheep_M1），它是食草动物，初始能量 0

**第 7 行 `Sheep_M1 DEVOURS Grass_1 USING SUM ;`**
- `DEVOURS` = 吞噬（to devour，大口吃掉）
- `USING SUM` = 使用"加法"算法（把两者能量相加）
- 整句翻译：羊（Sheep_M1）吞噬草（Grass_1），使用加法——羊的能量 = 羊 + 草 = 0+65 = 65

**第 11 行 `Fungus_1 DEVOURS Sheep_M1 USING SUM ;`**
- 真菌（分解者）吃掉羊
- 整句翻译：真菌吞噬羊，使用加法——真菌的能量 = 0+65 = 65

**第 12-13 行 `ROT Fungus_1 TO STDOUT ;`**
- `ROT` = 腐烂/分解（把有机物矿化成无机物，回归环境）
- `TO STDOUT` = 到标准输出（屏幕）
- 整句翻译：真菌把能量矿化到屏幕上
- 为什么写两次？因为 FCL 的 ROT 第一次输出字符，第二次输出 U+XXXX 码位

---

## 1.3 运行并看输出

保存为 `hello.fc`，运行：

```bash
fcl hello.fc
```

输出：

```
🧬A
U+0041
```

**输出对照表：**

| 输出 | 含义 |
|---|---|
| `🧬` | GMO 转基因标识（开了 GMO ENABLED 就有） |
| `A` | ASCII 码 65 对应的字符（65 = 'A'） |
| `U+0041` | 65 的十六进制写法（0x41 = 65 = 'A'） |

---

## 1.4 为什么输出"A"？（理解 ASCII + ROT 两次）

因为：
- 草的能量是 65
- 羊吃掉草，能量变成 65
- 真菌吃掉羊，能量变成 65
- ROT 第一次：65 → ASCII 字符 `A`（65 = 'A'）
- ROT 第二次：65 → 十六进制 `U+0041`

如果加 `NUMERIC OUTPUT ;`（数值输出模式）：

```foodchain
GMO ENABLED ;
NUMERIC OUTPUT ;   ← 就是这行
BIOME { ... }
```

输出变成：
```
🧬65
🧬65
```

直接显示数字 65，一目了然。

---

## 1.5 三段式的三条铁律

```
⚠️ 规则 1：三段必须齐全
   缺 BIOME → 🌍 生态崩溃
   缺 FOODWEB → 🌍 生态崩溃
   缺 DECAY → 🌍 生态崩溃

⚠️ 规则 2：三段顺序固定，不能颠倒
   FOODWEB { ... }
   BIOME { ... }     ← ❌ 不能把 BIOME 放中间！
   DECAY { ... }

⚠️ 规则 3：FOODWEB 段里必须至少有 1 次捕食行为（DEVOURS / SCENT / POUNCE 均可）
   没有捕食 → 🌍 生态崩溃（食物链断了！）
```

---

## ✏️ 第 1 课练习

1. 把草的能量从 `60+5` 改成 `60+6`，运行看输出是什么？（提示：66 = 'B'）
2. 把 `GMO ENABLED ;` 删掉，再运行——输出还一样吗？（答案：不一样，扣税后羊只获得 13）
3. 把 DECAY 段里的两行 `ROT` 改成一行，输出会变成什么？（答案：只有 `🧬A`，没有 U+0041）
4. 如果把 `NUMERIC OUTPUT ;` 放在 `GMO ENABLED ;` 之后、BIOME 之前，程序还能正常工作吗？（答案：可以）

---

# 📗 第 2 课：物种图鉴——FCL 里只能用这 10 个名字

> **读完这课你将学会：**
> 1. 记住 FCL 生态圈的 10 个合法物种名
> 2. 理解"营养级"的概念（谁吃谁）
> 3. 掌握群居 vs 独居物种的命名规则

---

## 2.1 生态学基础：什么是营养级？

在自然界，生物按"吃与被吃"的关系分成不同层级，这就是**营养级**（Trophic Level）：

```
营养级 1（生产者）：植物、藻类。靠光合作用生存，自己制造能量。
                    ↓ 被谁吃：食草动物
营养级 2（食草动物）：羊、兔子。吃植物。
                    ↓ 被谁吃：食肉动物
营养级 3（食肉动物）：狼、狐狸。吃食草动物。
                    ↓ 被谁吃：顶级掠食者
营养级 4（顶级掠食者）：虎、狮子。没有天敌，站在食物链顶端。
                    ↓ 被谁吃：分解者
营养级 5（分解者）：真菌、杆菌。把动植物遗体分解成无机物。
```

> **记忆口诀**：**"生产草羊狼虎菌"** —— 生(产者)草→羊→狼→虎→菌(分解者)

---

## 2.2 FCL 的 10 个在册物种（只能用来命名变量！）

| 营养级 | 关键字 | 中文名 | 英文学名 | 群居/独居 |
|---|---|---|---|---|
| 1 生产者 | `PRODUCER` | 草 | Grass | 种群 |
| 1 生产者 | `PRODUCER` | 藻 | Algae | 种群 |
| 2 食草 | `HERBIVORE` | 羊 | Sheep | **群居** |
| 2 食草 | `HERBIVORE` | 兔 | Rabbit | 独居 |
| 3 食肉 | `CARNIVORE` | 狼 | Wolf | **群居** |
| 3 食肉 | `CARNIVORE` | 狐 | Fox | 独居 |
| 4 顶级 | `APEX` | 虎 | Tiger | 独居 |
| 4 顶级 | `APEX` | 狮 | Lion | **群居** |
| 5 分解 | `DECOMPOSER` | 真菌 | Fungus | 菌落 |
| 5 分解 | `DECOMPOSER` | 杆菌 | Bacillus | 菌落 |

**记住了：FCL 里总共只有这 10 个物种，不能多，不能少，不能换！**

---

## 2.3 营养级铁律：谁可以吃谁？

```
草（PRODUCER）  →  羊/兔（HERBIVORE）  →  狼/狐（CARNIVORE）  →  虎/狮（APEX）
                                                        ↕
                                                    真菌/杆菌（DECOMPOSER）
```

**规则：捕食者的营养级必须比猎物高恰好 1 级。**

| 吃 | 合法？ | 原因 |
|---|---|---|
| 羊 吃 草 | ✅ 合法 | 食草(2) 比 草(1) 高 1 级 |
| 狼 吃 羊 | ✅ 合法 | 食肉(3) 比 草(2) 高 1 级 |
| 虎 吃 狼 | ✅ 合法 | 顶级(4) 比 食肉(3) 高 1 级 |
| 羊 吃 狼 | ❌ 非法 | 食草(2) 比 食肉(3) 低！（反过来吃了）|
| 狼 吃 草 | ❌ 非法 | 狼不吃草（分类学上就不对）|
| 真菌 吃 任意 | ✅ 合法 | 分解者在 DECAY 段内不受营养级限制 |

---

## 2.4 命名格式：物种 + 身份（★ 考试必考）

**群居物种**（羊、狼、狮）需要记录社会地位：

```
首领：Alpha_<物种>
      例：Alpha_Wolf（狼王）、Alpha_Lion（狮王）、Alpha_Sheep（头羊）

成员：<物种>_<M|F><编号>
      例：Wolf_M1（1号雄狼）、Wolf_F2（2号雌狼）
          Sheep_M1（1号雄羊）、Sheep_F3（3号雌羊）
          Lion_M1（1号雄狮）
```

**独居物种**（兔、狐、虎）只有编号：

```
<物种>_<编号>
例：Rabbit_1（1号兔）、Fox_1（1号狐狸）、Tiger_1（1号虎）、Tiger_2（2号虎）
```

**生产者/分解者**（草、藻、真菌、杆菌）：

```
<物种>_<编号>
例：Grass_1（1号草）、Algae_1（1号藻）
    Fungus_1（1号真菌）、Bacillus_2（2号杆菌）
```

---

## 2.5 三个经典错误（必须理解）

```foodchain
INTRODUCE Dragon_1 AS PRODUCER WITH 1 ;
// ❌ 龙不在 FCL 的生态圈名单里
// 报错：🌿 外来物种入侵，生态圈不予接纳！

INTRODUCE Wolf_M1 AS PRODUCER WITH 1 ;
// ❌ 狼是食肉动物，不能声明为生产者
// 报错：⚠️ 分类学混乱！狼 是 CARNIVORE，不是 PRODUCER

INTRODUCE Wolf_1 AS CARNIVORE WITH 1 ;
// ❌ 狼是群居物种，编号要带 M（雄）或 F（雌）
// 报错：⚠️ 族谱登记混乱！狼 是群居，需要 M/F 标签
```

---

## ✏️ 第 2 课练习

**判断下面哪些名字合法：**

| 名字 | 合法？ | 如果不合法，为什么？ |
|---|---|---|
| `Rabbit_1` | ？ | |
| `Alpha_Tiger` | ？ | |
| `Fox_F1` | ？ | |
| `Sheep_5` | ？ | |
| `Bacillus_2` | ？ | |
| `Grass_1` | ？ | |
| `Wolf_M3` | ？ | |
| `Lion_M2` | ？ | |
| `Algae_1` | ？ | |

**写出以下正确名字：**
1. 2 号雌性狮 → ？
2. 狼王 → ？
3. 3 号草丛 → ？
4. 1 号兔 → ？

**答案：**
- `Rabbit_1` ✅（兔独居，直接编号）
- `Alpha_Tiger` ❌（虎是独居动物，没有 Alpha）
- `Fox_F1` ❌（狐是独居动物，不带性别标签）
- `Sheep_5` ❌（羊是群居，必须带 M/F）
- `Bacillus_2` ✅（杆菌用种群编号）
- `Grass_1` ✅（草用种群编号）
- `Wolf_M3` ✅（群居成员）
- `Lion_M2` ✅（群居成员）
- `Algae_1` ✅（藻用种群编号）
- 正确名字：1) `Lion_F2`，2) `Alpha_Wolf`，3) `Grass_3`，4) `Rabbit_1`

---

# 📗 第 3 课：INTRODUCE——把物种引入生态系统

> **读完这课你将学会：**
> 1. 掌握 INTRODUCE 的完整语法
> 2. 理解五种营养级类型的关键区别
> 3. 理解 PRODUCER 支持浮点/分数，而 HERBIVORE/CARNIVORE 只支持整数

---

## 3.1 INTRODUCE 关键词拆解

**INTRODUCE** = 引入、引进（把一个新物种放进生态系统中）

英英词典定义：
> **Introduce** (v.): To bring something into use or existence for the first time.

在 FCL 里，`INTRODUCE` = 声明一个变量（给它起名字、定类型、给初始值）。

---

## 3.2 完整语法

```foodchain
INTRODUCE <物种名> AS <营养级> WITH <初始能量> ;
```

| 组成部分 | 必须/可选 | 说明 |
|---|---|---|
| `INTRODUCE` | 必须 | 关键字，固定不变 |
| `<物种名>` | 必须 | 10 个在册物种之一，遵循命名规则 |
| `AS` | 必须 | 关键字，固定不变 |
| `<营养级>` | 必须 | PRODUCER / HERBIVORE / CARNIVORE / APEX / DECOMPOSER |
| `WITH` | 必须 | 关键字，固定不变 |
| `<初始能量>` | 必须 | 数字、分数、表达式 |
| `;` | 必须 | 每条语句以分号结尾 |

---

## 3.3 初始能量的三种写法

```foodchain
INTRODUCE Grass_1   AS PRODUCER     WITH 65 ;        // 整数 65
INTRODUCE Grass_2   AS PRODUCER     WITH 3/2 ;        // 分数 3/2 = 1.5
INTRODUCE Grass_3   AS PRODUCER     WITH 60+5 ;       // 表达式 60+5 = 65
INTRODUCE Grass_4   AS PRODUCER     WITH (30+6)/2 ;   // 表达式 (36)/2 = 18
```

> **注意**：只有 `PRODUCER`（生产者）支持浮点和分数。HERBIVORE 和 CARNIVORE 会把小数四舍五入成整数。

---

## 3.4 五种营养级的关键区别

| 营养级 | 关键字 | 存储类型 | 数值范围 | 特殊规则 |
|---|---|---|---|---|
| 生产者 | `PRODUCER` | 浮点 | 0.0 ~ 9999.9 | 支持分数；不能作为捕食者（没有天敌例外时不能被吃）|
| 食草 | `HERBIVORE` | 整型 | 0 ~ 255 | 超过 255 → 胃溃疡溢出，归零 + 等 2 秒 |
| 食肉 | `CARNIVORE` | 整型 | 0 ~ 65535 | 行号奇数加速、偶数减速 |
| 顶级 | `APEX` | 布尔 | 0（HUNGRY）或 1（FULL）| 唯一能做乘除（PROD/QUOT）；唯一能承载 ASSESS 结果 |
| 分解 | `DECOMPOSER` | 数值 | 按 ASCII 解释 | 唯一能执行 ROT 输出 |

---

## 3.5 为什么要用 APEX 存储布尔（0/1）？

> **生态学解释**：在自然界，"裁决权"属于顶级掠食者——它们没有天敌，是食物链的最终裁判。

在 FCL 里，`APEX`（顶级）承担两个特殊职责：
1. **承载比较结果**：ASSESS 比较两个能量后，把"谁大"的结果存入 APEX（FULL=1 或 HUNGRY=0）
2. **做乘除法**：只有 APEX 可以用 PROD（乘）和 QUOT（除）

---

## ✏️ 第 3 课练习

1. 写一句引入：1 号虎（APEX），初始能量 FULL。（答案：`INTRODUCE Tiger_1 AS APEX WITH 1 ;`）
2. 写一句引入：2 号雄性狼（CARNIVORE），初始能量 50。（答案：`INTRODUCE Wolf_M2 AS CARNIVORE WITH 50 ;`）
3. PRODUCER 支持 `3/2` 这样的分数，HERBIVORE 支持吗？（答案：支持，但会四舍五入成整数 2）
4. HERBIVORE 的能量上限是多少？超过会怎样？（答案：255，超过 → 归零 + 等 2 秒）

---

# 📗 第 4 课：DEVOURS——FCL 的运算全靠"吃"

> **读完这课你将学会：**
> 1. 掌握 DEVOURS 的四种算法（SUM/DIFF/PROD/QUOT）
> 2. 理解"猎物被吃光"这一破坏性赋值
> 3. 理解为什么 PROD/QUOT 只有 APEX 能用

---

## 4.1 DEVOURS 关键词拆解

**DEVOUR**（动词）：to eat (food or prey) quickly and usually completely（狼吞虎咽地吃掉）

> 这个词比"eat"更强烈，暗示捕食者把猎物整个吞下去——猎物就消失了。

FCL 里 `DEVOURS` = **单向破坏性赋值**：A 吃 B，A 获得能量，B 归零。

---

## 4.2 四种算法（加/减/乘/除）

```foodchain
// 加法：A = A + B
Sheep_M1 DEVOURS Grass_1 USING SUM ;

// 减法：A = A - B（前提：A >= B，否则报错"捕食者饿死"）
Wolf_M1 DEVOURS Sheep_M1 USING DIFF ;

// 乘法：A = A × B（仅 APEX 可用）
Tiger_1 DEVOURS Fox_1 USING PROD ;

// 除法：A = A ÷ B（仅 APEX 可用，余数丢弃）
Lion_M1 DEVOURS Wolf_F1 USING QUOT ;
```

### 逐个拆解：

**SUM = 相加（能量合并）**
- 生态学含义：捕食者吃掉猎物后，猎物的能量合并到捕食者身上
- 例：羊能量 20 + 草能量 50 = 羊能量 70

**DIFF = 相减（大减小）**
- 生态学含义：捕食者在争夺食物时，可能要付出一定代价（类似大欺小）
- 例：狼能量 80 − 羊能量 30 = 狼能量 50
- ⚠️ 注意：狼的能量必须 >= 羊的能量，否则报错"🥀 捕食者饿死"

**PROD = 相乘（仅 APEX 虎/狮可用）**
- 生态学含义：顶级掠食者可以"聚合"多个猎物的能量（多个猎物共同加成）
- 例：虎能量 10 × 狐狸能量 3 = 虎能量 30

**QUOT = 相除（仅 APEX 虎/狮可用）**
- 生态学含义：顶级掠食者对猎物进行"分配"（类似把猎物分给多只幼崽）
- 例：狮能量 24 ÷ 狼能量 4 = 狮能量 6
- ⚠️ 余数丢弃：不是四舍五入，是直接丢弃

---

## 4.3 猎物被吃光了！（破坏性赋值的含义）

**这是 FCL 和普通编程语言最大的区别！**

普通 Python：
```python
x = 5
y = 3
x = x + y   # 结果：x = 8，y = 3（y 不变！）
```

FCL：
```foodchain
INTRODUCE Grass_1 AS PRODUCER WITH 65 ;
INTRODUCE Sheep_M1 AS HERBIVORE WITH 0 ;
Sheep_M1 DEVOURS Grass_1 USING SUM ;
// 结果：Sheep_M1 = 65，Grass_1 = 0（草被吃光了！）
```

**猎物归零 = 草被消化了。** 在自然界，被吃掉的动物就是死了，不能复活。

> **那如果我想让猎物保持不变怎么办？** 用 CLONE（克隆），下一课会讲。

---

## 4.4 为什么 PROD/QUOT 只有 APEX 能用？

**生态学解释**：乘法和除法是"高级能量操作"，只有顶级掠食者才有能力"聚合"或"分配"多个猎物。普通的羊、狼只能一级一级地单向传递能量。

**程序员视角**：APEX 在 FCL 里相当于"数值计算专家"，它能做的普通物种做不了，这叫**类型限制**（Type Restriction）。

---

## ✏️ 第 4 课练习

1. 虎能量 10，狐狸能量 5，APEX PROD 后虎变成多少？（答案：50）
2. 狮能量 30，狼能量 4，APEX QUOT 后狮变成多少？余数呢？（答案：7，余数 2 丢弃）
3. 草能量 100，羊能量 0，羊吃草 SUM 后，两者能量分别是多少？（答案：羊 20（扣税后），草 0）
4. 狼能量 5，羊能量 10，狼吃羊（DIFF）会怎样？（答案：报错"🥀 捕食者饿死"——狼不够羊吃）

---

# 📗 第 5 课：能量税与 GMO——为什么到手的能量只有 20%？

> **读完这课你将学会：**
> 1. 理解林德曼定律（生态学 10% 定律）
> 2. 理解为什么 FCL 默认只有 20% 能量到账
> 3. 掌握 GMO ENABLED 的用法和代价

---

## 5.1 林德曼定律——真实生态的能量传递

真实生态学中有一个著名定律：**林德曼定律（Lindeman's Law）**：

> 能量从一个营养级传递到下一个营养级，效率只有 **10%~20%**。

也就是说：
- 草有 1000 单位能量
- 羊吃掉草，最多只能获得 100~200 单位（10%~20%）
- 剩下的 800~900 单位，通过呼吸、排泄、粪便等形式散失

原因：生物活着需要消耗能量来呼吸、运动、消化——**这些消耗是不可避免的**。

> **记忆口诀**：**"传递 100，收到 20"** —— 传递效率 20%，其余 80% 散失。

---

## 5.2 FCL 的能量税：税率 80%，到手 20%

FCL 默认：**每次捕食（DEVOURS），捕食者只能获得猎物的 20% 能量。**

```foodchain
// 草能量 100
Sheep_M1 DEVOURS Grass_1 USING SUM ;
// 羊只获得：100 × 20% = 20
// 实际结果：Sheep_M1 = 20，Grass_1 = 0
```

为什么是 20% 而不是 10%？因为 FCL 的 20% 是一个**简化的默认值**，模拟了林德曼效率的下限（10%~20% 区间的中间值）。

---

## 5.3 GMO ENABLED——免能量税（转基因）

**GMO** = 转基因生物（Genetically Modified Organism）

生态学含义：转基因生物跳过了自然界的能量传递损耗，实现了"100% 传递效率"。

```foodchain
GMO ENABLED ;                              // ← 写在程序第一行！
BIOME {
    INTRODUCE Grass_1 AS PRODUCER WITH 100 ;
    INTRODUCE Sheep_M1 AS HERBIVORE WITH 0 ;
}
FOODWEB {
    Sheep_M1 DEVOURS Grass_1 USING SUM ;    // 羊直接获得 100（不扣税）
}
```

### GMO 的代价：输出带 🧬 标识

```foodchain
// GMO 模式下的 ROT 输出：
🧬65    ← 🧬 是"转基因产品"标识，提醒你这是 GMO 模式
```

### GMO 的好处：
- ✅ 能量 100% 传递（免税）
- ✅ 计算结果可预测（不受税的影响）
- ✅ 适合需要精确数值的算法（斐波那契、阶乘等）

### GMO 的坏处：
- ❌ 输出前面有 🧬 标识（如果是教学演示，不够"干净"）
- ❌ 但其实没什么实质性坏处——初学者强烈建议开 GMO

---

## 5.4 税收去哪儿了？

这是一个有趣的生态学问题：被扣掉的 80% 能量并没有凭空消失，而是：

```
草能量 100
    ↓ 羊吃掉（草归零）
    ├→ 20% → 羊（Sheep_M1 = 20）  ← 可见的能量
    └→ 80% → "呼吸散失"            ← 不可见，模拟生态系统的熵增
```

> 程序员视角：这 80% 类似于编程语言里的"隐式开销"——GC 垃圾回收的 CPU 时间、内存分配的系统调用等，不直接体现在你的代码里，但确实存在。

---

## ✏️ 第 5 课练习

1. 草能量 100，羊吃草（默认有税），羊最后是多少？（答案：20）
2. 草能量 100，羊吃草（开 GMO），羊最后是多少？（答案：100）
3. 草能量 50，羊吃草（默认有税），羊最后是多少？（答案：10，因为 50×0.2=10）
4. 草能量 50，开 GMO，羊最后是多少？（答案：50）

---

# 📗 第 6 课：CLONE——生态学版的"复制粘贴"

> **读完这课你将学会：**
> 1. 理解 CLONE 与 DEVOURS 的根本区别
> 2. 理解"物质循环"vs"能量流动"
> 3. 实战：用 CLONE 实现斐波那契数列

---

## 6.1 为什么需要 CLONE？

**DEVOURS 的问题**：捕食是单向破坏性的——猎物被吃光了。

现实例子：你想把一个数"复制"一份，原数保留：

```python
# Python：
a = 5
b = a       # b = 5，a 还是 5（没有变化！）
print(a)    # 5
```

但在 FCL 里，`b = a` 这种简单赋值不存在——因为 FCL 的赋值 = 捕食 = 单向 = 破坏性的。

```foodchain
# FCL（错误做法）：
Grass_2 DEVOURS Grass_1 USING SUM ;
// Grass_2 = Grass_2 + Grass_1，但 Grass_1 也归零了！
```

**CLONE = 解决方案**：给目标物种复制源物种的能量，但源物种不受影响。

---

## 6.2 CLONE 关键词拆解

**CLONE** = 克隆，无性繁殖（Clone: an identical genetic copy of an organism）

生态学含义：扦插繁殖——从母株上剪一段种下去，得到一株新植物，母株安然无恙。这完美对应了"复制但不破坏原数据"的需求。

---

## 6.3 语法

```foodchain
CLONE <目标> FROM <源> ;
```

| 组成部分 | 说明 |
|---|---|
| `CLONE` | 关键字 |
| `<目标>` | 接收副本的物种（必须已 INTRODUCE 过）|
| `FROM` | 关键字，从……复制 |
| `<源>` | 被复制的物种（不受影响）|

---

## 6.4 斐波那契实战（CLONE 的经典应用）

**斐波那契数列**：1, 1, 2, 3, 5, 8, 13, 21...（每一项 = 前两项之和）

```foodchain
GMO ENABLED ;
NUMERIC OUTPUT ;
BIOME {
    INTRODUCE Grass_1  AS PRODUCER WITH 1 ;    // F(n-2)
    INTRODUCE Grass_2  AS PRODUCER WITH 1 ;    // F(n-1)
    INTRODUCE Sheep_M1 AS HERBIVORE WITH 0 ;    // 累加器
    INTRODUCE Algae_1  AS PRODUCER WITH 0 ;    // 备份寄存器
    INTRODUCE Fungus_1 AS DECOMPOSER WITH 0 ;  // 输出
}
FOODWEB {
    MIGRATION Sheep_M1 OVER 5 {
        CLONE Algae_1 FROM Grass_2 ;           // 【1】备份 F(n-1)
        Sheep_M1 DEVOURS Grass_1 USING SUM ;    // 【2】累加 F(n-2)
        Sheep_M1 DEVOURS Grass_2 USING SUM ;    // 【3】再加 F(n-1) → F(n)
        CLONE Grass_1 FROM Algae_1 ;            // 【4】Grass_1 ← 旧 Grass_2（前移）
        CLONE Grass_2 FROM Sheep_M1 ;           // 【5】Grass_2 ← 新和
        INTRODUCE Sheep_M1 AS HERBIVORE WITH 0 ;   // 【6】重置累加器
        INTRODUCE Algae_1  AS PRODUCER WITH 0 ;    // 【7】重置备份
    }
}
DECAY {
    Fungus_1 DEVOURS Grass_2 USING SUM ;
    ROT Fungus_1 TO STDOUT ;
    ROT Fungus_1 TO STDOUT ;
}
```

**为什么要备份（【1】和【4】）？**

因为 `【3】` 里的 `Sheep_M1 DEVOURS Grass_2` 会把 Grass_2 归零。没有备份的话，Grass_2 就丢了，下一轮就不知道上一轮的 F(n-1) 是什么了。

> **类比理解**：就像你在抄作业——原卷不能涂改，必须先把原卷复印一份，再用复印本去答题，原卷保持原样。

---

## ✏️ 第 6 课练习

1. `CLONE Tiger_1 FROM Wolf_M1 ;` 执行后，Wolf_M1 会变成 0 吗？（答案：不会，Wolf_M1 保持原值）
2. `CLONE Tiger_1 FROM Grass_1 ;` — Tiger_1 必须是什么类型？（答案：APEX——因为 Tiger 是顶级）
3. 为什么斐波那契里每轮都要重置累加器（【6】【7】）？（答案：避免上轮残留影响下轮）
4. 把循环从 `OVER 5` 改成 `OVER 8`，输出会是什么？（答案：F(9)=34）

---

# 📗 第 7 课：ROT——只有分解者才能"说话"

> **读完这课你将学会：**
> 1. 理解为什么只有 DECOMPOSER 能执行 ROT
> 2. 掌握编码模式 vs 数值模式的区别
> 3. 理解 ROT 两次的含义

---

## 7.1 ROT 关键词拆解

**ROT**（动词）：to decay or cause something to decay（腐烂、分解）

生态学含义：分解者（真菌、细菌）把动植物遗体中的有机物分解成无机物（CO₂、水、无机盐），让养分回归环境。这是一个**输出**过程——物质从生态系统流向外部世界。

> **程序员视角**：`ROT xxx TO STDOUT` ≈ `print(x)`，但约束条件更严格。

---

## 7.2 为什么只有分解者能输出？

生态逻辑：能量必须经过分解者的"矿化"才能回归环境。草不能直接输出，必须先被羊吃，再被真菌分解——养分才能回到生态系统里。

```
草（PRODUCER） → 羊（HERBIVORE） → 真菌（DECOMPOSER） → 输出
```

**程序员的困惑**：为什么不能直接 `ROT Grass_1 TO STDOUT`？

> **生态学回答**：因为草不是分解者——在自然界，植物死亡后必须被分解者分解，能量才能以矿物质的形式回归土壤。直接让草"输出"，就像让尸体自己站起来说话——违反了生态逻辑。

---

## 7.3 两种输出模式

### 编码模式（默认）

```foodchain
// 不写任何声明，默认就是编码模式
ROT Fungus_1 TO STDOUT ;    // 第1次：输出 ASCII 字符
ROT Fungus_1 TO STDOUT ;    // 第2次：输出 U+XXXX（Unicode 码位）
```

**第一次 ROT**：能量 → ASCII 字符 → 屏幕
**第二次 ROT**：能量 → 十六进制码位（`U+0041` = 65 = 'A'）

| 能量 | 第1次 ROT | 第2次 ROT |
|---|---|---|
| 65 | `A` | `U+0041` |
| 55 | `7` | `U+0037` |
| 48 | `0` | `U+0030` |
| 8 | 退格符（乱码）| `U+0008` |
| 13 | 回车符（换行）| `U+000D` |

### 数值模式（v2.2 新增）

```foodchain
NUMERIC OUTPUT ;          // ← 开启数值模式
ROT Fungus_1 TO STDOUT ;  // 直接输出数值：65
ROT Fungus_1 TO STDOUT ;  // 还是输出数值：65
```

| 能量 | 编码模式输出 | 数值模式输出 |
|---|---|---|
| 65 | `A` + `U+0041` | `65` |
| 55 | `7` + `U+0037` | `55` |
| 48 | `0` + `U+0030` | `48` |

> **强烈建议初学者**：开 `NUMERIC OUTPUT ;`，输出数字，直观清晰！

---

## 7.4 理解 ROT 两次的设计意图

为什么 FCL 的 ROT 要写两次？

这是一个**刻意设计**的 Esolang 特性，模拟了两个生态学过程：

```
第一次 ROT：矿化 → 产出产物（字符）
第二次 ROT：检测 → 记录码位（U+XXXX，方便程序员对照）
```

| ROT 次数 | 含义 | 类比 |
|---|---|---|
| 第1次 | 能量 → 字符 | 土壤里的矿物质变成了你能看到的"东西" |
| 第2次 | 字符的"出生证" | 科学家记录这株草的 ASCII 编号是 65 |

---

## ✏️ 第 7 课练习

1. 真菌能量 72，编码模式下第一次 ROT 输出什么？（答案：字符 'H'，因为 ASCII 72 = 'H'）
2. 真菌能量 90，数值模式下 ROT 输出什么？（答案：90）
3. 真菌能量 13，编码模式下 ROT 输出什么？（答案：回车符，看不见）
4. 为什么 FCL 要设计"只有分解者能输出"这个规则？（答案：遵循生态逻辑——能量必须经过分解者矿化才能回归环境）

---

# 📗 第 8 课：SEASON——用天气做选择

> **读完这课你将学会：**
> 1. 理解 SEASON 的湿度机制（最近 3 条捕食决定天气）
> 2. 掌握 RAIN / DRY 二选一逻辑
> 3. 理解 STORM 的作用

---

## 8.1 SEASON 关键词拆解

**SEASON**（名词）：季节（一年中某个有特定气候特征的时期，如雨季、旱季）

生态学含义：在热带草原等地区，雨季降水充沛、植被茂盛，动物行为与旱季截然不同。**湿度（捕食频率）决定了生态系统的活跃状态**。

> **程序员视角**：`SEASON RAIN { } DRY { }` ≈ `if (humidity >= 2) { /* RAIN */ } else { /* DRY */ }`

---

## 8.2 湿度 = 最近 3 条捕食的次数

```foodchain
SEASON RAIN {
    // 如果湿度 >= 2（最近 3 条 DEVOURS 中有 >= 2 次），执行这里
} DRY {
    // 如果湿度 < 2，执行这里
}
```

**湿度计算规则**：看**最近 3 条执行过的 DEVOURS 语句**，数一数有多少个。

```
湿度 = min(最近3条DEVOURS数量, 2)   // 最多为 2

湿度 = 2 → RAIN（雨季，活跃）
湿度 = 1 → DRY（旱季，萧条）
湿度 = 0 → DRY（极度干旱）
```

---

## 8.3 完整例子

```foodchain
GMO ENABLED ;
NUMERIC OUTPUT ;
BIOME {
    INTRODUCE Grass_1 AS PRODUCER WITH 10 ;
    INTRODUCE Grass_2 AS PRODUCER WITH 20 ;
    INTRODUCE Sheep_M1 AS HERBIVORE WITH 0 ;
    INTRODUCE Fungus_1 AS DECOMPOSER WITH 0 ;
}
FOODWEB {
    Sheep_M1 DEVOURS Grass_1 USING SUM ;   // 第1次捕食
    Sheep_M1 DEVOURS Grass_2 USING SUM ;   // 第2次捕食 ← 湿度 = 2 → RAIN
    SEASON RAIN {
        Fungus_1 DEVOURS Sheep_M1 USING SUM ;  // RAIN：输出 30
    } DRY {
        INTRODUCE Fungus_1 AS DECOMPOSER WITH 1 ; // DRY：直接设 1
    }
}
DECAY {
    ROT Fungus_1 TO STDOUT ;
}
```

最近 3 条 DEVOURS：草1、草2 → 湿度 2 → **RAWN 分支** → 输出 30。

---

## 8.4 STORM——乱序的雨季

默认情况下，RAIN 块超过 3 条语句时**不乱序**（保证确定性）。

如果你在程序首行加了 `STORM ENABLED ;`，RAWN 块超过 3 条语句时，**语句随机乱序执行一次**（非确定性）。

> ⚠️ STORM 适合做"随机行为模拟"，但会破坏依赖执行顺序的程序。

---

## ✏️ 第 8 课练习

1. 最近 3 条 DEVOURS 是 [吃、吃、声明]，湿度是多少？→ RAIN 还是 DRY？（答案：湿度=2，RAIN）
2. 最近 3 条 DEVOURS 是 [吃、声明、声明]，湿度是多少？（答案：湿度=1，DRY）
3. 湿度用什么来判断 RAIN vs DRY 的分界线？（答案：2——>=2 → RAIN，<2 → DRY）

---

# 📗 第 9 课：MIGRATION——迁徙循环（定次循环）

> **读完这课你将学会：**
> 1. 理解 MIGRATION 的定次循环逻辑
> 2. 理解"迁徙消耗"（每轮后能量开平方根）
> 3. 避免踩坑：选对"迁徙者"

---

## 9.1 MIGRATION 关键词拆解

**MIGRATION**（名词）：迁徙（动物为寻找食物、繁殖或适宜气候而进行的季节性或周期性移动）

生态学含义：长途迁徙对动物的能量消耗极大——角马横渡马拉河、候鸟飞跃喜马拉雅——每迁徙一次，能量大量损耗。FCL 用**开平方根**来模拟这种消耗。

> **程序员视角**：`MIGRATION Sheep_M1 OVER 5 { }` ≈ `for (i = 0; i < 5; i++) { }`

---

## 9.2 语法

```foodchain
MIGRATION <迁徙者> OVER <次数> {
    // 循环体
}
```

| 组成部分 | 说明 |
|---|---|
| `<迁徙者>` | 承受"迁徙消耗"的物种（每轮后能量开平方根）|
| `<次数>` | 循环执行几轮（整数常量，不支持变量）|

---

## 9.3 迁徙消耗——每轮后能量开平方根

```foodchain
INTRODUCE Sheep_M1 AS HERBIVORE WITH 100 ;
MIGRATION Sheep_M1 OVER 4 {
    // 循环体
}
```

| 轮次 | Sheep_M1 的能量 | 说明 |
|---|---|---|
| 初始 | 100 | 刚出发 |
| 轮 1 后 | 10 | 100 → √100 = 10（长途跋涉，能量大幅消耗）|
| 轮 2 后 | 3.16 | 10 → √10 ≈ 3.16 |
| 轮 3 后 | 1.78 | 3.16 → √3.16 ≈ 1.78 |
| 轮 4 后 | 1.33 | 1.78 → √1.78 ≈ 1.33 |

> **记忆口诀**：**迁徙 100 → 10 → 3.16 → 1.78 → 1.33……** 能量越来越小，但不能为 0（最低趋近于 1）。

---

## 9.4 踩坑：别让迁徙者承载重要数据！

```foodchain
// ❌ 错误示范：把草当成迁徙者
INTRODUCE Grass_1 AS PRODUCER WITH 100 ;
MIGRATION Grass_1 OVER 3 {          // 草每轮后能量开平方根！
    Sheep_M1 DEVOURS Grass_1 USING SUM ;  // 草越来越小，数据丢失
}

// ✅ 正确做法：选一个每轮会被重置的物种当迁徙者
INTRODUCE Sheep_M1 AS HERBIVORE WITH 0 ;
MIGRATION Sheep_M1 OVER 3 {
    // 累加器 Sheep_M1 每轮被重置（INTRODUCE 覆盖）
    Sheep_M1 DEVOURS Grass_1 USING SUM ;
    Sheep_M1 DEVOURS Grass_2 USING SUM ;
    INTRODUCE Sheep_M1 AS HERBIVORE WITH 0 ;  // ← 重置！避免 sqrt 累积
}
```

---

## ✏️ 第 9 课练习

1. 迁徙者初始能量 81，执行 3 轮后能量是多少？（答案：81 → 9 → 3 → √3 ≈ 1.73）
2. 为什么斐波那契里选 Sheep_M1（累加器）当迁徙者而不是 Grass_1（数据源）？（答案：累加器每轮被重置为 0，sqrt 消耗不影响重要数据）
3. 迁徙者能量 16，3 轮后剩多少？（答案：16 → 4 → 2 → √2 ≈ 1.41）

---

# 📗 第 10 课：HIBERNATION——冬眠循环（条件循环）

> **读完这课你将学会：**
> 1. 理解 HIBERNATION 的条件循环逻辑
> 2. 理解"冬眠代谢"（每轮能量 × 0.9）
> 3. 理解防死循环机制（10000 轮强制唤醒）

---

## 10.1 HIBERNATION 关键词拆解

**HIBERNATION**（名词）：冬眠（动物在寒冷季节降低代谢、减少活动、进入类似睡眠的状态）

生态学含义：熊在冬天食物匮乏时进入冬眠，降低心率、减少能量消耗，以度过饥荒。但如果冬眠太久（超过生理极限），就必须醒来。

> **程序员视角**：`HIBERNATION Fungus_1 UNTIL Tiger_1 { }` ≈ `while (Tiger_1 != FULL) { /* 冬眠 */ }`

---

## 10.2 语法

```foodchain
HIBERNATION <冬眠者> UNTIL <APEX条件> {
    // 循环体
}
```

| 组成部分 | 说明 |
|---|---|
| `<冬眠者>` | 承受代谢消耗的物种（每轮能量 × 0.9）|
| `<APEX条件>` | 必须是 APEX 类型，当它 = FULL(1) 时停止循环 |
| `<循环体>` | 每次迭代执行的内容 |

---

## 10.3 冬眠代谢——每轮能量 × 0.9

```foodchain
INTRODUCE Tiger_1 AS APEX WITH 0 ;   // 条件变量：HUNGRY(0)，等待 FULL
INTRODUCE Fungus_1 AS DECOMPOSER WITH 100 ;  // 冬眠者
HIBERNATION Fungus_1 UNTIL Tiger_1 {
    // 每轮 Fungus_1 能量 × 0.9
    // 什么时候 Tiger_1 = FULL？需要 ASSESS 在循环里改变它
}
```

| 轮次 | Fungus_1 能量 | 说明 |
|---|---|---|
| 初始 | 100 | 开始冬眠 |
| 轮 1 后 | 90 | 100 × 0.9 = 90 |
| 轮 2 后 | 81 | 90 × 0.9 = 81 |
| ... | ... | |
| 轮 10 后 | 34.9 | 100 × 0.9^10 |
| 轮 100 后 | 0.00026 | 趋近于 0 |

---

## 10.4 防死循环：10000 轮强制唤醒

如果循环条件（APEX）始终达不到，会陷入无限循环。FCL 的保护机制：**超过 10000 轮自动唤醒**（⏰ 冬眠过久），避免程序卡死。

---

## ✏️ 第 10 课练习

1. 冬眠者初始能量 100，执行 5 轮后能量是多少？（答案：100 × 0.9^5 ≈ 59）
2. 什么情况下 HIBERNATION 会触发"10000 轮强制唤醒"？（答案：APEX 条件始终达不到，死循环）
3. HIBERNATION 里的 `<APEX条件>` 必须是什么类型？（答案：APEX——顶级掠食者有裁决权）

---

# 📗 第 11 课：比较与布尔——ASSESS + SYMBIOSIS / COMPETITION / MIMICRY

> **读完这课你将学会：**
> 1. 用 ASSESS 比较两个能量
> 2. 用 SYMBIOSIS（AND）、COMPETITION（OR）、MIMICRY（NOT）组合布尔逻辑
> 3. 理解为什么布尔运算需要 APEX

---

## 11.1 ASSESS——生态位评估（比较）

**ASSESS**（动词）：to evaluate or estimate the nature, ability, or quality of（评估、判定）

生态学含义：两个物种竞争同一个生态位时，需要判定谁占优势。裁决权属于顶级掠食者——它们是食物链的最高层。

```foodchain
ASSESS <A> AGAINST <B> TO <C> ;
```

- A ≥ B → C = FULL(1)
- A < B → C = HUNGRY(0)
- **C 必须是 APEX**

---

## 11.2 布尔三件套

**SYMBIOSIS**（名词）：互利共生（两个物种紧密合作，互惠互利）
→ 程序员理解：`AND`（两个都是 1 才结果为 1）

```foodchain
SYMBIOSIS Lion_M1 WITH Lion_F1 TO Tiger_1 ;
// Tiger_1 = Lion_M1 AND Lion_F1
```

**COMPETITION**（名词）：竞争（两个物种争夺同一资源，替代路径）
→ 程序员理解：`OR`（任一为 1 结果就为 1）

```foodchain
COMPETITION Lion_M1 OR Lion_F1 TO Tiger_1 ;
// Tiger_1 = Lion_M1 OR Lion_F1
```

**MIMICRY**（名词）：拟态（一个物种模仿另一个，伪装）
→ 程序员理解：`NOT`（颠倒 0↔1）

```foodchain
MIMICRY Lion_M1 TO Tiger_1 ;
// Tiger_1 = NOT Lion_M1
```

---

## 11.3 真值表

| A | B | A AND B | A OR B | NOT A |
|---|---|---|---|---|
| 0 | 0 | 0 | 0 | 1 |
| 0 | 1 | 0 | 1 | 1 |
| 1 | 0 | 0 | 1 | 0 |
| 1 | 1 | 1 | 1 | 0 |

---

## ✏️ 第 11 课练习

1. 狼能量 80，羊能量 20，ASSESS 结果存入虎（APEX），虎是多少？（答案：80≥20 → FULL → 1）
2. A=1, B=0，写出 A AND B = ?（答案：0）
3. A=0, B=1，写出 A OR B = ?（答案：1）
4. A=1，NOT A = ?（答案：0）

---

# 📗 第 12 课：MUTATION——基因变异的随机选择

> **读完这课你将学会：**
> 1. 理解 MUTATION 的概率变异机制
> 2. 用 MATCH() 检测是否发生变异
> 3. 理解为什么变异是 1/3 概率

---

## 12.1 MUTATION 关键词拆解

**MUTATION**（名词）：变异（基因序列的改变，导致后代与亲代不同）

生态学含义：基因突变是随机的、不可预测的——这是进化的原材料。FCL 的 MUTATION 模拟了这种随机性：物种名有 1/3 概率变成"变异名"。

> **程序员视角**：`MUTATION` ≈ `switch` + 随机 case + `MATCH()` 检测

**MUTATION 触发时会发生什么：**

1. **物种级改名**：块执行期间，该物种全部在册成员临时改名（`Wolf_M1` → `Wolv_M1`、`Alpha_Wolf` → `Alpha_Wolv`），块内对该物种的引用随之改写；块结束后恢复原名，块外引用不受影响
2. **随机表达一支**：从所有 `CASE` 分支中等概率随机选一支执行（不触发则整个块空转）
3. **变异可检测**：`MATCH(Wolf)` / `MATCH(Wolf_M1)` / `MATCH(Wolv_M1)` 均返回 1，且记录跨块持续存在

注意：变异名（如 `Wolv`）不在生态圈在册名录中，不能用于 `INTRODUCE` 引种——变异体是生态圈的外来者。

---

## 12.2 10 个物种的变异名

| 原名 | 变异名 |
|---|---|
| Grass | Grasse |
| Algae | Algee |
| Sheep | Sheepe |
| Rabbit | Rabbite |
| Wolf | Wolv |
| Fox | Foxy |
| Tiger | Tygre |
| Lion | Lyone |
| Fungus | Funge |
| Bacillus | Bacilluz |

---

## 12.3 完整例子

```foodchain
GMO ENABLED ;
NUMERIC OUTPUT ;
BIOME {
    INTRODUCE Wolf_M1  AS CARNIVORE WITH 50 ;
    INTRODUCE Sheep_M1 AS HERBIVORE WITH 30 ;
    INTRODUCE Tiger_1  AS APEX WITH 0 ;
    INTRODUCE Fungus_1 AS DECOMPOSER WITH 0 ;
}
FOODWEB {
    MUTATION Wolf_M1 {
        CASE "正常": Wolf_M1 DEVOURS Sheep_M1 USING SUM ;
        CASE "长毛": Wolf_M1 DEVOURS Sheep_M1 USING DIFF ;
    }
    ASSESS Wolf_M1 AGAINST Sheep_M1 TO Tiger_1 ;
}
DECAY {
    Fungus_1 DEVOURS Wolf_M1 USING SUM ;
    ROT Fungus_1 TO STDOUT ;
}
```

运行说明：变异触发（1/3）时随机表达一支——"正常"则 `Wolf_M1 = 50+30 = 80`（GMO 免税），"长毛"则 `Wolf_M1 = 50-30 = 20`；未触发（2/3）时块空转，`Wolf_M1` 保持 50。三种结局下 `Fungus_1 DEVOURS Wolf_M1`（DECAY 内分解者免税）分别得到 80 / 20 / 50。

---

## ✏️ 第 12 课练习

1. MUTATION 的变异概率是多少？（答案：1/3）
2. Wolf 的变异名是什么？（答案：Wolv）
3. `MATCH(Wolf_M1)` 在什么情况下返回 1？（答案：Wolf_M1 发生了变异，变成了 Wolv_M1）

---

# 📗 第 13 课：垃圾回收——真菌是生态清洁工

> **读完这课你将学会：**
> 1. 理解 FCL 的自动内存回收机制
> 2. 理解"活体豁免"规则
> 3. 掌握 EXTINCTION 手动回收

---

## 13.1 什么是垃圾回收？

**垃圾回收（GC）** = 自动清理不再使用的数据。

普通编程语言（Python/Java）：
```python
x = 5
x = 10    # 旧的 5 去哪儿了？→ Python 自动回收了
```

FCL 不用"内存"这个词——用的是**生态分解**：

```
能量 = 0 的变量 = "尸体"
真菌 = 清洁工，专门分解尸体
```

---

## 13.2 五条铁律

```
✅ 规则 1：活体豁免
   能量 > 0 的变量，永远不会被回收
   （活着的生物不会被分解）

❌ 规则 2：尸体分解
   能量 = 0 且连续 3 条指令没人碰 → 🍄 被分解

⏸ 规则 3：活跃期暂停
   BIOME 引种段、控制块执行期间 → 暂停分解
   （演戏进行中，不收拾道具）

🚫 规则 4：顶级豁免
   APEX（虎、狮）永不自动回收
   （顶级掠食者有豁免权）

💀 规则 5：手动灭绝
   EXTINCTION 立即回收，打印遗照
   （强制淘汰）
```

---

## 13.3 EXTINCTION——手动灭绝

```foodchain
EXTINCTION Grass_1 ;   // 立即回收，打印 16 行十六进制"遗照"
```

**遗照示例**：
```
🍄 Grass_1 被分解者回收
[16 行十六进制内存数据]
```

> **趣味用法**：`EXTINCTION Virus_Crash ;` → 强制终止整个程序（病毒只能被灭绝）

---

## ✏️ 第 13 课练习

1. 草能量 100，闲置 10 条指令，会被回收吗？（答案：不会，活体豁免）
2. 草能量 0，闲置 3 条指令，会被回收吗？（答案：会，🍄 分解）
3. APEX（虎）会自动被回收吗？（答案：不会，顶级豁免）
4. `EXTINCTION Virus_Crash ;` 是什么意思？（答案：强制终止程序）

---

# 📗 第 14 课：实战——用 FCL 算阶乘（5! = 120）

> **读完这课你将学会：**
> 综合运用 INTRODUCE、DEVOURS、CLONE、MIGRATION、ROT 写一个完整程序。

---

## 14.1 什么是阶乘？

```
5! = 5 × 4 × 3 × 2 × 1 = 120
n! = n × (n-1) × ... × 2 × 1
```

---

## 14.2 思路

- 虎（APEX）= 累乘器，初始 1
- 狼 = 因子源，从 2 开始
- 狐狸 = 搬运工（克隆狼，被虎吃）
- 羊 = +1 增量器（每轮给狼 +1）
- 真菌 = 输出

**为什么需要狐狸？** 因为虎吃狼会把狼归零——但狼是因子源，下一轮还要用。所以先 `CLONE Fox_1 FROM Wolf_M1`，虎吃狐狸，狼安然无恙。

---

## 14.3 完整代码

```foodchain
GMO ENABLED ;
NUMERIC OUTPUT ;
BIOME {
    INTRODUCE Tiger_1  AS APEX WITH 1 ;    // 累乘器，初始 1
    INTRODUCE Fox_1    AS CARNIVORE WITH 0 ;  // 搬运工
    INTRODUCE Wolf_M1  AS CARNIVORE WITH 2 ;  // 因子源，从 2 开始
    INTRODUCE Sheep_M1 AS HERBIVORE WITH 1 ;  // +1 增量器
    INTRODUCE Fungus_1 AS DECOMPOSER WITH 0 ;  // 输出
}
FOODWEB {
    MIGRATION Sheep_M1 OVER 4 {             // 循环 4 轮（乘 2、3、4、5）
        CLONE Fox_1 FROM Wolf_M1 ;          // ① 克隆狼 → 狐狸
        Tiger_1 DEVOURS Fox_1 USING PROD ;  // ② 虎吃狐狸 → 累乘
        INTRODUCE Sheep_M1 AS HERBIVORE WITH 1 ;  // ③ 重置羊
        Wolf_M1 DEVOURS Sheep_M1 USING SUM ; // ④ 狼 +1 → 下一个因子
    }
}
DECAY {
    Fungus_1 DEVOURS Tiger_1 USING SUM ;
    ROT Fungus_1 TO STDOUT ;
}
```

| 轮次 | 虎（累乘结果）| 狼（因子）|
|---|---|---|
| 初始 | 1 | 2 |
| 轮1 | 1×2=**2** | 3 |
| 轮2 | 2×3=**6** | 4 |
| 轮3 | 6×4=**24** | 5 |
| 轮4 | 24×5=**120** | 6 |

**输出：120** ✅

---

# 📗 第 15 课：图灵完备——FCL 能力极限

> **读完这课你将学会：**
> 1. 理解"图灵完备"是什么意思
> 2. 知道 FCL 做到图灵完备的三件套

---

## 15.1 图灵完备 = 能算任何可计算问题

"图灵完备"听起来很吓人，其实意思很简单：**一个语言/系统如果能模拟通用图灵机，它就是图灵完备的——理论上能解决任何可计算问题**。

> **类比**：一副扑克牌理论上可以玩任何牌类游戏（斗地主、桥牌、21点……），但并不是每副牌都方便——有的规则很别扭，但功能是齐全的。FCL 就是一副"规则别扭但功能齐全"的扑克牌。

---

## 15.2 FCL 的图灵完备三件套

| 能力 | FCL 实现 | 相当于普通语言的 |
|---|---|---|
| **比较** | `ASSESS A AGAINST B TO C` | `>`、`==`、`!=` |
| **条件循环** | `HIBERNATION ... UNTIL APEX` | `while` 循环 |
| **无限存储** | `INTRODUCE`（任意数量的变量）| 变量列表 |

**三件套齐了 = 图灵完备**。

---

## 15.3 FCL 的局限

虽然 FCL 是图灵完备的，但它并不"好用"：

| 局限 | 说明 |
|---|---|
| 能量上限 | HERBIVORE 最多 255，限制大数运算 |
| 数值精度 | PRODUCER 是浮点，有精度误差 |
| 生态约束 | 不能直接赋值，必须用捕食 |
| 没有数组/列表 | 只有标量变量 |

> **但这正是 Esolang 的魅力**：约束就是风格，风格就是语言。

---

# 📗 附录：词汇表（完整版）

| 术语 | 原文 | 含义 |
|---|---|---|
| INTRODUCE | introduce | 引入物种（声明变量） |
| DEVOURS | devours | 吞噬（捕食，破坏性赋值） |
| SUM | sum | 求和（加法）|
| DIFF | difference | 差值（减法，大减小）|
| PROD | product | 乘积（乘法，仅 APEX）|
| QUOT | quotient | 商（除法，仅 APEX）|
| CLONE | clone | 克隆（复制，不破坏原数据）|
| ROT | rot | 腐烂/矿化（输出，仅 DECOMPOSER）|
| ASSESS | assess | 评估（比较，存 APEX）|
| SYMBIOSIS | symbiosis | 互利共生（AND）|
| COMPETITION | competition | 竞争（OR）|
| MIMICRY | mimicry | 拟态（NOT）|
| SEASON | season | 季节（湿度驱动的 if/else）|
| RAIN | rain | 雨季（湿度高时的分支）|
| DRY | dry | 旱季（湿度低时的分支）|
| MIGRATION | migration | 迁徙（定次循环，每轮 sqrt）|
| HIBERNATION | hibernation | 冬眠（条件循环，每轮 ×0.9）|
| MUTATION | mutation | 变异（随机多路选择，1/3 概率）|
| EXTINCTION | extinction | 灭绝（手动回收）|
| GMO | GMO | 转基因（免能量税）|
| PRODUCER | producer | 生产者（第1营养级）|
| HERBIVORE | herbivore | 食草动物（第2营养级）|
| CARNIVORE | carnivore | 食肉动物（第3营养级）|
| APEX | apex | 顶级掠食者（第4营养级）|
| DECOMPOSER | decomposer | 分解者（第5营养级）|
| NUMERIC OUTPUT | — | 数值输出模式 |
| REAL MODE | — | 真实模式（生态等待生效）|
| CODE MODE | — | 代码模式（跳过等待，默认）|
| FULL | full | 饱腹（APEX=1）|
| HUNGRY | hungry | 饥饿（APEX=0）|
| MATCH | match | 变异检测（变异→1，未变→0）|

---

> 教学书 v3.0 ｜ 参考实现：[Huang-520-add/fcl](https://github.com/Huang-520-add/fcl)
> 语法速查：[FCL_REFERENCE.md](FCL_REFERENCE.md) ｜ 生态圈图鉴：[FCL_ECOLOGY.md](FCL_ECOLOGY.md) ｜ 语法规范：[FCL_SYNTAX.md](FCL_SYNTAX.md)
