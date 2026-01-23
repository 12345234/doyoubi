#include "object.h"
#include <cmath>

void object::initialize(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT4 color) noexcept {
	world_ = DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z);
	color_ = color;
}

void object::update() noexcept {
}

[[nodiscard]] DirectX::XMMATRIX object::world() const noexcept {
	return world_;
}

[[nodiscard]] DirectX::XMFLOAT4 object::color() const noexcept {
	return color_;
}
