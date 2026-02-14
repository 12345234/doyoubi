#include "object.h"
#include <cmath>
#include"input.h"
#include"Bullet.h"
#include"commandlist.h"
void object::initialize(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT4 color) noexcept {
	world_ = DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z);
	color_ = color;
}
void object::initializeBullet(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT4 color) noexcept {
    Bworld_ = DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z);
    Bcolor_ = color;
}
void object::Bullet(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 scale, DirectX::XMFLOAT4 a) noexcept
{
    Bcolor_ = a;

    DirectX::XMFLOAT3 p = {};
    float sp = 0.04f;

    p.z += sp;
    DirectX::XMVECTOR temp = DirectX::XMVectorSet(p.x, p.y, p.z, 0.0f);
    Bworld_.r[3] = DirectX::XMVectorAdd(Bworld_.r[3], temp);
}

void object::update(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 scale,DirectX::XMFLOAT4 a) noexcept
{
    color_ = a;
    
    DirectX::XMFLOAT3 p = {};
    DirectX::XMFLOAT3 Bsize = {1.5f,1.5f,1.5f};

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
    if (input::instance().getkey('B'))
    {
        Bullet(pos,Bsize,a);
    }
    
    DirectX::XMVECTOR temp = DirectX::XMVectorSet(p.x, p.y, p.z, 0.0f);
    world_.r[3] = DirectX::XMVectorAdd(world_.r[3], temp);
   

}

[[nodiscard]] DirectX::XMMATRIX object::world() const noexcept {
	return world_;
}

[[nodiscard]] DirectX::XMFLOAT4 object::color() const noexcept {
	return color_;
}

[[nodiscard]] DirectX::XMMATRIX object::Bworld() const noexcept {
    return Bworld_;
}

[[nodiscard]] DirectX::XMFLOAT4 object::Bcolor() const noexcept {
    return Bcolor_;
}
