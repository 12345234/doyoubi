// パイプラインステートオブジェクトクラス

#pragma once

#include "device.h"
#include "shader.h"
#include "root.h"

class pipline final {
public:
    pipline() = default;
    ~pipline();

    [[nodiscard]] bool create(const device& device, const shader& shader, const root& rootSignature) noexcept;


    [[nodiscard]] ID3D12PipelineState* get() const noexcept;

private:
    ID3D12PipelineState* pipelineState_ = {};  ///< パイプラインステート
};
