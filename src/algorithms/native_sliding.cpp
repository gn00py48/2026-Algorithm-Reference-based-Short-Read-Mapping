#include "algorithms.hpp"

#include <limits>

namespace srm {

// TODO(근우): Native Sliding Mapping 구현 예정.
// 현재는 팀원이 공통 인터페이스에 맞춰 개발할 수 있도록 no-op 형태만 제공합니다.
std::vector<MappingResult> runNativeSliding(
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
