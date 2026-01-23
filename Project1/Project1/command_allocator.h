#pragma once
#include "device.h"
class commandallocator
{
public:
	commandallocator() = default;

	~commandallocator();

	[[nodiscard]]bool create(const device& device, const D3D12_COMMAND_LIST_TYPE type) noexcept;

	void reset() noexcept;

	[[nodiscard]] ID3D12CommandAllocator* get() const noexcept;

	[[nodiscard]] D3D12_COMMAND_LIST_TYPE getType() const noexcept;

private:
	ID3D12CommandAllocator* commandAllocator{};
	D3D12_COMMAND_LIST_TYPE type_{};
};

