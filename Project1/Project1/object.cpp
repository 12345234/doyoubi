#include "object.h"
#include <cmath>
#include"input.h"

void object::initialize(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT4 color) noexcept {
	world_ = DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z);
	color_ = color;
}

void object::update(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 scale,DirectX::XMFLOAT4 a) noexcept
{
    color_ = a;

    DirectX::XMFLOAT3 p = {};
    float sp = 0.1f;
    if (input::instance().getkey('W'))
    {
        p.z += sp;
    }
    if (input::instance().getkey('S'))
    {
        p.z -= sp;
    }
    if (input::instance().getkey('D'))
    {
        p.x += sp;
    }
    if (input::instance().getkey('A'))
    {
        p.x -= sp;
    }
    
    DirectX::XMVECTOR temp = DirectX::XMVectorSet(p.x, p.y, p.z, 0.0f);
    world_.r[3] = DirectX::XMVectorAdd(world_.r[3], temp);
   
}
void object::Bullet(DirectX::XMFLOAT3 pos)
{
    world_ = DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z);
    float bsp = 0.03f;

}
void object::Bulletinstance()
{
    if (input::instance().getkey('B'))
    {
        Bullet({ .1f, 0.0f, -1.0f });
    }
}


[[nodiscard]] DirectX::XMMATRIX object::world() const noexcept {
	return world_;
}

[[nodiscard]] DirectX::XMFLOAT4 object::color() const noexcept {
	return color_;
}
