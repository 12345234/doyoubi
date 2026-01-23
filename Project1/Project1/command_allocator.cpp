
#include "command_allocator.h"
#include <cassert>

commandallocator::~commandallocator() {
    if (commandAllocator) {
        commandAllocator->Release();
        commandAllocator = nullptr;
    }
}
[[nodiscard]] bool commandallocator::create(const device& device, const D3D12_COMMAND_LIST_TYPE type) noexcept {

    type_ = type;

    const auto hr = device.get()->CreateCommandAllocator(type, IID_PPV_ARGS(&commandAllocator));
    if (FAILED(hr)) {
        assert(false && "コマンドアロケータの作成に失敗しました");
        return false;
    }

    return true;
}

void commandallocator::reset() noexcept {

    if (!commandAllocator) {
        assert(false && "コマンドアロケータが未作成です");
    }

    commandAllocator->Reset();
}
[[nodiscard]] ID3D12CommandAllocator* commandallocator::get() const noexcept {
    if (!commandAllocator) {
        assert(false && "コマンドアロケータが未作成です");
        return nullptr;
    }
    return commandAllocator;
}
[[nodiscard]] D3D12_COMMAND_LIST_TYPE commandallocator::getType() const noexcept {
    if (!commandAllocator) {
        assert(false && "コマンドリストのタイプが未設定です");
    }
    return type_;
}
