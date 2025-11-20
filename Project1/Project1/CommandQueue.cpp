#include<cassert>
#include<dxgi1_6.h>
#include<d3d12.h>
#include "Dire.h"

#pragma comment(lib,"dxgi.lib")

#pragma comment(lib,"d3d12.lib")

ID3D12CommandQueue* Dx12::CreateCommandQueue(ID3D12Device* device) {
    D3D12_COMMAND_QUEUE_DESC queueDesc = {};
    queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;       // 直接実行型
    queueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;  // 通常優先度
    queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;        // 特別フラグなし
    queueDesc.NodeMask = 0;                                    // 単一GPU使用

    ID3D12CommandQueue* commandQueue;
    HRESULT             hr = device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&commandQueue));

    if (FAILED(hr)) {
        OutputDebugString("Failed to create Command Queue\n");
        return nullptr;
    }

    commandQueue->SetName(L"Main Command Queue");

    return commandQueue;
}