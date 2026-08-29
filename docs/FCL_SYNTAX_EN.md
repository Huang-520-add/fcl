# FCL Syntax Specification (FCL Syntax Specification) v3.0

> This document uses both **formal grammar (EBNF)** and **keyword breakdowns** to precisely define FCL's lexical rules and syntactic structure. Intended for: compiler implementers, language researchers, and developers strictly aligning with the specification.
> For learning → [FCL_TUTORIAL.md](FCL_TUTORIAL.md); for a syntax quick-reference → [FCL_REFERENCE.md](FCL_REFERENCE.md).

---

## 1. Lexicon

### 1.1 Character Set

```
Characters allowed in FCL source:
  - Uppercase Latin letters: A-Z (keywords)
  - Lowercase Latin letters: a-z (comments, content)
  - Underscore: _ (naming connector)
  - Digits: 0-9 (values, indices)
  - Punctuation: + - * / ( ) ; , " { } . _
  - Space, Tab, newline (separators, no syntactic meaning)
  - Case-sensitive: Grass ≠ grass ≠ GRASS
```

### 1.2 Token Types

| Token type | Example | Description |
|---|---|---|
| `KEYWORD` | `BIOME`, `INTRODUCE`, `DEVOURS` | All uppercase |
| `IDENTIFIER` | `Grass_1`, `Wolf_M1`, `Alpha_Wolf` | Species naming |
| `NUMBER` | `60`, `3.14`, `2/3`, `60+5` | Integer / float / fraction / expression |
| `OPERATOR` | `+`, `-`, `*`, `/`, `>`, `=` | Arithmetic / comparison operators |
| `DELIMITER` | `;`, `{`, `}`, `(`, `)`, `,` | Separators |
| `STRING` | `"text"` | CASE label text |
| `COMMENT` | Line starting with lowercase letters, `OBSERVATION:` comment | Ignored |

### 1.3 Keyword Table (alphabetical)

```
Alpha_*, ASSESS, Bacillus_*,
COMPETITION, DECAY, DEVOURS, DRY,
EXTINCTION,
FOODWEB,
GMO,
HIBERNATION,
INTRODUCE,
Lion_*, MATCH,
MIMICRY, MIGRATION, MUTATION,
NUMERIC OUTPUT,
OBSERVATION:,
PRODUCER,
RAIN, REAL MODE, ROT, STORM,
SEASON, STDIN, STDOUT, SUM, SYMBIOSIS,
TO,
USING,
WITH,
```

> Keywords cannot be used as variable names (except lowercase — lowercase is itself a comment).

---

## 2. EBNF Grammar Definition

### 2.1 Program Structure

```
program       ::= shebang? gmoline? stormline? numline? modeDecl?
                  BIOME block
                  FOODWEB block
                  DECAY block

shebang       ::= "#!" ... NEWLINE          // optional, supports a shebang line
gmoline       ::= "GMO ENABLED" ";" NEWLINE
stormline     ::= "STORM ENABLED" ";" NEWLINE   // optional: once enabled, RAIN block with >3 statements is randomly shuffled
numline       ::= "NUMERIC OUTPUT" ";" NEWLINE
modeDecl      ::= ("REAL MODE" | "CODE MODE") ";" NEWLINE

block         ::= "{" statement* "}" NEWLINE?
statement     ::= (declaration | operation | controlFlow | ioStmt | extinction | comment) ";"
comment       ::= ("OBSERVATION:" [DATE] [COORD] text) | (line starting with lowercase letters)
```

> `BIOME`, `FOODWEB`, `DECAY` three sections are **all mandatory**, in fixed order, and not nestable.

### 2.2 Variable Declaration

