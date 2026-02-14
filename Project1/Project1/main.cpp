#include "window.h"
#include "input.h"

#include "device.h"
#include "DXGI.h"
#include "command_allocator.h"
#include "commandlist.h"
#include "commandque.h"
#include "swapchain.h"
#include "DescriptorHeap.h"
#include "renderTargets.h"
#include "fence.h"
#include "root.h"
#include "shader.h"
#include "pipline.h"
#include "constantbuffer.h"
#include "depthbuffer.h"

#include "trianglepolygon.h"
#include "quadpolygon.h"
#include "Bullet.h"

#include "camera.h"
#include "object.h"

#include <cassert>
#include <DirectXMath.h>

class main final {
public:

    main() = default;

    ~main() = default;

    [[nodiscard]] bool initialize(HINSTANCE instance) noexcept {
        if (S_OK != windowInstance_.create(instance, 1280, 720, "MyApp")) {
            assert(false && "ウィンドウの生成に失敗しました");
            return false;
        }

        if (!dxgiInstance_.setDisplayAdapter()) {
            assert(false && "DXGIのアダプタ設定に失敗しました");
            return false;
        }

        if (!deviceInstance_.create(dxgiInstance_)) {
            assert(false && "デバイスの作成に失敗しました");
            return false;
        }
        if (!commandQueueInstance_.create(deviceInstance_)) {
            assert(false && "コマンドキューの作成に失敗しました");
            return false;
        }
        if (!swapChainInstance_.create(dxgiInstance_, windowInstance_, commandQueueInstance_)) {
            assert(false && "スワップチェインの作成に失敗しました");
            return false;
        }

        if (!descriptorHeapInstance_.create(deviceInstance_, D3D12_DESCRIPTOR_HEAP_TYPE_RTV, swapChainInstance_.getDesc().BufferCount)) {
            assert(false && "ディスクリプタヒープの作成に失敗しました");
            return false;
        }

        if (!renderTargetInstance_.createBackBuffer(deviceInstance_, swapChainInstance_, descriptorHeapInstance_)) {
            assert(false && "レンダーターゲットの作成に失敗しました");
            return false;
        }

        if (!commandAllocatorInstance_[0].create(deviceInstance_, D3D12_COMMAND_LIST_TYPE_DIRECT)) {
            assert(false && "コマンドアロケータの作成に失敗しました");
            return false;
        }
        if (!commandAllocatorInstance_[1].create(deviceInstance_, D3D12_COMMAND_LIST_TYPE_DIRECT)) {
            assert(false && "コマンドアロケータの作成に失敗しました");
            return false;
        }

        if (!commandListInstance_.create(deviceInstance_, commandAllocatorInstance_[0])) {
            assert(false && "コマンドリストの作成に失敗しました");
            return false;
        }

        if (!fenceInstance_.create(deviceInstance_)) {
            assert(false && "フェンスの作成に失敗しました");
            return false;
        }

        if (!BulletpolygonInstance.create(deviceInstance_))
        {
            assert(false&&"弾の生成に失敗");
            return false;
        }
        //BulletInstance.initializeBullet({ -.1f, 0.0f, 0.1f }, { 1,1,1,1 });
        if (!trianglePolygonInstance_.create(deviceInstance_)) {
            assert(false && "三角形ポリゴンの作成に失敗しました");
            return false;
        }

        if (!quadPolygonInstance_.create(deviceInstance_)) {
            assert(false && "四角形ポリゴンの作成に失敗しました");
            return false;
        }

        if (!rootSignatureInstance_.create(deviceInstance_)) {
            assert(false && "ルートシグネチャの作成に失敗しました");
            return false;
        }
        if (!shaderInstance_.create(deviceInstance_)) {
            assert(false && "シェーダーの作成に失敗しました");
            return false;
        }
        if (!piplineStateObjectInstance_.create(deviceInstance_, shaderInstance_, rootSignatureInstance_)) {
            assert(false && "パイプラインステートオブジェクトの作成に失敗しました");
            return false;
        }
        cameraInstance_.initialize();

        if (!constantBufferDescriptorHeapInstance_.create(deviceInstance_, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, 3, true)) {
            assert(false && "定数バッファ用ディスクリプタヒープの作成に失敗しました");
            return false;
        }
        if (!cameraConstantBufferInstance_.create(deviceInstance_, constantBufferDescriptorHeapInstance_, sizeof(camera::ConstBufferData), 0)) {
            assert(false && "カメラ用コンスタントバッファの作成に失敗しました");
            return false;
        }

        if (!trianglePolygonConstantBufferInstance_.create(deviceInstance_, constantBufferDescriptorHeapInstance_, sizeof(TrianglePolygon::ConstBufferData), 1)) {
            assert(false && "三角形ポリゴン用コンスタントバッファの作成に失敗しました");
            return false;
        }
        if (!quadPolygonConstantBufferInstance_.create(deviceInstance_, constantBufferDescriptorHeapInstance_, sizeof(TrianglePolygon::ConstBufferData), 2)) {
            assert(false && "三角形ポリゴン用コンスタントバッファの作成に失敗しました");
            return false;
        }
        if (!depthBufferDescriptorHeapInstance_.create(deviceInstance_, D3D12_DESCRIPTOR_HEAP_TYPE_DSV, 1)) {
            assert(false && "デプスバッファ用ディスクリプタヒープの作成に失敗しました");
            return false;
        }
        if (!depthBufferInstance_.create(deviceInstance_, depthBufferDescriptorHeapInstance_, windowInstance_)) {
            assert(false && "デプスバッファの作成に失敗しました");
            return false;
        }

        return true;
    }

