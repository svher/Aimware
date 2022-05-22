#include "memory.h"
#include <vector>

void *Memory::PatternScan(HMODULE module, const char *pattern) {
    static auto patternToByte = [](const char* pattern) noexcept {
        std::vector<std::int32_t> bytes{};
        char *start = const_cast<char*>(pattern);
        const char *end = const_cast<char*>(pattern) + std::strlen(pattern);

        for (auto current = start; current < end; ++current) {
            if (*current == '?') {
                ++current;
                if (*current == '?') {
                    ++current;
                }
                bytes.push_back(-1);
            } else {
                bytes.push_back(std::strtoul(current, &current, 16));
            }
        }
        return bytes;
    };

    if (!module) {
        return nullptr;
    }
    const auto dosHeader = reinterpret_cast<PIMAGE_DOS_HEADER>(module);
    const auto ntHeaders = reinterpret_cast<PIMAGE_NT_HEADERS>(reinterpret_cast<uint8_t*>(module) + dosHeader->e_lfanew);

    const std::uintptr_t moduleSize = ntHeaders->OptionalHeader.SizeOfImage;
    const std::vector<std::int32_t> vecPattern = patternToByte(pattern);
    auto scanBytes = reinterpret_cast<std::uint8_t*>(module);
    const std::size_t patternSize = vecPattern.size();
    const std::int32_t* patternData = vecPattern.data();

    for (auto i = 0ul; i < moduleSize - patternSize; i++) {
        bool found = true;
        for (auto j = 0ul; j < patternSize; ++j) {
            if (scanBytes[i + j] != patternData[j] && patternData[j] != -1) {
                found = false;
                break;
            }
        }
        if (found) {
            return &scanBytes[i];
        }
    }
    return nullptr;
}
