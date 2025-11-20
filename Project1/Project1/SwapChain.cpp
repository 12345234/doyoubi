#include<cassert>
#include<dxgi1_6.h>
#include<d3d12.h>
#include "Dire.h"
#include "swapchain.h"

#pragma comment(lib,"dxgi.lib")

#pragma comment(lib,"d3d12.lib")


IDXGISwapChain3* Dx12::CreateSwapChain(IDXGIFactory4* factory, ID3D12CommandQueue* commandQueue, HWND hwnd) {
    // スワップチェーンの詳細設定
    DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
    swapChainDesc.BufferCount = 2;                                // ダブルバッファリング
    swapChainDesc.Width = 1280;                             // 画面幅
    swapChainDesc.Height = 720;                              // 画面高
    swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;       // ピクセルフォーマット
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;  // レンダーターゲット用
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;    // 高速切替
    swapChainDesc.SampleDesc.Count = 1;                                // マルチサンプリングなし

    IDXGISwapChain1* swapChain1;
    HRESULT          hr = factory->CreateSwapChainForHwnd(
        commandQueue,    // コマンドキュー
        hwnd,            // ターゲットウィンドウ
        &swapChainDesc,  // 設定
        nullptr,         // フルスクリーン設定
        nullptr,         // 出力制限
        &swapChain1      // 作成されるスワップチェーン
    );

    if (FAILED(hr)) {
        OutputDebugString("Failed to create Swap Chain\n");
        return nullptr;
    }

    // より高機能なインターフェースにキャスト
    IDXGISwapChain3* swapChain;
    hr = swapChain1->QueryInterface(IID_PPV_ARGS(&swapChain));
    swapChain1->Release();

    if (FAILED(hr)) {
        OutputDebugString("Failed to cast to SwapChain3\n");
        return nullptr;
    }

    return swapChain;
}