#pragma once

//#include "device.h"
//#include "swap_chain.h"
#include "DescriptorHeap.h"
#include <vector>
class renderTargets final
{
public:
	renderTargets() = default;//コンストラクタ

	~renderTargets();//デストラクタ

	[[nodiscard]] bool createBackBuffer(const Device& device, const SwapChain& swaphain, const DescriptorHeap& heap)noexcept;
};

