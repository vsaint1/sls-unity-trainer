#include "mem/memory.h"
#include "menu/imgui_hook.h"
#include "sdk/hooked.h"
#include <thread>


bool SetupHooks() {
    MH_Initialize();

    MH_CreateHook(reinterpret_cast<LPVOID*>(MODULE_BASE + offests::MAX_HP), &SetMaxHp_Hook,
        reinterpret_cast<LPVOID*>(&SetMaxHP_Original));

    MH_CreateHook(reinterpret_cast<LPVOID*>(MODULE_BASE + offests::ATK_SPEED), &SetAtkSpeed_Hook,
        reinterpret_cast<LPVOID*>(&SetAtkSpeed_Original));

    MH_CreateHook(reinterpret_cast<LPVOID*>(MODULE_BASE + offests::TAKE_DAMAGE), &TakeDamage_Hook,
        reinterpret_cast<LPVOID*>(&TakeDamage_Original));

    MH_CreateHook(reinterpret_cast<void**>(MODULE_BASE + offests::UNLOCK_STAFFS), &AllStaffsUnlocked_Hook,
        reinterpret_cast<void**>(&AllStaffsUnlocked_Original));

    MH_CreateHook(reinterpret_cast<LPVOID*>(MODULE_BASE + offests::SET_DAMAGE), &SetDamage_Hook,
        reinterpret_cast<LPVOID*>(&SetDamage_Original));

    MH_CreateHook(reinterpret_cast<LPVOID*>(MODULE_BASE + offests::SHOOT_FN), &Shoot_Hook,
        reinterpret_cast<LPVOID*>(&Shoot_Original));

    MH_CreateHook(reinterpret_cast<LPVOID*>(MODULE_BASE + offests::SELECTED_STAFF), &HasSelectedStaff_Hook,
        reinterpret_cast<LPVOID*>(&HasSelectedStaff_Original));

    MH_CreateHook(reinterpret_cast<LPVOID*>(MODULE_BASE + offests::SELECTED_STAFF_IDX), &SelectStaffWithIndex_Hook,
        reinterpret_cast<LPVOID*>(&SelectStaffWithIndex_Original));

    auto status = MH_EnableHook(MH_ALL_HOOKS);

    if (status != MH_OK) {
        spdlog::error("Failed to setup hooks: {}", MH_StatusToString(status));
        return false;
    }

    return true;
}

void ShutdownHooks() {
    MH_DisableHook(MH_ALL_HOOKS);
    MH_Uninitialize();
}

static unsigned long FakeEntry(HMODULE _hModule) {
    AllocConsole();
    FILE* file{0};
    freopen_s(&file, "CONOUT$", "w", __acrt_iob_func(1));
    MODULE_BASE = (uintptr_t) GetModuleHandle("GameAssembly.dll");
    spdlog::info("GameAssembly.dll base {}", MODULE_BASE);


    SetupHooks();

    bool bInitKiero = false;
    do {
        if (kiero::init(kiero::RenderType::D3D11) == kiero::Status::Success) {
            kiero::bind(8, (void**) &imguihook::Present_Original, Present_Hook);
            bInitKiero = true;
        }
    } while (!bInitKiero);


    configs::coins = fn::GetCoins();

    while (!configs::bWindowShouldClose || GetAsyncKeyState(VK_END)) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    fclose(file);
    FreeConsole();

    ShutdownHooks();

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
        SetWindowLongPtr(
            imguihook::window, GWLP_WNDPROC, (LONG_PTR) imguihook::oWndProc); // COMMENT: restore the original WndProc
        return TRUE;
    }

    return TRUE;
}