    void loop() noexcept {
        while (windowInstance_.messageLoop()) {
            cameraInstance_.update();
            
            triangleObjectInstance_.update({.1f, 0.0f, -1.0f}, { .0f, 0.0f, 0.0f }, { 1,1,1,1 });
            const auto backBufferIndex = swapChainInstance_.get()->GetCurrentBackBufferIndex();

            if (frameFenceValue_[backBufferIndex] != 0) {
                fenceInstance_.wait(frameFenceValue_[backBufferIndex]);
            }

            commandAllocatorInstance_[backBufferIndex].reset();
            commandListInstance_.reset(commandAllocatorInstance_[backBufferIndex]);

            auto pToRT = resourceBarrier(renderTargetInstance_.get(backBufferIndex), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
            commandListInstance_.get()->ResourceBarrier(1, &pToRT);

            D3D12_CPU_DESCRIPTOR_HANDLE handles[] = { renderTargetInstance_.getCpuDescriptorHandle(deviceInstance_, descriptorHeapInstance_, backBufferIndex) };
            D3D12_CPU_DESCRIPTOR_HANDLE depthHandle = depthBufferInstance_.getCpuDescriptorHandle();
            commandListInstance_.get()->OMSetRenderTargets(1, handles, false, &depthHandle);

            const float clearColor[] = { 0.2f, 0.2f, 0.2f, 1.0f };  // クリア
            commandListInstance_.get()->ClearRenderTargetView(handles[0], clearColor, 0, nullptr);

            commandListInstance_.get()->ClearDepthStencilView(depthHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

            commandListInstance_.get()->SetGraphicsRootSignature(rootSignatureInstance_.get());

            const auto [w, h] = windowInstance_.size();
            D3D12_VIEWPORT viewport{};
            viewport.TopLeftX = 0.0f;
            viewport.TopLeftY = 0.0f;
            viewport.Width = static_cast<float>(w);
            viewport.Height = static_cast<float>(h);
            viewport.MinDepth = 0.0f;
            viewport.MaxDepth = 1.0f;
            commandListInstance_.get()->RSSetViewports(1, &viewport);

            D3D12_RECT scissorRect{};
            scissorRect.left = 0;
            scissorRect.top = 0;
            scissorRect.right = w;
            scissorRect.bottom = h;
            commandListInstance_.get()->RSSetScissorRects(1, &scissorRect);

            ID3D12DescriptorHeap* p[] = { constantBufferDescriptorHeapInstance_.get() };
            commandListInstance_.get()->SetDescriptorHeaps(1, p);

            camera::ConstBufferData cameraData{
                DirectX::XMMatrixTranspose(cameraInstance_.viewMatrix()),
                DirectX::XMMatrixTranspose(cameraInstance_.projection()),
            };
            UINT8* pCameraData{};
            cameraConstantBufferInstance_.constantbuffer1()->Map(0, nullptr, reinterpret_cast<void**>(&pCameraData));
            memcpy_s(pCameraData, sizeof(cameraData), &cameraData, sizeof(cameraData));
            cameraConstantBufferInstance_.constantbuffer1()->Unmap(0, nullptr);
            commandListInstance_.get()->SetGraphicsRootDescriptorTable(0, cameraConstantBufferInstance_.getGpuDescriptorHandle());
            commandListInstance_.get()->SetPipelineState(piplineStateObjectInstance_.get());

            {
                Quadpolygon::ConstBufferData quadData{
                    DirectX::XMMatrixTranspose(quadObjectInstance_.world()),
                    quadObjectInstance_.color() };
                UINT8* pQuadData{};
                quadPolygonConstantBufferInstance_.constantbuffer1()->Map(0, nullptr, reinterpret_cast<void**>(&pQuadData));
                memcpy_s(pQuadData, sizeof(quadData), &quadData, sizeof(quadData));
                quadPolygonConstantBufferInstance_.constantbuffer1()->Unmap(0, nullptr);
                commandListInstance_.get()->SetGraphicsRootDescriptorTable(1, quadPolygonConstantBufferInstance_.getGpuDescriptorHandle());

                quadPolygonInstance_.draw(commandListInstance_);
            }

            {
                TrianglePolygon::ConstBufferData triangleData{
                    DirectX::XMMatrixTranspose(triangleObjectInstance_.world()),
                    triangleObjectInstance_.color() };
                UINT8* pTriangleData{};
                trianglePolygonConstantBufferInstance_.constantbuffer1()->Map(0, nullptr, reinterpret_cast<void**>(&pTriangleData));
                memcpy_s(pTriangleData, sizeof(triangleData), &triangleData, sizeof(triangleData));
                trianglePolygonConstantBufferInstance_.constantbuffer1()->Unmap(0, nullptr);
                commandListInstance_.get()->SetGraphicsRootDescriptorTable(1, trianglePolygonConstantBufferInstance_.getGpuDescriptorHandle());
                trianglePolygonInstance_.draw(commandListInstance_);
                
            }
           /* {
                Bullet::BulletData BData{
                    DirectX::XMMatrixTranspose(BulletInstance.Bworld()),
                    BulletInstance.Bcolor() };
                UINT8* pTriangleData{};
                BulletBufer.constantbuffer1()->Map(0, nullptr, reinterpret_cast<void**>(&pTriangleData));
                memcpy_s(pTriangleData, sizeof(BData), &BData, sizeof(BData));
                BulletBufer.constantbuffer1()->Unmap(0, nullptr);
                commandListInstance_.get()->SetGraphicsRootDescriptorTable(1, BulletBufer.getGpuDescriptorHandle());
                BulletpolygonInstance.draw(commandListInstance_);

            }*/


            auto rtToP = resourceBarrier(renderTargetInstance_.get(backBufferIndex), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
            commandListInstance_.get()->ResourceBarrier(1, &rtToP);
            commandListInstance_.get()->Close();

            ID3D12CommandList* ppCommandLists[] = { commandListInstance_.get() };
            commandQueueInstance_.get()->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

            swapChainInstance_.get()->Present(1, 0);

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
    window           windowInstance_{};             
    DXGI             dxgiInstance_{};               
    device           deviceInstance_{};           
    commandque       commandQueueInstance_{};         
    swapchain        swapChainInstance_{};          
    DescriptorHeap   descriptorHeapInstance_{};       
    render           renderTargetInstance_{};         
    commandallocator commandAllocatorInstance_[2]{};  
    commandlist      commandListInstance_{};          

    fence  fenceInstance_{};       
    UINT64 frameFenceValue_[2]{}; 
    UINT64 nextFenceValue_ = 1;    


    root     rootSignatureInstance_{};               
    shader             shaderInstance_{};                       
    pipline piplineStateObjectInstance_{};           
    DescriptorHeap     constantBufferDescriptorHeapInstance_{};  

    Bullet          BulletpolygonInstance{};
    object          BulletInstance{};
    TrianglePolygon trianglePolygonInstance_{};                
    object          triangleObjectInstance_{};                 
    constantbuffer  trianglePolygonConstantBufferInstance_{};  

    constantbuffer BulletBufer{};

    camera         cameraInstance_{};               
    constantbuffer cameraConstantBufferInstance_{}; 

    Quadpolygon    quadPolygonInstance_{};                
    object         quadObjectInstance_{};                 
    constantbuffer quadPolygonConstantBufferInstance_{}; 

    DescriptorHeap depthBufferDescriptorHeapInstance_{};  
    depthbuffer    depthBufferInstance_{};              
};
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    main app;

    if (!app.initialize(hInstance)) {
        assert(false && "アプリケーションの初期化に失敗しました");
    }

    app.loop();

    return 0;
}
