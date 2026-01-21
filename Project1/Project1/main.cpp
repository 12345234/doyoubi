#include "window.h"
#include "device.h"
#include "DXGI.h"
#include "command_allocator.h"
#include "commandlist.h"
#include "commandque.h"
#include "swapchain.h"
#include "descriptorheap.h"
#include "renderTargets.h"
#include "fence.h"
#include "root.h"
#include "shader.h"
#include "pipline.h"
#include "constantbuffer.h"
#include "depthbuffer.h"

#include"trianglepolygon.h"

#include <memory>
#include <vector>
#include <cassert>

namespace {
    constexpr UINT sceneShaderSlot_ = 0;  
}
class main final {
public:
    main() = default;

    ~main() = default;

    [[nodiscard]] bool initialize(HINSTANCE instance) noexcept {
        // ウィンドウの生成
        if (S_OK != window::instance().create(instance, 1280, 720, "MyApp")) {
            assert(false && "ウィンドウの生成に失敗しました");
            return false;
        }
        // デバイスの生成
        if (!device::instance().create()) {
            assert(false && "デバイスの作成に失敗しました");
            return false;
        }
        // コマンドキューの生成
        if (!commandQueueInstance_.create()) {
            assert(false && "コマンドキューの作成に失敗しました");
            return false;
        }
        // スワップチェインの生成
        if (!swapChainInstance_.create(commandQueueInstance_)) {
            assert(false && "スワップチェインの作成に失敗しました");
            return false;
        }
        // ディスクリプタヒープの生成
        if (!DescriptorHeapa::instance().create(D3D12_DESCRIPTOR_HEAP_TYPE_RTV, swapChainInstance_.getDesc().BufferCount)) {
            assert(false && "ディスクリプタヒープの作成に失敗しました");
            return false;
        }
        if (!DescriptorHeapa::instance().create(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, 5, true)) {
            assert(false && "定数バッファ用ディスクリプタヒープの作成に失敗しました");
            return false;
        }
        if (!DescriptorHeapa::instance().create(D3D12_DESCRIPTOR_HEAP_TYPE_DSV, 1)) {
            assert(false && "デプスバッファ用ディスクリプタヒープの作成に失敗しました");
            return false;
        }
        // レンダーターゲットの生成
        if (!renderTargetInstance_.createBackBuffer(swapChainInstance_)) {
            assert(false && "レンダーターゲットの作成に失敗しました");
            return false;
        }
        if (!depthBufferInstance_.create()) {
            assert(false && "デプスバッファの作成に失敗しました");
            return false;
        }
        // コマンドアロケータの生成
        if (!commandAllocatorInstance_[0].create( D3D12_COMMAND_LIST_TYPE_DIRECT)) {
            assert(false && "コマンドアロケータの作成に失敗しました");
            return false;
        }
        if (!commandAllocatorInstance_[1].create(D3D12_COMMAND_LIST_TYPE_DIRECT)) {
            assert(false && "コマンドアロケータの作成に失敗しました");
            return false;
        }
        // コマンドリストの生成
        if (!commandListInstance_.create(commandAllocatorInstance_[0])) {
            assert(false && "コマンドリストの作成に失敗しました");
            return false;
        }
        // フェンスの生成
        if (!fenceInstance_.create()) {
            assert(false && "フェンスの作成に失敗しました");
            return false;
        }
        
        // ルートシグネチャの生成
        if (!rootSignatureInstance_.create()) {
            assert(false && "ルートシグネチャの作成に失敗しました");
            return false;
        }
        // シェーダーの生成
        if (!shaderInstance_.create()) {
            assert(false && "シェーダーの作成に失敗しました");
            return false;
        }
        // パイプラインステートオブジェクトの生成
        if (!piplineStateObjectInstance_.create(shaderInstance_, rootSignatureInstance_)) {
            assert(false && "パイプラインステートオブジェクトの作成に失敗しました");
            return false;
        }

        // 三角形ポリゴンの生成
        if (!trianglePolygonInstance_.create(deviceInstance)) {
            assert(false && "三角形ポリゴンの作成に失敗しました");
            return false;
        }

        return true;
    }

