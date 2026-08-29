# FCL Textbook: The FoodChain Language — English Edition v2.4

> **The "For Real" Beginner's Guide to FCL**
>
> Every word. Every concept. Every step. Explained like you're a human — because you are.
>
> You don't need to know anything about programming. You need to know: how to type, how to count, and why cats are weird.
>
> What this book is: a patient, word-by-word walkthrough of FCL with ecological background, programmer's perspective, and output tables.
> What this book is NOT: a reference card. You can get that at [FCL_REFERENCE.md](FCL_REFERENCE.md).

---

# 🦁 Lesson 0: What Is FCL and Why Does It Exist?

> **Goals for this lesson:**
> 1. Feel what "programming" means (no jargon)
> 2. Understand why FCL is special among all programming languages
> 3. Learn ASCII — the key to understanding FCL's output (skip this and everything looks like gibberish)
> 4. Know how to actually run an FCL program

---

## 0.1 Programming — The Short Version

**Programming = writing a to-do list for a computer, in a language it understands.**

Example: You want a computer to calculate "1 + 1".

```
You (in English): Please add one and one.
Computer: The answer is 2.

You (in Python): print(1 + 1)
You (in FCL):    (a whole ecological drama — you'll see it in Lesson 1)
```

**The computer is a tool. Programming languages are translators.** FCL happens to be a translator that speaks in the language of ecology.

---

## 0.2 What's a "Variable"?

**A variable = a named box that holds a number.**

| Real-life analogy | Programming equivalent |
|---|---|
| A box labeled "x" with 5 candies inside | `x = 5` (variable `x` holds value 5) |
| Take 2 candies from box x, 3 remain | `x = x - 2` (subtraction) |
| A second box labeled "y" with 3 candies | `y = 3` |

In most languages (like Python), you can name boxes whatever you want:

```python
x = 5
apples = 5         # "apples" is fine
anything = 5       # "anything" is also fine
```

**In FCL, your boxes are species from an ecosystem.** You can't call a box "x" — it has to be a real plant or animal. That sounds weird, but here's the secret: **it's just a naming system. Under the hood, it's still a variable.** Once that clicks, everything else follows.

---

## 0.3 ASCII — The Secret Behind FCL's Output ★★★

> **This is the most important concept in all of FCL. If you skip this, every output will look like a glitch.**

**Computers don't know letters. They only know numbers.** So every character (letter, digit, symbol) is assigned a number. This mapping is called **ASCII** (American Standard Code for Information Interchange).

Key mappings to memorize:

| Number | Character | What it means |
|---|---|---|
| 48 | `0` | The digit zero |
| 49 | `1` | The digit one |
| 50 | `2` | The digit two |
| 55 | `7` | The digit seven |
| 65 | `A` | Capital letter A |
| 66 | `B` | Capital letter B |
| 90 | `Z` | Capital letter Z |
| 97 | `a` | Lowercase letter a |
| 120 | `x` | Lowercase letter x |
| 13 | (carriage return) | A newline — invisible |
| 8 | (backspace) | Deletes the previous character — invisible |

**FCL's default behavior: it doesn't output numbers. It outputs the character that the number maps to.**

Examples:
- Energy value 65 → FCL outputs `A` (not `65`)
- Energy value 55 → FCL outputs `7` (not `55`)
- Energy value 8 → FCL outputs **a backspace** (nothing visible on screen!)

> This is why beginners panic when they run their first program and see nothing or weird symbols. **It's not a bug — it's ASCII.**

**The fix (since v2.2):** Put `NUMERIC OUTPUT ;` on the first line of your program. FCL will then print the actual number instead of the character.

```
Memory trick: FCL default = "character mode" (shows letters). Add NUMERIC OUTPUT → "number mode" (shows numbers).
```

---

## 0.4 What Makes FCL Special

FCL reimagines every standard programming concept through ecology:

| Ordinary programming | FCL ecology version |
|---|---|
| Variables with arbitrary names | Species names: Grass, Sheep, Wolf, Fungus |
| Arithmetic: `+`, `-`, `*`, `/` | Devouring: `DEVOURS ... USING SUM/PROD/DIFF/QUOT` |
| Cloning without destroying | Cloning: `CLONE target FROM source` |
| Comparison: `>`, `==` | Ecological niche assessment: `ASSESS A AGAINST B TO C` |
| Logic: `AND`, `OR`, `NOT` | Symbiosis, Competition, Mimicry |
| No `if`/`for`/`while` | Weather (SEASON), Migration (MIGRATION), Hibernation (HIBERNATION) |
| Garbage collection | Ecological decomposition (Fungus eats corpses) |

**The one-line summary**: FCL is a Turing-complete esolang where the entire programming model is re-expressed as an ecosystem. It's weird, but it's not a toy — it can theoretically compute anything.

---

## 0.5 How to Run FCL (3 ways)

### Option 1: Download the binary (easiest)

Go to the GitHub Releases page, download the file for your OS:
- Windows → `fcl-windows-latest.exe`
- macOS → `fcl-macos-latest`
- Linux → `fcl-linux-latest`

Put the `.exe` (or binary) in the **same folder** as your `.fc` program file. Open a terminal in that folder (type `cmd` in the address bar on Windows) and run:

```bash
fcl myprogram.fc
```

### Option 2: Web Playground (no install needed)

Visit `fcl.pages.dev` — write code in your browser, hit run, see output instantly.

### Option 3: Build from source (for developers)

```bash
git clone https://github.com/Huang-520-add/fcl.git
cd fcl
make build
./fcl examples/example1.fc
```

---

## 0.6 Two Run Modes (remember: "default is fast")

| Mode | How to enable | When to use it |
|---|---|---|
| **Code mode** (default) | Do nothing | Learning the language — errors don't make you wait |
| **Real mode** | Put `REAL MODE ;` on the first line | Experiencing the "ecological immersion" — eating and decomposing take actual time |

```
Beginners → Code mode (default, fast)
After you understand FCL → Try Real mode for fun
```

---

## 0.7 When Something Goes Wrong

FCL error messages look like this:

```
[FCL-0003 @line 2] 🌿 An invasive species! The ecosystem rejects unknown species!
```

Breaking it down:
- `FCL-0003` = the error code (look it up)
- `@line 2` = the line where things broke
- The rest = an ecology-themed description of what went wrong

The three most common beginner errors:

| Error message | What it means | Where to look |
|---|---|---|
| 🌿 Invasive species! | You used a species name not in FCL's ecosystem (like `Dragon_1`) | Lesson 2 |
| 🦴 Dietary conflict! | The predator-prey relationship is wrong (Wolf eating Grass, not allowed) | Lesson 3 |
| 🌍 Ecosystem collapse! | Your program is missing BIOME/FOODWEB/DECAY | Lesson 1 |

---

## ✏️ Lesson 0 Exercises

1. What character does ASCII code 97 represent? (Answer: `a`)
2. What character does ASCII code 50 represent? (Answer: `2`)
3. FCL is in its default (character) mode. You ROT a decomposer with energy 65. What appears on screen? (Answer: the letter `A`)
4. How do you make FCL show the number 65 instead of the letter A? (Answer: add `NUMERIC OUTPUT ;` on the first line)
5. What are the three mandatory sections of every FCL program? (Answer: BIOME → FOODWEB → DECAY, in that exact order)

---

# 🌿 Lesson 1: The Three-Act Structure — Every FCL Program Is an Ecosystem Story

> **Goals for this lesson:**
> 1. Understand BIOME / FOODWEB / DECAY and why they exist
> 2. Run your first complete FCL program
> 3. Read the output table

---

## 1.1 Three Acts = Three Parts of an Ecosystem

Every FCL program follows this exact structure:

```
BIOME   { ... }   ← Act 1: INTRODUCE all species (variables), set their starting energy
FOODWEB { ... }   ← Act 2: The species eat each other (computation)
DECAY   { ... }   ← Act 3: Decomposers mineralize energy and output the result
```

