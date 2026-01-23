#pragma once

#include"device.h"
class commandque
{
public:
	commandque() = default;

	~commandque();

	[[nodiscard]] bool create(const device& device) noexcept;

	[[nodiscard]] ID3D12CommandQueue* get() const noexcept;

private:
	ID3D12CommandQueue* commandQueue{};
};

