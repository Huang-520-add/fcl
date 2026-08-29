# FoodChain Language (FCL) Official Technical Specification v3.0

Issued by: International Obfuscated Esolang Foundation (IOEF)
Protocol name: Trophic Pyramid Protocol
Design philosophy: "Code passes through devouring; truth manifests through decomposition."
Ecology Field Guide: see [FCL_ECOLOGY.md](FCL_ECOLOGY.md) (registered species, social structure, naming conventions)
Reference implementation: C++17 interpreter (fcl); build via `make build` or `g++ -std=c++17 -O2 -o fcl src/main.cpp src/interpreter.cpp src/parser.cpp src/expr.cpp src/ecology.cpp` (multi-file module, not a single file).

> **v2.0 revision notes** (based on v1.3):
> 1. **Ecosystem worldview**: variable names are no longer arbitrary; they must use registered ecosystem species (10 representative species, 5 trophic levels), following the social structure (solitary/social, Alpha leader, M/F gender tags, numbering). See FCL_ECOLOGY.md.
> 2. **New ASSESS** (niche assessment / dominant-species determination): `ASSESS <A> AGAINST <B> TO <C> ;`, A≥B → C (APEX) = FULL, implements numeric comparison.
> 3. **New SYMBIOSIS** (mutualism): `SYMBIOSIS <A> WITH <B> TO <C> ;`, C = A AND B.
> 4. **New COMPETITION** (alternative foraging path): `COMPETITION <A> OR <B> TO <C> ;`, C = A OR B.
> 5. **New MIMICRY** (mimicry): `MIMICRY <A> TO <B> ;`, B = NOT A.
> 6. **New HIBERNATION** (hibernation conditional loop): `HIBERNATION <species> UNTIL <APEX> { ... }`, loops until the condition becomes FULL then wakes; each hibernation cycle consumes 10% energy via metabolism.
> 7. **GC ecology**: decomposers only decompose "corpses" (variables with exhausted energy, value==0); living organisms (energy>0) are safe from decomposition; the BIOME introduction phase (creation stage) pauses decomposition.
> 8. **Turing completeness**: comparison + boolean logic + conditional loop are all in place, so FCL can express any computable problem.
> 9. The v1.2.1/v1.3 revisions (WITH rule, decomposer immunity, GMO marker, CLONE, GC active-period pause, etc.) are all retained.

## 1. Overview

FCL (FoodChain Language) is an esoteric programming language (Esolang) designed for biology enthusiasts and programming beginners. It maps variables to ecosystem species and maps computation to energy transfer between trophic levels, letting learners reinforce core ecological concepts — producers, consumers, decomposers, energy-transfer efficiency, food-chain hierarchy constraints, and social species structure — while writing code.

**The essence of FCL: tracing changes in the ecological tiers.** Every line of code is an ecological event; every run is an ecological succession.

Core educational goals:
- Let biology students bypass traditional programming-syntax barriers and write logic directly with familiar food-chain knowledge.
- Turn every variable assignment into a "bite", every loop into a "migration", forcing memorization of trophic-level order.
- Through mechanisms like the "energy-transfer efficiency (20%)" and "gastric-ulcer overflow", intuitively feel ecological thermodynamic laws manifest in computation.

Use cases: interdisciplinary course design for middle-school biology and IT; fun experiments for university general-programming courses; auxiliary tool for ecological-modeling teaching.
Non-use cases: production environments, financial systems, spacecraft control, and other serious engineering scenarios.

## 2. Character Set and Lexical Conventions

