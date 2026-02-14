#pragma once

#include "device.h"
#include "commandlist.h"
#include<DirectXMath.h>
#include "input.h"
class Bullet
{
public:
	static Bullet& instance() noexcept {
		static Bullet instance;
		return instance;
	}

	struct BulletData
	{
		DirectX::XMMATRIX Bworld_{};
		DirectX::XMMATRIX Bcolor_{};
	};
	DirectX::XMFLOAT3 pos;

	Bullet() = default;
	~Bullet();
	bool create(device& device);

	void draw(commandlist& cmd);
private:
	[[nodiscard]] bool createVertexBuffer(const device& device) noexcept;

	[[nodiscard]] bool createIndexBuffer(const device& device) noexcept;

	DirectX::XMMATRIX Bworld_ = DirectX::XMMatrixIdentity();
	ID3D12Resource* vertexBuffer_{};
	ID3D12Resource* indexBuffer_{};

	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_ = {};
	D3D12_INDEX_BUFFER_VIEW  indexBufferView_ = {};
};

