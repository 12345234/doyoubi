
#include "commandque.h"
#include <cassert>

commandque::~commandque() {
    if (commandQueue) {
        commandQueue->Release();
        commandQueue = nullptr;
    }
}
[[nodiscard]] bool commandque::create(const device& device) noexcept {
    D3D12_COMMAND_QUEUE_DESC desc{};
    desc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;      
    desc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;  
    desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;      
    desc.NodeMask = 0;                               

    const auto hr = device.get()->CreateCommandQueue(&desc,IID_PPV_ARGS(&commandQueue));
    if (FAILED(hr)) {
        assert(false && "コマンドキューの作成に失敗");
        return false;
    }
    return true;
}
[[nodiscard]] ID3D12CommandQueue* commandque::get() const noexcept {
    if (!commandQueue) {
        assert(false && "コマンドキューが未作成です");
        return nullptr;
    }
    return commandQueue;
}
