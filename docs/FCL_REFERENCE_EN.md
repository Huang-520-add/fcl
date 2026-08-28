# FCL Language Reference — English Edition v2.4

> **The authoritative technical reference for FCL.**
> Use this as a lookup guide: syntax, types, instructions, control flow, and garbage collection.
> For learning FCL from scratch → [FCL_TUTORIAL_EN.md](FCL_TUTORIAL_EN.md).
> For the ecosystem worldview → [FCL_ECOLOGY.md](FCL_ECOLOGY.md).

---

## 1. Language Overview

FCL (FoodChain Language) is a **Turing-complete esoteric programming language** that reimagines every standard programming concept through ecology. It is also an **interpreted, imperative, strongly-typed** language.

The core re-mapping:

| Ordinary concept | FCL ecology equivalent |
|---|---|
| Variable | Registered species in the ecosystem (10 species, bound to a trophic level) |
| Assignment / computation | Devouring (DEVOURS) — one-way, destructive, with energy tax |
| Copy without destroy | Asexual reproduction (CLONE) |
| Comparison | Ecological niche assessment (ASSESS) |
| Boolean logic | Symbiosis / Competition / Mimicry (SYMBIOSIS / COMPETITION / MIMICRY) |
| if/else | Rain/Dry season (SEASON, humidity-driven) |
| for loop | Migration (MIGRATION, fixed-count with square-root decay) |
| while loop | Hibernation (HIBERNATION, conditional with metabolic decay) |
| Output | Mineralization (ROT, decomposers only, ASCII interpretation) |
| Garbage collection | Ecological decomposition (only "corpses" are decomposed) |

**Program structure**: Every valid FCL program has three mandatory sections in fixed order: `BIOME` (introduction) → `FOODWEB` (computation) → `DECAY` (output and cleanup).

### 1.1 Ecological Design Rationale

| FCL feature | Real ecology basis |
|---|---|
| 20% energy tax per devour | **Lindeman's Law**: ~10–20% energy transfer efficiency per trophic level; rest lost as heat, respiration |
| Prey energy becomes 0 after devour | Energy flow is strictly one-way: energy entering a consumer does not return to the prey |
| Decomposer 100% recovery, any level | Decomposers (detritivores) completely break down organic matter; no residual energy waste |
| Only decomposers can ROT | Nutrients must be mineralized by decomposers before returning to the environment |
| CLONE = asexual reproduction | Nutrients cycle back to lower trophic levels and are reused (cuttings/cloning) |
| Social species have Alpha leaders | Wolf packs and lion prides have strict hierarchies; the alpha coordinates group behavior |
| Solitary species use bare numbers | Solitary carnivores (tigers, foxes) maintain territories; no social hierarchy needed |
| Food chain is 4–5 levels | Energy pyramid narrows at each level; no food chain can sustain more levels |
| Decomposers only hit "corpses" (energy=0) | Decomposers do not attack living organisms, only dead organic matter |
| SEASON humidity / MIGRATION decay / HIBERNATION metabolism / MUTATION randomness | Environmental drivers of ecological dynamics (precipitation, migration cost, dormancy, genetic mutation) |

---

## 2. Program Structure

```
BIOME   { ... }   // Introduction: variable declarations only (INTRODUCE)
FOODWEB { ... }   // Computation: core logic, must contain at least one DEVOURS
DECAY   { ... }   // Decomposition: output (ROT) and cleanup (EXTINCTION)
```

Rules:
- All three sections are **mandatory and ordered** — missing any → 🌍 Ecosystem collapse
- FOODWEB must contain at least one DEVOURS — otherwise → 🌍 Ecosystem collapse
- `GMO ENABLED` goes on the first line, outside the three sections
- Statements end with `;`; blocks use `{ }`

---

## 3. Lexicon and Naming

### 3.1 Character Set

