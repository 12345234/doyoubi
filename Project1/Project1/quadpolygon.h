#pragma once

#include "device.h"
#include "commandlist.h"
#include <DirectXMath.h>
class quadpolygon final {
public:

    struct ConstBufferData {
        DirectX::XMMATRIX world_{};
        DirectX::XMFLOAT4 color_{};
    };

public:

    quadpolygon() = default;

    ~quadpolygon();
    [[nodiscard]] bool create(const device& device) noexcept;
    void draw(const commandlist& commandList) noexcept;

private:
    [[nodiscard]] bool createVertexBuffer(const device& device) noexcept;

    [[nodiscard]] bool createIndexBuffer(const device& device) noexcept;

private:
    ID3D12Resource* vertexBuffer{};
    ID3D12Resource* indexBuffer{};

    D3D12_VERTEX_BUFFER_VIEW vertexBufferView_ = {};
    D3D12_INDEX_BUFFER_VIEW  indexBufferView_ = {};
};
