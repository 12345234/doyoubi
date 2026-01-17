// パイプラインステートオブジェクトクラス

#pragma once

#include "device.h"
#include "shader.h"
#include "root.h"
#include <wrl/client.h>

class pipline final {
public:
    pipline() = default;
    ~pipline() = default;

    [[nodiscard]] bool create(const shader& shader, const root& rootSignature) noexcept;


    [[nodiscard]] ID3D12PipelineState* get() const noexcept;

private:
    Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState_ = {};
};
