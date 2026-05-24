#include "algorithms.hpp"

namespace srm {

// TODO: KMP-based Seed Search 구현 담당자가 채울 함수입니다.
std::vector<MappingResult> runKMPSeed(
    const std::string& /*reference*/,
    const std::vector<Read>& reads,
    int /*allowedMismatch*/
) {
    std::vector<MappingResult> results;
    results.reserve(reads.size());

    for (const auto& read : reads) {
        results.push_back(makeUnmappedResult(read));
    }

    return results;
}

} // namespace srm