**Why these three, and in this order?**

Because FCL is a faithful model of how real ecosystems work:

- **BIOME (biome)**: A biome is a geographic region with a particular climate and community of organisms. Before an ecosystem runs, you introduce all the species. Think of it as setting the stage before the play begins.
- **FOODWEB (food web)**: The network of who-eats-whom. This is where the action happens — where energy flows.
- **DECAY (decay)**: Decomposers break down dead organic matter into minerals that return to the soil. This is the cleanup — and also where output happens.

> **Analogy**: It's like a play. Act 1 introduces the cast (BIOME). Act 2 is the drama (FOODWEB). Act 3 is the curtain call and cleanup (DECAY). You can't skip any act, and you can't run them out of order.

---

## 1.2 Your First FCL Program (Line by Line)

Read it through first, then I'll explain every line:

```foodchain
GMO ENABLED ;                              ← [1] Turn on GMO mode (optional)
BIOME {                                    ← [2] BIOME section begins
    INTRODUCE Grass_1 AS PRODUCER WITH 60+5 ;   ← [3] Introduce: Grass_1, Producer, energy 65
    INTRODUCE Sheep_M1 AS HERBIVORE WITH 0 ;    ← [4] Introduce: Sheep_M1, Herbivore, energy 0
}                                          ← [5] BIOME section ends
FOODWEB {                                  ← [6] FOODWEB begins
    Sheep_M1 DEVOURS Grass_1 USING SUM ;    ← [7] Sheep eats Grass: energy = 0+65 = 65
}                                          ← [8] FOODWEB ends
DECAY {                                    ← [9] DECAY begins
    INTRODUCE Fungus_1 AS DECOMPOSER WITH 0 ;    ← [10] Introduce Fungus_1, Decomposer, energy 0
    Fungus_1 DEVOURS Sheep_M1 USING SUM ;        ← [11] Fungus eats Sheep: energy = 0+65 = 65
    ROT Fungus_1 TO STDOUT ;                     ← [12] Decomposer outputs to screen
    ROT Fungus_1 TO STDOUT ;                     ← [13] Decomposer outputs again (see below)
}                                          ← [14] DECAY ends
```

### Line-by-line translation:

**Line 1 — `GMO ENABLED ;`**

- `GMO` = Genetically Modified Organism
- `ENABLED` = turned on
- What it does: waives the energy-transfer loss (more on this in Lesson 5). Without this, a herbivore only gets 20% of what it eats. With GMO, it gets 100%.
- Side effect: outputs will have a 🧬 label in front (like a GMO food label).
- You can skip this line, but then your math won't add up correctly.

**Line 3 — `INTRODUCE Grass_1 AS PRODUCER WITH 60+5 ;`**

- `INTRODUCE` = introduce (create a new species in the ecosystem)
- `Grass_1` = Grass species, individual #1 (FCL requires species to follow a naming convention — see Lesson 2)
- `AS PRODUCER` = it's a Producer (trophic level 1 — the bottom of the food chain)
- `WITH 60+5` = starting energy is 65 (the `+5` is just arithmetic — FCL evaluates it)
- Plain English: "Create a Grass_1 of type PRODUCER with energy 65"

**Line 4 — `INTRODUCE Sheep_M1 AS HERBIVORE WITH 0 ;`**

- `Sheep_M1` = Sheep, Male, #1 (M = Male, F = Female — social species need gender tags)
- `AS HERBIVORE` = Herbivore (trophic level 2 — eats Producers)
- `WITH 0` = starts with zero energy (it's born, hasn't eaten yet)
- Plain English: "Create a Sheep_M1 of type HERBIVORE with energy 0"

**Line 7 — `Sheep_M1 DEVOURS Grass_1 USING SUM ;`**