```
declaration   ::= "INTRODUCE" identifier "AS" trophicLevel "WITH" expression

identifier    ::= speciesName ("_" (alphaId | ("M"|"F") digit+))?
               | "Alpha_" speciesName
               | speciesName "_" digit+

speciesName   ::= "Grass" | "Algae" | "Sheep" | "Rabbit"
               | "Wolf" | "Fox" | "Tiger" | "Lion"
               | "Fungus" | "Bacillus"

trophicLevel  ::= "PRODUCER" | "HERBIVORE" | "CARNIVORE" | "APEX" | "DECOMPOSER"

alphaId       ::= "1" | "2" | "3" | ...           // solitary index
```

### 2.3 Devour Operation

```
operation     ::= devour | clone | assess | symbios | competition | mimicry | sprout

devour        ::= predator "DEVOURS" prey "USING" algorithm
                 // predicate: predator.trophicLevel = prey.trophicLevel + 1
                 //             OR (inside DECAY block AND predator is DECOMPOSER)

predicate     ::= identifier                      // species variable name
algorithm     ::= "SUM" | "DIFF" | "PROD" | "QUOT"
                 // SUM/DIFF: any valid predator
                 // PROD/QUOT: APEX only
```

### 2.4 Clone

```
clone         ::= "CLONE" targetIdentifier "FROM" sourceIdentifier
                 // predicate: target already declared (type known); source unaffected
```

### 2.5 Assess (Comparison)

```
assess        ::= "ASSESS" exprA "AGAINST" exprB "TO" apexVariable
                 // A.value >= B.value → apexVariable = FULL(1)
                 // otherwise          → apexVariable = HUNGRY(0)
                 // predicate: the third parameter must be APEX
```

### 2.6 Boolean Logic

```
symbios       ::= "SYMBIOSIS" apexA "WITH" apexB "TO" apexC
                 // C = A AND B
competition   ::= "COMPETITION" apexA "OR" apexB "TO" apexC
                 // C = A OR B
mimicry       ::= "MIMICRY" apexA "TO" apexB
                 // B = NOT A
```

### 2.7 Input

```
scent         ::= "SCENT" speciesIdentifier "TO" apexIdentifier
                 // predicate: the result variable must be an APEX species (Tiger/Lion)
                 // semantics: non-blocking STDIN readiness probe, stores 1.0 if ready, else 0.0
lurk          ::= "LURK" speciesIdentifier "FOR" integerLiteral
                 // predicate: the species must be registered
                 // semantics: dormant wait of N beats (REAL 100ms/beat, CODE 1ms/beat, clamped 0–600)
pounce        ::= "POUNCE" speciesIdentifier
                 // semantics: non-blocking pounce, reads a number if STDIN is ready, otherwise pounces empty
                 // constraint: all three allowed only inside FOODWEB blocks (v3.0 composed input, replacing SPROUT)
```

### 2.8 Output

```
ioStmt        ::= "ROT" decomposerIdentifier "TO" "STDOUT"
                 // predicate: only DECOMPOSER may execute
                 // code mode (default): first output char(int(energy)); second output U+XXXX
                 // numeric mode (NUMERIC OUTPUT): output the numeric value directly
```

### 2.9 Control Flow

```
controlFlow   ::= season | migration | hibernation | mutation

season        ::= "SEASON" "RAIN" block "DRY" block
                 // humidity = DEVOURS count among the last 3 instructions; >=2 → RAIN, otherwise DRY

migration     ::= "MIGRATION" identifier "OVER" integer block
                 // fixed-count loop; after each round identifier's energy takes a square root

hibernation   ::= "HIBERNATION" identifier "UNTIL" apexVariable block
                 // conditional loop; until apex = FULL(1); each round identifier *= 0.9
                 // anti-deadlock: >10000 rounds force-wakes

mutation      ::= "MUTATION" identifier "{" caseClause+ "}"
caseClause    ::= "CASE" stringLiteral ":" statement+
                 // at runtime the species has a 1/3 probability of mutating (root-name
                 // renaming; in-block references are rewritten accordingly)
                 // when triggered, exactly one caseClause is expressed, chosen uniformly
                 // at random; otherwise the whole block is a no-op
                 // detectable via MATCH(identifier) (resolved by species root name)
```

