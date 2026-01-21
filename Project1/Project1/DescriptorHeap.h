#pragma once
#include "device.h"
#include <cassert>
#include <unordered_map>
#include <optional>
#include <memory>
using namespace std;

class DescriptorHeap;

class DescriptorHeapa 
{
public:
	static DescriptorHeapa& instance()
	{
		static DescriptorHeapa instance;
		return instance;
	}

	[[nodiscard]] bool create(D3D12_DESCRIPTOR_HEAP_TYPE type, UINT numDescriptors, bool shaderVisible = false) noexcept;

	void applyPending();
	[[nodiscard]] ID3D12DescriptorHeap* get(D3D12_DESCRIPTOR_HEAP_TYPE type) const noexcept;

	optional<UINT>allocateDescriptor(D3D12_DESCRIPTOR_HEAP_TYPE t);

	void releaseDescriptor(D3D12_DESCRIPTOR_HEAP_TYPE typea, UINT descriptorIndex);
private:
	DescriptorHeapa() = default;//コンストラクタ

	~DescriptorHeapa();//デストラクタ

	DescriptorHeapa(const DescriptorHeapa& r) = delete;
	DescriptorHeapa& operator=(const DescriptorHeapa& r) = delete;
	DescriptorHeapa(DescriptorHeapa&& r) = delete;
	DescriptorHeapa& operator=(DescriptorHeapa&& r) = delete;
private:
	unordered_map<UINT, std::unique_ptr<DescriptorHeap>> map{};
};

