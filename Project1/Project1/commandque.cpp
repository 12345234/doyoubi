
#include "commandque.h"
#include <cassert>

[[nodiscard]] bool commandque::create() noexcept {
    D3D12_COMMAND_QUEUE_DESC desc{};
    desc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;      
    desc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;  
    desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;      
    desc.NodeMask = 0;                               

    const auto hr = device::instance().get()->CreateCommandQueue(&desc,IID_PPV_ARGS(&commandQueue_));
    if (FAILED(hr)) {
        assert(false && "コマンドキューの作成に失敗");
        return false;
    }
    return true;
}
[[nodiscard]] ID3D12CommandQueue* commandque::get() const noexcept {
    if (!commandQueue_) {
        assert(false && "コマンドキューが未作成です");
        return nullptr;
    }
    return commandQueue_.Get();
}
