#pragma once
#include "../kiero/kiero.h"
#include "gui/menu.h"
#include <d3d11.h>
#include <dxgi.h>

#include <Windows.h>

typedef HRESULT(__stdcall* Present)(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
typedef LRESULT(CALLBACK* WNDPROC)(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
HRESULT __stdcall Present_Hook(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);

namespace imguihook {


    inline Present Present_Original                     = nullptr;
    inline WNDPROC oWndProc                             = nullptr;
    inline ID3D11Device* pDevice                        = nullptr;
    inline ID3D11DeviceContext* pContext                = nullptr;
    inline ID3D11RenderTargetView* mainRenderTargetView = nullptr;
    inline HWND window                                  = nullptr;

    inline bool bInitialized = false;
    
    void Initialize(HWND hwnd, ID3D11Device* device, ID3D11DeviceContext* context);
    void Cleanup();
} // namespace imguihook
