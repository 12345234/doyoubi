#include "player.h"
#include"input.h"
#include"quadpolygon.h"


void player::initialize()
{
}

void player::update()
{
	float sp = 0.05f;

	DirectX::XMFLOAT3 pos{};
	if (input::instance().getkey('W'))
	{
		pos.z += sp;
	}
	if (input::instance().getkey('S'))
	{
		pos.z -= sp;
	}
	if (input::instance().getkey('D'))
	{
		pos.x += sp;
	}
	if (input::instance().getkey('W'))
	{
		pos.x -= sp;
	}

	DirectX::XMVECTOR temp = DirectX::XMVectorSet(pos.x,pos.y,pos.z,0.0f);
}