### 2.10 Extinction

```
extinction    ::= "EXTINCTION" identifier
                 // immediately reclaim the variable and print a 16-line hexadecimal death portrait
                 // EXTINCTION Virus_Crash → forcibly terminate the process
```

### 2.11 Expression

```
expression    ::= term (('+' | '-') term)*
term          ::= factor (('*' | '/') factor)*
factor        ::= number | fraction | '(' expression ')' | "MATCH(" identifier ")"

number        ::= integer | float
fraction      ::= integer '/' integer          // 1/3 → 0.333...
MATCH()       ::= "MATCH(" identifier ")"      // mutation detection: mutated→1, unchanged→0
```

---

## 3. Trophic Constraint Matrix

```
Producer PRODUCER (1):
  Can be eaten by: HERBIVORE, DECOMPOSER (within DECAY block)
  Can eat: none (top source)

Herbivore HERBIVORE (2):
  Can be eaten by: CARNIVORE, DECOMPOSER (within DECAY block)
  Can eat: PRODUCER

Carnivore CARNIVORE (3):
  Can be eaten by: APEX, DECOMPOSER (within DECAY block)
  Can eat: HERBIVORE

Apex APEX (4):
  Can be eaten by: DECOMPOSER (within DECAY block)
  Can eat: CARNIVORE
  Special: may use PROD/QUOT, may carry ASSESS results

Decomposer DECOMPOSER (5):
  Can be eaten by: none (terminal)
  Can eat: any (only within DECAY block)
```

---

## 4. Precedence & Associativity

| Precedence | Operator | Description | Associativity |
|---|---|---|---|
| 1 (highest) | `()` | Parentheses | — |
| 2 | `*` `/` | Multiply / divide | Left |
| 3 | `+` `-` | Add / subtract | Left |
| 4 | `MATCH()` | Mutation detection | — |
| 5 | `DEVOURS` and similar instructions | Statement-level (not expression) | — |

---

## 5. Keyword-to-Error Mapping Table

| Keyword | Error scenario | Error code |
|---|---|---|
| `INTRODUCE` + unregistered species | 🌿 Invasive species | FCL-0003 |
| `INTRODUCE` + type/species mismatch | ⚠️ Classification error | FCL-0004 |
| `INTRODUCE` + naming-format error | ⚠️ Family registry error | FCL-0005 |
| `DEVOURS` + trophic-level difference ≠ 1 | 🦴 Diet conflict | FCL-0002 |
| `DEVOURS` + non-APEX using PROD/QUOT | 🦴 Diet conflict | FCL-0002 |
| `ROT` + non-DECOMPOSER | 🦴 Diet conflict | FCL-0002 |
| `SCENT` result variable non-APEX | ⚠️ Classification error | FCL-0004 |
| `ASSESS` + third parameter non-APEX | 🦴 Diet conflict | FCL-0002 |
| `DEVOURS` + DIFF with insufficient predator energy | 🥀 Predator starves | FCL-0008 |
| HERBIVORE energy > 255 | 🤢 Gastric-ulcer overflow | FCL-0009 |
| Division by zero | 🔥 Drought breaks the food chain | FCL-0007 |
| HIBERNATION over 10000 rounds | ⏰ Hibernation too long | FCL-0010 |
| Missing BIOME / FOODWEB / DECAY | 🌍 Ecosystem collapse | FCL-0006 |
| No predation act (DEVOURS/SCENT/POUNCE) inside FOODWEB | 🌍 Ecosystem collapse | FCL-0006 |
| Expression parse failure | 🌿 Mutant-species invasion | FCL-0001 |
| Unknown statement | 🌿 Mutant-species invasion | FCL-0001 |

---

> Syntax specification version: v3.0
> Reference implementation: [Huang-520-add/fcl](https://github.com/Huang-520-add/fcl)
