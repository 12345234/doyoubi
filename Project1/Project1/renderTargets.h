#pragma once

#include "device.h"
#include "swapchain.h"
#include "DescriptorHeap.h"
#include <vector>
class renderTargets final
{
public:
	renderTargets() = default;

	~renderTargets();

	[[nodiscard]] bool createBackBuffer(const swapchain& swaphain)noexcept;

	[[nodiscard]] D3D12_CPU_DESCRIPTOR_HANDLE getDescriptorHandle(UINT index) const noexcept;

	[[nodiscard]] ID3D12Resource* get(uint32_t index) const noexcept;

private:
	vector<Microsoft::WRL::ComPtr<ID3D12Resource>> renderTargets_;
};

