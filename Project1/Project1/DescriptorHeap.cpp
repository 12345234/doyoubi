#include "DescriptorHeap.h"
#include <cassert>

DescriptorHeap::~DescriptorHeap() {
    if (heap) {
        heap->Release();
        heap = nullptr;
    }
}
[[nodiscard]] bool DescriptorHeap::create(const device& device, D3D12_DESCRIPTOR_HEAP_TYPE heaptype, UINT numDescriptors, bool shaderVisible) noexcept {
    D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
    heapDesc.Type = heaptype;
    heapDesc.NumDescriptors = numDescriptors;
    heapDesc.Flags = shaderVisible ? D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE : D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

    type = heaptype; 

    HRESULT hr = device.get()->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&heap));
    if (FAILED(hr)) {
        assert(false && "ディスクリプタヒープの生成に失敗しました");
        return false;
    }

    return true;
}

[[nodiscard]] ID3D12DescriptorHeap* DescriptorHeap::get() const{
    if (!heap) {
        assert(false && "ディスクリプタヒープが未生成です");
    }

    return heap;
}

[[nodiscard]] D3D12_DESCRIPTOR_HEAP_TYPE DescriptorHeap::gettype() const {
    if (!heap) {
        assert(false && "ディスクリプタヒープが未生成です");
    }
    return type;
}
