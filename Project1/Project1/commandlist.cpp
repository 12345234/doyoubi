
#include "commandlist.h"
#include <cassert>

[[nodiscard]] bool commandlist::create(const command_allocator& commandAllocator) noexcept {

    const auto hr = device::instance().get()->CreateCommandList(0, commandAllocator.getType(), commandAllocator.get(), nullptr, IID_PPV_ARGS(&commandList));
    if (FAILED(hr)) {
        assert(false && "コマンドリストの作成に失敗しました");
        return false;
    }

    commandList->Close();
    return true;
}
void commandlist::reset(const command_allocator& commandAllocator) noexcept {
    if (!commandList) {
        assert(false && "コマンドリストが未作成です");
    }

    commandList->Reset(commandAllocator.get(), nullptr);
}

[[nodiscard]] ID3D12GraphicsCommandList* commandlist::get() const noexcept {
    if (!commandList) {
        assert(false && "コマンドリストが未作成です");
        return nullptr;
    }
    return commandList.Get();
}