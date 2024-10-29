#include "imgui_hook.h"


bool bMenuOpened = false;

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace ImGuiHook {

    Present Present_Original                             = nullptr;
    WNDPROC oWndProc                             = nullptr;
    ID3D11Device* pDevice                        = nullptr;
    ID3D11DeviceContext* pContext                = nullptr;
    ID3D11RenderTargetView* mainRenderTargetView = nullptr;
    HWND window                                  = nullptr;

    bool bInitialized = false;
    void Initialize(HWND hwnd, ID3D11Device* device, ID3D11DeviceContext* context) {
        window   = hwnd;
        pDevice  = device;
        pContext = context;

        ImGui::CreateContext();
        ImGuiIO& io    = ImGui::GetIO();
        io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;

        ImGui_ImplWin32_Init(window);
        ImGui_ImplDX11_Init(pDevice, pContext);
    }

    void Cleanup() {
        ImGui_ImplDX11_Shutdown();
        ImGui_ImplWin32_Shutdown();
        ImGui::DestroyContext();
    }
} // namespace ImGuiHook

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam)) {
        return true;
    }
    return CallWindowProc(ImGuiHook::oWndProc, hWnd, uMsg, wParam, lParam);
}

HRESULT __stdcall Present_Hook(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags) {
    if (GetAsyncKeyState(VK_INSERT) & 1) {
        bMenuOpened = !bMenuOpened;
    }

    if (!ImGuiHook::bInitialized) {
        if (SUCCEEDED(pSwapChain->GetDevice(__uuidof(ID3D11Device), (void**) &ImGuiHook::pDevice))) {
            ImGuiHook::pDevice->GetImmediateContext(&ImGuiHook::pContext);
            DXGI_SWAP_CHAIN_DESC sd;
            pSwapChain->GetDesc(&sd);
            ImGuiHook::window = sd.OutputWindow;

            ID3D11Texture2D* pBackBuffer;
            pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*) &pBackBuffer);
            ImGuiHook::pDevice->CreateRenderTargetView(pBackBuffer, NULL, &ImGuiHook::mainRenderTargetView);
            pBackBuffer->Release();

            ImGuiHook::oWndProc = (WNDPROC) SetWindowLongPtr(ImGuiHook::window, GWLP_WNDPROC, (LONG_PTR) WndProc);
            ImGuiHook::Initialize(ImGuiHook::window, ImGuiHook::pDevice, ImGuiHook::pContext);
            ImGuiHook::bInitialized = true;
        } else {
            return ImGuiHook::Present_Original(pSwapChain, SyncInterval, Flags);
        }
    }

    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    if (bMenuOpened) {
        ImGui::Begin("ImGui Kiero");
        ImGui::Text("Hello world");
        ImGui::End();
    }

    ImGui::Render();
    ImGuiHook::pContext->OMSetRenderTargets(1, &ImGuiHook::mainRenderTargetView, NULL);
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

    return ImGuiHook::Present_Original(pSwapChain, SyncInterval, Flags);
}
