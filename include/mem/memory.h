#ifndef MEMORY_H
#define MEMORY_H

#include <intrin.h>
#include <memory>
#include <optional>
#include <utility>
#include <vector>

#include <windows.h>


namespace memory {

    uintptr_t GetModuleBase(const char* moduleName);

    uintptr_t ResolveAddress(uintptr_t baseAddress, const std::vector<uintptr_t>& offsets);
    
    template <typename T>
    T GetValue(uintptr_t address) {
        return *(reinterpret_cast<T*>(address));
    }

    template <typename T>
    void SetValue(uintptr_t address, const T& value) {
        *(T*) address = value;
    }

    template <typename T>
    T GetValue(uintptr_t baseAddress, const std::vector<uintptr_t>& offsets) {
        uintptr_t finalAddress = ResolveAddress(baseAddress, offsets);
        return *(reinterpret_cast<T*>(finalAddress));
    }

    template <typename T>
    void SetValue(uintptr_t baseAddress, const std::vector<uintptr_t>& offsets, const T& value) {
        uintptr_t finalAddress                = ResolveAddress(baseAddress, offsets);
        *(reinterpret_cast<T*>(finalAddress)) = value;
    }

} // namespace memory


#endif // MEMORY_H
