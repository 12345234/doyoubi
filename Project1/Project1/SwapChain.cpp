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
    const auto [w, h] = window.size();

    swapChainDesc = {};
    swapChainDesc.BufferCount = 2;                               
    swapChainDesc.Width = w;                               
    swapChainDesc.Height = h;                                
    swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;     
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;  
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;    
    swapChainDesc.SampleDesc.Count = 1;                                
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
        const auto hr = tempSwapChain->QueryInterface(IID_PPV_ARGS(&swapChain));
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
