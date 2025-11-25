#include "trianglepolygon.h"
#include<cassert>
#include<DirectXMath.h>

namespace {
	struct Vertex
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT4 color;
	};
}

trianglepolygon::~trianglepolygon()
{
	if (vertexBuffer)
	{
		indexBuffer->Release();
		indexBuffer = nullptr;
	}

	if (indexBuffer)
	{
		indexBuffer->Release();
		indexBuffer = nullptr;
	}
}

[[nodiscard]] bool trianglepolygon::create(const device& device)noexcept
{
	if (!createvertexbuffer(device))
	{
		return false;
	}
	if (!createindexbuffer(device))
	{
		return false;
	}
	return true;
}

[[nodiscard]] bool trianglepolygon::createvertexbuffer(const device& device) noexcept
{
	Vertex triangleverteces[] = {
		{{0.0f,0.5f,0.0f},{1.0f,0.0f,0.0f,1.0f}},
		{{0.5f,-0.5f,0.0f},{0.0f,1.0f,0.0f,1.0f}},
		{{-0.5f,-0.5f,0.0f},{0.0f,0.0f,1.0f,1.0f}}
	};

	//データサイズ
	const auto vertexbufforsize = sizeof(triangleverteces);

	//ヒープ設定
	//cpuからアクセス可能なメモリを利用するための設定
	D3D12_HEAP_PROPERTIES heapProperty{};
	heapProperty.Type = D3D12_HEAP_TYPE_UPLOAD;
	heapProperty.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	heapProperty.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	heapProperty.CreationNodeMask = 1;
	heapProperty.VisibleNodeMask = 1;

	//-----リソース設定
	D3D12_RESOURCE_DESC resourcedesc{};
	resourcedesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resourcedesc.Alignment = 0;
	resourcedesc.Width = vertexbufforsize;
	resourcedesc.Height = 1;
	resourcedesc.DepthOrArraySize = 1;
	resourcedesc.MipLevels = 1;
	resourcedesc.Format = DXGI_FORMAT_UNKNOWN;
	resourcedesc.SampleDesc.Count = 1;
	resourcedesc.SampleDesc.Quality = 0;
	resourcedesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	resourcedesc.Flags = D3D12_RESOURCE_FLAG_NONE;

	//---頂点バッファの生成する
	auto res = device.get()->CreateCommittedResource(
		&heapProperty,
		D3D12_HEAP_FLAG_NONE,
		&resourcedesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertexBuffer));
	if (FAILED(res))
	{
		assert(false && "頂点バッファの作成に失敗");
		return false;
	}

	//頂点バッファ用のアドレス取得
	Vertex* data{};

	//バッファのマップ(data経由)
	res = vertexBuffer->Map(0, nullptr, reinterpret_cast<void**>(&data));
	if (FAILED(res))
	{
		assert(false && "頂点バッファの作成に失敗");
		return false;
	}

	//頂点データをコピー
	memcpy_s(data, vertexbufforsize, triangleverteces, vertexbufforsize);

	vertexBuffer->Unmap(0, nullptr);

	//頂点バッファビューの設定
	vertexbufferview.BufferLocation = vertexBuffer->GetGPUVirtualAddress();
	vertexbufferview.SizeInBytes = vertexbufforsize;
	vertexbufferview.StrideInBytes = sizeof(Vertex);

	return true;
}

//インデックスバッファ
[[nodiscard]] bool trianglepolygon::createindexbuffer(const device& device)noexcept
{
	uint16_t triangleindeces[] = {
		0,1,2
	};

	const auto indexbuffersize = sizeof(triangleindeces);

	D3D12_HEAP_PROPERTIES heapproperty{};
	heapproperty.Type = D3D12_HEAP_TYPE_UPLOAD;
	heapproperty.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	heapproperty.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	heapproperty.CreationNodeMask = 1;
	heapproperty.VisibleNodeMask = 1;


	D3D12_RESOURCE_DESC resourcedesc{};
	resourcedesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resourcedesc.Alignment = 0;
	resourcedesc.Width = indexbuffersize;
	resourcedesc.Height = 1;
	resourcedesc.DepthOrArraySize = 1;
	resourcedesc.MipLevels = 1;
	resourcedesc.Format = DXGI_FORMAT_UNKNOWN;
	resourcedesc.SampleDesc.Count = 1;
	resourcedesc.SampleDesc.Quality = 0;
	resourcedesc.Layout =D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	resourcedesc.Flags = D3D12_RESOURCE_FLAG_NONE;

	auto res = device.get()->CreateCommittedResource(
		&heapproperty,
		D3D12_HEAP_FLAG_NONE,
		&resourcedesc,
		D3D12_RESOURCE_STATE_DEPTH_READ,
		nullptr,
		IID_PPV_ARGS(&indexBuffer));
	if (FAILED(res))
	{
		assert(false && "インデックスバッファの作成に失敗");
		return false;
	}

	uint16_t* data{};
	res = indexBuffer->Map(0, nullptr, reinterpret_cast<void**>(&data));
	if (FAILED(res))
	{
		assert(false&&"インデックスバッファのマップに失敗");
		return false;
	}

}