- Valid characters: uppercase Latin letters (A-Z), underscore (_), digits (0-9), semicolon (;), braces ({}) and the newline character.
- **Identifier rules (ecosystem naming system)**: variable names must use a registered ecosystem species + social identifier; the format is determined by the social structure:
  - Social species (Wolf/Lion/Sheep): leader `Alpha_<species>` (wolf king Alpha_Wolf); member `<species>_<M|F><number>` (Wolf_M1 male #1, Sheep_F2 female #2)
  - Solitary species (Tiger/Fox/Rabbit): `<species>_<number>` (Tiger_1)
  - Producer/Decomposer (Grass/Algae/Fungus/Bacillus): `<species>_<number>` (Grass_1, Fungus_1)
  - The species name must be registered (Grass/Algae/Sheep/Rabbit/Wolf/Fox/Tiger/Lion/Fungus/Bacillus), and its trophic level must match the declared type
- Violation errors: species outside the ecosystem → `🌿 Invasive species! The ecosystem refuses to accept it!`; trophic-level mismatch → `⚠️ Classification error!`; social-format error → `⚠️ Family registry error!`
- Keywords are forced uppercase; lowercase content is treated as comments (Section 8).
- Whitespace is ignored in non-keyword positions.

## 3. Mandatory Three-Section Program Structure

Any valid FCL program must strictly contain the following three code blocks, in unchangeable order:

```foodchain
BIOME {
/* Introduction phase: only variable declarations and initial energy injection allowed */
}
FOODWEB {
/* Predation phase: core computation logic, must contain at least one predation act (DEVOURS / SCENT / POUNCE) */
}
DECAY {
/* Decomposition phase: responsible for output and memory reclamation */
}
```

Violation penalty: missing any block → the compiler outputs `🌍 Ecosystem collapse — food chain broken!` and terminates.

## 4. Data Types and Trophic Levels

| Type keyword | Alias (trophic level) | Storage range | Property |
|---|---|---|---|
| PRODUCER | Producer (level 1) | Float (0.0~9999.9) | Supports fractional input (e.g., 3/2 stored as 1.5) |
| HERBIVORE | Primary consumer (level 2) | Integer (0~255) | Gastric-ulcer overflow: exceeding 255 resets to zero and triggers a 2-second thread block |
| CARNIVORE | Secondary consumer (level 3) | Integer (0~65535) | Computation speed tied to the parity of the code line number (odd lines faster, even lines slower) |
| APEX | Apex predator (level 4) | Boolean (FULL=1 / HUNGRY=0) | Logical judgment depends on "bite distance" (the variable's address offset in memory) |
| DECOMPOSER | Decomposer (level 5) | String (value interpreted as ASCII) | Rendered as ASCII characters on output |

Internal storage: all variables uniformly store a `double value` + trophic-level tag; HERBIVORE/CARNIVORE are rounded to integer after operations (round half up).

## 5. Instruction Set

### 5.1 Variable Declaration (Introduction)

```ebnf
INTRODUCE <identifier> AS <trophicLevel> WITH <energyExpression> ;
```

Example: `INTRODUCE Grass_1 AS PRODUCER WITH 1/3 ;`
The energy expression supports: bare numbers, fractions (a/b), arithmetic (+ - * /), and parentheses. `WITH 0 ;` is legal.

### 5.2 Data Transfer (Devouring/Predation)

```ebnf
<predator> DEVOURS <prey> USING <algorithm> ;
```

Hard rules:
- The two trophic levels must differ by exactly 1, and the predator's trophic level > the prey's trophic level.
- **Exemption clause**: within the DECAY block, a DECOMPOSER may devour any trophic level, with 100% reclamation efficiency (no tax).
- Algorithms:
  - SUM (addition, energy merge)
  - DIFF (subtraction, larger minus smaller; negative → predator starves, error `🥀`)
  - PROD (multiplication, APEX only, consumes a double time slice)
  - QUOT (division, APEX only, remainder discarded)
- APEX's PROD/QUOT is affected by "bite distance": when the storage-address difference between predator and prey is even, the bite misses and the result is halved (`🐾`).
- After a successful devour, the prey's energy becomes 0 (digested).

### 5.3 Energy-Transfer Tax (Second Law of Thermodynamics)

Every DEVOURS operation (except decomposers inside the DECAY block) has a forced 20% energy-transfer efficiency: `actual result = theoretical result × 0.2`.

Exemption clause: add `GMO ENABLED ;` on the first program line to restore 100% efficiency, but before each ROT output the 🧬 GMO-product marker is printed.

### 5.4 Input (SCENT/LURK/POUNCE Combination)

v3.0 removes the single input primitive SPROUT (a blocking read into a PRODUCER): in an esolang, input is not one statement but a **combination of three atomic behaviors** — sniff (SCENT) → lurk (LURK) → pounce (POUNCE). Each primitive alone is nearly useless; only together can they catch prey. All three primitives may appear **only inside FOODWEB blocks** (where predation/operations live; the FOODWEB validity check accepts DEVOURS / SCENT / POUNCE).

- **SCENT (sniff, non-blocking)**: `SCENT <sniffer> TO <APEX variable> ;` — probes whether STDIN has data ready: ready → stores 1.0 into the APEX species variable, otherwise 0.0. Prints `👃 X sniffs the scent on the wind → Y FULL (prey detected)` or `HUNGRY (no scent)`. The result variable must be an APEX-trophic species (Tiger/Lion).
- **LURK (lurk, dormant waiting)**: `LURK <species> FOR <beats> ;` — waits N beats (REAL mode 100ms/beat, CODE mode 1ms/beat, clamped 0–600). Prints `🕳️ X lurks for N beats`. The species must be registered.
- **POUNCE (pounce, non-blocking)**: `POUNCE <predator> ;` — if STDIN is ready, reads a number into the species (prints `🦅 X pounces and hits, capturing energy N`); if not ready, pounces empty and energy is unchanged (`🐾 X pounces at empty air (no prey scent), energy unchanged`); EOF/invalid input counts as spoiled prey (`🦠 X's caught prey has spoiled, energy unchanged`). In WASM/browser builds it always pounces empty.

Combination pattern (replaces the old SPROUT):

```foodchain
SCENT Wolf_M1 TO Tiger_1 ;
HIBERNATION Wolf_M1 UNTIL Tiger_1 { LURK Wolf_M1 FOR 10 ; SCENT Wolf_M1 TO Tiger_1 ; }
POUNCE Wolf_M1 ;
```

### 5.5 Output (ROT)

Only executable by a DECOMPOSER. Syntax: `ROT <decomposer name> TO STDOUT ;`
- First ROT: output as an ASCII character (energy 65 outputs 'A').
- Consecutive second ROT: output the Unicode code point (format `U+XXXX`).

### 5.6 Asexual Reproduction (Clone)

```ebnf
CLONE <target> FROM <source> ;
```

The target receives a copy of the source's value (the clone does not consume the source; the source's energy is unaffected). The target must already have been introduced via INTRODUCE (otherwise its type is unknown). Ecological basis: vegetative reproduction / cloning technology (cuttings, Dolly the sheep).

Example: `CLONE Grass_2 FROM Grass_1 ;` — A becomes a copy of T.

> **Design motivation**: FCL's energy flow is strictly one-way (prey becomes 0 once eaten; values can only flow to higher trophic levels), making "write-back" (such as `Y_new = X_old` in a Fibonacci recurrence) impossible. CLONE is the most natural ecological copying mechanism, filling in that gap.

### 5.7 Niche Assessment and Logic (v2.0)

Boolean values are carried by APEX species (FULL=1 / HUNGRY=0) — only apex predators hold the "right to judge".

```ebnf
ASSESS <A> AGAINST <B> TO <C> ;      (* Niche assessment: A≥B → C=FULL *)
SYMBIOSIS <A> WITH <B> TO <C> ;      (* Mutualism: C = A AND B *)
COMPETITION <A> OR <B> TO <C> ;      (* Alternative foraging path: C = A OR B *)
MIMICRY <A> TO <B> ;                 (* Mimicry: B = NOT A *)
```

- ASSESS's A and B may be any trophic level; C must be APEX.
- SYMBIOSIS/COMPETITION's A, B, C must all be APEX.
- MIMICRY's A, B must be APEX.
- A non-APEX carrying a judgment → `🦴 Diet conflict — only apex predators (APEX) may carry judgments`.

### 5.8 Hibernation (Conditional Loop, v2.0)

```ebnf
HIBERNATION <species> UNTIL <APEX variable> { ... }
```

Loops executing the block statements until the specified APEX variable becomes FULL, then "wakes" (while loop). Each iteration the species' energy decays 10% via hibernation metabolism (×0.9). Loops exceeding 10000 iterations force a wake (preventing ecological deadlock).

## 6. Control Flow

FCL has no traditional if/for; all logic is driven by environmental keywords:

- **Conditional branch**: `SEASON RAIN { ... } DRY { ... }` (choose one)
  Humidity = the number of DEVOURS among the last 3 instructions; ≥2 is humid → execute RAIN, otherwise DRY.
  RAIN executes in written order by default; only after declaring `STORM ENABLED ;` on the first program line, and when the RAIN block has >3 statements, will the block's statements be randomly shuffled once (🌩️). **By default (STORM not declared) RAIN is never shuffled**, ensuring determinism.
- **Loop**: `MIGRATION <species> OVER <count> { ... }`
  Each iteration, the species' energy value auto-decays by square root (simulating long-distance migration cost).
- **Multi-way selection**: `MUTATION <variable> { CASE "trait": ... }`
  The species mutates at runtime with 1/3 probability: its root name is replaced by a mutant name (e.g., Wolf → Wolv, Grass → Grasse, Tiger → Tygre; full table in the Ecology Field Guide), and in-block references are rewritten accordingly (original names are restored after the block); when triggered, exactly one CASE branch is expressed uniformly at random, otherwise the block is a no-op; detectable via MATCH() by species root name.

## 7. Garbage Collection

- **Automatic reclamation**: decomposers only decompose "corpses" — variables with exhausted energy (value==0) that have not been referenced for 3 consecutive instructions trigger DECOMPOSE; **living organisms (energy>0) are safe from decomposition**. In real mode the release randomly blocks the main thread for 100ms~1000ms (simulating microbial decomposition speed). APEX variables are exempt. **Active-period pause**: decomposition is paused during the BIOME introduction phase (creation stage) and during execution of composite control blocks (MIGRATION/SEASON/MUTATION/HIBERNATION).
- **Manual reclamation**: `EXTINCTION <variable name> ;`
  Immediately reclaims and prints the variable's memory as a hexadecimal "death portrait" (16-line dump). `EXTINCTION Virus_Crash ;` forcibly terminates the process.

## 8. Comments

```ebnf
OBSERVATION: YYYY-MM-DD, Lat:<latitude>, Lon:<longitude>, <content>
```

Example: `OBSERVATION: 2026-08-27, Lat:36.1, Lon:-115.1, this code needs optimization`
Missing the date or coordinates → ignored with warning `🔭 Lack of scientific spirit!`. Lowercase-content lines are likewise treated as comments.

## 9. Error Message Cross-Reference Table

| Traditional error | FCL error |
|---|---|
| Syntax Error | 🌿 Mutant-species invasion — syntax immune system failure |
| Type Mismatch | 🦴 Diet conflict — predator refuses to eat |
| Out of Memory | 💀 Mass extinction — reduce reproduction (loops) |
| Timeout | ❄️ Hibernation too long — process dormant |
| Division by Zero | 🔥 Drought breaks the food chain |
| Taxonomic violation | ⚠️ Classification error! |
| Structural violation | 🌍 Ecosystem collapse — food chain broken! |
| Starvation (negative) | 🥀 Predator starves — energy negative |
| Overflow | 🤢 Gastric-ulcer overflow — energy reset to zero |

## Appendix A: Complete Code Examples (verified by reference implementation)

### Example 1 (original, GMO tax-free, demonstrates spec output)

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

Simulation: Algae_1=3, Algae_2=5, GMO tax-free → Sheep_M1=8 → decomposer reclaims full 8 → outputs ASCII 8 (backspace) + `U+0008`. Actual output: `🧬\b🧬U+0008`.

### Example 2 (with tax, demonstrates 20% energy-transfer efficiency)

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

Simulation: Grass_1=65 → Sheep_M1=65×0.2=13 → Fungus_1=13 → outputs ASCII 13 (carriage return) + `U+000D`.

### Example 3 (recommended demo: outputs the visible character 'A')

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

Simulation: Grass_1=65, GMO tax-free → Sheep_M1=65 → Fungus_1=65 → outputs `'A'` + `U+0041`. Actual output: `🧬A🧬U+0041`.

### Example 4 (Fibonacci: MIGRATION + CLONE recurrence, outputs F(7)=13)

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

Recurrence logic (A=F(n-1), B=F(n), H is the accumulator, T is the backup):
1. `CLONE T FROM B`: back up F(n) into T (otherwise the old value is lost after B is eaten)
2. `H DEVOURS A`, `H DEVOURS B`: H = F(n-1) + F(n) = F(n+1)
3. `CLONE Grass_2 FROM Grass_1`: A = F(n) (the old value of B) → becomes the next round's F(n-1)
4. `CLONE B FROM H`: B = F(n+1) → becomes the next round's F(n)
5. Reset H/T (INTRODUCE overwrites to 0)

Simulation: round 1 → A=1, B=2; round 2 → A=2, B=3; round 3 → A=3, B=5; round 4 → A=5, B=8; round 5 → A=8, **B=13=F(7)**.
DECAY output: ASCII 13 (carriage return) + `U+000D`. Actual output: `🧬\r🧬U+000D`.

## Appendix B: Standard Library (preset species)

- `Virus_Crash`: specifically for forcibly exiting the program (`EXTINCTION Virus_Crash ;`)

## Appendix C: Ecological Observations (fun output)

- 🔬 Recommended to use binomial nomenclature (Genus_species) — single-name variable reminder
- 🍄 `<variable> reclaimed by decomposer` — GC triggered
- 🧬 Mutation: `Wolf → Wolv` — MUTATION triggered (1/3 name-change probability)
- 🐾 Bite misses, energy halved — APEX bite-distance judgment
- 🌩️ RAIN block statements randomly shuffled — only occurs after `STORM ENABLED` on the first program line AND when the block has >3 statements

Document version: v3.0
Last updated: 2026-08-27

�️ RAIN downgraded to STORM — condition block statement count exceeded

Document version: v3.0
Last updated: 2026-08-27
