#ifndef CONFIGS_H
#define CONFIGS_H

inline uintptr_t MODULE_BASE = 0;

namespace configs {

    inline bool bMenuOpened          = false;
    inline bool bWindowShouldClose   = false;
    inline bool bInvulnerable        = false;
    inline int selectedStaff         = 0;
    inline int selectedHat           = 0;
    inline int damageMultiplier      = 1;
    inline float atkSpeedMultiplier  = 1.0f;
    inline bool removeShootsCooldown = false;
    inline int shootsQuantity        = 1;
    inline int maxHp                 = 20;
    inline int coins                 = 0;
    inline static const char* hatNames[]{"Wizard", "Helmet", "Propeller", "Uncommon", "Challenger's", "Fedora"};

    inline static const char* staffNames[]{
        "Wizard", "Emerald ", "Trident", "Boomstaff", "Thunder", "Frozen", "Rainbow"};

}; // namespace configs


namespace offests {
    constexpr uintptr_t MAX_HP             = 0x339250;
    constexpr uintptr_t ATK_SPEED          = 0x339220;
    constexpr uintptr_t TAKE_DAMAGE        = 0x339980;
    constexpr uintptr_t UNLOCK_STAFFS      = 0x3374F0;
    constexpr uintptr_t UNLOCK_HATS        = 0x337290;
    constexpr uintptr_t SET_DAMAGE         = 0x339230;
    constexpr uintptr_t SHOOT_FN           = 0x335CC0;
    constexpr uintptr_t SELECTED_STAFF     = 0x338910;
    constexpr uintptr_t SELECTED_STAFF_IDX = 0x338EB0;
    constexpr uintptr_t COINS              = 0x2221698;


}; // namespace offests

#endif // CONFIGS_H
