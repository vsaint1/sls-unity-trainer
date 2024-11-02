#ifndef HOOKED_H
#define HOOKED_H

#include "utils.h"



#pragma region Hooks
void(__fastcall* SetMaxHP_Original)(DWORD*, int32_t, DWORD*);
void __stdcall SetMaxHp_Hook(DWORD* __this, int32_t _val, DWORD* __methodInfo) {

    _val = 1000000;

    return SetMaxHP_Original(__this, _val, __methodInfo);
}

void(__fastcall* SetAtkSpeed_Original)(DWORD*, float, DWORD*);

void __stdcall SetAtkSpeed_Hook(DWORD* __this, float _val, DWORD* __methodInfo) {

    _val = 1000.0f;
    return SetAtkSpeed_Original(__this, _val, __methodInfo);
}

void(__fastcall* TakeDamage_Original)(DWORD*, float, DWORD*);

bool(__fastcall* AllStaffsUnlocked_Original)(DWORD*, DWORD*);
bool __stdcall AllStaffsUnlocked_Hook(DWORD* __this, DWORD* __methodInfo) {
    spdlog::info("All staffs were unlocked!");
    return true;
}

bool(__fastcall* AllHatsUnlocked_Original)(DWORD*, DWORD*);

bool __stdcall AllHatsUnlocked_Hook(DWORD* __this, DWORD* __methodInfo) {
    spdlog::info("All hats were unlocked!");
    return true;
}

void __stdcall TakeDamage_Hook(DWORD* __this, float _dmg, DWORD* __methodInfo) {

    spdlog::info("PlayerStats_o* {} took {} damage", (uintptr_t) __this, _dmg);
    _dmg = configs::bInvulnerable ? 0.0f : _dmg;

    return TakeDamage_Original(__this, _dmg, __methodInfo);
}


void(__fastcall* SetDamage_Original)(DWORD*, float, DWORD*);

void __stdcall SetDamage_Hook(DWORD* __this, float _val, DWORD* __methodInfo) {
    _val = 1000000.0f;
    return SetDamage_Original(__this, _val, __methodInfo);
}

void(__fastcall* Shoot_Original)(DWORD*, DWORD*);

void __stdcall Shoot_Hook(DWORD* __this, DWORD* __methodInfo) {
    for (int i = 0; i < configs::shootsQuantity; ++i) {
        Shoot_Original(__this, __methodInfo);
    }
}

bool(__fastcall* HasSelectedStaff_Original)(DWORD*, DWORD*);

bool __stdcall HasSelectedStaff_Hook(DWORD* __this, DWORD* __methodInfo) {
    return true;
}

void(__fastcall* SelectStaffWithIndex_Original)(DWORD*, int32_t, DWORD*);

void __stdcall SelectStaffWithIndex_Hook(DWORD* __this, int32_t _index, DWORD* __methodInfo) {
    _index = configs::selectedStaff;
    return SelectStaffWithIndex_Original(__this, _index, __methodInfo);
}


#pragma endregion



#endif // HOOKED_H