    void loop() noexcept {
        while (window::instance().messageLoop()) {
            // 現在のバックバッファインデックスを取得
            const auto backBufferIndex = swapChainInstance_.get()->GetCurrentBackBufferIndex();
            // 以前のフレームの GPU の処理が完了しているか確認して待機する
            if (frameFenceValue_[backBufferIndex] != 0) {
                fenceInstance_.wait(frameFenceValue_[backBufferIndex]);
            }
            // コマンドアロケータリセット
            commandAllocatorInstance_[backBufferIndex].reset();
            // コマンドリストリセット
            commandListInstance_.reset(commandAllocatorInstance_[backBufferIndex]);

            // リソースバリアでレンダーターゲットを Present から RenderTarget へ変更
            auto pToRT = resourceBarrier(renderTargetInstance_.get(backBufferIndex), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
            commandListInstance_.get()->ResourceBarrier(1, &pToRT);

            // レンダーターゲットの設定
            D3D12_CPU_DESCRIPTOR_HANDLE handles[] = { renderTargetInstance_.getDescriptorHandle(backBufferIndex) };
            commandListInstance_.get()->OMSetRenderTargets(1, handles, false, nullptr);

            // レンダーターゲットのクリア
            const float clearColor[] = { 0.2f, 0.2f, 0.2f, 1.0f };  // 背景色のクリア
            commandListInstance_.get()->ClearRenderTargetView(handles[0], clearColor, 0, nullptr);

            // パイプラインステートの設定
            commandListInstance_.get()->SetPipelineState(piplineStateObjectInstance_.get());
            // ルートシグネチャの設定
            commandListInstance_.get()->SetGraphicsRootSignature(rootSignatureInstance_.get());
            // ビューポートの設定
            const auto [w, h] = window::instance().size();
            D3D12_VIEWPORT viewport{};
            viewport.TopLeftX = 0.0f;
            viewport.TopLeftY = 0.0f;
            viewport.Width = static_cast<float>(w);
            viewport.Height = static_cast<float>(h);
            viewport.MinDepth = 0.0f;
            viewport.MaxDepth = 1.0f;
            commandListInstance_.get()->RSSetViewports(1, &viewport);
            // シザー矩形の設定
            D3D12_RECT scissorRect{};
            scissorRect.left = 0;
            scissorRect.top = 0;
            scissorRect.right = w;
            scissorRect.bottom = h;
            commandListInstance_.get()->RSSetScissorRects(1, &scissorRect);
            // ポリゴンの描画
            trianglePolygonInstance_.draw(commandListInstance_);
           // リソースバリアでレンダーターゲットを RenderTarget から Present へ変更
            auto rtToP = resourceBarrier(renderTargetInstance_.get(backBufferIndex), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
            commandListInstance_.get()->ResourceBarrier(1, &rtToP);
            // コマンドリストクローズ
            commandListInstance_.get()->Close();
            // コマンドキューにコマンドリストを送信
            ID3D12CommandList* ppCommandLists[] = { commandListInstance_.get() };
            commandQueueInstance_.get()->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);
            // プレゼント
            swapChainInstance_.get()->Present(1, 0);
            // フェンス値を設定
            commandQueueInstance_.get()->Signal(fenceInstance_.get(), nextFenceValue_);
            frameFenceValue_[backBufferIndex] = nextFenceValue_;
            nextFenceValue_++;
        }

    }
    D3D12_RESOURCE_BARRIER resourceBarrier(ID3D12Resource* resource, D3D12_RESOURCE_STATES from, D3D12_RESOURCE_STATES to) noexcept {
        D3D12_RESOURCE_BARRIER barrier{};
        barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
        barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
        barrier.Transition.pResource = resource;
        barrier.Transition.StateBefore = from;
        barrier.Transition.StateAfter = to;
        barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

        return barrier;
    }

private:
    device        deviceInstance{};
    commandque     commandQueueInstance_{};
    swapchain        swapChainInstance_{};            
    renderTargets     renderTargetInstance_{};
    depthbuffer      depthBufferInstance_{};
    command_allocator commandAllocatorInstance_[2]{};
    commandlist      commandListInstance_{};          

    fence  fenceInstance_{};       
    UINT64 frameFenceValue_[2]{};  
    UINT64 nextFenceValue_ = 1;    

    root     rootSignatureInstance_{};       
    shader             shaderInstance_{};              
    pipline piplineStateObjectInstance_{};  

    //unique_ptr<game::Camera> camera_{};  
    trianglepolygon    trianglePolygonInstance_{};     
};


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    // アプリケーションクラスのインスタンスを生成
    main app;

    if (!app.initialize(hInstance)) {
        assert(false && "アプリケーションの初期化に失敗しました");
    }
    // アプリケーションループを開始
    app.loop();

    return 0;
}