Valid characters: uppercase Latin letters (A–Z), lowercase (a–z), digits (0–9), underscore (_), operators (+-*/), punctuation (;{}()"`.`), whitespace. Case-sensitive.

### 3.2 The 10 Registered Species

Variable names **must** be one of these 10 species:

| Trophic level | Keyword | Species | Social structure | Naming format |
|---|---|---|---|---|
| 1 Producer | `PRODUCER` | Grass | Colony | `Grass_<N>` |
| 1 Producer | `PRODUCER` | Algae | Colony | `Algae_<N>` |
| 2 Herbivore | `HERBIVORE` | Sheep | **Social** | `Alpha_Sheep` / `Sheep_M<N>` / `Sheep_F<N>` |
| 2 Herbivore | `HERBIVORE` | Rabbit | Solitary | `Rabbit_<N>` |
| 3 Carnivore | `CARNIVORE` | Wolf | **Social** | `Alpha_Wolf` / `Wolf_M<N>` / `Wolf_F<N>` |
| 3 Carnivore | `CARNIVORE` | Fox | Solitary | `Fox_<N>` |
| 4 Apex | `APEX` | Tiger | Solitary | `Tiger_<N>` |
| 4 Apex | `APEX` | Lion | **Social** | `Alpha_Lion` / `Lion_M<N>` / `Lion_F<N>` |
| 5 Decomposer | `DECOMPOSER` | Fungus | Colony | `Fungus_<N>` |
| 5 Decomposer | `DECOMPOSER` | Bacillus | Colony | `Bacillus_<N>` |

Naming violations:
- Unknown species → `🌿 Invasive species! Unknown to the ecosystem!`
- Species/trophic level mismatch → `⚠️ Classification error! <species> is <actual level>, not <declared level>`
- Social species missing M/F or Alpha → `⚠️ Family registry error!`

---

## 4. Type System

### 4.1 Five Trophic Levels

| Type keyword | Level | Storage | Range | Special |
|---|---|---|---|---|
| `PRODUCER` | 1 | Float | 0.0–9999.9 | Fraction input (3/2 → 1.5) |
| `HERBIVORE` | 2 | Integer | 0–255 | **Overflow → "ulcer": >255 resets to 0 + 2s block** |
| `CARNIVORE` | 3 | Integer | 0–65535 | Odd line numbers → faster; even → 2ms slower |
| `APEX` | 4 | Boolean | FULL=1 / HUNGRY=0 | Only one that can use PROD/QUOT; carries ASSESS results |
| `DECOMPOSER` | 5 | ASCII | Interprets number as character | Only one that can execute ROT |

### 4.2 The Trophic Law (Predator-Prey Hierarchy)

```
PRODUCER(1) → HERBIVORE(2) → CARNIVORE(3) → APEX(4)
                              ↕
                    DECOMPOSER(5) decomposes all levels
```

Rules:
- Predator trophic level must be **exactly 1 higher** than prey (exception: Decomposer in DECAY section is exempt)
- Same-level or skipped-level devouring → `🦴 Dietary conflict!`
- Energy flow is strictly one-way: prey energy becomes 0 after being devoured

---

## 5. Expressions

Used in `INTRODUCE ... WITH <expr>` and `MATCH()`:

```
expr   := term (('+' | '-') term)*
term   := factor (('*' | '/') factor)*
factor := number | fraction | '(' expr ')' | "MATCH(" identifier ")"

number   := integer | float
fraction := integer '/' integer   // 1/3 → 0.333…
MATCH()  := "MATCH(" identifier ")"  // Returns 1 if species mutated, 0 otherwise
```

Division by zero → `🔥 Drought collapses the food chain!`

---

## 6. Instructions

### 6.1 Introduction (INTRODUCE)

```foodchain
INTRODUCE <species_name> AS <trophic_level> WITH <expression> ;
```

Declares a variable and injects initial energy. APEX initial values are auto-normalized to 0/1.

### 6.2 Devouring (DEVOURS)

```foodchain
<predator> DEVOURS <prey> USING <algorithm> ;
```

| Algorithm | Operation | Valid predator |
|---|---|---|
| `SUM` | Predator = Predator + Prey | Any valid devour pair |
| `DIFF` | Predator = Predator − Prey | Predator must have ≥ Prey's energy or → 🥀 Starvation |
| `PROD` | Predator = Predator × Prey | **APEX only** |
| `QUOT` | Predator = Predator ÷ Prey (remainder discarded) | **APEX only** |

General rules:
- **Energy tax**: `result = theoretical × 0.2` (exceptions: GMO mode, DECAY decomposer)
- **Prey destroyed**: prey energy → 0 after devour
- **APEX PROD/QUOT "bite distance"**: if predator and prey storage address difference is even → bite misses, result halved (🐾)
- **HERBIVORE overflow** (>255): reset to 0 + 2s block (🤢 ulcer)

### 6.3 Cloning (CLONE)

```foodchain
CLONE <target> FROM <source> ;
```

Target = numerical copy of source; source is unchanged. Target must already be declared. This is FCL's only mechanism for "copy without destroy" (substance cycling).

### 6.4 Assessment (ASSESS)

```foodchain
ASSESS <A> AGAINST <B> TO <C> ;
```

A.value ≥ B.value → C = FULL(1); otherwise C = HUNGRY(0). C **must be APEX**.

Non-APEX destination → `🦴 Dietary conflict! Only apex predators have the authority to judge.`

### 6.5 Boolean Logic

```foodchain
SYMBIOSIS  <A> WITH <B> TO <C> ;    // C = A AND B
COMPETITION <A> OR <B> TO <C> ;     // C = A OR B
MIMICRY    <A> TO <B> ;             // B = NOT A
```

All operands must be APEX.

### 6.6 Input (SPROUT)

```foodchain
SPROUT <producer_name> FROM STDIN ;
```

Injects value into a PRODUCER. Plays a 2-second tone; user must type a number within the timeout → 0 on timeout. Non-PRODUCER → 🦴.

### 6.7 Output (ROT)

```foodchain
ROT <decomposer_name> TO STDOUT ;
```

Decomposers only → 🦴 otherwise.

**Output modes (since v2.2, choose one):**

| Mode | How to enable | ROT behavior |
|---|---|---|
| Character mode (default) | No declaration | 1st ROT: `char(int(energy))`; 2nd ROT: `U+XXXX` (Unicode code point) |
| Number mode | `NUMERIC OUTPUT ;` (anywhere) | Every ROT outputs the raw number |

### 6.8 Seasonal Branching (SEASON)

```foodchain
SEASON RAIN { ... } DRY { ... } ;
```

- **Humidity** = number of DEVOURS in the last 3 instructions; ≥2 → RAIN, else DRY
- **STORM is an explicit switch** (v2.2): default RAIN blocks >3 statements are NOT shuffled (deterministic). Only with `STORM ENABLED ;` at program start do RAIN blocks >3 statements shuffle randomly once (🌩️). ⚠️ STORM introduces non-determinism — use with caution.

### 6.9 Migration Loop (MIGRATION)

```foodchain
MIGRATION <species> OVER <count> { ... } ;
```

Fixed-count loop. **Side effect**: after each round, the migrator's energy square-roots (√). Do NOT use a migrator that carries important state — instead use a "worker" species that gets reset each round (see Fibonacci example).

### 6.10 Hibernation Loop (HIBERNATION)

```foodchain
HIBERNATION <species> UNTIL <APEX_variable> { ... } ;
```

Conditional loop (while). Loops until APEX = FULL(1). **Side effect**: each round the hibernator's energy ×0.9. Anti-infinite-loop: >10,000 rounds → forced wake (⏰).

### 6.11 Mutation (MUTATION)

```foodchain
MUTATION <species> { CASE "feature": <statement> ... } ;
```

Runtime: the species name has 1/3 probability of mutating to an alternate form (Wolf→Wolv, Sheep→Sheepe, etc.). All statements inside referencing the mutated species change accordingly. `MATCH(name)` detects mutation.

Mutated names:

| Original | Mutated |
|---|---|
| Grass | Grasse | Algae | Algue | Sheep | Sheepe | Rabbit | Rabbitt | Wolf | Wolv | Fox | Faux | Tiger | Tygre | Lion | Lioon | Fungus | Fingu | Bacillus | Bacilluz |

### 6.12 Extinction (EXTINCTION)

```foodchain
EXTINCTION <species> ;
```

Immediate collection with 16-line hex "death portrait." `EXTINCTION Virus_Crash ;` → process termination.

### 6.13 Transgenic Mode (GMO)

```foodchain
GMO ENABLED ;   // Must be on the first line, outside the three sections
```

- Energy tax → 0% (100% transfer efficiency)
- Cost: every ROT output prefixed with 🧬

### 6.14 Run Modes (REAL MODE / CODE MODE)

```foodchain
REAL MODE ;    // Realistic ecological timing enabled
CODE MODE ;    // Skip all wait times, fast execution (default)
```

| Wait item | Code mode | Real mode |
|---|---|---|
| HERBIVORE ulcer penalty (>255) | Skipped (instant reset) | Blocks 2 seconds |
| Carnivore even-line delay | Skipped | 2ms |
| GC decomposition blocking | Skipped (instant) | 100–1000ms random |
| SPROUT timeout | 2s interactive (can't skip) | Same |

---

## 7. Control Flow Summary

| Need | Ordinary language | FCL |
|---|---|---|
| Sequential | Line by line | Within sections, in order |
| Conditional branch | if/else | SEASON RAIN/DRY (humidity ≥2 → RAIN) |
| Fixed-count loop | for | MIGRATION (√ decay each round) |
| Conditional loop | while | HIBERNATION UNTIL (×0.9 decay each round) |
| Multi-way | switch | MUTATION CASE (random mutation) |
| Jump/exit | break/return | EXTINCTION Virus_Crash |

---

## 8. Garbage Collection (Ecological Decomposition)

1. **Living creature exemption**: any species with energy > 0 is NEVER collected
2. **Corpse decomposition**: energy = 0 AND untouched for 3 instructions → 🍄 collected; in Real mode: random 100–1000ms block
3. **Active period suspension**: during BIOME introductions and inside control blocks → decomposition paused
4. **Apex exemption**: APEX (Tiger, Lion) is NEVER automatically collected
5. Manual: EXTINCTION (with hex portrait)

---

## 9. Comments

```foodchain
OBSERVATION: YYYY-MM-DD, Lat:<lat>, Lon:<lon>, <text>
```

Missing date or coordinates → warning `🔭 Lack of scientific rigor!`. Any line starting with a lowercase letter is also ignored as a comment.

---

## 10. Error Reference

All errors are structured: `[error_code @line] ecological message` (since v2.2).

| Code | FCL message | Triggers |
|---|---|---|
| FCL-0001 | 🌿 Species mutation! Syntax error! | Unknown statement or expression error |
| FCL-0002 | 🦴 Dietary conflict! | Trophic level mismatch / wrong type / non-APEX PROD/QUOT |
| FCL-0003 | 🌿 Invasive species! | Species not in registry |
| FCL-0004 | ⚠️ Classification error! | Species trophic level mismatches declared type |
| FCL-0005 | ⚠️ Family registry error! | Social species missing M/F label |
| FCL-0006 | 🌍 Ecosystem collapse! | Missing section / no DEVOURS in FOODWEB |
| FCL-0007 | 🔥 Drought! | Division by zero |
| FCL-0008 | 🥀 Predator starved! | DIFF: predator energy < prey energy |
| FCL-0009 | 🤢 Ulcer overflow! | HERBIVORE energy > 255 |
| FCL-0010 | ⏰ Hibernation timeout (>10000 rounds)! | HIBERNATION never wakes |
| FCL-0011 | 🌿 Species mutation! Expression unresolvable! | Expression contains invalid tokens |

---

## 11. Appendix

### 11.1 Standard Library

- `Bacillus_Stdio`: Decomposer with stdin/stdout MOS code handling
- `Rhizobium_Math`: High-precision symbiotic bacteria for APEX multiplication
- `Virus_Crash`: Process exit (triggered via `EXTINCTION Virus_Crash ;`)

### 11.2 Example Index

| File | Demonstrates |
|---|---|
| `../examples/example1.fc` | 3+5=8 (backspace char + U+0008) |
| `../examples/example2.fc` | 20% energy tax (65×0.2=13) |
| `../examples/example3.fc` | GMO no-tax output 'A' + U+0041 |
| `../examples/fib.fc` | Fibonacci (CLONE substance cycling) |
| `../examples/plus1.fc` | 1+1 (ASCII control character version) |
| `../examples/plus1_visible.fc` | 25+25=50='2' (visible character) |
| `../examples/eco_logic.fc` | ASSESS / SYMBIOSIS / COMPETITION / MIMICRY |
| `../examples/eco_hibernate.fc` | HIBERNATION conditional loop |
| `../examples/control_*.fc` | Control flow and I/O series |
| `../examples/err_*.fc` | Error demonstration series |

### 11.3 Building and Running

```bash
make build                # g++ -std=c++17 -O2 -o fcl src/*.cpp
./fcl program.fc          # Normal (code mode, fast)
./fcl --real program.fc  # Real mode (all ecological waits enabled)
```

### 11.4 Module Structure

```
src/
├── main.cpp         # Entry point, argument parsing, version banner
├── fcl_error.h      # Error code enum and structured error output
├── ecology.h/.cpp   # Ecosystem: trophic levels, species registry, naming validation
├── expr.h/.cpp      # Expression evaluation (recursive descent parser)
├── parser.h/.cpp    # Statement parsing
└── interpreter.h/.cpp # Execution engine: instructions, control flow, GC
```

---

> Reference Manual v2.4 (English Edition)
> Reference Implementation: [Huang-520-add/fcl](https://github.com/Huang-520-add/fcl)
> Chinese Edition: [FCL_REFERENCE.md](FCL_REFERENCE.md)
