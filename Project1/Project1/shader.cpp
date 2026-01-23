#include "shader.h"
#include <cassert>
#include <string>
#include <D3Dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")

shader::~shader() {
    if (vertexshader) {
        vertexshader->Release();
        vertexshader = nullptr;
    }
    if (pixelshader) {
        pixelshader->Release();
        pixelshader = nullptr;
    }
}

[[nodiscard]] bool shader::create(const device& device) noexcept {

    const std::string  filePath = "../Project1/Shader.hlsl";//hlslファイルのパス
    const std::wstring temp = std::wstring(filePath.begin(), filePath.end());
    ID3DBlob* error{};

    auto res = D3DCompileFromFile(temp.data(), nullptr, nullptr, "vs", "vs_5_0", D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, &vertexshader, &error);
    if (FAILED(res)) {
        char* p = static_cast<char*>(error->GetBufferPointer());
        assert(false && "頂点シェーダのコンパイルに失敗しました");
    }
    res = D3DCompileFromFile(temp.data(), nullptr, nullptr, "ps", "ps_5_0", D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, &pixelshader, &error);
    if (FAILED(res)) {
        char* p = static_cast<char*>(error->GetBufferPointer());
        assert(false && "ピクセルシェーダのコンパイルに失敗しました");
    }

    if (error) {
        error->Release();
    }

    return true;
}

[[nodiscard]] ID3DBlob* shader::vertexShader() const noexcept {
    if (!vertexshader) {
        assert(false && "頂点シェーダが未作成です");
    }

    return vertexshader;
}

[[nodiscard]] ID3DBlob* shader::pixelShader() const noexcept {
    if (!pixelshader) {
        assert(false && "ピクセルシェーダが未作成です");
    }

    return pixelshader;
}
