# 食物链语言（FCL）官方技术规范 v3.1.0

发布机构：国际深奥编程语言基金会（IOEF）
协议名称：金字塔协议（Trophic Pyramid Protocol）
设计哲学："代码在吞噬中传递，真理在分解中显现。"
生态圈图鉴：见 [FCL_ECOLOGY.md](FCL_ECOLOGY.md)（在册物种、族群结构、命名规范）
参考实现：C++17 解释器（fcl），`make build` 或 `g++ -std=c++17 -O2 -o fcl src/main.cpp src/interpreter.cpp src/parser.cpp src/expr.cpp src/ecology.cpp` 编译（多文件模块，非单文件）。

> **v2.0 修订说明**（在 v1.3 基础上）：
> 1. **生态圈世界观**：变量名不再任意，必须使用生态圈在册物种（10 个代表物种，5 个营养级），遵循族群结构（独居/群居、Alpha 首领、M/F 性别标签、编号）。详见 FCL_ECOLOGY.md。
> 2. **新增 ASSESS**（生态位评估/优势种判定）：`ASSESS <A> AGAINST <B> TO <C> ;`，A≥B → C（APEX）= FULL，实现数值比较。
> 3. **新增 SYMBIOSIS**（互利共生）：`SYMBIOSIS <A> WITH <B> TO <C> ;`，C = A AND B。
> 4. **新增 COMPETITION**（替代觅食路径）：`COMPETITION <A> OR <B> TO <C> ;`，C = A OR B。
> 5. **新增 MIMICRY**（拟态）：`MIMICRY <A> TO <B> ;`，B = NOT A。
> 6. **新增 HIBERNATION**（冬眠条件循环）：`HIBERNATION <物种> UNTIL <APEX> { ... }`，循环直到条件 FULL 才醒，每轮冬眠代谢消耗 10% 能量。
> 7. **GC 生态化**：分解者只分解"尸体"（能量耗尽的变量，value==0）；活体（能量>0）不受分解威胁；BIOME 引种段（创造阶段）暂停分解。
> 8. **图灵完备（自 v3.1.0 起）**：内建无界存储带（TAPE）+ 可移动读写头（FORWARD/BACKWARD）+ 算术（BUMP）+ 读写（LOAD/STORE）+ 无界循环（WHILE），与 Brainfuck / 图灵机等价；比较、布尔逻辑、条件循环（HIBERNATION）等旧特性全部保留。
> 9. v1.2.1/v1.3 的修订（WITH 规则、分解者豁免、GMO 标识、CLONE、GC 活跃期暂停等）全部保留。

## 1. 概述

FCL（FoodChain Language）是一门专为生物学爱好者和编程初学者设计的深奥编程语言（Esolang）。它将变量映射为生态系统的物种，将运算映射为营养级间的能量传递，让学习者在编写代码的过程中巩固生态学核心概念——生产者、消费者、分解者、能量传递效率、食物链层级约束、族群社会结构。

**FCL 的本质：推演生态阶的变化。** 每一行代码都是生态事件，每一次运行都是一次生态演替。

核心教育目标：
- 让生物专业学生绕过传统编程语法障碍，用熟悉的食物链知识直接上手写逻辑。
- 让每次变量赋值都变成一次"啃食"，每次循环都变成一次"迁徙"，强制记忆营养级顺序。
- 通过"能量传递效率 20%"和"胃溃疡溢出"等机制，直观感受生态学热力学定律在计算中的体现。

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
/* 捕食段：核心运算逻辑，必须包含至少一次捕食行为（DEVOURS / SCENT / POUNCE） */
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

示例：`INTRODUCE Grass_1 AS PRODUCER WITH 1/3 ;`
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

### 5.3 能量传递效率（热力学第二定律）

每次 DEVOURS 操作（DECAY 块内分解者除外），能量传递效率强制为 20%：`实际结果 = 理论结果 × 0.2`。

豁免条款：程序首行添加 `GMO ENABLED ;` 恢复 100% 效率；REAL MODE 下每次 ROT 输出前打印 🧬 转基因产品标识，CODE MODE（默认）不显示（变异静默生效）。

### 5.4 输入（SCENT/LURK/POUNCE 组合）

v3.0 废除单一输入原语 SPROUT（阻塞式读入 PRODUCER）：深奥语言的输入不是一条指令，而是**三个原子行为的组合**——嗅探（SCENT）→ 潜伏（LURK）→ 猛扑（POUNCE），单独一个原语几乎无用，组合起来才能捕获猎物。三个原语**只能出现在 FOODWEB 块内**（捕食/运算的发生地；FOODWEB 有效性校验接受 DEVOURS / SCENT / POUNCE）。

