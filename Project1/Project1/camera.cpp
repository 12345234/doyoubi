#include "camera.h"
#include <cmath>

#include "input.h"

namespace {
    constexpr float destTargetToView_ = -7.0f; 
} 

void camera::initialize() noexcept {
    position = DirectX::XMFLOAT3(0.0f, 0.0f, destTargetToView_);
    target = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
    up = DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f);
    projection_ = DirectX::XMMatrixPerspectiveFovLH(
        DirectX::XM_PIDIV4,
        1280.0f / 720.0f,    
        0.1f,                
        100.0f            
    );
}

void camera::update() noexcept {

    view = DirectX::XMMatrixLookAtLH(
        DirectX::XMLoadFloat3(&position),
        DirectX::XMLoadFloat3(&target),
        DirectX::XMLoadFloat3(&up));

    
}
[[nodiscard]] DirectX::XMMATRIX XM_CALLCONV camera::viewMatrix() const noexcept {
    return view;
}

[[nodiscard]] DirectX::XMMATRIX XM_CALLCONV camera::projection() const noexcept {
    return projection_;
}