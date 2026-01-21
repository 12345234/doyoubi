#pragma once
#include"commandque.h"
class swapchain
{
public:
	swapchain() = default;

	~swapchain() = default;

	[[nodiscard]] bool create(const commandque& commandQueue) noexcept;

	[[nodiscard]] IDXGISwapChain3* get() const noexcept;

	[[nodiscard]] const DXGI_SWAP_CHAIN_DESC1& getDesc() const noexcept;

private:
	Microsoft::WRL::ComPtr<IDXGISwapChain3> swapChain{};
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};
};

