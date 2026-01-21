#pragma once
#include "device.h"
#include <wrl/client.h>
using namespace std;
class shader final {
public:

    shader() = default;
    ~shader() = default;

    [[nodiscard]] bool create() noexcept;


    [[nodiscard]] ID3DBlob* vertexShader() const noexcept;


    [[nodiscard]] ID3DBlob* pixelShader() const noexcept;


private:
    Microsoft::WRL::ComPtr<ID3DBlob>  vertexShader_{};  /// 頂点シェーダ
    Microsoft::WRL::ComPtr<ID3DBlob> pixelShader_{};   /// ピクセルシェーダ
};
