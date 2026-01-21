#include "constantbuffer.h"
#include <cassert>

namespace {
	constexpr auto type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
}
constantbuffer::~constantbuffer()
{
	DescriptorHeapa::instance().releaseDescriptor(type, descriptorIndex);
}

bool constantbuffer::create(UINT buffersize)
{
	const auto size = (sizeof(buffersize)+255)& ~255;

	D3D12_HEAP_PROPERTIES heapProps;
	heapProps.Type = D3D12_HEAP_TYPE_UPLOAD;
	D3D12_RESOURCE_DESC resourcedesc;
	resourcedesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resourcedesc.Width = size;
	resourcedesc.Height = 1;
	resourcedesc.DepthOrArraySize = 1;
	resourcedesc.MipLevels = 1;
	resourcedesc.Format = DXGI_FORMAT_UNKNOWN;
	resourcedesc.SampleDesc.Count = 1;
	resourcedesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	const auto res = device::instance().get()->CreateCommittedResource(
		&heapProps,
		D3D12_HEAP_FLAG_NONE,
		&resourcedesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constantBuffer1));
	if (FAILED(res)) {
		assert(false && "コンスタントバッファの作成に失敗しました");
		return false;
	}

	const auto descriptorIndexa = DescriptorHeapa::instance().allocateDescriptor(type);
	if (!descriptorIndexa.has_value()) {
		assert(false && "コンスタントバッファのディスクリプタ確保に失敗しました");
		return false;
	}

	descriptorIndex = descriptorIndexa.value();

	auto heap = DescriptorHeapa::instance().get(type);

	D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc{};
	cbvDesc.BufferLocation = constantBuffer1->GetGPUVirtualAddress();
	cbvDesc.SizeInBytes = size;

	UINT cbvDescriptorSize = device::instance().get()->GetDescriptorHandleIncrementSize(type);

	D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle = heap->GetCPUDescriptorHandleForHeapStart();

	cpuHandle.ptr += descriptorIndex * cbvDescriptorSize;

	device::instance().get()->CreateConstantBufferView(&cbvDesc, cpuHandle);

	gpuHandle = heap->GetGPUDescriptorHandleForHeapStart();
	
	gpuHandle.ptr += descriptorIndex * cbvDescriptorSize;

	return true;
}
ID3D12Resource* constantbuffer::constantbuffer1() const noexcept {
	assert(constantBuffer1 && "コンスタントバッファが未作成です");
	return constantBuffer1.Get();
}
[[nodiscard]] D3D12_GPU_DESCRIPTOR_HANDLE constantbuffer::getGpuDescriptorHandle() const noexcept {
	assert(constantBuffer1 && "コンスタントバッファが未作成です");
	return gpuHandle;
}
