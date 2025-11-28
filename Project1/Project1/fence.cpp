#include "fence.h"
#include <cassert>

fence::~fence() {
	// フェンスの解放
	if (fence_) {
		fence_->Release();
		fence_ = nullptr;
	}
}
[[nodiscard]] bool fence::create(const device& device) noexcept {

	// フェンスの生成
	HRESULT hr = device.get()->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence_));
	if (FAILED(hr)) {
		assert(false && "フェンスの作成に失敗しました");
		return false;
	}
	// GPU 同期用のイベントハンドルを作成
	waitGpuEvent = CreateEvent(nullptr, false, false, "WAIT_GPU");
	if (!waitGpuEvent) {
		assert(false && "GPU 同期用のイベントハンドルの作成に失敗しました");
		return false;
	}
	return true;
}

void fence::wait(UINT64 fenceValue) const noexcept {
	if (!fence_) {
		assert(false && "フェンスが未作成です");
		return;
	}

	// フェンスの値が指定された値に達するまで待機
	if (fence_->GetCompletedValue() < fenceValue) {
		// GPU がフェンス値に到達するまで待つ
		fence_->SetEventOnCompletion(fenceValue, waitGpuEvent);
		WaitForSingleObject(waitGpuEvent, INFINITE);
	}
}
[[nodiscard]] ID3D12Fence* fence::get() const noexcept {
	if (!fence_) {
		assert(false && "フェンスが未作成です");
		return nullptr;
	}
	return fence_;
}
