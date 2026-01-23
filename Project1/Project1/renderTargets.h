#pragma once

#include "device.h"
#include "swapchain.h"
#include "DescriptorHeap.h"
#include <vector>

class render final {
public:
    render() = default;

    ~render();


    [[nodiscard]] bool createBackBuffer(const device& device, const swapchain& swapChain, const DescriptorHeap& heap) noexcept;

    [[nodiscard]] D3D12_CPU_DESCRIPTOR_HANDLE getCpuDescriptorHandle(const device& device, const DescriptorHeap& heap, UINT index) const noexcept;


    [[nodiscard]] ID3D12Resource* get(UINT index) const noexcept;


private:
    std::vector<ID3D12Resource*> renderTargets_;

};

