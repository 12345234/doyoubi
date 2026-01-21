#include "DescriptorHeap.h"
#include<cassert>
#include"device.h"

class DescriptorHeap final {
public:

    DescriptorHeap() = default;


    ~DescriptorHeap() = default;

public:

    [[nodiscard]] bool create(D3D12_DESCRIPTOR_HEAP_TYPE type, UINT numDescriptors, bool shaderVisible = false) noexcept {
        D3D12_DESCRIPTOR_HEAP_DESC heapDesc{};
        heapDesc.Type = type;
        heapDesc.NumDescriptors = numDescriptors;
        heapDesc.Flags = shaderVisible ? D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE : D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

        type_ = type; 

        HRESULT hr = device::instance().get()->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&heap_));
        if (FAILED(hr)) {
            assert(false && "ディスクリプタヒープの生成に失敗しました");
            return false;
        }

        maxDescriptorCount_ = numDescriptors;
        for (UINT i = 0; i < maxDescriptorCount_; ++i) {
            freeIndices_.push_back(i);
        }

        return true;
    }

    void applyPendingFree() noexcept {
        if (pendingFreeIndices_.empty()) {
            return;
        }

        for (auto i : pendingFreeIndices_) {
            freeIndices_.push_back(i);
        }
        pendingFreeIndices_.clear();
    }
    [[nodiscard]] ID3D12DescriptorHeap* get() const noexcept {
        if (!heap_) {
            assert(false && "ディスクリプタヒープが未生成です");
        }
        return heap_.Get();
    }

    [[nodiscard]] D3D12_DESCRIPTOR_HEAP_TYPE getType() const noexcept {
        if (!heap_) {
            assert(false && "ディスクリプタヒープが未生成です");
        }
        return type_;
    }

    [[nodiscard]] std::optional<UINT> allocateDescriptor() noexcept {
        if (freeIndices_.empty()) {
            return std::nullopt;
        }
        const auto index = freeIndices_.back();
        freeIndices_.pop_back();
        return index;
    }

    void releaseDescriptor(UINT descriptorIndex) noexcept {
        pendingFreeIndices_.push_back(descriptorIndex);
    }


private:
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> heap_{};                /// ディスクリプタヒープ
    D3D12_DESCRIPTOR_HEAP_TYPE                   type_{};                /// ヒープのタイプ
    UINT                                         maxDescriptorCount_{};  /// 最大ディスクリプタ数
    std::vector<UINT>                            freeIndices_{};         /// 空きディスクリプタインデックスのリスト
    std::vector<UINT>                            pendingFreeIndices_{};  /// 解放待ちディスクリプタインデックスのリスト
};

DescriptorHeapa::DescriptorHeapa() = default;
DescriptorHeapa::~DescriptorHeapa() {
    map.clear();
}

[[nodiscard]] bool DescriptorHeapa::create(D3D12_DESCRIPTOR_HEAP_TYPE type, UINT numDescriptors, bool shaderVisible) noexcept {
    if (map.find(type) != map.end()) {
        return false;
    }

    auto p = std::make_unique<DescriptorHeap>();
    if (p->create(type, numDescriptors, shaderVisible)) {
        map.emplace(type, std::move(p));
    }

    return true;
}

void DescriptorHeapa::applyPending() noexcept {
    for (auto& [key, p] : map) {
        p->applyPendingFree();
    }
}

[[nodiscard]] ID3D12DescriptorHeap* DescriptorHeapa::get(D3D12_DESCRIPTOR_HEAP_TYPE type) const noexcept {
    const auto it = map.find(type);
    if (it == map.end()) {
        assert(false && "ディスクリプタヒープがありません");
        return nullptr;
    }

    return it->second->get();
}

[[nodiscard]] optional<UINT> DescriptorHeapa::allocateDescriptor(D3D12_DESCRIPTOR_HEAP_TYPE type) noexcept {
    const auto it = map.find(type);
    if (it == map.end()) {
        assert(false && "ディスクリプタヒープがありません");
        return nullopt;
    }

    return it->second->allocateDescriptor();
}

void DescriptorHeapa::releaseDescriptor(D3D12_DESCRIPTOR_HEAP_TYPE type, UINT descriptorIndex) noexcept {
    const auto it = map.find(type);
    if (it == map.end()) {
        assert(false && "ディスクリプタヒープがありません");
    }

    return it->second->releaseDescriptor(descriptorIndex);
}