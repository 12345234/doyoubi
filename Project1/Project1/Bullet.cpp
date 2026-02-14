#include "Bullet.h"
namespace {
	struct Vertex
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT4 color;
	};
}
Vertex BulleVertices[] =
{
	{{0.0f,0.0f,0.0f},{1.0f,0.0f,0.0f,0.0f}},
	{{0.0f,0.0f,0.0f},{1.0f,0.0f,0.0f,0.0f}},
	{{0.0f,0.0f,0.0f},{1.0f,0.0f,0.0f,0.0f}}
};

Bullet::~Bullet()
{
	if (vertexBuffer_)
	{
		vertexBuffer_->Release();
		vertexBuffer_ = nullptr;
	}
	if (indexBuffer_)
	{
		indexBuffer_->Release();
		indexBuffer_ = nullptr;
	}
}

bool Bullet::create(device& device)
{
	if (!createVertexBuffer(device))
	{
		return false;
	}
	if (!createIndexBuffer(device))
	{
		return false;
	}
	return true;
}

bool Bullet::createIndexBuffer(const device& device) noexcept {
    uint16_t triangleIndices[] = {
        0, 1, 2
    };

    const auto indexBufferSize = sizeof(triangleIndices);
    D3D12_HEAP_PROPERTIES heapProperty{};
    heapProperty.Type = D3D12_HEAP_TYPE_UPLOAD;
    heapProperty.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
    heapProperty.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
    heapProperty.CreationNodeMask = 1;
    heapProperty.VisibleNodeMask = 1;

    D3D12_RESOURCE_DESC resourceDesc{};
    resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    resourceDesc.Alignment = 0;
    resourceDesc.Width = indexBufferSize;
    resourceDesc.Height = 1;
    resourceDesc.DepthOrArraySize = 1;
    resourceDesc.MipLevels = 1;
    resourceDesc.Format = DXGI_FORMAT_UNKNOWN;
    resourceDesc.SampleDesc.Count = 1;
    resourceDesc.SampleDesc.Quality = 0;
    resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
    resourceDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

    auto res = device.get()->CreateCommittedResource(
        &heapProperty,
        D3D12_HEAP_FLAG_NONE,
        &resourceDesc,
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&indexBuffer_));
    if (FAILED(res)) {
        assert(false && "インデックスバッファの作成に失敗");
        return false;
    }
    uint16_t* data{};
    res = indexBuffer_->Map(0, nullptr, reinterpret_cast<void**>(&data));
    if (FAILED(res)) {
        assert(false && "インデックスバッファのマップに失敗");
        return false;
    }

    memcpy_s(data, indexBufferSize, triangleIndices, indexBufferSize);
    indexBuffer_->Unmap(0, nullptr);

    indexBufferView_.BufferLocation = indexBuffer_->GetGPUVirtualAddress();
    indexBufferView_.SizeInBytes = indexBufferSize;
    indexBufferView_.Format = DXGI_FORMAT_R16_UINT;

    return true;
}
bool Bullet::createVertexBuffer(const device& device) noexcept {


    const auto vertexBufferSize = sizeof(BulleVertices);
    D3D12_HEAP_PROPERTIES heapProperty{};
    heapProperty.Type = D3D12_HEAP_TYPE_UPLOAD;
    heapProperty.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
    heapProperty.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
    heapProperty.CreationNodeMask = 1;
    heapProperty.VisibleNodeMask = 1;

    D3D12_RESOURCE_DESC resourceDesc{};
    resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    resourceDesc.Alignment = 0;
    resourceDesc.Width = vertexBufferSize;
    resourceDesc.Height = 1;
    resourceDesc.DepthOrArraySize = 1;
    resourceDesc.MipLevels = 1;
    resourceDesc.Format = DXGI_FORMAT_UNKNOWN;
    resourceDesc.SampleDesc.Count = 1;
    resourceDesc.SampleDesc.Quality = 0;
    resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
    resourceDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

    auto res = device.get()->CreateCommittedResource(
        &heapProperty,
        D3D12_HEAP_FLAG_NONE,
        &resourceDesc,
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&vertexBuffer_));
    if (FAILED(res)) {
        assert(false && "頂点バッファの作成に失敗");
        return false;
    }
    Vertex* data{};
    res = vertexBuffer_->Map(0, nullptr, reinterpret_cast<void**>(&data));
    if (FAILED(res)) {
        assert(false && "頂点バッファのマップに失敗");
        return false;
    }

    memcpy_s(data, vertexBufferSize, BulleVertices, vertexBufferSize);
    vertexBuffer_->Unmap(0, nullptr);

    vertexBufferView_.BufferLocation = vertexBuffer_->GetGPUVirtualAddress();
    vertexBufferView_.SizeInBytes = vertexBufferSize;
    vertexBufferView_.StrideInBytes = sizeof(Vertex);

    return true;
}
void Bullet::draw(commandlist& cmd)
{
    cmd.get()->IASetVertexBuffers(0,1,&vertexBufferView_);
    cmd.get()->IASetIndexBuffer(&indexBufferView_);
    cmd.get()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    cmd.get()->DrawIndexedInstanced(3,1,0,0,0);

}