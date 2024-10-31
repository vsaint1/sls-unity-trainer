#include "imgui_hook.h"


extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);


namespace imguihook {

   
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
    return CallWindowProc(imguihook::oWndProc, hWnd, uMsg, wParam, lParam);
}

HRESULT __stdcall Present_Hook(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags) {
    if (GetAsyncKeyState(VK_INSERT) & 1) {
        configs::bMenuOpened = !configs::bMenuOpened;
    }

    if (!imguihook::bInitialized) {
        if (SUCCEEDED(pSwapChain->GetDevice(__uuidof(ID3D11Device), (void**) &imguihook::pDevice))) {
            imguihook::pDevice->GetImmediateContext(&imguihook::pContext);
            DXGI_SWAP_CHAIN_DESC sd;
            pSwapChain->GetDesc(&sd);
            imguihook::window = sd.OutputWindow;
            sd.BufferCount    = 2; // COMMENT: Double buffering

            ID3D11Texture2D* pBackBuffer;
            pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*) &pBackBuffer);
            imguihook::pDevice->CreateRenderTargetView(pBackBuffer, NULL, &imguihook::mainRenderTargetView);
            pBackBuffer->Release();

            imguihook::oWndProc = (WNDPROC) SetWindowLongPtr(imguihook::window, GWLP_WNDPROC, (LONG_PTR) WndProc);
            imguihook::Initialize(imguihook::window, imguihook::pDevice, imguihook::pContext);
            imguihook::bInitialized = true;
        } else {
            return imguihook::Present_Original(pSwapChain, SyncInterval, Flags);
        }
    }

    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    menu::Render();
    
    ImGui::Render();
    imguihook::pContext->OMSetRenderTargets(1, &imguihook::mainRenderTargetView, NULL);
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

    return imguihook::Present_Original(pSwapChain, SyncInterval, Flags);
}
