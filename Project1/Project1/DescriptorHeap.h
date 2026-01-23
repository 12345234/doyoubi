#pragma once

#include "device.h"



class DescriptorHeap {
public:
     DescriptorHeap() = default;

    ~DescriptorHeap();

    [[nodiscard]] bool create(const device& device, D3D12_DESCRIPTOR_HEAP_TYPE heaptype, UINT numDescriptors, bool shaderVisible = false) noexcept;


    [[nodiscard]] ID3D12DescriptorHeap* get() const;

    [[nodiscard]] D3D12_DESCRIPTOR_HEAP_TYPE gettype() const;



private:
    ID3D12DescriptorHeap* heap{};
    D3D12_DESCRIPTOR_HEAP_TYPE type{};
};
