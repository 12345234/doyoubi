#include "constantbuffer.h"
#include <cassert>

constantbuffer::~constantbuffer() {
    if (constantBuffer1) {
        constantBuffer1->Release();
        constantBuffer1 = nullptr;
    }
}

bool constantbuffer::create(const device& device, const DescriptorHeap& heap, UINT bufferSize, UINT descriptorIndex) 
{
    const auto size = (sizeof(bufferSize) + 255) & ~255;

    D3D12_HEAP_PROPERTIES heapProps{};
    heapProps.Type = D3D12_HEAP_TYPE_UPLOAD;
    D3D12_RESOURCE_DESC resourceDesc{};
    resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    resourceDesc.Width = size;
    resourceDesc.Height = 1;
    resourceDesc.DepthOrArraySize = 1;
    resourceDesc.MipLevels = 1;
    resourceDesc.Format = DXGI_FORMAT_UNKNOWN;
    resourceDesc.SampleDesc.Count = 1;
    resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

    const auto res = device.get()->CreateCommittedResource(
        &heapProps,
        D3D12_HEAP_FLAG_NONE,
        &resourceDesc,
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&constantBuffer1));
    if (FAILED(res)) {
        assert(false && "コンスタントバッファの作成に失敗しました");
        return false;
    }

    auto heapType = heap.gettype();
    if (heapType != D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV) {
        assert(false && "ディスクリプタヒープのタイプが CBV_SRV_UAV ではありません");
        false;
    }

    D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc{};
    cbvDesc.BufferLocation = constantBuffer1->GetGPUVirtualAddress();
    cbvDesc.SizeInBytes = size;

    UINT cbvDescriptorSize = device.get()->GetDescriptorHandleIncrementSize(heap.gettype());

    D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle = heap.get()->GetCPUDescriptorHandleForHeapStart();
    cpuHandle.ptr += descriptorIndex * cbvDescriptorSize;

    device.get()->CreateConstantBufferView(&cbvDesc, cpuHandle);

    gpuHandle = heap.get()->GetGPUDescriptorHandleForHeapStart();

    gpuHandle.ptr += descriptorIndex * cbvDescriptorSize;

    return true;
}

[[nodiscard]] ID3D12Resource* constantbuffer::constantbuffer1() const noexcept {
    assert(constantBuffer1 && "コンスタントバッファが未作成です");
    return constantBuffer1;
}

[[nodiscard]] D3D12_GPU_DESCRIPTOR_HANDLE constantbuffer::getGpuDescriptorHandle() const noexcept {
    assert(constantBuffer1 && "コンスタントバッファが未作成です");
    return gpuHandle;
}