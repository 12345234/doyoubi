#pragma once

#include <DirectXMath.h>

class camera final {
public:

    struct ConstBufferData {
        DirectX::XMMATRIX view_{};       
        DirectX::XMMATRIX projection_{};  
    };

public:
    camera() = default;
    ~camera() = default;

public:
    void initialize() noexcept;

    void update() noexcept;

    [[nodiscard]] DirectX::XMMATRIX XM_CALLCONV viewMatrix() const noexcept;
    [[nodiscard]] DirectX::XMMATRIX XM_CALLCONV projection() const noexcept;

private:
    DirectX::XMMATRIX view{};       
    DirectX::XMMATRIX projection_{}; 

    DirectX::XMFLOAT3 position{};  
    DirectX::XMFLOAT3 target{};    
    DirectX::XMFLOAT3 up{};       
};
