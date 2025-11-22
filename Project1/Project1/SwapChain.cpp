// スワップチェイン制御クラス

#include "swapchain.h"
#include"window.h"

#include <cassert>
swapchain::~swapchain() {
    if (swapChain) {
        swapChain->Release();
        swapChain = nullptr;
    }
}

[[nodiscard]] bool swapchain::create(const DXGI& dxgi, const window& window, const commandque& commandQueue) noexcept {
    // ウィンドウサイズを取得
    const auto [w, h] = window.size();

    swapChainDesc = {};
    swapChainDesc.BufferCount = 2;                                // バックバッファの数（ダブルバッファ）
    swapChainDesc.Width = w;                                // バックバッファの横幅
    swapChainDesc.Height = h;                                // バックバッファの縦幅
    swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;       // バックバッファのフォーマット
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;  // レンダーターゲットとして使用
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;    // 毎フレーム画面更新するので描画が終わったらバッファを破棄
    swapChainDesc.SampleDesc.Count = 1;                                // マルチサンプリングなし

    IDXGISwapChain1* tempSwapChain{};
    {
        const auto hr = dxgi.factory()->CreateSwapChainForHwnd(commandQueue.get(), window.handle(),
            &swapChainDesc, nullptr, nullptr, &tempSwapChain);
        if (FAILED(hr)) {
            assert(false && "スワップチェインの作成に失敗");
            return false;
        }
    }

    {
        // 一時的なスワップチェインを IDXGISwapChain3 に変換
        const auto hr = tempSwapChain->QueryInterface(IID_PPV_ARGS(&swapChain));
        // 一時的なスワップチェインは解放
        tempSwapChain->Release();

        if (FAILED(hr)) {
            assert(false && "スワップチェインのアップグレードに失敗");
            return false;
        }
    }

    return true;
}

[[nodiscard]] IDXGISwapChain3* swapchain::get() const noexcept {
    if (!swapChain) {
        assert(false && "スワップチェインが未作成です");
        return nullptr;
    }
    return swapChain;
}

[[nodiscard]] const DXGI_SWAP_CHAIN_DESC1& swapchain::getDesc() const noexcept {
    if (!swapChain) {
        assert(false && "スワップチェインが未作成です");
    }
    return swapChainDesc;
}
