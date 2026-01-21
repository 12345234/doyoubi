#include "renderTargets.h"
#include <cassert>

namespace {
    auto heaptype = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
}
renderTargets::~renderTargets() {
    
    renderTargets_.clear();
}

[[nodiscard]] bool renderTargets::createBackBuffer(const swapchain& swapChain) noexcept {
    const auto& desc = swapChain.getDesc();

    renderTargets_.resize(desc.BufferCount);

    auto heap = DescriptorHeapa::instance().get(heaptype);

    auto handle = heap->GetCPUDescriptorHandleForHeapStart();


    for (uint8_t i = 0; i < desc.BufferCount; ++i) {
        const auto hr = swapChain.get()->GetBuffer(i, IID_PPV_ARGS(&renderTargets_[i]));
        if (FAILED(hr)) {
            assert(false && "バックバッファの取得に失敗しました");
            return false;
        }

        device::instance().get()->CreateRenderTargetView(renderTargets_[i].Get(), nullptr, handle);

        handle.ptr += device::instance().get()->GetDescriptorHandleIncrementSize(heaptype);
    }

    return true;
}

[[nodiscard]] D3D12_CPU_DESCRIPTOR_HANDLE renderTargets::getDescriptorHandle(UINT index) const noexcept {

    if (index >= renderTargets_.size() || !renderTargets_[index]) {
        assert(false && "不正なレンダーターゲットです");
    }

    auto heap = DescriptorHeapa::instance().get(heaptype);


    auto handle = heap->GetCPUDescriptorHandleForHeapStart();

    handle.ptr += index * device::instance().get()->GetDescriptorHandleIncrementSize(heaptype);
    return handle;
}
[[nodiscard]] ID3D12Resource* renderTargets::get(uint32_t index) const noexcept {
    if (index >= renderTargets_.size() || !renderTargets_[index]) {
        assert(false && "不正なレンダーターゲットです");
        return nullptr;
    }
    return renderTargets_[index].Get();
}
