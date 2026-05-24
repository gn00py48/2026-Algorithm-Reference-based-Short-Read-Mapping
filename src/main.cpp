#include "algorithms.hpp"
#include "common.hpp"

#include <chrono>
#include <iostream>
#include <random>

using namespace srm;

template <typename Func>
void runAndReport(const std::string& name, Func&& func) {
    auto start = std::chrono::steady_clock::now();
    auto results = func();
    auto end = std::chrono::steady_clock::now();

    auto elapsedMs = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    auto summary = evaluateResults(results);
    printEvaluationSummary(name, summary);
    std::cout << "Running time         : " << elapsedMs << " ms\n";
}

int main() {
    ExperimentConfig config;

    // 디버깅을 쉽게 하려면 아래 값을 작게 바꾸세요.
    // config.referenceLength = 100;
    // config.readCount = 5;
    // config.readLength = 10;
    // config.allowedMismatch = 1;

    std::mt19937 rng(config.randomSeed);

    std::cout << "Reference-based Short Read Mapping Scaffold\n";
    std::cout << "N=" << config.referenceLength
              << ", M=" << config.readCount
              << ", L=" << config.readLength
              << ", D=" << config.allowedMismatch
              << ", SNP rate=" << config.snpRate
              << ", max read error=" << config.maxReadError
              << "\n";

    std::string reference = generateRandomGenome(config.referenceLength, rng);
    std::string personalGenome = mutateGenomeBySNP(reference, config.snpRate, rng);
    std::vector<Read> reads = generateReads(
        personalGenome,
        config.readCount,
        config.readLength,
        config.maxReadError,
        rng
    );

    std::cout << "\nSample read check\n";
    for (int i = 0; i < std::min(3, static_cast<int>(reads.size())); ++i) {
        std::cout << "read_" << reads[i].id
                  << " trueStart=" << reads[i].trueStart
                  << " seq=" << reads[i].seq.substr(0, std::min(20, static_cast<int>(reads[i].seq.size())))
                  << "...\n";
    }

    if (!reads.empty()) {
        auto seeds = splitReadIntoSeeds(reads.front().seq, config.allowedMismatch);
        std::cout << "\nSeed split example for read_0\n";
        for (const auto& seed : seeds) {
            std::cout << "offset=" << seed.offset << ", seed=" << seed.seq << "\n";
        }
    }

    // 현재 알고리즘 함수들은 no-op 스텁입니다. 각 담당자가 내부를 구현하면 그대로 결과 비교가 가능합니다.
    runAndReport("Native Sliding", [&] {
        return runNativeSliding(reference, reads, config.allowedMismatch);
    });

    runAndReport("KMP Seed", [&] {
        return runKMPSeed(reference, reads, config.allowedMismatch);
    });

    runAndReport("Hash Seed", [&] {
        return runHashSeed(reference, reads, config.allowedMismatch);
    });

    runAndReport("Weighted Hash Seed", [&] {
        return runWeightedHashSeed(reference, reads, config.allowedMismatch);
    });

    return 0;
}
