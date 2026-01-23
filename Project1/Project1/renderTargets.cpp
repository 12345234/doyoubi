#include "renderTargets.h"
#include <cassert>

render::~render() {
    for (auto& rt : renderTargets_) {
        if (rt) {
            rt->Release();
            rt = nullptr;
        }
    }
    renderTargets_.clear();
}

[[nodiscard]] bool render::createBackBuffer(const device& device, const swapchain& swapChain, const DescriptorHeap& heap) noexcept {
    const auto& desc = swapChain.getDesc();

    renderTargets_.resize(desc.BufferCount);

    auto handle = heap.get()->GetCPUDescriptorHandleForHeapStart();
    auto heapType = heap.gettype();
    assert(heapType == D3D12_DESCRIPTOR_HEAP_TYPE_RTV && "ディスクリプタヒープのタイプが RTV ではありません");

    for (uint8_t i = 0; i < desc.BufferCount; ++i) {
        const auto hr = swapChain.get()->GetBuffer(i, IID_PPV_ARGS(&renderTargets_[i]));
        if (FAILED(hr)) {
            assert(false && "バックバッファの取得に失敗しました");
            return false;
        }

        device.get()->CreateRenderTargetView(renderTargets_[i], nullptr, handle);

        handle.ptr += device.get()->GetDescriptorHandleIncrementSize(heapType);
    }

    return true;
}
[[nodiscard]] D3D12_CPU_DESCRIPTOR_HANDLE render::getCpuDescriptorHandle(const device& device, const DescriptorHeap& heap, UINT index) const noexcept {

    if (index >= renderTargets_.size() || !renderTargets_[index]) {
        assert(false && "不正なレンダーターゲットです");
    }

    auto handle = heap.get()->GetCPUDescriptorHandleForHeapStart();

    auto heapType = heap.gettype();
    assert(heapType == D3D12_DESCRIPTOR_HEAP_TYPE_RTV && "ディスクリプタヒープのタイプが RTV ではありません");

    handle.ptr += index * device.get()->GetDescriptorHandleIncrementSize(heapType);
    return handle;
}

[[nodiscard]] ID3D12Resource* render::get(UINT index) const noexcept {
    if (index >= renderTargets_.size() || !renderTargets_[index]) {
        assert(false && "不正なレンダーターゲットです");
        return nullptr;
    }
    return renderTargets_[index];
}
