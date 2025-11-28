// 三角形ポリゴンクラス

#include "trianglepolygon.h"
#include <cassert>
#include <DirectXMath.h>

namespace {

    struct Vertex {
        DirectX::XMFLOAT3 position;  // 頂点座標（x, y, z）
        DirectX::XMFLOAT4 color;     // 頂点色（r, g, b, a）
    };
}  

trianglepolygon::~trianglepolygon() {
    // 頂点バッファの解放
    if (vertexBuffer) {
        vertexBuffer->Release();
        vertexBuffer = nullptr;
    }

    // インデックスバッファの解放
    if (indexBuffer) {
        indexBuffer->Release();
        indexBuffer = nullptr;
    }
}

[[nodiscard]] bool trianglepolygon::create(const device& device) noexcept {
    // 頂点バッファの生成
    if (!createvertexbuffer(device)) {
        return false;
    }
    // インデックスバッファの生成
    if (!createindexbuffer(device)) {
        return false;
    }
    return true;
}

[[nodiscard]] bool trianglepolygon::createvertexbuffer(const device& device) noexcept {
    Vertex triangleVertices[] = {
        {  {0.0f, 0.5f, 0.0f}, {1.0f, 0.0f, 0.0f, 1.0f}}, // 上頂点（赤色）
        { {0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f, 1.0f}}, // 右下頂点（緑色）
        {{-0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 1.0f, 1.0f}}  // 左下頂点（青色）
    };

    // 頂点データのサイズ
    const auto vertexBufferSize = sizeof(triangleVertices);

    // ヒープの設定を指定
    // CPU からアクセス可能なメモリを利用する為の設定
    D3D12_HEAP_PROPERTIES heapProperty{};
    heapProperty.Type = D3D12_HEAP_TYPE_UPLOAD;
    heapProperty.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
    heapProperty.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
    heapProperty.CreationNodeMask = 1;
    heapProperty.VisibleNodeMask = 1;

    // どんなリソースを作成するかの設定
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

    // 頂点バッファの生成
    auto res = device.get()->CreateCommittedResource(
        &heapProperty,
        D3D12_HEAP_FLAG_NONE,
        &resourceDesc,
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&vertexBuffer));
    if (FAILED(res)) {
        assert(false && "頂点バッファの作成に失敗");
        return false;
    }

    // 頂点バッファにデータを転送する
    // CPU からアクセス可能なアドレスを取得
    Vertex* data{};

    // バッファをマップ（CPUからアクセス可能にする）
    // vertexBuffer_ を直接利用するのではなく、data を介して更新するイメージ
    res = vertexBuffer->Map(0, nullptr, reinterpret_cast<void**>(&data));
    if (FAILED(res)) {
        assert(false && "頂点バッファのマップに失敗");
        return false;
    }

    // 頂点データをコピー
    memcpy_s(data, vertexBufferSize, triangleVertices, vertexBufferSize);

    // コピーが終わったのでマップ解除（CPUからアクセス不可にする）
    // ここまで来たら GPU が利用するメモリ領域（VRAM）にコピー済みなので、triangleVertices は不要になる
    vertexBuffer->Unmap(0, nullptr);

    // 頂点バッファビューの設定
    vertexbufferview.BufferLocation = vertexBuffer->GetGPUVirtualAddress();  // 頂点バッファのアドレス
    vertexbufferview.SizeInBytes = vertexBufferSize;                       // 頂点バッファのサイズ
    vertexbufferview.StrideInBytes = sizeof(Vertex);                         // 1頂点あたりのサイズ

    return true;
}

[[nodiscard]] bool trianglepolygon::createindexbuffer(const device& device) noexcept {
    uint16_t triangleIndices[] = {
        0, 1, 2  // 三角形を構成する頂点のインデックス
    };
    const auto indexBufferSize = sizeof(triangleIndices);

    D3D12_HEAP_PROPERTIES heapProperty{};
    heapProperty.Type = D3D12_HEAP_TYPE_UPLOAD;
    heapProperty.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
    heapProperty.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
    heapProperty.CreationNodeMask = 1;
    heapProperty.VisibleNodeMask = 1;

    // リソースの設定を行う
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

    // インデックスバッファの生成
    auto res = device.get()->CreateCommittedResource(
        &heapProperty,
        D3D12_HEAP_FLAG_NONE,
        &resourceDesc,
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&indexBuffer));
    if (FAILED(res)) {
        assert(false && "インデックスバッファの作成に失敗");
        return false;
    }

    // インデックスバッファにデータを転送する
    uint16_t* data{};
    res = indexBuffer->Map(0, nullptr, reinterpret_cast<void**>(&data));
    if (FAILED(res)) {
        assert(false && "インデックスバッファのマップに失敗");
        return false;
    }

    memcpy_s(data, indexBufferSize, triangleIndices, indexBufferSize);
    // ここまで来たら GPU が利用するメモリ領域（VRAM）にコピー済みなので、triangleIndices は不要になる
    indexBuffer->Unmap(0, nullptr);

    // インデックスバッファビュー作成
    indexbufferview.BufferLocation = indexBuffer->GetGPUVirtualAddress();
    indexbufferview.SizeInBytes = indexBufferSize;
    indexbufferview.Format = DXGI_FORMAT_R16_UINT;  // triangleIndices の型が 16bit 符号なし整数なので R16_UINT

    return true;
}

[[nodiscard]] void trianglepolygon::draw(const commandlist& commandList) noexcept {
    commandList.get()->IASetVertexBuffers(0, 1, &vertexbufferview);
    commandList.get()->IASetIndexBuffer(&indexbufferview);
    commandList.get()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    commandList.get()->DrawIndexedInstanced(3, 1, 0, 0, 0);
}
