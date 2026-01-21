#pragma once
#include "device.h"
class depthbuffer
{
public:

    depthbuffer() = default;


    ~depthbuffer() = default;


    [[nodiscard]] bool create() noexcept;

    [[nodiscard]] ID3D12Resource* depthBufferr() const noexcept;

    [[nodiscard]] D3D12_CPU_DESCRIPTOR_HANDLE getCpuDescriptorHandle() const noexcept;

private:
    Microsoft::WRL::ComPtr<ID3D12Resource> depthBuffer;  
    D3D12_CPU_DESCRIPTOR_HANDLE            handle;     
};

