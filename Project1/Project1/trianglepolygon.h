#pragma once

#include"device.h"
#include"commandlist.h"
class trianglepolygon
{
public:
	trianglepolygon() = default;

	~trianglepolygon();

	[[nodiscard]] bool create(const device& device)noexcept;

	[[nodiscard]] void draw(const commandlist& commandlist)noexcept;

private:
	[[nodiscard]] bool createvertexbuffer(const device& device)noexcept;

	[[nodiscard]] bool createindexbuffer(const device& device)noexcept;


	ID3D12Resource* vertexBuffer{};
	ID3D12Resource* indexBuffer{};

	D3D12_VERTEX_BUFFER_VIEW vertexbufferview = {};
	D3D12_INDEX_BUFFER_VIEW indexbufferview = {};
};

