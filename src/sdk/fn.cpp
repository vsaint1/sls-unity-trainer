#include "sdk/fn.h"

namespace fn {

    int32_t GetCoins() {
        auto value = memory::GetValue<int32_t>(MODULE_BASE + offests::COINS, {0xB8, 0x10, 0x1C8, 0x18});
        return value;
    }
    
    void AddCoins(int32_t _value) {
        memory::SetValue<int32_t>(MODULE_BASE + offests::COINS, {0xB8, 0x10, 0x1C8, 0x18}, _value);
    }
} // namespace fn