- **SCENT（嗅探，非阻塞）**：`SCENT <嗅探者> TO <APEX变量> ;` —— 探测 STDIN 是否有数据就绪：就绪 → 向 APEX 物种变量存入 1.0，否则存入 0.0。输出 `👃 X 嗅探风中气味 → Y FULL（嗅到猎物）` 或 `HUNGRY（无气味）`。结果变量必须是 APEX 营养级物种（Tiger/Lion）。
- **LURK（潜伏，休眠等待）**：`LURK <物种> FOR <节拍数> ;` —— 等待 N 拍（真实模式 100ms/拍，代码模式 1ms/拍，钳制 0–600）。输出 `🕳️ X 潜伏 N 拍`。物种必须已注册。
- **POUNCE（猛扑，非阻塞）**：`POUNCE <捕食者> ;` —— STDIN 就绪 → 读入数值存入该物种（输出 `🦅 X 猛扑命中，捕获能量 N`）；未就绪 → 扑空、能量保持（`🐾 X 扑空（无猎物气味），能量保持`）；EOF/非法输入视为腐坏猎物（`🦠 X 扑到的猎物已腐坏，能量保持`）。WASM/浏览器构建中永远扑空。

组合范式（替代旧 SPROUT）：

```foodchain
SCENT Wolf_M1 TO Tiger_1 ;
HIBERNATION Wolf_M1 UNTIL Tiger_1 { LURK Wolf_M1 FOR 10 ; SCENT Wolf_M1 TO Tiger_1 ; }
POUNCE Wolf_M1 ;
```

### 5.5 输出（ROT）

只能由 DECOMPOSER 执行。语法：`ROT <分解者名> TO STDOUT ;`
- 第一次 ROT：按 ASCII 字符输出（能量 65 输出 'A'）。
- 连续第二次 ROT：输出 Unicode 码位（格式 `U+XXXX`）。

### 5.6 无性繁殖（克隆）

```ebnf
CLONE <目标> FROM <源> ;
```

目标获得源的值的副本（克隆不消耗源，源的能量不受影响）。目标必须已通过 INTRODUCE 引种（否则类型未知）。生态依据：营养繁殖/克隆技术（扦插、多利羊）。

示例：`CLONE Grass_2 FROM Grass_1 ;` —— A 变成 T 的副本。

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
  RAIN 默认按书写顺序执行；仅在程序首行声明 `STORM ENABLED ;` 后，RAIN 块内语句数 >3 时块内语句才随机乱序执行一次（🌩️）。**默认（未声明 STORM）RAIN 绝不乱序**，保证确定性。
- **循环**：`MIGRATION <物种> OVER <次数> { ... }`
  每轮迭代，该物种能量值自动开平方根衰减（模拟长途迁徙消耗）。
- **多路选择**：`MUTATION <变量> { CASE "特征": ... }`
  该物种在运行时以 1/3 概率变异：根名替换为变异名（如 Wolf → Wolv、Grass → Grasse、Tiger → Tygre，全表见生态圈图鉴），块内引用随之改写（块结束后恢复原名）；触发时等概率随机表达一个 CASE 分支，未触发则块空转；可用 MATCH() 按物种根名检测。

## 7. 垃圾回收

