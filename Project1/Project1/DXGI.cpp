#include "DXGI.h"
#include <cassert>

#pragma comment(lib, "dxgi.lib")


[[nodiscard]] bool DXGI::setdisplayAdapter() noexcept {
    if (_DEBUG)
    {
        ID3D12Debug* debug;
        if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debug)))) {
            debug->EnableDebugLayer();
        }
    }

    {
        UINT createFactoryFlags = 0;
        if (_DEBUG)
        {
            createFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
        }
        const auto hr = CreateDXGIFactory2(createFactoryFlags, IID_PPV_ARGS(&dxgiFactory));
        if (FAILED(hr)) {
            assert(false && "DXGIファクトリーの作成に失敗");
            return false;
        }
    }
    {
        
        auto           select = 0;
        Microsoft::WRL::ComPtr<IDXGIAdapter1> dxgiAdapterr;

        
        while (dxgiFactory->EnumAdapters1(select, &dxgiAdapterr) != DXGI_ERROR_NOT_FOUND) {

            DXGI_ADAPTER_DESC1 desc{};
            dxgiAdapterr->GetDesc1(&desc);

            select++;

            
            if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) {
                dxgiAdapterr->Release();
                continue;
            }

            
            if (FAILED(D3D12CreateDevice(dxgiAdapterr.Get(), D3D_FEATURE_LEVEL_11_0, _uuidof(ID3D12Device), nullptr))) {
                dxgiAdapterr->Release();
                continue;
            }

            dxgiAdapter_ = dxgiAdapterr;
            break;
        }

        if (!dxgiAdapter_) {
            assert(false && "アダプタの取得に失敗");
            return false;
        }
    }

    return true;
}
[[nodiscard]] IDXGIFactory4* DXGI::factory() const noexcept {
    if (!dxgiFactory) {
        assert(false && "DXGIファクトリーが未作成です");
    }

    return dxgiFactory.Get();
}


[[nodiscard]] IDXGIAdapter1* DXGI::displayAdapter() const noexcept {
    if (!dxgiAdapter_) {
        assert(false && "ディスプレイアダプターが未作成です");
    }

    return dxgiAdapter_.Get();
}
