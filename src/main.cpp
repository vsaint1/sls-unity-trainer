#include <MinHook.h>
#include <filesystem>
#include <thread>

#include <Windows.h>

#define UNITY_CALLING_CONVENTION __fastcall*

uintptr_t MODULE_BASE = (uintptr_t) GetModuleHandleA("GameAssembly.dll");


static unsigned long FakeEntry(HMODULE _hModule) {
    AllocConsole();
    FILE* file{0};
    freopen_s(&file, "CONOUT$", "w", __acrt_iob_func(1));

    printf("GameAssembly.dll base %p\n", MODULE_BASE);
    while (!GetAsyncKeyState(VK_END)) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    fclose(file);
    FreeConsole();

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
        return TRUE;
    }

    return TRUE;
}
