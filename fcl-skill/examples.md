# FCL 示例集（v3.0.2）

> 示例 1~3 为官方教程原文，逐字保留。示例 4~5 为按材料语法自写的简短示例，严格符合命名规范、营养级限制与指令位置约束。

---

## 示例1：Hello World（三段式 + INTRODUCE + DEVOURS + ROT + GMO）

演示语法点：全局开关 `GMO ENABLED`、三段式结构、BIOME 内 `INTRODUCE`、FOODWEB 内 `DEVOURS`（捕食者 HERBIVORE 吃 PRODUCER，级差恰1，GMO 免税）、DECAY 内 DECOMPOSER 豁免捕食(`Fungus_1 DEVOURS Sheep_M1`)、`ROT` 两次输出 ASCII。

```fc
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

注解：
- `Grass_1 = 65`（60+5）。`GMO ENABLED` → 能量税 0%。`Sheep_M1 DEVOURS Grass_1 USING SUM` → `Sheep_M1 = 65`，`Grass_1` 归零。
- DECAY 段内 DECOMPOSER（`Fungus_1`）豁免，可吃任意级 100%：`Fungus_1 = 65`。
- `ROT Fungus_1` 首次输出 `char(65)` = `A`，第二次输出 `char(65)` = `A` → 屏幕打印 `AA`。

---

## 示例2：斐波那契（CLONE + MIGRATION）

演示语法点：`NUMERIC OUTPUT`、`CLONE <目标> FROM <源>`、FOODWEB 内 `MIGRATION ... OVER <次数> { }` 定次循环（每轮后物种能量开平方根衰减）、循环内复用 INTRODUCE 重置能量、斐波那契递推。

```fc
GMO ENABLED ;
NUMERIC OUTPUT ;
BIOME {
    INTRODUCE Grass_1  AS PRODUCER WITH 1 ;
    INTRODUCE Grass_2  AS PRODUCER WITH 1 ;
    INTRODUCE Sheep_M1 AS HERBIVORE WITH 0 ;
    INTRODUCE Algae_1  AS PRODUCER WITH 0 ;
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
        INTRODUCE Algae_1  AS PRODUCER WITH 0 ;
    }
}
DECAY {
    Fungus_1 DEVOURS Grass_2 USING SUM ;
    ROT Fungus_1 TO STDOUT ;
    ROT Fungus_1 TO STDOUT ;
}
```

注解：
- `CLONE Algae_1 FROM Grass_2`：Algae_1 获得 Grass_2 副本，Grass_2 不变。
- 循环体内用 `CLONE` 把递推值前移（Grass_1←Algae_1 旧值，Grass_2←Sheep_M1 累加值），实现斐波那契。
- 每轮后 `Sheep_M1` 能量开平方根衰减；循环内 `INTRODUCE ... WITH 0` 重置能量供下一轮累加。
- DECAY 段 `NUMERIC OUTPUT` 使 `ROT` 直出数值（斐波那契末项）。

---

## 示例3：SEASON 湿度分支

演示语法点：`SEASON RAIN { } DRY { }`、湿度由最近 3 条 DEVOURS 次数决定（≥2→RAIN 否则 DRY）、分支内 `DEVOURS` 与 `INTRODUCE`。

```fc
GMO ENABLED ;
NUMERIC OUTPUT ;
BIOME {
    INTRODUCE Grass_1 AS PRODUCER WITH 10 ;
    INTRODUCE Grass_2 AS PRODUCER WITH 20 ;
    INTRODUCE Sheep_M1 AS HERBIVORE WITH 0 ;
    INTRODUCE Fungus_1 AS DECOMPOSER WITH 0 ;
}
FOODWEB {
    Sheep_M1 DEVOURS Grass_1 USING SUM ;
    Sheep_M1 DEVOURS Grass_2 USING SUM ;
    SEASON RAIN {
        Fungus_1 DEVOURS Sheep_M1 USING SUM ;
    } DRY {
        INTRODUCE Fungus_1 AS DECOMPOSER WITH 1 ;
    }
}
DECAY {
    ROT Fungus_1 TO STDOUT ;
}
```

注解：
- FOODWEB 内有 2 条 `DEVOURS` → 最近3条计数 = 2 ≥ 2 → 进入 RAIN 块：`Fungus_1` 捕食 `Sheep_M1`（DECAY 外但在 FOODWEB，按正常捕食规则；此处走 RAIN 分支）。
- `NUMERIC OUTPUT` 使最后 `ROT` 直出数值。

---

## 示例4：HIBERNATION 短例（自写，严格符合规则）

演示语法点：FOODWEB 内 `HIBERNATION <物种> UNTIL <APEX变量> { }` while 循环（直到 APEX=FULL 停止，每轮物种能量 ×0.9，超 10000 轮强制唤醒）、`ASSESS <A> AGAINST <B> TO <C>`（C 须 APEX）、DECAY 内 DECOMPOSER 捕食与 `NUMERIC OUTPUT` 直出数值。

```fc
GMO ENABLED ;
NUMERIC OUTPUT ;
BIOME {
    INTRODUCE Grass_1  AS PRODUCER WITH 100 ;
    INTRODUCE Algae_1  AS PRODUCER WITH 0 ;
    INTRODUCE Sheep_M1 AS HERBIVORE WITH 0 ;
    INTRODUCE Tiger_1  AS APEX WITH 0 ;
    INTRODUCE Fungus_1 AS DECOMPOSER WITH 0 ;
}
FOODWEB {
    Sheep_M1 DEVOURS Grass_1 USING SUM ;           // Sheep_M1 = 100, Grass_1 -> 0（保证 FOODWEB 至少一次 DEVOURS，满足 FCL-0006）
    HIBERNATION Sheep_M1 UNTIL Tiger_1 {            // 每轮 Sheep_M1 *= 0.9，直到 Tiger_1 = FULL
        ASSESS Algae_1 AGAINST Grass_1 TO Tiger_1 ; // Algae_1(0) >= Grass_1(0) -> Tiger_1 = FULL(1)
    }
}
DECAY {
    Fungus_1 DEVOURS Sheep_M1 USING SUM ;           // DECOMPOSER 豁免：可吃任意级 100%
    ROT Fungus_1 TO STDOUT ;                        // NUMERIC OUTPUT -> 直出 Sheep_M1 衰减后数值
}
```

注解：
- `Tiger_1`（APEX）初始 HUNGRY(0)。HIBERNATION 进入循环：首轮 `ASSESS Algae_1 AGAINST Grass_1 TO Tiger_1` 中 `0 >= 0` 成立 → `Tiger_1 = FULL(1)`，循环条件（直到 FULL）满足 → 退出。循环共执行 1 轮，`Sheep_M1 = 100 × 0.9 = 90`（HERBIVORE 为整数，截断为 90）。
- 若需多轮衰减，可在循环体内让 `Tiger_1` 保持 HUNGRY 若干轮后再置 FULL（本例为展示语法的最小正确形态）。
- `Fungus_1 DEVOURS Sheep_M1` 放在 DECAY 段：DECOMPOSER 在 DECAY 内豁免，可吃任意级 100%；`ROT` 仅允许在 DECAY。

---

## 示例5：MUTATION 短例（自写，严格符合规则）

演示语法点：FOODWEB 内 `MUTATION <物种> { CASE "特征": <语句> ... }`（1/3 概率变异，根名替换 Wolf→Wolv，块内引用随之改写，触发随机表达一个 CASE，未触发空转）、`MATCH(标识符)` 在表达式中检测变异（变异→1 / 未变→0）、DECAY 内 DECOMPOSER 捕食与 `NUMERIC OUTPUT`。

```fc
GMO ENABLED ;
NUMERIC OUTPUT ;
BIOME {
    INTRODUCE Wolf_M1  AS CARNIVORE WITH 5 ;
    INTRODUCE Sheep_M1 AS HERBIVORE WITH 2 ;
    INTRODUCE Fungus_1 AS DECOMPOSER WITH 0 ;
}
FOODWEB {
    Wolf_M1 DEVOURS Sheep_M1 USING SUM ;           // 保证 FOODWEB 至少一次 DEVOURS（满足 FCL-0006）；CARNIVORE>HERBIVORE 级差恰1
    MUTATION Wolf_M1 {
        CASE "sharp_teeth": Wolf_M1 DEVOURS Sheep_M1 USING SUM ;  // 级差1：CARNIVORE>HERBIVORE
        CASE "fast_legs":   Wolf_M1 DEVOURS Sheep_M1 USING DIFF ;
    }
    MIGRATION Sheep_M1 OVER 1 {
        // MATCH(标识符) 在表达式中检测变异：变异->1，未变->0
        INTRODUCE Sheep_M1 AS HERBIVORE WITH MATCH(Wolf_M1) ;
    }
}
DECAY {
    Fungus_1 DEVOURS Wolf_M1 USING SUM ;           // DECOMPOSER 豁免：可吃任意级 100%
    ROT Fungus_1 TO STDOUT ;
}
```

注解：
- `Wolf_M1`（CARNIVORE）`DEVOURS Sheep_M1`（HERBIVORE）：级差恰 1 且捕食者>猎物，SUM 合法（PROD/QUOT 才限 APEX）；`GMO ENABLED` 免税 → Wolf = 5+2 = 7，Sheep → 0。同时保证 FOODWEB 至少一次 DEVOURS（满足 FCL-0006）。
- `MUTATION Wolf_M1`：1/3 概率把 `Wolf_M1` 变异为 `Wolv_M1`，块内 `Wolf_M1` 引用随之改写为 `Wolv_M1`；触发时随机选一个 CASE 执行（CASE 内 DEVOURS Sheep_M1 级差恰 1，合法），未触发则空转。
- `MIGRATION Sheep_M1 OVER 1` 块内 `INTRODUCE ... WITH MATCH(Wolf_M1)`：`MATCH(Wolf_M1)` 作为表达式 factor，变异返回 1、未变返回 0（与官方示例2在 MIGRATION 内使用 INTRODUCE 的写法一致）。
- DECAY 段 `Fungus_1`（DECOMPOSER）捕食 `Wolf_M1`：DECAY 内 DECOMPOSER 豁免，可吃任意级 100%；`NUMERIC OUTPUT` 下 `ROT` 直出数值。
