#include<cassert>
#include<dxgi1_6.h>
#include<d3d12.h>
#include "Dire.h"
#include "device.h"

#pragma comment(lib,"dxgi.lib")

#pragma comment(lib,"d3d12.lib")

ID3D12Device* Dx12::CreateD3D12Device(IDXGIAdapter1* adapter) {
    ID3D12Device* device;

    HRESULT hr = D3D12CreateDevice(
        adapter,                 
        D3D_FEATURE_LEVEL_11_0,  
        IID_PPV_ARGS(&device)    
    );

    if (FAILED(hr)) {
        
        hr = D3D12CreateDevice(
            nullptr,  
            D3D_FEATURE_LEVEL_11_0,
            IID_PPV_ARGS(&device));

        if (FAILED(hr)) {
            OutputDebugString("Failed to create D3D12 Device\n");
            return nullptr;
        }

        OutputDebugString("Using software adapter (WARP)\n");
    }

    return device;
}