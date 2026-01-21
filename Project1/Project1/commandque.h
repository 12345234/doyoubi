#pragma once

#include"device.h"
class commandque
{
public:
	commandque() = default;

	~commandque() =default;

	[[nodiscard]] bool create() noexcept;

	[[nodiscard]] ID3D12CommandQueue* get() const noexcept;

private:
	Microsoft::WRL::ComPtr<ID3D12CommandQueue> commandQueue_{};
};

