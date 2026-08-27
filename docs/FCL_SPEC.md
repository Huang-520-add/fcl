# 食物链语言（FCL）官方技术规范 v2.3

发布机构：国际深奥编程语言基金会（IOEF）
协议名称：金字塔协议（Trophic Pyramid Protocol）
设计哲学："代码在吞噬中传递，真理在分解中显现。"
生态圈图鉴：见 [FCL_ECOLOGY.md](FCL_ECOLOGY.md)（在册物种、族群结构、命名规范）
参考实现：C++17 解释器（fcl），g++ -std=c++17 -O2 -o fcl main.cpp 编译

> **v2.0 修订说明**（在 v1.3 基础上）：
> 1. **生态圈世界观**：变量名不再任意，必须使用生态圈在册物种（10 个代表物种，5 个营养级），遵循族群结构（独居/群居、Alpha 首领、M/F 性别标签、编号）。详见 FCL_ECOLOGY.md。
> 2. **新增 ASSESS**（生态位评估/优势种判定）：`ASSESS <A> AGAINST <B> TO <C> ;`，A≥B → C（APEX）= FULL，实现数值比较。
> 3. **新增 SYMBIOSIS**（互利共生）：`SYMBIOSIS <A> WITH <B> TO <C> ;`，C = A AND B。
> 4. **新增 COMPETITION**（替代觅食路径）：`COMPETITION <A> OR <B> TO <C> ;`，C = A OR B。
> 5. **新增 MIMICRY**（拟态）：`MIMICRY <A> TO <B> ;`，B = NOT A。
> 6. **新增 HIBERNATION**（冬眠条件循环）：`HIBERNATION <物种> UNTIL <APEX> { ... }`，循环直到条件 FULL 才醒，每轮冬眠代谢消耗 10% 能量。
> 7. **GC 生态化**：分解者只分解"尸体"（能量耗尽的变量，value==0）；活体（能量>0）不受分解威胁；BIOME 引种段（创造阶段）暂停分解。
> 8. **图灵完备**：比较 + 布尔逻辑 + 条件循环齐备，FCL 可表达一切可计算问题。
> 9. v1.2.1/v1.3 的修订（WITH 规则、分解者豁免、GMO 标识、CLONE、GC 活跃期暂停等）全部保留。

## 1. 概述

FCL（FoodChain Language）是一门专为生物学爱好者和编程初学者设计的深奥编程语言（Esolang）。它将变量映射为生态系统的物种，将运算映射为营养级间的能量传递，让学习者在编写代码的过程中巩固生态学核心概念——生产者、消费者、分解者、能量传递效率、食物链层级约束、族群社会结构。

**FCL 的本质：推演生态阶的变化。** 每一行代码都是生态事件，每一次运行都是一次生态演替。

核心教育目标：
- 让生物专业学生绕过传统编程语法障碍，用熟悉的食物链知识直接上手写逻辑。
- 让每次变量赋值都变成一次"啃食"，每次循环都变成一次"迁徙"，强制记忆营养级顺序。
- 通过"能量传递税（20%）"和"胃溃疡溢出"等机制，直观感受生态学热力学定律在计算中的体现。

适用场景：中学生物与信息技术跨学科课程设计；大学编程通识课趣味实验；生态建模教学辅助工具。
不适用场景：生产环境、金融系统、航天器控制等严肃工程场景。

## 2. 字符集与词法规范

- 有效字符：大写拉丁字母（A-Z）、下划线（_）、数字（0-9）、分号（;）、花括号（{}）及换行符。
- **标识符规则（生态圈命名制）**：变量名必须使用生态圈在册物种 + 族群标识，格式由族群结构决定：
  - 群居物种（狼/狮/羊）：首领 `Alpha_<物种>`（狼王 Alpha_Wolf）；成员 `<物种>_<M|F><编号>`（Wolf_M1 雄性1号、Sheep_F2 雌性2号）
  - 独居物种（虎/狐/兔）：`<物种>_<编号>`（Tiger_1）
  - 生产者/分解者（草/藻/真菌/杆菌）：`<物种>_<编号>`（Grass_1、Fungus_1）
  - 物种名必须在册（Grass/Algae/Sheep/Rabbit/Wolf/Fox/Tiger/Lion/Fungus/Bacillus），且营养级必须与声明类型一致
- 违规报错：生态圈外物种 → `🌿 外来物种入侵，生态圈不予接纳！`；营养级错配 → `⚠️ 分类学混乱！`；族群格式错误 → `⚠️ 族谱登记混乱！`
- 关键字强制大写；小写内容视为注释（第 8 节）。
- 空白字符在非关键字位置被忽略。