- `DEVOURS` = to eat quickly and completely (the predator consumes the prey entirely)
- `USING SUM` = use the SUM algorithm (add the two energies together)
- What happens: Sheep gains Grass's energy. Grass's energy becomes 0 (it's been eaten).
- Plain English: "Sheep_M1 devours Grass_1 using SUM: Sheep gets Grass's energy (65), Grass becomes 0"

**Line 11 — `Fungus_1 DEVOURS Sheep_M1 USING SUM ;`**

- The Decomposer (Fungus) eats the Sheep. This is the "mineralization" step.
- In FCL, **only Decomposers can output** — energy must go through a Decomposer to reach the screen.

**Lines 12–13 — `ROT Fungus_1 TO STDOUT ;` (twice)**

- `ROT` = to rot / to mineralize (Decomposers break organic matter down into minerals — that's output)
- `TO STDOUT` = to standard output (your screen)
- Why twice? The first ROT outputs the character. The second ROT outputs the Unicode code point (`U+0041`). More on this in Lesson 7.

---

## 1.3 Run It and Look at the Output

Save as `hello.fc`, run:

```bash
fcl hello.fc
```

Output:

```
🧬A
U+0041
```

**Output table:**

| What you see | What it means |
|---|---|
| `🧬` | GMO label (you enabled GMO mode — this is the "GMO food" sticker) |
| `A` | ASCII character for code 65 (65 = 'A') |
| `U+0041` | Hexadecimal representation of 65 (0x41 = 65) |

---

## 1.4 Why "A"? (Understanding ASCII + ROT twice)

Because:
- Grass's energy = 65
- Sheep eats Grass → Sheep's energy = 65
- Fungus eats Sheep → Fungus's energy = 65
- ROT (1st time): 65 → ASCII character `A` (65 maps to 'A')
- ROT (2nd time): 65 → Unicode code point `U+0041`

**With `NUMERIC OUTPUT ;` enabled:**

```foodchain
GMO ENABLED ;
NUMERIC OUTPUT ;   ← Add this line
BIOME { ... }
```

Output becomes:
```
🧬65
🧬65
```

Numbers, plain and simple.

> **Recommendation for beginners**: Always add `NUMERIC OUTPUT ;`. You'll see the actual numbers. Once you understand how ASCII works, try the default mode for the "art" of it.

---

## 1.5 Three铁律 (Three Rules — Memorize These)

```
⚠️ Rule 1: All three sections are mandatory
   Missing BIOME     → 🌍 Ecosystem collapse!
   Missing FOODWEB   → 🌍 Ecosystem collapse!
   Missing DECAY     → 🌍 Ecosystem collapse!

⚠️ Rule 2: The order is fixed — never swap them
   FOODWEB { ... }
   BIOME { ... }    ← ❌ Invalid! BIOME must come first!
   DECAY { ... }

⚠️ Rule 3: FOODWEB must contain at least one DEVOURS
   No eating → 🌍 Ecosystem collapse (the food chain is broken!)
```

---

## ✏️ Lesson 1 Exercises

1. Change Grass's energy from `60+5` to `60+6` and run the program. What character appears? (Hint: 66 = 'B')
2. Delete the `GMO ENABLED ;` line and run again. Does the output change? (Hint: YES — without GMO, the sheep only gets 20% of the grass's energy)
3. Change the two ROT lines in DECAY to just one. What output do you get? (Answer: only `🧬A`, no `U+0041`)
4. If you put `NUMERIC OUTPUT ;` *after* `GMO ENABLED ;` but *before* the BIOME section, will the program still work? (Answer: Yes — it takes effect globally from wherever it's declared)

---

# 🌿 Lesson 2: The Species Registry — Only These 10 Names Are Legal

> **Goals for this lesson:**
> 1. Memorize FCL's 10 registered species
> 2. Understand "trophic levels" (who can eat whom)
> 3. Master the naming rules for social vs solitary species

---

## 2.1 Ecology Basics: Trophic Levels

In nature, organisms are classified by their position in the food chain:

```
Trophic Level 1 (Producers): Plants, algae. They make their own energy from sunlight.
                              ↓ eaten by:
Trophic Level 2 (Herbivores): Sheep, rabbits. They eat plants.
                              ↓ eaten by:
Trophic Level 3 (Carnivores): Wolves, foxes. They eat herbivores.
                              ↓ eaten by:
Trophic Level 4 (Apex Predators): Tigers, lions. No natural predators.
                              ↓ eaten by:
Trophic Level 5 (Decomposers): Fungi, bacteria. They break down dead organic matter.
```

> **Memory aid**: **"Grasses get eaten by sheep, sheep get eaten by wolves, wolves get eaten by lions, lions get eaten by fungi"** — one direction, one step at a time.

---

## 2.2 The 10 Registered Species (Only These Names Are Legal!)

FCL doesn't let you make up names. Your variables must be one of these 10 species:

| Trophic level | Keyword | Name | Social structure |
|---|---|---|---|
| 1 Producer | `PRODUCER` | Grass | Colony |
| 1 Producer | `PRODUCER` | Algae | Colony |
| 2 Herbivore | `HERBIVORE` | Sheep | **Social** (has an alpha, gender tags) |
| 2 Herbivore | `HERBIVORE` | Rabbit | Solitary (just a number) |
| 3 Carnivore | `CARNIVORE` | Wolf | **Social** (has an alpha, gender tags) |
| 3 Carnivore | `CARNIVORE` | Fox | Solitary (just a number) |
| 4 Apex | `APEX` | Tiger | Solitary |
| 4 Apex | `APEX` | Lion | **Social** |
| 5 Decomposer | `DECOMPOSER` | Fungus | Colony |
| 5 Decomposer | `DECOMPOSER` | Bacillus | Colony |

**Write these down. You cannot use any other names. Period.**

---

## 2.3 The Golden Rule: Predators Must Be Exactly One Level Higher Than Their Prey

```
Grass (1) → Sheep/Rabbit (2) → Wolf/Fox (3) → Tiger/Lion (4)
                                                       ↕
                                              Fungus/Bacillus (5)
```

| Eating scenario | Valid? | Why |
|---|---|---|
| Sheep eats Grass | ✅ Yes | Level 2 eats Level 1 (diff = 1) |
| Wolf eats Sheep | ✅ Yes | Level 3 eats Level 2 (diff = 1) |
| Tiger eats Wolf | ✅ Yes | Level 4 eats Level 3 (diff = 1) |
| Sheep eats Wolf | ❌ No | Level 2 eating Level 3 — that's backwards! |
| Wolf eats Grass | ❌ No | Wolves don't eat plants — classification error |
| Fungus eats anything | ✅ Yes | Decomposers in the DECAY section are exempt from the level rule |

---

## 2.4 How to Name Your Species: Social vs Solitary

**Social species** (Sheep, Wolf, Lion) need to track their social hierarchy:

```
Alpha leader:   Alpha_<Species>
                Example: Alpha_Wolf (the wolf pack's alpha), Alpha_Lion (the lion pride's king)

Members:        <Species>_<M|F><Number>
                Example: Wolf_M1 (Male Wolf #1), Wolf_F2 (Female Wolf #2)
                          Sheep_M1 (Male Sheep #1), Sheep_F3 (Female Sheep #3)
                          Lion_M1 (Male Lion #1)
```

**Solitary species** (Rabbit, Fox, Tiger) just need a number:

```
<Species>_<Number>
Example: Rabbit_1 (Rabbit #1), Fox_1 (Fox #1), Tiger_1 (Tiger #1), Tiger_2 (Tiger #2)
```

**Producers and Decomposers** (Grass, Algae, Fungus, Bacillus):

```
<Species>_<Number>
Example: Grass_1, Grass_2, Algae_1, Fungus_1, Bacillus_2
```

---

## 2.5 Three Classic Mistakes (Exam Material!)

```foodchain
INTRODUCE Dragon_1 AS PRODUCER WITH 1 ;
// ❌ Dragons are not in FCL's ecosystem
// Error: 🌿 Invasive species! Unknown to the ecosystem!

INTRODUCE Wolf_M1 AS PRODUCER WITH 1 ;
// ❌ Wolves are carnivores, not producers
// Error: ⚠️ Classification error! Wolf is CARNIVORE, not PRODUCER

INTRODUCE Wolf_1 AS CARNIVORE WITH 1 ;
// ❌ Wolves are social animals — they need M/F tags, not bare numbers
// Error: ⚠️ Family registry error! Wolf is social, needs M/F label
```

---

## ✏️ Lesson 2 Exercises

**Are these names legal?**

| Name | Legal? | If not, why not? |
|---|---|---|
| `Rabbit_1` | ? | |
| `Alpha_Tiger` | ? | |
| `Fox_F1` | ? | |
| `Sheep_5` | ? | |
| `Bacillus_2` | ? | |
| `Grass_1` | ? | |
| `Wolf_M3` | ? | |
| `Lion_M2` | ? | |
| `Algae_1` | ? | |

**Write the correct names for:**
1. Female Lion #2 → ?
2. The wolf pack's alpha → ?
3. Grass #3 → ?
4. Rabbit #1 → ?

**Answers:**
- `Rabbit_1` ✅ (rabbits are solitary, number is fine)
- `Alpha_Tiger` ❌ (tigers are solitary — no alpha!)
- `Fox_F1` ❌ (foxes are solitary — no gender tags!)
- `Sheep_5` ❌ (sheep are social — need M or F)
- `Bacillus_2` ✅ (colony species use numbers)
- `Grass_1` ✅ (colony species use numbers)
- `Wolf_M3` ✅ (social member, M3 = Male #3)
- `Lion_M2` ✅ (social member, M2 = Male #2)
- `Algae_1` ✅ (colony species)
- Correct names: 1) `Lion_F2`, 2) `Alpha_Wolf`, 3) `Grass_3`, 4) `Rabbit_1`

---

# 🌿 Lesson 3: INTRODUCE — Putting Species Into the Ecosystem

> **Goals for this lesson:**
> 1. Master the INTRODUCE keyword's full syntax
> 2. Understand the critical differences between the five trophic types
> 3. Know which types support decimals vs integers

---

## 3.1 INTRODUCE Keyword Breakdown

**INTRODUCE** (verb): To bring something into use or existence for the first time.

In FCL, `INTRODUCE` = declare a variable (name it, assign its type, give it a starting value).

---

## 3.2 Full Syntax

```foodchain
INTRODUCE <species_name> AS <trophic_level> WITH <initial_energy> ;
```

| Part | Required? | Meaning |
|---|---|---|
| `INTRODUCE` | Yes | Keyword — creates the variable |
| `<species_name>` | Yes | Must be from the 10 registered species |
| `AS` | Yes | Keyword — "of type" |
| `<trophic_level>` | Yes | PRODUCER / HERBIVORE / CARNIVORE / APEX / DECOMPOSER |
| `WITH` | Yes | Keyword — "with starting energy" |
| `<initial_energy>` | Yes | Number, fraction, or expression |
| `;` | Yes | Every statement ends with a semicolon |

---

## 3.3 Three Ways to Write Initial Energy

```foodchain
INTRODUCE Grass_1 AS PRODUCER WITH 65 ;        // Integer: 65
INTRODUCE Grass_2 AS PRODUCER WITH 3/2 ;        // Fraction: 3/2 = 1.5
INTRODUCE Grass_3 AS PRODUCER WITH 60+5 ;       // Expression: 60+5 = 65
INTRODUCE Grass_4 AS PRODUCER WITH (30+6)/2 ;   // Expression: (36)/2 = 18
```

> **Important**: Only `PRODUCER` (plants/algae) supports fractions. `HERBIVORE` and `CARNIVORE` will round fractions to the nearest integer.

---

## 3.4 The Five Trophic Levels in Detail

| Level | Keyword | Stores | Range | Special behavior |
|---|---|---|---|---|
| Producer | `PRODUCER` | Float | 0.0 ~ 9999.9 | Supports fractions; can be eaten by Herbivores |
| Herbivore | `HERBIVORE` | Integer | 0 ~ 255 | **Overflow = "ulcer": >255 → reset to 0 + 2 second wait** |
| Carnivore | `CARNIVORE` | Integer | 0 ~ 65535 | Odd line numbers = faster; even = 2ms slower |
| Apex | `APEX` | Boolean | 0 (HUNGRY) or 1 (FULL) | Only one that can do multiply/divide; carries ASSESS results |
| Decomposer | `DECOMPOSER` | Number → ASCII | N/A | Only one that can execute ROT (output) |

---

## 3.5 Why Does APEX Hold Only 0 or 1?

**Ecological rationale**: Apex predators are the ultimate decision-makers in an ecosystem — they have no natural enemies. In FCL, `APEX` carries two special jobs:

1. **Storing comparison results**: When you compare two numbers with `ASSESS`, the result (who is bigger?) is stored in an APEX variable as FULL (1) or HUNGRY (0).
2. **Doing multiplication and division**: Only APEX can use `PROD` (multiply) and `QUOT` (divide) — these are "advanced" operations reserved for the top of the food chain.

---

## ✏️ Lesson 3 Exercises

1. Write the INTRODUCE for Tiger #1 (APEX), starting at FULL (1). (Answer: `INTRODUCE Tiger_1 AS APEX WITH 1 ;`)
2. Write the INTRODUCE for Wolf #2 (CARNIVORE), starting at energy 50. (Answer: `INTRODUCE Wolf_M2 AS CARNIVORE WITH 50 ;`)
3. Can HERBIVORE use fractions like `3/2`? (Answer: Yes, but it rounds to the nearest integer — 2)
4. What happens to a HERBIVORE when its energy exceeds 255? (Answer: "Ulcer overflow" — energy resets to 0 and the program waits 2 seconds)

---

# 🌿 Lesson 4: DEVOURS — Eating Is Computing

> **Goals for this lesson:**
> 1. Master all four DEVOURS algorithms (SUM/DIFF/PROD/QUOT)
> 2. Understand that eating destroys the prey (destructive assignment)
> 3. Understand why only APEX can multiply and divide

---

## 4.1 DEVOURS Keyword Breakdown

**DEVOUR** (verb): to eat (food or prey) quickly and completely, usually without chewing.

FCL uses "devour" instead of "eat" because it's **destructive** — when A devours B, A gets B's energy and B becomes 0. The prey is gone.

---

## 4.2 The Four Algorithms

```foodchain
// ADD: A = A + B
Sheep_M1 DEVOURS Grass_1 USING SUM ;

// SUBTRACT: A = A - B (A must have >= B's energy, or it "starves")
Wolf_M1 DEVOURS Sheep_M1 USING DIFF ;

// MULTIPLY: A = A × B (APEX only)
Tiger_1 DEVOURS Fox_1 USING PROD ;

// DIVIDE: A = A ÷ B, remainder discarded (APEX only)
Lion_M1 DEVOURS Wolf_F1 USING QUOT ;
```

**SUM = addition (energy merges)**
- Ecology: When a predator eats prey, the prey's energy transfers to the predator.
- Example: Sheep energy 20 + Grass energy 50 → Sheep energy 70.

**DIFF = subtraction (the bigger wins, smaller loses)**
- Ecology: The stronger predator wins a confrontation; the loser loses energy.
- Example: Wolf energy 80 − Sheep energy 30 → Wolf energy 50.
- ⚠️ The predator must have *at least* as much energy as the prey, or you get: "🥀 The predator starves"

**PROD = multiplication (APEX only)**
- Ecology: An apex predator can aggregate the energy of multiple prey at once.
- Example: Tiger energy 10 × Fox energy 5 → Tiger energy 50.

**QUOT = division (APEX only)**
- Ecology: An apex predator can distribute prey among its offspring.
- Example: Lion energy 24 ÷ Wolf energy 4 → Lion energy 6 (remainder 2 is discarded).

---

## 4.3 The Prey Gets Devoured! (Destructive Assignment)

**This is the biggest difference between FCL and ordinary languages.**

Ordinary Python:
```python
x = 5
y = 3
x = x + y   # Result: x = 8, y = 3 (y is untouched!)
```

FCL:
```foodchain
INTRODUCE Grass_1 AS PRODUCER WITH 65 ;
INTRODUCE Sheep_M1 AS HERBIVORE WITH 0 ;
Sheep_M1 DEVOURS Grass_1 USING SUM ;
// Result: Sheep_M1 = 65, Grass_1 = 0 (Grass is gone!)
```

**The prey becoming 0 = the prey has been eaten. In real ecology, it is dead and cannot return.**

> **If you need to use a value without destroying it? → Use CLONE (Lesson 6).**

---

## 4.4 Why Only APEX Can Use PROD and QUOT?

**Ecological rationale**: Multiplication and division are "advanced energy operations" that only apex predators have the capacity to coordinate. Regular animals can only pass energy one step at a time along the food chain — apex predators are the only ones with the "overview" to aggregate or distribute energy across multiple levels.

**Programmer's perspective**: APEX is FCL's "math specialist" type — it has capabilities that other types don't. This is called **type restriction**.

---

## ✏️ Lesson 4 Exercises

1. Tiger energy 10, Fox energy 5, PROD → Tiger becomes what? (Answer: 50)
2. Lion energy 30, Wolf energy 4, QUOT → Lion becomes what? What happened to the remainder? (Answer: 7, remainder 2 is discarded)
3. Grass energy 100, Sheep energy 0, SUM (with tax) → Sheep and Grass become what? (Answer: Sheep = 20, Grass = 0)
4. Wolf energy 5, Sheep energy 10, DIFF → What happens? (Answer: Error "🥀 The predator starves" — Wolf doesn't have enough energy to eat Sheep)

---

# 🌿 Lesson 5: The Energy Tax and GMO — Why Do You Only Keep 20% of a Meal?

> **Goals for this lesson:**
> 1. Understand Lindeman's Law (the 10% rule in real ecology)
> 2. Know why FCL defaults to 20% energy transfer per devour
> 3. Use GMO ENABLED to bypass the tax

---

## 5.1 Lindeman's Law — Real Ecological Energy Transfer

There's a famous law in ecology called **Lindeman's Law** (the 10% Rule):

> Only about 10% to 20% of the energy at one trophic level transfers to the next level. The rest is lost as heat, respiration, and waste.

Example:
- Grass has 1,000 units of energy from sunlight
- A sheep eats the grass → the sheep only gains 100–200 units
- The other 800–900 units are spent on: breathing, moving, digesting, etc.

> **Memory aid**: **"Transfer 100, receive 20"** — FCL uses 20% as the default, representing the upper end of that 10–20% range.

---

## 5.2 FCL's Energy Tax: 80% Levied, 20% Kept

**By default: every devour only transfers 20% of the prey's energy to the predator.**

```foodchain
// Grass energy = 100
Sheep_M1 DEVOURS Grass_1 USING SUM ;
// Sheep receives: 100 × 20% = 20
// Sheep energy = 20, Grass energy = 0
```

The "lost" 80% doesn't disappear — it represents the inevitable energy lost through biological processes. This is FCL's tribute to thermodynamics.

---

## 5.3 GMO ENABLED — Skip the Tax

**GMO** = Genetically Modified Organism. In FCL: a transgenic creature that bypasses natural energy transfer losses.

```foodchain
GMO ENABLED ;                              ← Put this as the FIRST LINE
BIOME {
    INTRODUCE Grass_1 AS PRODUCER WITH 100 ;
    INTRODUCE Sheep_M1 AS HERBIVORE WITH 0 ;
}
FOODWEB {
    Sheep_M1 DEVOURS Grass_1 USING SUM ;    // Sheep gets 100 directly (no tax)
}
```

### GMO's cost: every output has a 🧬 prefix

```
🧬65    ← The 🧬 means "this result is from GMO mode"
```

### GMO's benefits:
- ✅ 100% energy transfer (tax waived)
- ✅ Predictable results (no tax distortion)
- ✅ Great for precise algorithms (Fibonacci, factorial, etc.)

### GMO's "downside":
- ❌ Outputs are prefixed with 🧬
- ❌ But honestly, for learning purposes, you should always use GMO — it's much more intuitive

---

## 5.4 Where Does the Tax Go?

```
Grass energy 100
    ↓ Sheep devours Grass (Grass = 0)
    ├→ 20% → Sheep (Sheep_M1 = 20)  ← visible energy
    └→ 80% → "respiratory loss"      ← invisible, ecological entropy
```

> **Programmer analogy**: That 80% is like "implicit overhead" in a programming language — garbage collection CPU time, system calls for memory allocation — invisible in your code, but definitely happening.

---

## ✏️ Lesson 5 Exercises

1. Grass energy 100, Sheep eats Grass (with tax) → Sheep = ? (Answer: 20)
2. Grass energy 100, Sheep eats Grass (with GMO) → Sheep = ? (Answer: 100)
3. Grass energy 50, Sheep eats Grass (with tax) → Sheep = ? (Answer: 10, because 50×0.2=10)
4. Grass energy 50, Sheep eats Grass (with GMO) → Sheep = ? (Answer: 50)

---

# 🌿 Lesson 6: CLONE — The Ecological "Copy-Paste"

> **Goals for this lesson:**
> 1. Understand why CLONE is needed (DEVOURS destroys the prey)
> 2. Understand "substance cycling" vs "energy flow"
> 3. Build a working Fibonacci sequence using CLONE

---

## 6.1 Why Do We Need CLONE?

**The DEVOURS problem**: Devouring is destructive — the prey is gone.

In Python, copying a number is trivial:
```python
a = 5
b = a       # b = 5, a is still 5
```

But in FCL, there's no simple assignment — everything is devouring. `b DEVOURS a` would destroy `a`. **That's where CLONE comes in.**

---

## 6.2 CLONE Keyword Breakdown

**CLONE** (noun/verb): a genetically identical copy of an organism.

Ecological analogy: Plant cloning (taking a cutting and growing a new plant) — you get an identical copy, and the original plant is unharmed. This is the perfect metaphor for "copy without destroying the source."

---

## 6.3 Syntax

```foodchain
CLONE <target> FROM <source> ;
```

| Part | Meaning |
|---|---|
| `CLONE` | Make a copy |
| `<target>` | The species that receives the copy (must be already INTRODUCEd) |
| `FROM` | Copy from... |
| `<source>` | The species being copied (not affected) |

---

## 6.4 Fibonacci in FCL (CLONE in Action)

**The Fibonacci sequence**: 1, 1, 2, 3, 5, 8, 13, 21... (each number = sum of the two before it).

```foodchain
GMO ENABLED ;
NUMERIC OUTPUT ;
BIOME {
    INTRODUCE Grass_1  AS PRODUCER WITH 1 ;    // F(n-2)
    INTRODUCE Grass_2  AS PRODUCER WITH 1 ;    // F(n-1)
    INTRODUCE Sheep_M1 AS HERBIVORE WITH 0 ;    // Accumulator
    INTRODUCE Algae_1  AS PRODUCER WITH 0 ;    // Backup register
    INTRODUCE Fungus_1 AS DECOMPOSER WITH 0 ;  // Output
}
FOODWEB {
    MIGRATION Sheep_M1 OVER 5 {
        CLONE Algae_1 FROM Grass_2 ;            // [1] Backup F(n-1)
        Sheep_M1 DEVOURS Grass_1 USING SUM ;    // [2] Add F(n-2)
        Sheep_M1 DEVOURS Grass_2 USING SUM ;   // [3] Add F(n-1) → F(n)
        CLONE Grass_1 FROM Algae_1 ;            // [4] Grass_1 ← old Grass_2 (shift)
        CLONE Grass_2 FROM Sheep_M1 ;           // [5] Grass_2 ← new sum
        INTRODUCE Sheep_M1 AS HERBIVORE WITH 0 ;    // [6] Reset accumulator
        INTRODUCE Algae_1  AS PRODUCER WITH 0 ;    // [7] Reset backup
    }
}
DECAY {
    Fungus_1 DEVOURS Grass_2 USING SUM ;
    ROT Fungus_1 TO STDOUT ;
    ROT Fungus_1 TO STDOUT ;
}
```

**Why do we need to backup (steps [1] and [4])?**

Because step [3] (`Sheep_M1 DEVOURS Grass_2`) sets Grass_2 to 0. Without the backup, Grass_2 (which holds F(n-1)) is gone — the next round can't continue the sequence.

> **Analogy**: You're copying homework answers. You photocopy the original before writing on it, so the original stays pristine.

---

## ✏️ Lesson 6 Exercises

1. After `CLONE Tiger_1 FROM Wolf_M1 ;`, is Wolf_M1 reset to 0? (Answer: No, Wolf_M1 keeps its value)
2. In `CLONE Tiger_1 FROM Grass_1 ;`, what type must Tiger_1 be? (Answer: APEX — Tiger is an apex predator)
3. Why do we reset the accumulator and backup in each Fibonacci loop iteration? (Answer: To avoid leftover energy from the previous iteration affecting the next)
4. Change `OVER 5` to `OVER 8`. What gets output? (Answer: F(9) = 34)

---

# 🌿 Lesson 7: ROT — Only Decomposers Can Speak

> **Goals for this lesson:**
> 1. Understand why only DECOMPOSER can execute ROT
> 2. Master character mode vs number mode
> 3. Understand why ROT must be called twice

---

## 7.1 ROT Keyword Breakdown

**ROT** (verb): to decay; to break down organic matter into minerals that return to the environment.

Ecological meaning: Decomposers (fungi, bacteria, earthworms) transform dead organic matter into minerals (CO₂, water, inorganic salts) that enrich the soil. This is output — matter leaving the ecosystem and returning to the environment.

> **Programmer's equivalent**: `ROT xxx TO STDOUT` ≈ `print(x)`, but with stricter rules.

---

## 7.2 Why Only Decomposers Can Output?

Ecological logic: Energy must go through a decomposer's mineralization to return to the environment. Grass can't directly "speak" — it must first be eaten by a herbivore, then eaten by a decomposer. The nutrients must pass through the decomposition stage.

```
Grass (PRODUCER) → Sheep (HERBIVORE) → Fungus (DECOMPOSER) → Output
```

**Why can't Grass just ROT directly?**

> **Ecological answer**: Because in nature, dead organic matter must be decomposed before nutrients return to the soil. Having Grass output directly is like a dead body standing up and speaking — it violates ecological logic.

---

## 7.3 Two Output Modes

### Character mode (default)

```foodchain
// No declaration = character mode (default)
ROT Fungus_1 TO STDOUT ;    // 1st time: outputs the ASCII character
ROT Fungus_1 TO STDOUT ;    // 2nd time: outputs U+XXXX (Unicode code point)
```

| Energy | 1st ROT output | 2nd ROT output |
|---|---|---|
| 65 | `A` | `U+0041` |
| 55 | `7` | `U+0037` |
| 48 | `0` | `U+0030` |
| 8 | backspace (invisible!) | `U+0008` |
| 13 | carriage return (invisible!) | `U+000D` |

### Number mode (since v2.2)

```foodchain
NUMERIC OUTPUT ;           // ← Turn on number mode
ROT Fungus_1 TO STDOUT ;  // Always outputs the actual number: 65
ROT Fungus_1 TO STDOUT ;  // Still outputs the number: 65
```

| Energy | Character mode | Number mode |
|---|---|---|
| 65 | `A` + `U+0041` | `65` |
| 55 | `7` + `U+0037` | `55` |
| 48 | `0` + `U+0030` | `48` |

> **Recommendation**: Always use `NUMERIC OUTPUT ;` when starting out. It's like turning on subtitles — you'll actually understand what's happening.

---

## 7.4 Why Two ROT Calls?

This is a **deliberate Esolang design choice** — two ROT calls simulate two ecological processes:

```
1st ROT:  Mineralization → produces output (the character)
2nd ROT:  Scientific cataloging → records the code point (U+XXXX, for reference)
```

| ROT call | Meaning | Analogy |
|---|---|---|
| 1st | Energy → Character | Minerals in soil become visible "things" |
| 2nd | Character's "birth certificate" | A scientist records: "Grass_1's ASCII code is 65" |

---

## ✏️ Lesson 7 Exercises

1. Fungus energy 72, character mode, 1st ROT → what appears? (Answer: 'H' — ASCII 72 = 'H')
2. Fungus energy 72, number mode, ROT → what appears? (Answer: `72`)
3. Fungus energy 13, character mode, 1st ROT → what appears? (Answer: a carriage return — invisible)
4. Why did FCL design it so that only decomposers can output? (Answer: To follow ecological logic — nutrients must be mineralized by decomposers before returning to the environment)

---

# 🌿 Lesson 8: SEASON — Making Decisions Based on Weather

> **Goals for this lesson:**
> 1. Understand the humidity mechanism (last 3 devours decide the weather)
> 2. Master RAIN / DRY two-way logic
> 3. Understand what STORM does

---

## 8.1 SEASON Keyword Breakdown

**SEASON** (noun): a division of the year marked by particular climate conditions and animal behaviors.

Ecological meaning: In savannas and other seasonal climates, animal behavior changes dramatically between wet and dry seasons. Rainfall (how often devours happen) determines how "active" the ecosystem is.

> **Programmer's equivalent**: `SEASON RAIN { } DRY { }` ≈ `if (humidity >= 2) { /* RAIN */ } else { /* DRY */ }`

---

## 8.2 Humidity = How Many DEVOURS Happened in the Last 3 Instructions

```foodchain
SEASON RAIN {
    // If humidity >= 2 (at least 2 DEVOURS in the last 3 instructions) → execute here
} DRY {
    // If humidity < 2 → execute here
}
```

**How to calculate humidity**: Look at the **last 3 executed DEVOURS statements**. Count them.

```
Humidity = min(count_of_devours_in_last_3, 2)   // maximum is 2

Humidity = 2 → RAIN (wet season, ecosystem is active)
Humidity = 1 → DRY (dry season, ecosystem is dormant)
Humidity = 0 → DRY (extreme drought)
```

---

## 8.3 Full Example

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
    Sheep_M1 DEVOURS Grass_1 USING SUM ;   // 1st devour ← humidity starts rising
    Sheep_M1 DEVOURS Grass_2 USING SUM ;   // 2nd devour ← humidity = 2 → RAIN
    SEASON RAIN {
        Fungus_1 DEVOURS Sheep_M1 USING SUM ;  // RAIN branch: output 30
    } DRY {
        INTRODUCE Fungus_1 AS DECOMPOSER WITH 1 ; // DRY branch: just set to 1
    }
}
DECAY {
    ROT Fungus_1 TO STDOUT ;
}
```

Last 3 DEVOURS: Grass1, Grass2 → humidity = 2 → **RAIN branch** → outputs 30.

---

## 8.4 STORM — Randomly Ordered Rain

By default, RAIN blocks with more than 3 statements are **not shuffled** (deterministic behavior).

If you add `STORM ENABLED ;` at the top of your program, RAIN blocks with more than 3 statements are **randomly shuffled once** (non-deterministic).

> ⚠️ STORM is useful for simulating random ecological events, but it breaks programs that depend on statement order.

---

## ✏️ Lesson 8 Exercises

1. Last 3 instructions: [devour, devour, INTRODUCE]. Humidity = ? → RAIN or DRY? (Answer: humidity = 2 → RAIN)
2. Last 3 instructions: [devour, INTRODUCE, INTRODUCE]. Humidity = ? (Answer: humidity = 1 → DRY)
3. What is the threshold that determines RAIN vs DRY? (Answer: 2 — humidity >= 2 → RAIN, < 2 → DRY)

---

# 🌿 Lesson 9: MIGRATION — The Migration Loop (Fixed-Count Loop)

> **Goals for this lesson:**
> 1. Understand the fixed-count loop mechanism
> 2. Understand "migration cost" (energy square-roots each round)
> 3. Choose the right species as the "migrator"

---

## 9.1 MIGRATION Keyword Breakdown

**MIGRATION** (noun): the movement of animals from one region to another, typically seasonally, in search of food or breeding grounds.

Ecological meaning: Long-distance migration (wildebeest crossing the Mara River, birds crossing the Himalayas) is energetically devastating. FCL models this energy loss as **square-root decay** each round.

> **Programmer's equivalent**: `MIGRATION Sheep_M1 OVER 5 { }` ≈ `for (int i = 0; i < 5; i++) { }`

---

## 9.2 Syntax

```foodchain
MIGRATION <migrator> OVER <count> {
    // loop body
}
```

| Part | Meaning |
|---|---|
| `<migrator>` | The species that bears the "migration cost" (energy gets square-rooted each round) |
| `<count>` | How many times to loop (an integer constant — no variables) |

---

## 9.3 Migration Cost — Energy Gets Square-Rooted Each Round

```foodchain
INTRODUCE Sheep_M1 AS HERBIVORE WITH 100 ;
MIGRATION Sheep_M1 OVER 4 {
    // loop body
}
```

| Round | Sheep_M1's energy | What happened |
|---|---|---|
| Start | 100 | Just arrived |
| After round 1 | 10 | 100 → √100 = 10 |
| After round 2 | 3.16 | 10 → √10 ≈ 3.16 |
| After round 3 | 1.78 | 3.16 → √3.16 ≈ 1.78 |
| After round 4 | 1.33 | 1.78 → √1.78 ≈ 1.33 |

> **Memory aid**: **"100 → 10 → 3.16 → 1.78 → 1.33…"** Energy drops sharply then plateaus near 1.

---

## 9.4 Trap: Don't Let the Migrator Hold Important Data!

```foodchain
// ❌ WRONG: Making Grass the migrator
INTRODUCE Grass_1 AS PRODUCER WITH 100 ;
MIGRATION Grass_1 OVER 3 {
    Sheep_M1 DEVOURS Grass_1 USING SUM ;  // Grass gets smaller each round — data lost!
}

// ✅ RIGHT: Use a species that gets reset every round
INTRODUCE Sheep_M1 AS HERBIVORE WITH 0 ;
MIGRATION Sheep_M1 OVER 3 {
    Sheep_M1 DEVOURS Grass_1 USING SUM ;
    Sheep_M1 DEVOURS Grass_2 USING SUM ;
    INTRODUCE Sheep_M1 AS HERBIVORE WITH 0 ;  // ← Reset! Avoids sqrt accumulation
}
```

The key insight: the **migrator** should be a "worker" species (like an accumulator) that gets reset each round, not a data source.

---

## ✏️ Lesson 9 Exercises

1. Migrator starts at 81 energy, runs 3 rounds → energy = ? (Answer: 81 → 9 → 3 → √3 ≈ 1.73)
2. In the Fibonacci example, why is Sheep_M1 (the accumulator) the migrator rather than Grass_1 (the data)? (Answer: because Sheep_M1 is reset to 0 each round, so sqrt doesn't accumulate on important data)
3. Migrator starts at 16 energy, runs 3 rounds → energy = ? (Answer: 16 → 4 → 2 → √2 ≈ 1.41)

---

# 🌿 Lesson 10: HIBERNATION — The Hibernation Loop (Conditional Loop)

> **Goals for this lesson:**
> 1. Understand the conditional loop mechanism
> 2. Understand "hibernation metabolism" (energy × 0.9 each round)
> 3. Understand the anti-infinite-loop safeguard (10,000 rounds)

---

## 10.1 HIBERNATION Keyword Breakdown

**HIBERNATION** (noun): a state of deep sleep and metabolic depression that some animals enter during winter to conserve energy when food is scarce.

Ecological meaning: Bears hibernate through winter when food is scarce, dramatically reducing their metabolism. But they can't hibernate forever — eventually they must wake up.

> **Programmer's equivalent**: `HIBERNATION Fungus_1 UNTIL Tiger_1 { }` ≈ `while (Tiger_1 != FULL) { }`

---

## 10.2 Syntax

```foodchain
HIBERNATION <hibernator> UNTIL <APEX_condition> {
    // loop body
}
```

| Part | Meaning |
|---|---|
| `<hibernator>` | The species that bears the metabolic cost (energy × 0.9 each round) |
| `<APEX_condition>` | An APEX variable — loop stops when this = FULL (1) |
| `<loop body>` | What runs each iteration |

---

## 10.3 Hibernation Metabolism — Energy × 0.9 Each Round

```foodchain
INTRODUCE Tiger_1 AS APEX WITH 0 ;   // Condition: HUNGRY (0), waiting for FULL
INTRODUCE Fungus_1 AS DECOMPOSER WITH 100 ;  // Hibernator
HIBERNATION Fungus_1 UNTIL Tiger_1 {
    // Each round: Fungus_1 energy × 0.9
    // Some ASSESS inside should eventually make Tiger_1 = FULL
}
```

| Round | Fungus_1 energy | What happened |
|---|---|---|
| Start | 100 | Bear goes to sleep |
| After round 1 | 90 | 100 × 0.9 = 90 |
| After round 2 | 81 | 90 × 0.9 = 81 |
| ... | ... | |
| After round 10 | 34.9 | 100 × 0.9^10 |
| After round 100 | 0.00026 | Approaches zero |

---

## 10.4 Anti-Infinite-Loop: 10,000 Rounds Maximum

If the APEX condition is never met, the loop would run forever. FCL protects against this: **after 10,000 iterations, FCL forcibly wakes the program** (⏰ hibernation timeout).

---

## ✏️ Lesson 10 Exercises

1. Hibernator starts at 100 energy, runs 5 rounds → energy = ? (Answer: 100 × 0.9^5 ≈ 59)
2. When does HIBERNATION trigger the "10,000 rounds" timeout? (Answer: when the APEX condition is never met — infinite loop)
3. What type must the condition in `UNTIL` be? (Answer: APEX — only apex predators have the "decision-making" right)

---

# 🌿 Lesson 11: Comparison and Boolean Logic — ASSESS + SYMBIOSIS / COMPETITION / MIMICRY

> **Goals for this lesson:**
> 1. Use ASSESS to compare two energy values
> 2. Combine boolean logic with SYMBIOSIS (AND), COMPETITION (OR), MIMICRY (NOT)
> 3. Understand why APEX is required for boolean operations

---

## 11.1 ASSESS — Ecological Niche Assessment (Comparison)

**ASSESS** (verb): to evaluate or estimate the nature, ability, or quality of.

Ecological meaning: When two species compete for the same niche, you need to determine dominance. The decision-making power belongs to apex predators — they're at the top of the food chain.

```foodchain
ASSESS <A> AGAINST <B> TO <C> ;
```

- A ≥ B → C = FULL(1) ("A won the niche competition")
- A < B → C = HUNGRY(0) ("A lost")
- **C must be APEX**

---

## 11.2 The Three Boolean Operations

**SYMBIOSIS** (noun): a close and long-term biological interaction between two different species (mutual benefit).
→ Programmer's `AND`: both must be 1 to result in 1.

```foodchain
SYMBIOSIS Lion_M1 WITH Lion_F1 TO Tiger_1 ;
// Tiger_1 = Lion_M1 AND Lion_F1
```

**COMPETITION** (noun): a rivalry between organisms competing for the same resources.
→ Programmer's `OR`: either being 1 results in 1.

```foodchain
COMPETITION Lion_M1 OR Lion_F1 TO Tiger_1 ;
// Tiger_1 = Lion_M1 OR Lion_F1
```

**MIMICRY** (noun): when one species resembles another to gain an advantage.
→ Programmer's `NOT`: flips 0↔1.

```foodchain
MIMICRY Lion_M1 TO Tiger_1 ;
// Tiger_1 = NOT Lion_M1
```

---

## 11.3 Truth Tables

| A | B | A AND B | A OR B | NOT A |
|---|---|---|---|---|
| 0 | 0 | 0 | 0 | 1 |
| 0 | 1 | 0 | 1 | 1 |
| 1 | 0 | 0 | 1 | 0 |
| 1 | 1 | 1 | 1 | 0 |

---

## ✏️ Lesson 11 Exercises

1. Wolf energy 80, Sheep energy 20, ASSESS result stored in Tiger (APEX) → Tiger = ? (Answer: 80≥20 → FULL → 1)
2. A=1, B=0. What is A AND B? (Answer: 0)
3. A=0, B=1. What is A OR B? (Answer: 1)
4. A=1. What is NOT A? (Answer: 0)

---

# 🌿 Lesson 12: MUTATION — Random Genetic Variation

> **Goals for this lesson:**
> 1. Understand the 1/3 probability mutation mechanism
> 2. Use MATCH() to detect whether mutation occurred
> 3. Write programs that adapt to random changes

---

## 12.1 MUTATION Keyword Breakdown

**MUTATION** (noun): a change in the DNA sequence of an organism, resulting in a new trait.

Ecological meaning: Genetic mutations are random and unpredictable — they're the raw material of evolution. FCL's MUTATION models this: a species name has a 1/3 chance of changing to a "mutated name" at runtime.

> **Programmer's equivalent**: `MUTATION` ≈ `switch` + random case + `MATCH()` detection

**What happens when MUTATION triggers:**

1. **Species-level renaming**: for the duration of the block, every registered member of that species is temporarily renamed (`Wolf_M1` → `Wolv_M1`, `Alpha_Wolf` → `Alpha_Wolv`), and references inside the block are rewritten accordingly; original names are restored after the block, so outside references are unaffected
2. **One random case is expressed**: a single `CASE` branch is chosen uniformly at random and executed (if mutation doesn't trigger, the whole block is a no-op)
3. **Mutation is detectable**: `MATCH(Wolf)` / `MATCH(Wolf_M1)` / `MATCH(Wolv_M1)` all return 1, and the record persists across blocks

Note: mutated names (like `Wolv`) are not in the ecosystem registry — you can't `INTRODUCE` them. A mutant is an alien to the ecosystem.

---

## 12.2 The 10 Mutated Names

| Original | Mutated |
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

## 12.3 MATCH() — Detecting Mutations

```foodchain
MATCH(Wolf_M1)   // Returns 1 if the Wolf species mutated to Wolv at runtime; 0 if unchanged
```

MATCH() resolves by species root name, so `MATCH(Wolf)`, `MATCH(Wolf_M1)` and `MATCH(Wolv_M1)` are all equivalent. Use `MATCH()` inside expressions to make decisions based on whether mutation occurred.

---

## ✏️ Lesson 12 Exercises

1. What is the mutation probability in FCL? (Answer: 1/3)
2. What is the mutated name of Wolf? (Answer: Wolv)
3. `MATCH(Wolf_M1)` returns 1 — what happened? (Answer: Wolf_M1 mutated into Wolv_M1 at runtime)

---

# 🌿 Lesson 13: Garbage Collection — Fungi Are the Cleanup Crew

> **Goals for this lesson:**
> 1. Understand FCL's automatic memory cleanup mechanism
> 2. Master the "living creature exemption" rule
> 3. Use EXTINCTION for manual cleanup

---

## 13.1 What Is Garbage Collection?

**Garbage collection (GC)** = automatically cleaning up data that is no longer needed.

Ordinary languages (Python/Java):
```python
x = 5
x = 10    # The old value 5 → Python automatically collected it
```

FCL doesn't use "memory" — it uses **ecological decomposition**:

```
Energy = 0 → "a corpse"
Fungus = cleanup crew → decomposes the corpse
```

---

## 13.2 The Five Rules of Decomposition

```
✅ Rule 1: Living creatures are exempt
   Any species with energy > 0 is NEVER collected
   (Living organisms are not decomposed)

❌ Rule 2: Corpses decompose
   Any species with energy = 0 AND untouched for 3 instructions → 🍄 collected

⏸  Rule 3: Active periods pause collection
   During BIOME introductions and control block execution → decomposition pauses

🚫 Rule 4: Apex is exempt
   APEX species (Tiger, Lion) are NEVER automatically collected

💀 Rule 5: Manual extinction
   EXTINCTION → immediate collection + 16-line hex "death portrait"
```

---

## 13.3 EXTINCTION — Manual Cleanup

```foodchain
EXTINCTION Grass_1 ;   // Immediate collection, prints a 16-line hex "portrait"
```

**Bonus usage**: `EXTINCTION Virus_Crash ;` → forcibly terminates the entire program (a virus outside the ecosystem can only be "extincted").

---

## ✏️ Lesson 13 Exercises

1. Grass energy 100, left untouched for 10 instructions. Will it be collected? (Answer: No — living creatures are exempt)
2. Grass energy 0, untouched for 3 instructions. Will it be collected? (Answer: Yes — 🍄 decomposed)
3. Can an APEX species (Tiger) be automatically collected? (Answer: No — apex exemption)
4. What does `EXTINCTION Virus_Crash ;` do? (Answer: Force-terminates the program)

---

# 🌿 Lesson 14: Project — Factorial in FCL (5! = 120)

> **Goals for this lesson:**
> See all the concepts working together in a real program.

---

## 14.1 What Is a Factorial?

```
5! = 5 × 4 × 3 × 2 × 1 = 120
n! = n × (n-1) × ... × 2 × 1
```

## 14.2 The Design

- Tiger (APEX) = multiplier accumulator, starts at 1
- Wolf = factor source, starts at 2
- Fox = carrier (clone Wolf, gets eaten by Tiger so Wolf survives)
- Sheep = +1 incrementer (each round gives Wolf +1)
- Fungus = output

**Why the Fox?** Because Tiger eating Wolf destroys Wolf. But Wolf is the factor source — we need it for the next round. So: `CLONE Fox_1 FROM Wolf_M1` → Tiger eats Fox → Wolf survives for next round.

---

## 14.3 Full Code

```foodchain
GMO ENABLED ;
NUMERIC OUTPUT ;
BIOME {
    INTRODUCE Tiger_1  AS APEX WITH 1 ;    // Multiplier, starts at 1
    INTRODUCE Fox_1    AS CARNIVORE WITH 0 ;  // Carrier
    INTRODUCE Wolf_M1  AS CARNIVORE WITH 2 ;  // Factor source, starts at 2
    INTRODUCE Sheep_M1 AS HERBIVORE WITH 1 ;  // +1 incrementer
    INTRODUCE Fungus_1 AS DECOMPOSER WITH 0 ;  // Output
}
FOODWEB {
    MIGRATION Sheep_M1 OVER 4 {             // Loop 4 rounds (×2, ×3, ×4, ×5)
        CLONE Fox_1 FROM Wolf_M1 ;          // [1] Clone Wolf → Fox
        Tiger_1 DEVOURS Fox_1 USING PROD ;  // [2] Tiger eats Fox → multiply
        INTRODUCE Sheep_M1 AS HERBIVORE WITH 1 ;  // [3] Reset Sheep
        Wolf_M1 DEVOURS Sheep_M1 USING SUM ; // [4] Wolf +1 → next factor
    }
}
DECAY {
    Fungus_1 DEVOURS Tiger_1 USING SUM ;
    ROT Fungus_1 TO STDOUT ;
}
```

| Round | Tiger (accumulator) | Wolf (factor) |
|---|---|---|
| Start | 1 | 2 |
| Round 1 | 1×2=**2** | 3 |
| Round 2 | 2×3=**6** | 4 |
| Round 3 | 6×4=**24** | 5 |
| Round 4 | 24×5=**120** | 6 |

**Output: 120** ✅

---

# 🌿 Lesson 15: Turing Completeness — How Powerful Is FCL?

> **Goals for this lesson:**
> 1. Understand what "Turing complete" means
> 2. Know FCL's three enabling capabilities
> 3. Understand FCL's practical limitations

---

## 15.1 What Does "Turing Complete" Mean?

**Turing complete** = a system that can simulate any universal Turing machine — meaning, in theory, it can compute anything that is computable.

> **Simple analogy**: A deck of cards can theoretically be used to play any card game (Texas Hold'em, Bridge, Poker...). A deck is "Turing complete" for card games. It's not the *best* tool for every game, but it *can* play them all. FCL is the "weird-rules deck" of programming languages.

---

## 15.2 FCL's Three Turing-Complete Capabilities

| Capability | FCL implementation | Ordinary language equivalent |
|---|---|---|
| **Comparison** | `ASSESS A AGAINST B TO C` | `>`, `==`, `!=` |
| **Conditional loop** | `HIBERNATION ... UNTIL APEX` | `while` loop |
| **Unlimited storage** | `INTRODUCE` (arbitrarily many variables) | Variable list |

**All three together = Turing complete.**

---

## 15.3 FCL's Practical Limits

FCL is Turing complete, but it's not *convenient*:

| Limitation | What it means |
|---|---|
| Energy caps | HERBIVORE maxes out at 255 — bad for large numbers |
| Floating-point rounding | PRODUCER stores floats — small precision errors |
| No direct assignment | Everything must go through devouring — indirect |
| No arrays/lists | Only scalar variables — no collections |

> **But that's the beauty of Esolangs**: constraints are style, style is language.

---

# 📖 Appendix: Complete Glossary

| Term | What it means |
|---|---|
| `INTRODUCE` | Create a variable (species) in the ecosystem |
| `DEVOURS` | Devour (destructive assignment — prey is eaten and set to 0) |
| `SUM` | Add the two energies together |
| `DIFF` | Subtract (predator minus prey, predator must have ≥ prey) |
| `PROD` | Multiply (APEX only) |
| `QUOT` | Divide (APEX only, remainder discarded) |
| `CLONE` | Copy a value without destroying the source |
| `ROT` | Mineralize to output (DECOMPOSER only) |
| `ASSESS` | Compare two values, store result in APEX |
| `SYMBIOSIS` | Boolean AND |
| `COMPETITION` | Boolean OR |
| `MIMICRY` | Boolean NOT |
| `SEASON` | Humidity-driven if/else (RAWN vs DRY) |
| `RAIN` | Wet season branch (humidity ≥ 2) |
| `DRY` | Dry season branch (humidity < 2) |
| `MIGRATION` | Fixed-count loop, energy square-roots each round |
| `HIBERNATION` | Conditional loop, energy × 0.9 each round |
| `MUTATION` | Random multi-way choice, 1/3 probability mutation |
| `EXTINCTION` | Manual garbage collection |
| `GMO` | Transgenic mode — bypasses energy tax |
| `PRODUCER` | Trophic level 1 (plants/algae) |
| `HERBIVORE` | Trophic level 2 (plant-eaters) |
| `CARNIVORE` | Trophic level 3 (meat-eaters) |
| `APEX` | Trophic level 4 (top predators) |
| `DECOMPOSER` | Trophic level 5 (cleanup crew) |
| `NUMERIC OUTPUT` | Output numbers instead of ASCII characters |
| `REAL MODE` | Ecological wait times are enabled |
| `CODE MODE` | Skip all wait times (default) |
| `FULL` | APEX = 1 (the comparison condition was met) |
| `HUNGRY` | APEX = 0 (the comparison condition was not met) |
| `MATCH` | Detect whether a species mutated at runtime |

---

> Textbook v2.4 | Reference Implementation: [Huang-520-add/fcl](https://github.com/Huang-520-add/fcl)
> Quick Reference: [FCL_REFERENCE.md](FCL_REFERENCE.md) | Ecology Guide: [FCL_ECOLOGY.md](FCL_ECOLOGY.md) | Syntax Spec: [FCL_SYNTAX.md](FCL_SYNTAX.md)
