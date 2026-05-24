#ifndef ALGORITHMS_HPP
#define ALGORITHMS_HPP

#include "common.hpp"
#include <string>
#include <vector>

namespace srm {

std::vector<MappingResult> runNativeSliding(
    const std::string& reference,
    const std::vector<Read>& reads,
    int allowedMismatch
);

std::vector<MappingResult> runKMPSeed(
    const std::string& reference,
    const std::vector<Read>& reads,
    int allowedMismatch
);

std::vector<MappingResult> runHashSeed(
    const std::string& reference,
    const std::vector<Read>& reads,
    int allowedMismatch
);

std::vector<MappingResult> runWeightedHashSeed(
    const std::string& reference,
    const std::vector<Read>& reads,
    int allowedMismatch
);

} // namespace srm

#endif // ALGORITHMS_HPP
