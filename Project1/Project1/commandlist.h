#pragma once

#include "device.h"
#include "command_allocator.h"
class commandlist
{
	commandlist() = default;

	~commandlist();

	[[nodiscard]] bool create(const device& device, const command_allocator& commandAllocator) noexcept;

	void reset(const command_allocator& commandAllocator) noexcept;

	[[nodiscard]] ID3D12GraphicsCommandList* get() const noexcept;

private:
	ID3D12GraphicsCommandList* commandList{};
};

