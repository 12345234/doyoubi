
#pragma once

#include "device.h"
#include "swapchain.h"
#include "descriptorheap.h"
#include <vector>

class render final {
public:
    render() = default;

    ~render();


    //---------------------------------------------------------------------------------
    /**
     * @brief	バックバッファを生成する
     * @param	device		デバイスクラスのインスタンス
     * @param	swapChain	スワップチェインのポインタ
     * @param	heap		ディスクリプターヒープのインスタンス
     * @return	生成の成否
     */
    [[nodiscard]] bool createBackBuffer(const device& device, const swapchain& swapChain, const DescriptorHeap& heap) noexcept;

    [[nodiscard]] D3D12_CPU_DESCRIPTOR_HANDLE getDescriptorHandle(const device& device, const DescriptorHeap& heap, UINT index) const noexcept;

    [[nodiscard]] ID3D12Resource* get(uint32_t index) const noexcept;


private:
    std::vector<ID3D12Resource*> renderTargets_;  /// レンダーターゲットリソースの配列
};
