#pragma once
#include "device.h"
#include "DescriptorHeap.h"
class constantbuffer
{
public:
	constantbuffer() = default;
	~constantbuffer();

	[[nodiscard]] bool create(UINT buffersize)noexcept;
	[[nodiscard]] ID3D12Resource* constantbuffer1() const noexcept;

	[[nodiscard]] D3D12_GPU_DESCRIPTOR_HANDLE getGpuDescriptorHandle() const noexcept;
private:
	Microsoft::WRL::ComPtr<ID3D12Resource> constantBuffer1{};
	UINT                                   descriptorIndex{};
	D3D12_GPU_DESCRIPTOR_HANDLE            gpuHandle{};
};

