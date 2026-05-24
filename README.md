# Reference-based Short Read Mapping

## 1. Project Overview

This project compares several algorithms for **Reference-based Short Read Mapping**.

Given a reference genome `R` and multiple short reads, the goal is to find the position in `R` where each read most likely originated.

We focus on a step-by-step improvement structure:

1. Native Sliding Mapping
2. KMP-based Seed Search
3. Hash-based Seed-and-Extend
4. Weighted Hash Seed-and-Extend

This structure connects course algorithms such as sequential search, brute-force string matching, KMP, hashing/Rabin-Karp style indexing, greedy selection, and candidate ranking.

---

## 2. Problem Definition

### Input

- Reference genome `R`
- Short reads
- Read length `L`
- Allowed mismatch count `D`

### Output

For each read:

- predicted mapping position
- true start position, for evaluation only
- mismatch count
- candidate count
- exact/valid mapping result

---

## 3. Shared Assumptions

- DNA alphabet is `{A, C, G, T}`.
- A random reference genome `R` is generated.
- A personal genome `G` is generated from `R` by applying SNP mutations.
- Short reads are sampled from `G`.
- Additional sequencing errors may be added to each read.
- Only substitution mismatches are considered.
- Insertions and deletions are not considered.
- Reverse complement is not considered.
- `trueStart` is used only for evaluation. Mapping algorithms must not use it.

---

## 4. Default Parameters

| Parameter | Meaning | Default |
|---|---|---:|
| `N` | reference length | 10,000 |
| `M` | number of reads | 1,000 |
| `L` | read length | 50 |
| `D` | allowed mismatch count | 2 |
| `snpRate` | SNP mutation rate | 0.003 |
| `maxReadError` | max per-read substitution error | 1 |
| `randomSeed` | random seed | 42 |

For debugging, use smaller values:

```cpp
N = 100;
M = 5;
L = 10;
D = 1;
```

---

## 5. Seed Rule

Seed-based algorithms use the same seed split rule:

```text
seedCount = D + 1
seedLength = L / seedCount
the last seed includes remaining characters
```

Example:

```text
L = 50, D = 2
seedCount = 3
seed lengths = 16, 16, 18
```

Candidate start position is calculated as:

```text
candidateStart = seedPos - seedOffset
```

Invalid candidates are removed:

```text
candidateStart < 0
candidateStart + L > N
duplicated candidateStart
```

---

## 6. Repository Structure

```text
.
├── CMakeLists.txt
├── Makefile
├── README.md
├── docs/
│   └── MEETING_DECISIONS.md
├── include/
│   ├── algorithms.hpp
│   └── common.hpp
├── src/
│   ├── main.cpp
│   ├── common.cpp
│   └── algorithms/
│       ├── native_sliding.cpp
│       ├── kmp_seed.cpp
│       ├── hash_seed.cpp
│       └── weighted_hash_seed.cpp
├── data/
├── scripts/
└── tests/
```

---

## 7. Build and Run

### Option A. Makefile

```bash
make
make run
```

### Option B. CMake

```bash
cmake -S . -B build
cmake --build build
./build/short_read_mapping
```

---

## 8. Team Implementation Guide

All algorithms must use the same function style:

```cpp
std::vector<MappingResult> runNativeSliding(
    const std::string& reference,
    const std::vector<Read>& reads,
    int allowedMismatch
);
```

Each algorithm must return `std::vector<MappingResult>`.

Do not change common structs without team agreement.

---

## 9. Current Status

The initial scaffold includes:

- common data structures
- random reference genome generator
- SNP mutation generator
- short read generator
- read error insertion
- seed splitting utility
- mismatch counting utility
- candidate boundary validation
- common evaluation summary
- algorithm function stubs

The algorithm files currently contain TODO stubs. Each teammate should implement their assigned algorithm inside the corresponding file.

---

## 10. Role Distribution Draft

| Member | Main Responsibility |
|---|---|
| Geunwoo | common setup, data generation, assumptions, Native Sliding baseline |
| Member A | KMP-based Seed Search |
| Member B | Hash-based Seed-and-Extend |
| Member C | Weighted Hash Seed-and-Extend, evaluation/result analysis |