## 3. 程序强制性三段式结构

任何合法 FCL 程序必须严格包含以下三个代码块，顺序不可颠倒：

```foodchain
BIOME {
/* 引种段：仅允许变量声明与初始能量注入 */
}
FOODWEB {
/* 捕食段：核心运算逻辑，必须包含至少一次 DEVOURS 操作 */
}
DECAY {
/* 分解段：负责输出与内存回收 */
}
```

违规惩罚：缺少任一区块，编译器输出 `🌍 生态崩溃，食物链断裂！` 并终止。

## 4. 数据类型与营养级

| 类型关键字 | 别名（营养级） | 存储范围 | 特性 |
|---|---|---|---|
| PRODUCER | 生产者（1级） | 浮点（0.0~9999.9） | 支持分数输入（如 3/2，存为 1.5） |
| HERBIVORE | 初级消费者（2级） | 整型（0~255） | 胃溃疡溢出：超过 255 归零并触发 2 秒线程阻塞 |
| CARNIVORE | 次级消费者（3级） | 整型（0~65535） | 运算速度与代码行号奇偶性挂钩（奇数行加速，偶数行减速） |
| APEX | 顶级掠食者（4级） | 布尔（FULL=1 / HUNGRY=0） | 逻辑判断依赖"扑咬距离"（变量在内存中的地址偏移量） |
| DECOMPOSER | 分解者（5级） | 字符串（数值按 ASCII 解释） | 输出时按 ASCII 字符渲染 |

内部存储：所有变量统一存储 `double value` + 营养级标签；HERBIVORE/CARNIVORE 运算后取整（四舍五入）。

## 5. 指令集

### 5.1 变量声明（引种）

```ebnf
INTRODUCE <标识符> AS <营养级> WITH <能量表达式> ;
```

示例：`INTRODUCE Trifolium_Repens AS PRODUCER WITH 1/3 ;`
能量表达式支持：纯数字、分数（a/b）、四则运算（+ - * /）与括号。`WITH 0 ;` 合法。

### 5.2 数据传递（啃食/捕食）

```ebnf
<捕食者> DEVOURS <猎物> USING <算法> ;
```

硬性规则：
- 两者营养级必须相差恰好 1，且捕食者营养级 > 猎物营养级。
- **豁免条款**：DECAY 块内，DECOMPOSER 可捕食任意营养级，且回收效率 100%（不扣税）。
- 算法：
  - SUM（加法，能量合并）
  - DIFF（减法，大减小，负值则捕食者饿死报错 `🥀`）
  - PROD（乘法，仅限 APEX 级使用，消耗双倍时间片）
  - QUOT（除法，仅限 APEX 级使用，余数丢弃）
- APEX 的 PROD/QUOT 受"扑咬距离"影响：捕食者与猎物存储地址差为偶数时扑咬落空，结果减半（`🐾`）。
- 捕食成功后猎物能量归零（被消化）。

### 5.3 能量传递税（热力学第二定律）

每次 DEVOURS 操作（DECAY 块内分解者除外），能量传递效率强制为 20%：`实际结果 = 理论结果 × 0.2`。

豁免条款：程序首行添加 `GMO ENABLED ;` 恢复 100% 效率，但每次 ROT 输出前打印 🧬 转基因产品标识。

### 5.4 输入（SPROUT）

只能注入 PRODUCER。语法：`SPROUT <生产者名> FROM STDIN ;`
控制台播放 2 秒随机音调，程序员需在声音停止前按下空格键"捕捉"数值，超时则输入为 0。

### 5.5 输出（ROT）

只能由 DECOMPOSER 执行。语法：`ROT <分解者名> TO STDOUT ;`
- 第一次 ROT：按 ASCII 字符输出（能量 65 输出 'A'）。
- 连续第二次 ROT：输出 Unicode 码位（格式 `U+XXXX`）。

### 5.6 无性繁殖（克隆）

```ebnf
CLONE <目标> FROM <源> ;
```

目标获得源的值的副本（克隆不消耗源，源的能量不受影响）。目标必须已通过 INTRODUCE 引种（否则类型未知）。生态依据：营养繁殖/克隆技术（扦插、多利羊）。

示例：`CLONE A FROM T ;` —— A 变成 T 的副本。

> **设计动机**：FCL 的能量流严格单向（猎物被吃即归零，值只能流向更高营养级），导致"回写"（如斐波那契递推中的 `Y_new = X_old`）无法实现。CLONE 是生态学上最自然的复制机制，补上了这一环。

### 5.7 生态位评估与逻辑（v2.0）

布尔量由 APEX 物种承载（FULL=1 / HUNGRY=0）——只有顶级掠食者拥有"裁决权"。

