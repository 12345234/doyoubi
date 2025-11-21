#pragma once

#include"DXGI.h"
#include"commandque.h"
//#include "../window/window.h"
class swapchain
{
public:
	swapchain() = default;

	~swapchain();

	[[nodiscard]] bool create(const DXGI& dxgi, const window& window, const commandque& commandQueue) noexcept;

	[[nodiscard]] IDXGISwapChain3* get() const noexcept;

	[[nodiscard]] const DXGI_SWAP_CHAIN_DESC1& getDesc() const noexcept;

private:
	IDXGISwapChain3* swapChain{};
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};
};

