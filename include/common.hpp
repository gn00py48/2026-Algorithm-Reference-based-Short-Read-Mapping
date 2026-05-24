#ifndef COMMON_HPP
#define COMMON_HPP

#include <random>
#include <string>
#include <vector>

namespace srm {

struct ExperimentConfig {
    int referenceLength = 10000;       // N
    int readCount = 1000;              // M
    int readLength = 50;               // L
    int allowedMismatch = 2;           // D
    double snpRate = 0.003;            // 0.3%
    int maxReadError = 1;              // per read substitution error count range: 0~maxReadError
    unsigned int randomSeed = 42;      // reproducibility
};

struct Read {
    int id = -1;
    std::string seq;
    int trueStart = -1;                // for evaluation only. Do NOT use inside mapping algorithms.
};

struct Seed {
    std::string seq;
    int offset = 0;                    // seed start offset inside read
};

struct Candidate {
    int start = -1;                    // candidate start position in reference
    int support = 0;                   // number of seeds supporting this start
    double score = 0.0;                // weighted score
    int mismatch = -1;                 // mismatch after verification
};

struct MappingResult {
    int readId = -1;
    int predictedStart = -1;           // -1 means not mapped
    int trueStart = -1;
    int mismatchCount = -1;
    int candidateCount = 0;
    bool isExactCorrect = false;
    bool isValidMatch = false;
};

struct EvaluationSummary {
    int totalReads = 0;
    int mappedReads = 0;
    int exactCorrect = 0;
    int validMatches = 0;
    double exactAccuracy = 0.0;
    double validAccuracy = 0.0;
    double avgCandidateCount = 0.0;
    double avgMismatchCount = 0.0;
};

char randomBase(std::mt19937& rng);
char randomBaseExcept(char original, std::mt19937& rng);

std::string generateRandomGenome(int length, std::mt19937& rng);

std::string mutateGenomeBySNP(
    const std::string& reference,
    double snpRate,
    std::mt19937& rng
);

std::vector<Read> generateReads(
    const std::string& personalGenome,
    int readCount,
    int readLength,
    int maxReadError,
    std::mt19937& rng
);

int countMismatchWithLimit(
    const std::string& reference,
    const std::string& read,
    int start,
    int allowedMismatch
);

std::vector<Seed> splitReadIntoSeeds(
    const std::string& read,
    int allowedMismatch
);

bool isValidCandidateStart(int start, int referenceLength, int readLength);

MappingResult makeUnmappedResult(const Read& read);

MappingResult makeMappingResult(
    const Read& read,
    int predictedStart,
    int mismatchCount,
    int candidateCount,
    const std::string& reference,
    int allowedMismatch
);

EvaluationSummary evaluateResults(const std::vector<MappingResult>& results);

void printEvaluationSummary(const std::string& algorithmName, const EvaluationSummary& summary);

} // namespace srm

#endif // COMMON_HPP