```ebnf
ASSESS <A> AGAINST <B> TO <C> ;      (* 生态位评估：A≥B → C=FULL *)
SYMBIOSIS <A> WITH <B> TO <C> ;      (* 互利共生：C = A AND B *)
COMPETITION <A> OR <B> TO <C> ;      (* 替代觅食路径：C = A OR B *)
MIMICRY <A> TO <B> ;                 (* 拟态：B = NOT A *)
```

- ASSESS 的 A、B 可为任意营养级；C 必须为 APEX。
- SYMBIOSIS/COMPETITION 的 A、B、C 必须全部为 APEX。
- MIMICRY 的 A、B 必须为 APEX。
- 非 APEX 承载判定 → `🦴 食性冲突，只有顶级掠食者（APEX）能承载判定`。

### 5.8 冬眠（条件循环，v2.0）

```ebnf
HIBERNATION <物种> UNTIL <APEX变量> { ... }
```

循环执行块内语句，直到指定 APEX 变量为 FULL 才"苏醒"（while 循环）。每轮迭代该物种能量按冬眠代谢消耗 10%（×0.9）。循环超过 10000 轮强制唤醒（防止生态僵死）。

## 6. 控制流

FCL 没有传统 if/for，一切逻辑由环境关键字驱动：

- **条件分支**：`SEASON RAIN { ... } DRY { ... }`（二选一）
  湿度 = 最近 3 条指令中 DEVOURS 次数；≥2 为湿润 → 执行 RAIN，否则执行 DRY。
  若 RAIN 块内语句超过 3 条，RAIN 自动降级为 STORM，块内语句随机乱序执行一次。
- **循环**：`MIGRATION <物种> OVER <次数> { ... }`
  每轮迭代，该物种能量值自动开平方根衰减（模拟长途迁徙消耗）。
- **多路选择**：`MUTATION <变量> { CASE "特征": ... }`
  变量名在运行时会被随机替换为同音异形词（如 Canis → Kannis），需用内置正则函数 MATCH() 捕获。

## 7. 垃圾回收

- **自动回收**：分解者只分解"尸体"——能量耗尽的变量（value==0）连续 3 条指令未被引用才触发 DECOMPOSE；**活体（能量>0）不受分解威胁**。释放过程随机阻塞主线程 100ms~1000ms（模拟微生物分解速度）。APEX 级变量豁免。**活跃期暂停**：BIOME 引种段（创造阶段）与复合控制块（MIGRATION/SEASON/MUTATION/HIBERNATION）执行期间分解作用暂停。
- **手动回收**：`EXTINCTION <变量名> ;`
  立即回收，并打印该变量内存的十六进制"遗照"（16 行 dump）。`EXTINCTION Virus_Crash ;` 强制终止进程。

## 8. 注释

```ebnf
OBSERVATION: YYYY-MM-DD, Lat:<纬度>, Lon:<经度>, <内容>
```

示例：`OBSERVATION: 2026-08-27, Lat:36.1, Lon:-115.1, 此处代码需优化`
缺少日期或坐标 → 忽略并警告 `🔭 缺乏科考精神！`。小写内容行同样视为注释。

## 9. 错误信息对照表

| 传统报错 | FCL 报错 |
|---|---|
| Syntax Error | 🌿 变异物种入侵，语法免疫系统失效 |
| Type Mismatch | 🦴 食性冲突，捕食者拒绝进食 |
| Out of Memory | 💀 生物大灭绝，请减少繁殖（循环） |
| Timeout | ❄️ 冬眠期过长，进程休眠 |
| Division by Zero | 🔥 干旱导致食物链断裂 |
| 分类学违规 | ⚠️ 分类学混乱！ |
| 结构违规 | 🌍 生态崩溃，食物链断裂！ |
| 饿死（负值） | 🥀 捕食者饿死，能量为负 |
| 溢出 | 🤢 胃溃疡溢出，能量归零 |

## 附录 A：完整代码示例（参考实现已验证）

### 示例 1（原版，GMO 免税，展示规范输出）

```foodchain
GMO ENABLED ;
BIOME {
INTRODUCE Algae_A AS PRODUCER WITH (1+2) ;
INTRODUCE Algae_B AS PRODUCER WITH (3+2) ;
INTRODUCE Zooplankton AS HERBIVORE WITH 0 ;
}
FOODWEB {
Zooplankton DEVOURS Algae_A USING SUM ;
Zooplankton DEVOURS Algae_B USING SUM ;
}
DECAY {
INTRODUCE Bacteria_Ecoli AS DECOMPOSER WITH 0 ;
Bacteria_Ecoli DEVOURS Zooplankton USING SUM ;
ROT Bacteria_Ecoli TO STDOUT ;
ROT Bacteria_Ecoli TO STDOUT ;
}
```

