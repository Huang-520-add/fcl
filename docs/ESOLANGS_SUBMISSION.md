# esolangs.org Entry Submission — FoodChain Language (FCL)

> Submission draft for https://esolangs.org — copy the sections below into a new wiki page titled **FoodChain Language**.
> 投稿用条目草稿：将以下内容粘贴到 esolangs.org 新建的 "FoodChain Language" 词条。

---

## Infobox fields

| Field | Value |
|---|---|
| **Name** | FoodChain Language (FCL) |
| **Author** | by.Huang (design) / ima.copilot (reference implementation) |
| **Year** | 2026 |
| **File extension** | `.fc` |
| **Dimensionality** | 1-dimensional |
| **Paradigm** | Imperative, structured (mandatory three-phase: BIOME → FOODWEB → DECAY) |
| **Type system** | Strong; trophic-level-bound (5 levels); no implicit conversion |
| **Turing complete** | Yes (ASSESS comparison + HIBERNATION conditional loop + INTRODUCE unbounded storage) |
| **Reference implementation** | C++17 interpreter (~1200 lines); also compiled to WebAssembly for an online playground |
| **License** | MIT |
| **Repository** | https://github.com/Huang-520-add/fcl |
| **Online playground** | https://huang-520-add.github.io/fcl/ |

---

## Entry body (suggested wiki text)

**FoodChain Language (FCL)** is an ecology-themed esolang in which every program is a food chain. Its motto: *"Code passes through devouring; truth appears through decomposition."*

Programs are organized in three mandatory phases mirroring ecosystem energy flow:

- `BIOME { }` — introduction: declare species (variables) and inject initial energy
- `FOODWEB { }` — predation: the core computation (must contain at least one `DEVOURS`)
- `DECAY { }` — decomposition: output (`ROT`) and memory reclamation

### Variables are species

Variables may only be named after the 10 registered species, and each name encodes its trophic level and social structure:

| Level | Species | Naming |
|---|---|---|
| 1 Producer | Grass, Algae | `Grass_1` |
| 2 Herbivore | Sheep (pack), Rabbit (solitary) | `Alpha_Sheep`, `Sheep_M1`, `Rabbit_1` |
| 3 Carnivore | Wolf (pack), Fox (solitary) | `Alpha_Wolf`, `Wolf_M1`, `Fox_1` |
| 4 Apex | Tiger (solitary), Lion (pack) | `Tiger_1`, `Alpha_Lion` |
| 5 Decomposer | Fungus, Bacillus | `Fungus_1` |

Names outside the registry are rejected: `🌿 外来物种入侵，生态圈不予接纳！` ("invasive species — the ecosystem refuses to accept it"). Even variable names must pass customs.

### Computation is predation

`Predator DEVOURS Prey USING SUM|DIFF|PROD|QUOT ;`

- The predator's trophic level must be exactly one above the prey's (a wolf cannot eat grass).
- Prey is consumed: its energy becomes 0 (unidirectional energy flow).
- Each predation transfers only 20% of the energy to the predator (Lindeman's trophic efficiency law). `GMO ENABLED` on the first line waives the loss for 100% transfer, at the cost of a 🧬 GMO label on every output.
- Only apex predators may multiply (`PROD`) or divide (`QUOT`) — and even then, a parity rule on storage addresses can make the pounce "miss", halving the result.

### No `if` / `for` — only ecology

| Construct | Ecological driver |
|---|---|
| `SEASON RAIN { } DRY { }` | humidity of the last 3 instructions |
| `MIGRATION sp OVER n { }` | fixed loop; the migrant's energy is square-rooted after each lap |
| `HIBERNATION sp UNTIL apex { }` | while-loop; the hibernator loses 10% energy per round |
| `MUTATION sp { CASE "trait": }` | runtime random renaming of the species (1/3 chance); when triggered, exactly one CASE branch is expressed at random; detectable via MATCH() |
| `ASSESS A AGAINST B TO apex` | comparison; result stored in an apex predator |
| `SYMBIOSIS / COMPETITION / MIMICRY` | boolean AND / OR / NOT (apex-only) |

### Output is decomposition

Only decomposers may output (`ROT decomposer TO STDOUT ;`), mineralizing a number as an ASCII character (odd calls) or a Unicode codepoint (even calls). `NUMERIC OUTPUT` mode prints the raw number instead.

### Garbage collection is decomposition

Only "corpses" (variables whose energy reached 0) that stayed untouched for 3 instructions are reclaimed — living organisms are never decomposed. In REAL MODE, reclamation sleeps 100–1000 ms to simulate microbial speed; CODE MODE (default) reclaims instantly.

---

## Trivia (for the entry)

- FCL's "Hello World" computes 3+5 = 8 — ASCII 8 is the **backspace character**.
- Fibonacci in FCL uses `CLONE` (asexual reproduction) to move values back down the pyramid — ecologically, this is *material cycling* breaking the one-way energy law.
- All error messages are ecological events: `🦴 食性冲突` (trophic conflict), `🥀 捕食者饿死` (predator starved), `🤢 胃溃疡溢出` (herbivore stomach ulcer overflow when energy > 255).
- Apex predators store booleans only — the higher you climb the food chain, the less energy you can hold, but the more judgment you wield.

## Examples

See the repository's `examples/` directory: factorial, triangular numbers, Fibonacci, hibernation loops, and a full test suite (38 cases).
