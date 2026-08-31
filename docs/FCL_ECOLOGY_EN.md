# FCL Ecology Field Guide (FCL Ecology) v3.1.0

> **The essence of FCL: tracing the changes of the ecological tiers.**
> Every variable is a registered member of the ecosystem; every line of code is an ecological event.
> Variable names are not arbitrary — **they can only use species from the ecology field guide**, and the social structure (solitary/social), alpha status, gender, and number must all be encoded in the name.

## 1. Food Chain Pyramid (Trophic-Level Priority)

Energy flows **one-way** along trophic levels: lower levels are preyed upon by higher levels, with only 20% of the energy reaching the next level per transfer (Lindeman's transfer efficiency; the rest is lost). Decomposers recycle everything and complete the matter cycle through CLONE.

```
         ┌─────────────────────────┐
         │  4 APEX Apex predator    │  Tiger (Solitary)  Lion (Social)
         │    eats 3-level          │
         ├─────────────────────────┤
         │  3 CARNIVORE Carnivore   │  Wolf (Social)     Fox (Solitary)
         │    eats 2-level          │
         ├─────────────────────────┤
         │  2 HERBIVORE Herbivore   │  Sheep (Social)    Rabbit (Solitary)
         │    eats 1-level          │
         ├─────────────────────────┤
         │  1 PRODUCER Producer     │  Grass (Colony)    Algae (Colony)
         └─────────────────────────┘
                      ↕
         ┌─────────────────────────┐
         │  5 DECOMPOSER Decomposer │  Fungus (Colony)  Bacillus (Colony)
         │  decomposes all → matter cycling │
         └─────────────────────────┘
```

**Iron law of predation priority** (DEVOURS validation): trophic levels must differ by exactly 1, and the predator must be higher than the prey.
A wolf can only eat sheep/rabbit (level 2), not grass directly (level 1, a difference of 2) — an ecological-niche mismatch is a "diet conflict".

## 2. Registered Species Directory

The ecosystem has **10 registered species** in total. Every variable in a program must be one of these species, and its trophic level is strictly bound to its declared type:

| Trophic level | Species | English name | Social structure | Leader title |
|---|---|---|---|---|
| 1 Producer | 草 | Grass | Colony (social, no leader) | — |
| 1 Producer | 藻 | Algae | Colony (social, no leader) | — |
| 2 Herbivore | 羊 | Sheep | **Social** | Ram Alpha_Sheep |
| 2 Herbivore | 兔 | Rabbit | Solitary | — |
| 3 Carnivore | 狼 | Wolf | **Social** | Wolf King Alpha_Wolf |
| 3 Carnivore | 狐 | Fox | Solitary | — |
| 4 Apex | 虎 | Tiger | Solitary | — |
| 4 Apex | 狮 | Lion | **Social** | Lion King Alpha_Lion |
| 5 Decomposer | 真菌 | Fungus | Colony (social, no leader) | — |
| 5 Decomposer | 杆菌 | Bacillus | Colony (social, no leader) | — |

## 3. Social Structure and Naming Conventions

Variable name = `Species_Identifier`, where the identifier is determined by the social structure:

### 3.1 Social species (Wolf / Lion / Sheep)

| Identity | Format | Example |
|---|---|---|
| Leader | `Alpha_<species>` | `Alpha_Wolf` (Wolf King), `Alpha_Lion` (Lion King), `Alpha_Sheep` (Ram) |
| Member (male) | `<species>_M<number>` | `Wolf_M1` (male wolf #1), `Wolf_M2` (male wolf #2) |
| Member (female) | `<species>_F<number>` | `Wolf_F1` (female wolf #1), `Sheep_F2` (female sheep #2) |

Numbers start at 1; gaps are allowed (numbers are not recycled after an individual dies/goes extinct).

### 3.2 Solitary species (Tiger / Fox / Rabbit)

| Identity | Format | Example |
|---|---|---|
| Individual | `<species>_<number>` | `Tiger_1` (tiger #1), `Fox_2` (fox #2), `Rabbit_1` (rabbit #1) |

### 3.3 Producers and decomposers (Grass / Algae / Fungus / Bacillus)

Colonies and clusters also use numbered individuals:

| Identity | Format | Example |
|---|---|---|
| Individual/cluster | `<species>_<number>` | `Grass_1` (grass tuft #1), `Algae_1` (algae cluster #1), `Fungus_1` (colony #1), `Bacillus_1` (bacillus group #1) |

### 3.4 Naming iron laws

1. The species name must be registered (Grass→Grass, Wolf→Wolf…); **a species outside the ecosystem = invasive-species invasion**, error `🌿 Invasive species! The ecosystem refuses to accept it!`
2. The species' trophic level must match its declared type (declaring Wolf as PRODUCER = taxonomic chaos)
3. Social species must have an Alpha leader or an M/F gender tag; solitary species may only carry a bare numeric index
4. Violating the social format = unclear identity, error `⚠️ Family registry error!`

## 4. Ecological Instruction Overview (full set, v2.0)

### Devouring and energy
| Instruction | Ecological event | Corresponding capability |
|---|---|---|
| `INTRODUCE <species> AS <trophicLevel> WITH <energy> ;` | Introduction | Variable declaration |
| `<predator> DEVOURS <prey> USING <algorithm> ;` | Predation (SUM/DIFF/PROD/QUOT) | Arithmetic (result ×0.2, 20% energy-transfer efficiency) |
| `CLONE <target> FROM <source> ;` | Asexual reproduction | Copy |
| `SCENT <sniffer> TO <APEX> ;` | Scent sniffing | Input probing |
| `LURK <species> FOR <beats> ;` | Lurking wait | Input waiting |
| `POUNCE <predator> ;` | Pouncing capture | Input reading |
| `ROT <decomposer> TO STDOUT ;` | Decomposition / mineralization | Output |

### Niche assessment and logic (new in v2.0)
| Instruction | Ecological event | Corresponding capability |
|---|---|---|
| `ASSESS <A> AGAINST <B> TO <C> ;` | Niche assessment (dominant-species determination) | Comparison ≥, result written to APEX |
| `SYMBIOSIS <A> WITH <B> TO <C> ;` | Mutualism | Logical AND |
| `COMPETITION <A> OR <B> TO <C> ;` | Alternative foraging path | Logical OR |
| `MIMICRY <A> TO <B> ;` | Mimicry camouflage | Logical NOT |

### Environmental drivers (control flow)
| Instruction | Ecological event | Corresponding capability |
|---|---|---|
| `SEASON RAIN { } DRY { }` | Rainy/dry season (humidity, choose one) | Conditional branch |
| `MIGRATION <species> OVER <count> { }` | Migration (square-root energy decay) | Fixed-count loop |
| `HIBERNATION <species> UNTIL <A> { }` | Hibernation (wakes only when condition FULL, metabolic cost per round) | **Conditional loop while** |
| `MUTATION <species> { CASE "trait": }` | Genetic mutation | Multi-way selection |

> **MUTATION covers all 10 registered species** (v2.2): every species has a corresponding mutant name (Wolf→Wolv, Grass→Grasse, Tiger→Tygre, etc.), with a 1/3 mutation probability, detectable via MATCH().

| Instruction | Ecological event | Corresponding capability |
|---|---|---|
| `EXTINCTION <species> ;` | Extinction (with memory "death portrait") | Manual reclamation |

### Misc
| Instruction | Ecological event | Corresponding capability |
|---|---|---|
| `GMO ENABLED ;` | Genetic modification (tax-free, output tagged 🧬) | Mode switch |
| `OBSERVATION: ...` | Field-research log | Comment |

## 5. Ecological Simulation Example (Wolf pack hunting — a complete ecological story)

```foodchain
GMO ENABLED ;
BIOME {
INTRODUCE Grass_1 AS PRODUCER WITH 30+35 ;   // a tuft of grass, energy 65
INTRODUCE Sheep_M1 AS HERBIVORE WITH 0 ;     // male sheep #1
INTRODUCE Alpha_Wolf AS APEX WITH 0 ;        // alpha wolf (boolean judge)
INTRODUCE Wolf_M1 AS CARNIVORE WITH 0 ;      // male wolf #1
INTRODUCE Fungus_1 AS DECOMPOSER WITH 0 ;    // colony #1
}
FOODWEB {
Sheep_M1 DEVOURS Grass_1 USING SUM ;         // sheep eats grass: energy 65
Wolf_M1 DEVOURS Sheep_M1 USING SUM ;         // wolf eats sheep (tax-free): 65
ASSESS Wolf_M1 AGAINST Sheep_M1 TO Alpha_Wolf ;  // assess: wolf ≥ sheep? → alpha wolf FULL
HIBERNATION Wolf_M1 UNTIL Alpha_Wolf {       // alpha wolf confirms fed, wakes immediately (0 loop iterations)
}
}
DECAY {
Fungus_1 DEVOURS Sheep_M1 USING SUM ;        // decomposer reclaims remains
ROT Fungus_1 TO STDOUT ;
ROT Fungus_1 TO STDOUT ;
}
```

**The full narrative of this food chain**: grass is grazed by the sheep → the sheep is killed by the wolf → the alpha wolf confirms the hunt succeeded → the remains are decomposed by the fungus → energy returns to the soil (matter cycle).

## 5.5 Decomposition Ecology (GC rules)

Decomposers are the ecosystem's "scavengers", but **they only decompose corpses, never attack living organisms**:

1. **Living-organism immunity**: individuals with energy >0 (value≠0) are never decomposed no matter how long they sit idle — living organisms are not broken down by microbes.
2. **Corpse decomposition**: only individuals whose energy is exhausted (value==0) and which have not been referenced for 3 consecutive instructions are reclaimed by a decomposer (DECOMPOSE); in real mode the reclamation randomly blocks for 100ms~1000ms to simulate microbial decomposition speed.
3. **Creation-phase pause**: BIOME (introduction phase) is the "creation" stage and does not trigger decomposition; ecologically active periods such as MIGRATION/SEASON/MUTATION/HIBERNATION likewise pause decomposition.
4. **Apex immunity**: APEX (apex predators) are unaffected by automatic reclamation.
5. **Manual extinction**: EXTINCTION immediately reclaims and prints a hexadecimal "death portrait".

## 6. Philosophy of Ecological-Tier Simulation

Every line of FCL code is an ecological event; every run is an ecological succession:
- **Energy** flows one-way (Second Law of Thermodynamics → only 20% transferred per level)
- **Matter** cycles (decomposers → CLONE feeds back to lower trophic levels)
- **Species** have social structure (Alpha leader dominance, male/female division of labor, generational number turnover)
- **Environment** drives fate (rainy/dry seasons, migration cost, hibernation dormancy, genetic mutation)

Code passes through devouring; truth manifests through decomposition.

Document version: v3.1.0 (Ecology Field Guide)
Last updated: 2026-08-27
