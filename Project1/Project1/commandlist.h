#pragma once

#include "device.h"
#include "command_allocator.h"
class commandlist
{
public:
	commandlist() = default;

	~commandlist();

	[[nodiscard]] bool create(const device& device, const commandallocator& commandAllocator) noexcept;

	void reset(const commandallocator& commandAllocator) noexcept;

	[[nodiscard]] ID3D12GraphicsCommandList* get() const noexcept;

private:
	ID3D12GraphicsCommandList* commandList{};
};

