#include "mem/memory.h"


namespace memory {

    uintptr_t GetModuleBase(const char* moduleName) {

        return (uintptr_t) GetModuleHandle(moduleName);
    }

    
    uintptr_t ResolveAddress(uintptr_t baseAddress, const std::vector<uintptr_t>& offsets) {
        uintptr_t address = baseAddress;
        for (const auto& offset : offsets) {
            address = *reinterpret_cast<uintptr_t*>(address);
            address += offset;
        }
        return address;
    }

} // namespace memory
