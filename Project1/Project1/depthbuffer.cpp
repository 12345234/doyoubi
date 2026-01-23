#include "depthbuffer.h"
#include <cassert>

depthbuffer::~depthbuffer() {
    if (depthBuffer) {
        depthBuffer->Release();
        depthBuffer = nullptr;
    }
}
[[nodiscard]] bool depthbuffer::create(const device& device, const DescriptorHeap& heap, const window& window) noexcept {
    const auto [w, h] = window.size();

    D3D12_HEAP_PROPERTIES heapProps{};
    heapProps.Type = D3D12_HEAP_TYPE_DEFAULT;
    D3D12_RESOURCE_DESC depthDesc{};
    depthDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
    depthDesc.Width = w;
    depthDesc.Height = h;
    depthDesc.DepthOrArraySize = 1;
    depthDesc.MipLevels = 1;
    depthDesc.Format = DXGI_FORMAT_D32_FLOAT;
    depthDesc.SampleDesc.Count = 1;
    depthDesc.SampleDesc.Quality = 0;
    depthDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
    depthDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

    D3D12_CLEAR_VALUE clearValue{};
    clearValue.Format = depthDesc.Format;
    clearValue.DepthStencil.Depth = 1.0f;
    clearValue.DepthStencil.Stencil = 0;

    const auto res = device.get()->CreateCommittedResource(
        &heapProps,
        D3D12_HEAP_FLAG_NONE,
        &depthDesc,
        D3D12_RESOURCE_STATE_DEPTH_WRITE,
        &clearValue,
        IID_PPV_ARGS(&depthBuffer));

    if (FAILED(res)) {
        assert(false && "デプスバッファの作成に失敗しました");
        return false;
    }

    auto heapType = heap.gettype();
    if (heapType != D3D12_DESCRIPTOR_HEAP_TYPE_DSV) {
        assert(false && "ディスクリプタヒープのタイプが DSV ではありません");
        false;
    }
    D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc{};
    dsvDesc.Format = depthDesc.Format;
    dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
    dsvDesc.Flags = D3D12_DSV_FLAG_NONE;

    handle = heap.get()->GetCPUDescriptorHandleForHeapStart();
    device.get()->CreateDepthStencilView(depthBuffer, &dsvDesc, handle);

    return true;
}

[[nodiscard]] ID3D12Resource* depthbuffer::depthBufferr() const noexcept {
    assert(depthBuffer && "デプスバッファが未生成です");
    return depthBuffer;
}

[[nodiscard]] D3D12_CPU_DESCRIPTOR_HANDLE depthbuffer::getCpuDescriptorHandle() const noexcept {
    assert(depthBuffer && "デプスバッファが未生成です");
    return handle;
}
