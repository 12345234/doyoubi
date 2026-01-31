#pragma once

#include "device.h"
#include "commandlist.h"
#include <DirectXMath.h>
#include"input.h"

class TrianglePolygon final {
public:
    struct ConstBufferData {
        DirectX::XMMATRIX world_{}; 
        DirectX::XMFLOAT4 color_{}; 
    };
    DirectX::XMFLOAT3 poss;

    
    

    TrianglePolygon() = default;

    ~TrianglePolygon();
    [[nodiscard]] bool create(const device& device) noexcept;
    void draw(const commandlist& commandList) noexcept;
private:
    [[nodiscard]] bool createVertexBuffer(const device& device) noexcept;

    [[nodiscard]] bool createIndexBuffer(const device& device) noexcept;
    
    DirectX::XMMATRIX world_ = DirectX::XMMatrixIdentity();
    ID3D12Resource* vertexBuffer_{};  
    ID3D12Resource* indexBuffer_{};   

    D3D12_VERTEX_BUFFER_VIEW vertexBufferView_ = {}; 
    D3D12_INDEX_BUFFER_VIEW  indexBufferView_ = {};
};
