#include <MinHook.h>
#include <thread>
#include "menu/imgui_hook.h"

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


}; // namespace offests

uintptr_t MODULE_BASE = (uintptr_t) GetModuleHandle("GameAssembly.dll");

void(__fastcall* SetMaxHP_Original)(DWORD*, int32_t, DWORD*);
void __stdcall SetMaxHp_Hook(DWORD* __this, int32_t _val, DWORD* __methodInfo) {

    _val = 1000000;

    return SetMaxHP_Original(__this, _val, __methodInfo);
}

void(__fastcall* SetAtkSpeed_Original)(DWORD*, float, DWORD*);

void __stdcall SetAtkSpeed_Hook(DWORD* __this, float _val, DWORD* __methodInfo) {

    printf("SetAtkSpeed: %f\n", _val);
    _val = 1000.0f;
    return SetAtkSpeed_Original(__this, _val, __methodInfo);
}

void(__fastcall* TakeDamage_Original)(DWORD*, float, DWORD*);

bool(__fastcall* AllStaffsUnlocked_Original)(DWORD*, DWORD*);
bool __stdcall AllStaffsUnlocked_Hook(DWORD* __this, DWORD* __methodInfo) {
    printf("All staffs were unlocked!\n");
    return true;
}

bool(__fastcall* AllHatsUnlocked_Original)(DWORD*, DWORD*);

bool __stdcall AllHatsUnlocked_Hook(DWORD* __this, DWORD* __methodInfo) {
    printf("All hats were unlocked!\n");
    return true;
}

void __stdcall TakeDamage_Hook(DWORD* __this, float _dmg, DWORD* __methodInfo) {

    _dmg = 0.0f;

    return TakeDamage_Original(__this, _dmg, __methodInfo);
}


void(__fastcall* SetDamage_Original)(DWORD*, float, DWORD*);

void __stdcall SetDamage_Hook(DWORD* __this, float _val, DWORD* __methodInfo) {
    _val = 1000000.0f;
    return SetDamage_Original(__this, _val, __methodInfo);
}

void(__fastcall* Shoot_Original)(DWORD*, DWORD*);

void __stdcall Shoot_Hook(DWORD* __this, DWORD* __methodInfo) {
    for (int i = 0; i < 100; ++i) {
        Shoot_Original(__this, __methodInfo);
    }
}

bool(__fastcall* HasSelectedStaff_Original)(DWORD*, DWORD*);

bool __stdcall HasSelectedStaff_Hook(DWORD* __this, DWORD* __methodInfo) {
    printf("Unlocking selected staff\n");
    return true;
}

void(__fastcall* SelectStaffWithIndex_Original)(DWORD*, int32_t, DWORD*);

void __stdcall SelectStaffWithIndex_Hook(DWORD* __this, int32_t _index, DWORD* __methodInfo) {
    _index = 7;
    return SelectStaffWithIndex_Original(__this, _index, __methodInfo);
}


static unsigned long FakeEntry(HMODULE _hModule) {
    AllocConsole();
    FILE* file{0};
    freopen_s(&file, "CONOUT$", "w", __acrt_iob_func(1));


    printf("GameAssembly.dll base %p\n", MODULE_BASE);
    MH_Initialize();
    MH_CreateHook(reinterpret_cast<LPVOID*>(MODULE_BASE + offests::MAX_HP), &SetMaxHp_Hook,
        reinterpret_cast<LPVOID*>(&SetMaxHP_Original));

    MH_CreateHook(reinterpret_cast<LPVOID*>(MODULE_BASE + offests::ATK_SPEED), &SetAtkSpeed_Hook,
        reinterpret_cast<LPVOID*>(&SetAtkSpeed_Original));

    MH_CreateHook(reinterpret_cast<LPVOID*>(MODULE_BASE + offests::TAKE_DAMAGE), &TakeDamage_Hook,
        reinterpret_cast<LPVOID*>(&TakeDamage_Original));

    MH_CreateHook(reinterpret_cast<LPVOID*>(MODULE_BASE + offests::UNLOCK_STAFFS), &AllStaffsUnlocked_Hook,
        reinterpret_cast<LPVOID*>(&AllStaffsUnlocked_Original));

    MH_CreateHook(reinterpret_cast<LPVOID*>(MODULE_BASE + offests::SET_DAMAGE), &SetDamage_Hook,
        reinterpret_cast<LPVOID*>(&SetDamage_Original));

    MH_CreateHook(reinterpret_cast<LPVOID*>(MODULE_BASE + offests::SHOOT_FN), &Shoot_Hook,
        reinterpret_cast<LPVOID*>(&Shoot_Original));

    MH_CreateHook(reinterpret_cast<LPVOID*>(MODULE_BASE + offests::SELECTED_STAFF), &HasSelectedStaff_Hook,
        reinterpret_cast<LPVOID*>(&HasSelectedStaff_Original));

    MH_CreateHook(reinterpret_cast<LPVOID*>(MODULE_BASE + offests::SELECTED_STAFF_IDX), &SelectStaffWithIndex_Hook,
        reinterpret_cast<LPVOID*>(&SelectStaffWithIndex_Original));

    MH_EnableHook(MH_ALL_HOOKS);

    bool init_hook = false;
    do {
        if (kiero::init(kiero::RenderType::D3D11) == kiero::Status::Success) {
            kiero::bind(8, (void**) &ImGuiHook::Present_Original, Present_Hook);
            init_hook = true;
        }
    } while (!init_hook);

    while (!GetAsyncKeyState(VK_END)) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    fclose(file);
    FreeConsole();
    MH_DisableHook(MH_ALL_HOOKS);
    MH_Uninitialize();

    FreeLibraryAndExitThread(_hModule, NULL);
}

BOOL APIENTRY DllMain(HMODULE _hModule, DWORD ul_reason_for_call, LPVOID _lpReserved) {
    UNREFERENCED_PARAMETER(_lpReserved);

    if (ul_reason_for_call == DLL_PROCESS_ATTACH) {

        DisableThreadLibraryCalls(_hModule);

        auto hThread = CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE) FakeEntry, _hModule, 0, nullptr);

        if (hThread) {
            CloseHandle(hThread);
        }
    }

    if (ul_reason_for_call == DLL_PROCESS_DETACH) {
        kiero::shutdown();
        SetWindowLongPtr(ImGuiHook::window, GWLP_WNDPROC, (LONG_PTR) ImGuiHook::oWndProc); // COMMENT: restore the original WndProc
        return TRUE;
    }

    return TRUE;
}
