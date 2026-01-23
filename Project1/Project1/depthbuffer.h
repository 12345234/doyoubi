#pragma once
#include "device.h"
#include"DescriptorHeap.h"
#include"window.h"
class depthbuffer
{
public:

    depthbuffer() = default;


    ~depthbuffer();


    [[nodiscard]] bool create(const device& device, const DescriptorHeap& heap, const window& window) noexcept;

    [[nodiscard]] ID3D12Resource* depthBufferr() const noexcept;

    [[nodiscard]] D3D12_CPU_DESCRIPTOR_HANDLE getCpuDescriptorHandle() const noexcept;

private:
    ID3D12Resource* depthBuffer;  
    D3D12_CPU_DESCRIPTOR_HANDLE            handle;     
};

