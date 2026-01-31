#pragma once

#include <basetsd.h>
#include <DirectXMath.h>
#include"quadpolygon.h"

class object final {
public:
    struct ConstBufferData {
        DirectX::XMMATRIX world_{};
        DirectX::XMFLOAT4 color_{};  
    };

public:
    object() = default;

    ~object() = default;

    void initialize(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT4 color) noexcept;

    void update(DirectX::XMFLOAT3 pos,DirectX::XMFLOAT3 scale,DirectX::XMFLOAT4 a) noexcept;

    void Bullet(DirectX::XMFLOAT3 pos);
    void Bulletinstance();


    [[nodiscard]] DirectX::XMMATRIX world() const noexcept;
    [[nodiscard]] DirectX::XMFLOAT4 color() const noexcept;

private:
    DirectX::XMMATRIX world_ = DirectX::XMMatrixIdentity();               
    DirectX::XMFLOAT4 color_ = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f); 

    float move{};  
};
