#pragma once

#include "device.h"

class root final {
public:
    root() = default;

    ~root();

    [[nodiscard]] bool create(const device& device) noexcept;

    [[nodiscard]] ID3D12RootSignature* get() const noexcept;

private:
    ID3D12RootSignature* rootsigu{};
};