- **自动回收**：分解者只分解"尸体"——能量耗尽的变量（value==0）连续 3 条指令未被引用才触发 DECOMPOSE；**活体（能量>0）不受分解威胁**。释放过程在真实模式下随机阻塞主线程 100ms~1000ms（模拟微生物分解速度）。APEX 级变量豁免。**活跃期暂停**：BIOME 引种段（创造阶段）与复合控制块（MIGRATION/SEASON/MUTATION/HIBERNATION）执行期间分解作用暂停。
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
INTRODUCE Algae_1 AS PRODUCER WITH (1+2) ;
INTRODUCE Algae_2 AS PRODUCER WITH (3+2) ;
INTRODUCE Sheep_M1 AS HERBIVORE WITH 0 ;
}
FOODWEB {
Sheep_M1 DEVOURS Algae_1 USING SUM ;
Sheep_M1 DEVOURS Algae_2 USING SUM ;
}
DECAY {
INTRODUCE Bacillus_1 AS DECOMPOSER WITH 0 ;
Bacillus_1 DEVOURS Sheep_M1 USING SUM ;
ROT Bacillus_1 TO STDOUT ;
ROT Bacillus_1 TO STDOUT ;
}
```

推演：Algae_1=3, Algae_2=5，GMO 免税 → Sheep_M1=8 → 分解者全额回收 8 → 输出 ASCII 8（退格符）+ `U+0008`。实测输出：`🧬\b🧬U+0008`。

### 示例 2（有税，展示能量传递效率 20%）

```foodchain
BIOME {
INTRODUCE Grass_1 AS PRODUCER WITH 30+35 ;
INTRODUCE Sheep_M1 AS HERBIVORE WITH 0 ;
}
FOODWEB {
Sheep_M1 DEVOURS Grass_1 USING SUM ;
}
DECAY {
INTRODUCE Fungus_1 AS DECOMPOSER WITH 0 ;
Fungus_1 DEVOURS Sheep_M1 USING SUM ;
ROT Fungus_1 TO STDOUT ;
ROT Fungus_1 TO STDOUT ;
}
```

推演：Grass_1=65 → Sheep_M1=65×0.2=13 → Fungus_1=13 → 输出 ASCII 13（回车符）+ `U+000D`。

### 示例 3（推荐演示：输出可见字符 'A'）

```foodchain
GMO ENABLED ;
BIOME {
INTRODUCE Grass_1 AS PRODUCER WITH 60+5 ;
INTRODUCE Sheep_M1 AS HERBIVORE WITH 0 ;
}
FOODWEB {
Sheep_M1 DEVOURS Grass_1 USING SUM ;
}
DECAY {
INTRODUCE Fungus_1 AS DECOMPOSER WITH 0 ;
Fungus_1 DEVOURS Sheep_M1 USING SUM ;
ROT Fungus_1 TO STDOUT ;
ROT Fungus_1 TO STDOUT ;
}
```

推演：Grass_1=65，GMO 免税 → Sheep_M1=65 → Fungus_1=65 → 输出 `'A'` + `U+0041`。实测输出：`🧬A🧬U+0041`。

### 示例 4（斐波那契：MIGRATION + CLONE 递推，输出 F(7)=13）

```foodchain
GMO ENABLED ;
BIOME {
INTRODUCE Grass_1 AS PRODUCER WITH 1 ;
INTRODUCE Grass_2 AS PRODUCER WITH 1 ;
INTRODUCE Sheep_M1 AS HERBIVORE WITH 0 ;
INTRODUCE Algae_1 AS PRODUCER WITH 0 ;
INTRODUCE Fungus_1 AS DECOMPOSER WITH 0 ;
}
FOODWEB {
MIGRATION Sheep_M1 OVER 5 {
CLONE Algae_1 FROM Grass_2 ;
Sheep_M1 DEVOURS Grass_1 USING SUM ;
Sheep_M1 DEVOURS Grass_2 USING SUM ;
CLONE Grass_1 FROM Algae_1 ;
CLONE Grass_2 FROM Sheep_M1 ;
INTRODUCE Sheep_M1 AS HERBIVORE WITH 0 ;
INTRODUCE Algae_1 AS PRODUCER WITH 0 ;
}
}
DECAY {
Fungus_1 DEVOURS Grass_2 USING SUM ;
ROT Fungus_1 TO STDOUT ;
ROT Fungus_1 TO STDOUT ;
}
```

递推逻辑（Grass_1=F(n-1), Grass_2=F(n)，Sheep_M1 为累加器，Algae_1 为备份）：
1. `CLONE Algae_1 FROM Grass_2`：备份 F(n) 到 Algae_1（否则 Grass_2 被吃后旧值丢失）
2. `Sheep_M1 DEVOURS Grass_1`、`Sheep_M1 DEVOURS Grass_2`：Sheep_M1 = F(n-1) + F(n) = F(n+1)
3. `CLONE Grass_1 FROM Algae_1`：Grass_1 = F(n)（旧 Grass_2 的值）→ 成为下一轮的 F(n-1)
4. `CLONE Grass_2 FROM Sheep_M1`：Grass_2 = F(n+1) → 成为下一轮的 F(n)
5. 重置 Sheep_M1/Algae_1（INTRODUCE 覆盖为 0）

推演：轮 1 → Grass_1=1, Grass_2=2；轮 2 → Grass_1=2, Grass_2=3；轮 3 → Grass_1=3, Grass_2=5；轮 4 → Grass_1=5, Grass_2=8；轮 5 → Grass_1=8，**Grass_2=13=F(7)**。
DECAY 输出：ASCII 13（回车符）+ `U+000D`。实测输出：CODE 模式 `\r` + `U+000D`（REAL 模式为 `🧬\r🧬U+000D`）。

## 附录 B：标准库（预置物种）

- `Virus_Crash`：专门用于强制退出程序（`EXTINCTION Virus_Crash ;`）

## 附录 C：生态观察（趣味输出）

- 🔬 建议使用双名法（属名_种加词）——单名变量提醒
- 🍄 `<变量> 被分解者回收`——GC 触发
- 🧬 变异：`Wolf → Wolv`——MUTATION 触发（1/3 概率改名）
- 🐾 扑咬落空，能量减半——APEX 扑咬距离判定
- 🌩️ RAIN 块语句随机乱序——仅在程序首行 `STORM ENABLED` 后、且块内 >3 条语句时发生

文档版本：v3.1.0
最后更新：2026-08-30


