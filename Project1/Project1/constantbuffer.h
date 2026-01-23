#pragma once
#include "device.h"
#include "DescriptorHeap.h"
class constantbuffer
{
public:
	constantbuffer() = default;
	~constantbuffer();

	bool create(const device& device,const DescriptorHeap& heap, UINT buffersize, UINT descriptorIndex);
	[[nodiscard]] ID3D12Resource* constantbuffer1() const noexcept;

	[[nodiscard]] D3D12_GPU_DESCRIPTOR_HANDLE getGpuDescriptorHandle() const noexcept;
private:
	ID3D12Resource*                      constantBuffer1{};
	D3D12_GPU_DESCRIPTOR_HANDLE          gpuHandle{};
};

