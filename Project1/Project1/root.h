#pragma once

#include "device.h"
class root final {
public:

    root() = default;


    ~root() = default;

    [[nodiscard]] bool create() noexcept;

    [[nodiscard]] ID3D12RootSignature* get() const noexcept;

private:
    Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature;
};