推演：Algae_A=3, Algae_B=5，GMO 免税 → Zooplankton=8 → 分解者全额回收 8 → 输出 ASCII 8（退格符）+ `U+0008`。实测输出：`🧬\b🧬U+0008`。

### 示例 2（有税，展示能量传递效率 20%）

```foodchain
BIOME {
INTRODUCE Grass AS PRODUCER WITH 30+35 ;
INTRODUCE Sheep AS HERBIVORE WITH 0 ;
}
FOODWEB {
Sheep DEVOURS Grass USING SUM ;
}
DECAY {
INTRODUCE Fungus AS DECOMPOSER WITH 0 ;
Fungus DEVOURS Sheep USING SUM ;
ROT Fungus TO STDOUT ;
ROT Fungus TO STDOUT ;
}
```

推演：Grass=65 → Sheep=65×0.2=13 → Fungus=13 → 输出 ASCII 13（回车符）+ `U+000D`。

### 示例 3（推荐演示：输出可见字符 'A'）

```foodchain
GMO ENABLED ;
BIOME {
INTRODUCE Grass AS PRODUCER WITH 60+5 ;
INTRODUCE Sheep AS HERBIVORE WITH 0 ;
}
FOODWEB {
Sheep DEVOURS Grass USING SUM ;
}
DECAY {
INTRODUCE Fungus AS DECOMPOSER WITH 0 ;
Fungus DEVOURS Sheep USING SUM ;
ROT Fungus TO STDOUT ;
ROT Fungus TO STDOUT ;
}
```

推演：Grass=65，GMO 免税 → Sheep=65 → Fungus=65 → 输出 `'A'` + `U+0041`。实测输出：`🧬A🧬U+0041`。

### 示例 4（斐波那契：MIGRATION + CLONE 递推，输出 F(7)=13）

```foodchain
GMO ENABLED ;
BIOME {
INTRODUCE A AS PRODUCER WITH 1 ;
INTRODUCE B AS PRODUCER WITH 1 ;
INTRODUCE H AS HERBIVORE WITH 0 ;
INTRODUCE T AS PRODUCER WITH 0 ;
INTRODUCE Fungus AS DECOMPOSER WITH 0 ;
}
FOODWEB {
MIGRATION H OVER 5 {
CLONE T FROM B ;
H DEVOURS A USING SUM ;
H DEVOURS B USING SUM ;
CLONE A FROM T ;
CLONE B FROM H ;
INTRODUCE H AS HERBIVORE WITH 0 ;
INTRODUCE T AS PRODUCER WITH 0 ;
}
}
DECAY {
Fungus DEVOURS B USING SUM ;
ROT Fungus TO STDOUT ;
ROT Fungus TO STDOUT ;
}
```

递推逻辑（A=F(n-1), B=F(n)，H 为累加器，T 为备份）：
1. `CLONE T FROM B`：备份 F(n) 到 T（否则 B 被吃后旧值丢失）
2. `H DEVOURS A`、`H DEVOURS B`：H = F(n-1) + F(n) = F(n+1)
3. `CLONE A FROM T`：A = F(n)（旧 B 的值）→ 成为下一轮的 F(n-1)
4. `CLONE B FROM H`：B = F(n+1) → 成为下一轮的 F(n)
5. 重置 H/T（INTRODUCE 覆盖为 0）

推演：轮 1 → A=1, B=2；轮 2 → A=2, B=3；轮 3 → A=3, B=5；轮 4 → A=5, B=8；轮 5 → A=8, **B=13=F(7)**。
DECAY 输出：ASCII 13（回车符）+ `U+000D`。实测输出：`🧬\r🧬U+000D`。

## 附录 B：标准库（预置物种）

- `Bacillus_Stdio`：标准输入输出分解者（自动处理摩斯电码）
- `Rhizobium_Math`：内置高精度乘法共生菌（可被 APEX 调用）
- `Virus_Crash`：专门用于强制退出程序（`EXTINCTION Virus_Crash ;`）

## 附录 C：生态观察（趣味输出）

- 🔬 建议使用双名法（属名_种加词）——单名变量提醒
- 🍄 `<变量> 被分解者回收`——GC 触发
- 🧬 变异：`Canis → Kannis`——MUTATION 触发
- 🐾 扑咬落空，能量减半——APEX 扑咬距离判定
- 🌩️ RAIN 降级为 STORM——条件块语句超限

文档版本：v1.2.1
最后更新：2026-08-27
