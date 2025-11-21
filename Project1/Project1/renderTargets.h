#pragma once

#include "device.h"
#include "swapchain.h"
#include "DescriptorHeap.h"
#include <vector>
class renderTargets final
{
public:
	renderTargets() = default;//コンストラクタ

	~renderTargets();//デストラクタ

	[[nodiscard]] bool createBackBuffer(const device& device, const swapchain& swaphain, const DescriptorHeap& heap)noexcept;

	[[nodiscard]] D3D12_CPU_DESCRIPTOR_HANDLE getDescriptorHandle(const device& device, const DescriptorHeap& heap, UINT index) const noexcept;

	[[nodiscard]] ID3D12Resource* get(uint32_t index) const noexcept;

private:
	std::vector<ID3D12Resource*> renderTargets_;
};

