#pragma once

#include "device.h"

class shader final {
public:
    shader() = default;

    ~shader();

    [[nodiscard]] bool create(const device& device) noexcept;

    [[nodiscard]] ID3DBlob* vertexShader() const noexcept;

    [[nodiscard]] ID3DBlob* pixelShader() const noexcept;


private:
    ID3DBlob* vertexshader{}; 
    ID3DBlob* pixelshader{};  
};
