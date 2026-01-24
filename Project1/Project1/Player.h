#pragma once
#include"input.h"
#include"constantbuffer.h"
#include"commandlist.h"
#include"device.h"
#include<DirectXMath.h>
class player
{
public:
	player() = default;

	~player() = default;

	void update();

	void initialize();

	constantbuffer constant;
	DirectX::CXMMATRIX world_ = DirectX::XMMatrixIdentity();
	DirectX::XMFLOAT4 color_ = DirectX::XMFLOAT4(1.0f,1.0f,1.0f,1.0f);
};

