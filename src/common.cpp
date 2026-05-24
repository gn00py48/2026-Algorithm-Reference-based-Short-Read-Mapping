#include "common.hpp"

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <stdexcept>
#include <unordered_set>

namespace srm {

char randomBase(std::mt19937& rng) {
    static const char bases[] = {'A', 'C', 'G', 'T'};
    std::uniform_int_distribution<int> dist(0, 3);
    return bases[dist(rng)];
}

char randomBaseExcept(char original, std::mt19937& rng) {
    char next = original;
    while (next == original) {
        next = randomBase(rng);
    }
    return next;
}

std::string generateRandomGenome(int length, std::mt19937& rng) {
    if (length <= 0) {
        throw std::invalid_argument("Genome length must be positive.");
    }

    std::string genome;
    genome.reserve(static_cast<std::size_t>(length));

    for (int i = 0; i < length; ++i) {
        genome.push_back(randomBase(rng));
    }

    return genome;
}

std::string mutateGenomeBySNP(
    const std::string& reference,
    double snpRate,
    std::mt19937& rng
) {
    if (snpRate < 0.0 || snpRate > 1.0) {
        throw std::invalid_argument("SNP rate must be between 0 and 1.");
    }

    std::string personalGenome = reference;
    std::bernoulli_distribution mutate(snpRate);

    for (char& base : personalGenome) {
        if (mutate(rng)) {
            base = randomBaseExcept(base, rng);
        }
    }

    return personalGenome;
}

std::vector<Read> generateReads(
    const std::string& personalGenome,
    int readCount,
    int readLength,
    int maxReadError,
    std::mt19937& rng
) {
    if (readCount < 0) {
        throw std::invalid_argument("Read count cannot be negative.");
    }
    if (readLength <= 0) {
        throw std::invalid_argument("Read length must be positive.");
    }
    if (maxReadError < 0) {
        throw std::invalid_argument("Max read error cannot be negative.");
    }
    if (static_cast<std::size_t>(readLength) > personalGenome.size()) {
        throw std::invalid_argument("Read length cannot be larger than genome length.");
    }

    std::vector<Read> reads;
    reads.reserve(static_cast<std::size_t>(readCount));

    std::uniform_int_distribution<int> startDist(
        0,
        static_cast<int>(personalGenome.size()) - readLength
    );
    std::uniform_int_distribution<int> errorCountDist(0, maxReadError);
    std::uniform_int_distribution<int> readPosDist(0, readLength - 1);

    for (int id = 0; id < readCount; ++id) {
        int start = startDist(rng);
        std::string seq = personalGenome.substr(static_cast<std::size_t>(start), static_cast<std::size_t>(readLength));

        int errorCount = errorCountDist(rng);
        std::unordered_set<int> usedPositions;

        while (static_cast<int>(usedPositions.size()) < errorCount) {
            int pos = readPosDist(rng);
            if (usedPositions.insert(pos).second) {
                seq[static_cast<std::size_t>(pos)] = randomBaseExcept(seq[static_cast<std::size_t>(pos)], rng);
            }
        }

        reads.push_back(Read{id, seq, start});
    }

    return reads;
}

int countMismatchWithLimit(
    const std::string& reference,
    const std::string& read,
    int start,
    int allowedMismatch
) {
    if (!isValidCandidateStart(start, static_cast<int>(reference.size()), static_cast<int>(read.size()))) {
        return allowedMismatch + 1;
    }

    int mismatch = 0;
    for (int i = 0; i < static_cast<int>(read.size()); ++i) {
        if (reference[static_cast<std::size_t>(start + i)] != read[static_cast<std::size_t>(i)]) {
            ++mismatch;
            if (mismatch > allowedMismatch) {
                return mismatch;
            }
        }
    }

    return mismatch;
}

std::vector<Seed> splitReadIntoSeeds(
    const std::string& read,
    int allowedMismatch
) {
    if (allowedMismatch < 0) {
        throw std::invalid_argument("Allowed mismatch cannot be negative.");
    }
    if (read.empty()) {
        throw std::invalid_argument("Read cannot be empty.");
    }

    int seedCount = allowedMismatch + 1;
    int readLength = static_cast<int>(read.size());
    int baseLength = std::max(1, readLength / seedCount);

    std::vector<Seed> seeds;
    seeds.reserve(static_cast<std::size_t>(seedCount));

    for (int i = 0; i < seedCount; ++i) {
        int offset = i * baseLength;
        if (offset >= readLength) {
            break;
        }

        int length = (i == seedCount - 1) ? (readLength - offset) : baseLength;
        seeds.push_back(Seed{read.substr(static_cast<std::size_t>(offset), static_cast<std::size_t>(length)), offset});
    }

    return seeds;
}

bool isValidCandidateStart(int start, int referenceLength, int readLength) {
    return start >= 0 && readLength >= 0 && start + readLength <= referenceLength;
}

MappingResult makeUnmappedResult(const Read& read) {
    return MappingResult{
        read.id,
        -1,
        read.trueStart,
        -1,
        0,
        false,
        false
    };
}

MappingResult makeMappingResult(
    const Read& read,
    int predictedStart,
    int mismatchCount,
    int candidateCount,
    const std::string& reference,
    int allowedMismatch
) {
    bool exact = predictedStart == read.trueStart;
    bool valid = false;

    if (predictedStart >= 0) {
        int verifiedMismatch = countMismatchWithLimit(reference, read.seq, predictedStart, allowedMismatch);
        valid = verifiedMismatch <= allowedMismatch;
    }

    return MappingResult{
        read.id,
        predictedStart,
        read.trueStart,
        mismatchCount,
        candidateCount,
        exact,
        valid
    };
}

EvaluationSummary evaluateResults(const std::vector<MappingResult>& results) {
    EvaluationSummary summary;
    summary.totalReads = static_cast<int>(results.size());

    if (results.empty()) {
        return summary;
    }

    long long candidateSum = 0;
    long long mismatchSum = 0;
    int mismatchCounted = 0;

    for (const auto& result : results) {
        if (result.predictedStart >= 0) {
            ++summary.mappedReads;
        }
        if (result.isExactCorrect) {
            ++summary.exactCorrect;
        }
        if (result.isValidMatch) {
            ++summary.validMatches;
        }

        candidateSum += result.candidateCount;
        if (result.mismatchCount >= 0) {
            mismatchSum += result.mismatchCount;
            ++mismatchCounted;
        }
    }

    summary.exactAccuracy = 100.0 * summary.exactCorrect / summary.totalReads;
    summary.validAccuracy = 100.0 * summary.validMatches / summary.totalReads;
    summary.avgCandidateCount = static_cast<double>(candidateSum) / summary.totalReads;
    summary.avgMismatchCount = mismatchCounted == 0 ? 0.0 : static_cast<double>(mismatchSum) / mismatchCounted;

    return summary;
}

void printEvaluationSummary(const std::string& algorithmName, const EvaluationSummary& summary) {
    std::cout << "\n[" << algorithmName << "]\n";
    std::cout << "Total reads          : " << summary.totalReads << '\n';
    std::cout << "Mapped reads         : " << summary.mappedReads << '\n';
    std::cout << "Exact accuracy       : " << std::fixed << std::setprecision(2) << summary.exactAccuracy << "%\n";
    std::cout << "Valid accuracy       : " << std::fixed << std::setprecision(2) << summary.validAccuracy << "%\n";
    std::cout << "Avg candidate count  : " << std::fixed << std::setprecision(2) << summary.avgCandidateCount << '\n';
    std::cout << "Avg mismatch count   : " << std::fixed << std::setprecision(2) << summary.avgMismatchCount << '\n';
}

} // namespace srm
