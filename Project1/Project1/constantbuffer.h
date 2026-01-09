#pragma once
#include "device.h"
#include "DescriptorHeap.h"
class constantbuffer
{
public:
	constantbuffer() = default;
	~constantbuffer();

	[[nodiscard]] bool create(const device& device, const DescriptorHeap& heap, UINT buffersize, UINT descripterIndex)noexcept;
	[[nodiscard]] ID3D12Resource* constantbuffer1() const noexcept;

	[[nodiscard]] D3D12_GPU_DESCRIPTOR_HANDLE getGpuDescriptorHandle() const noexcept;
private:
	ID3D12Resource* onstantbuffer1{};
	D3D12_GPU_DESCRIPTOR_HANDLE  gpuHandle{};
};

