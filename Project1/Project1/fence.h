#pragma once
#include "device.h"

class fence final {
public:
 
    fence() = default;

    ~fence();
    [[nodiscard]] bool create(const device& device) noexcept;

    void wait(UINT64 fenceValue) const noexcept;

    [[nodiscard]] ID3D12Fence* get() const noexcept;


private:
    ID3D12Fence* fence_{};         
    HANDLE       waitgpuevent{}; 
};
