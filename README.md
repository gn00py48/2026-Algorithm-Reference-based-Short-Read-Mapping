# 2026 Algorithm Short Read Mapping

## 2026 알고리즘 설계 프로젝트: Reference Genome 기반 Short Read Mapping 알고리즘 비교

본 프로젝트는 **Reference Genome 기반 Short Read Mapping 문제**를 다룬다.

Reference genome과 여러 short read가 주어졌을 때, 각 read가 reference genome의 어느 위치에서 유래했는지 찾는 알고리즘을 구현하고 성능을 비교한다.

---

## 1. Repository 정보

```text
Repository name: 2026-algorithm-short-read-mapping
Project title: 2026 알고리즘 설계 프로젝트: Reference Genome 기반 Short Read Mapping 알고리즘 비교
```

---

## 2. 프로젝트 목표

본 프로젝트의 목표는 다음과 같다.

```text
1. Reference-based Short Read Mapping 문제를 정의한다.
2. 수업 시간에 다룬 알고리즘을 활용해 여러 mapping 알고리즘을 구현한다.
3. 동일한 데이터와 동일한 평가 기준으로 알고리즘 성능을 비교한다.
4. 각 알고리즘의 시간복잡도, 정확도, 장단점을 분석한다.
```

---

## 3. 구현 알고리즘

본 프로젝트에서는 다음 4가지 알고리즘을 구현한다.

| 번호 | 알고리즘 | 수업 연결 |
|---:|---|---|
| 1 | Native Sliding Mapping | 순차 탐색, Brute Force 문자열 매칭 |
| 2 | KMP-based Seed Search | KMP, prefix/suffix, SP table |
| 3 | Hash-based Seed-and-Extend | Rabin-Karp, Hashing, Indexing |
| 4 | Weighted Hash Seed-and-Extend | Hashing, Greedy, Selection/Ranking |

---

## 4. 공통 가정

```text
- DNA alphabet은 A, C, G, T 네 문자로 제한한다.
- Reference genome R은 랜덤하게 생성한다.
- Personal genome G는 R에 SNP mutation을 적용해 생성한다.
- Short read는 personal genome G에서 추출한다.
- 알고리즘은 reference genome R과 short reads만 사용한다.
- trueStart는 평가용으로만 사용한다.
- insertion/deletion은 고려하지 않는다.
- reverse complement는 고려하지 않는다.
- substitution mismatch만 고려한다.
- Denovo assembly와 BWT/FM-index는 구현하지 않고 Future Work로만 언급한다.
```

---

## 5. 기본 파라미터

| 파라미터 | 의미 | 기본값 |
|---|---|---:|
| `N` | reference genome 길이 | 10,000 |
| `M` | read 개수 | 1,000 |
| `L` | read 길이 | 50 |
| `D` | 허용 mismatch 수 | 2 |
| `snpRate` | SNP mutation 비율 | 0.3% |
| `maxReadError` | read 하나당 추가 error 최대 개수 | 1 |

---

## 6. 데이터 생성 방식

```text
1. 길이 N의 reference genome R을 랜덤 생성한다.
2. R에 SNP mutation을 적용해 personal genome G를 생성한다.
3. G에서 길이 L의 short read를 M개 추출한다.
4. 각 read에 0 ~ maxReadError개의 substitution error를 추가한다.
5. 각 read에 id, seq, trueStart를 저장한다.
6. 알고리즘은 R과 reads만 사용하여 predictedStart를 찾는다.
7. trueStart는 평가 단계에서만 사용한다.
```

---

## 7. Seed 분할 규칙

Seed 기반 알고리즘은 다음 규칙을 공통으로 사용한다.

```text
seedCount = D + 1
seedLength = L / seedCount
마지막 seed는 남은 길이를 포함한다.
```

예시:

```text
L = 50
D = 2
seedCount = 3
seed 길이 = 16, 16, 18
```

---

## 8. 프로젝트 구조

```text
project/
├── CMakeLists.txt
├── Makefile
├── README.md
├── CONTRIBUTING.md
├── .gitignore
├── docs/
│   ├── PROJECT_DESIGN.md
│   ├── GITHUB_GUIDE.md
│   └── TEAM_GUIDE.md
├── include/
│   ├── common.hpp
│   └── algorithms.hpp
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

## 9. 빌드 및 실행 방법

### Makefile 사용

```bash
make
make run
```

### CMake 사용

```bash
cmake -S . -B build
cmake --build build
./build/short_read_mapping
```

---

## 10. 역할 분담

알고리즘 구현 외에도 공통 환경 설계, 성능 분석, 결과 해석, 발표 준비 등의 역할이 필요하다고 판단하여 역할을 다음과 같이 분담하였다.

| 이름 | 역할 |
|---|---|
| 근우 | 문제 정의 확정, 공통 가정 정리, 데이터 생성 방식 설계, 기본 파라미터 설정, 공통 구조체 설계, 공통 유틸 함수 작성 등 프로젝트 기본 환경 구축 |
| 동원 | 알고리즘 성능 분석 |
| 현욱 | 결과 해석 및 비교 분석 |
| 재범 | 발표 장표 제작 |

---

## 11. GitHub 협업 방식

GitHub를 처음 사용하는 팀원을 고려하여 각자 본인 이니셜 브랜치에서 작업한다.

| 이름 | 브랜치명 |
|---|---|
| 근우 | `kgw` |
| 동원 | `ldw` |
| 현욱 | `hhw` |
| 재범 | `jjb` |

핵심 규칙:

```text
1. main 브랜치에 직접 push하지 않는다.
2. 본인 이니셜 브랜치에서 작업한다.
3. 작업 시작 전 main을 최신화한다.
4. 작업 완료 후 Pull Request를 생성한다.
5. 공통 구조체와 함수 시그니처는 임의로 변경하지 않는다.
```

자세한 협업 규칙은 `docs/GITHUB_GUIDE.md`를 참고한다.

---

## 12. 평가 지표

모든 알고리즘은 다음 지표로 비교한다.

| 지표 | 설명 |
|---|---|
| Running Time | 알고리즘 실행 시간 |
| Exact Position Accuracy | `predictedStart == trueStart`인 read 비율 |
| Valid Mapping Accuracy | `predictedStart` 위치에서 mismatch가 `D` 이하인 read 비율 |
| Average Candidate Count | read 하나당 생성된 후보 위치 수 평균 |
| Average Mismatch Count | 최종 선택 위치에서의 평균 mismatch 수 |
| Index Size / Memory Usage | hash index 등에 저장된 항목 수 또는 메모리 사용량 추정 |

---

## 13. 실험 계획

### 실험 1. 알고리즘별 기본 성능 비교

```text
N = 10,000
M = 1,000
L = 50
D = 2
snpRate = 0.3%
maxReadError = 1
```

### 실험 2. SNP rate 변화

```text
snpRate = 0.1%, 0.3%, 0.5%, 1.0%
```

### 실험 3. repeat 여부 비교

```text
random reference
repeat-inserted reference
```

---

## 14. 참고 문서

```text
docs/GITHUB_GUIDE.md       // GitHub 협업 가이드
docs/PROJECT_DESIGN.md     // 프로젝트 기초 설계 설명 문서
docs/TEAM_GUIDE.md         // 팀 역할 및 진행 가이드
```
