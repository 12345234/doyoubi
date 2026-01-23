#include "DXGI.h"
#include <cassert>

#pragma comment(lib, "dxgi.lib")

DXGI::~DXGI() {
    if (dxgifactory) {
        dxgifactory->Release();
        dxgifactory = nullptr;
    }
    if (dxgiadapter) {
        dxgiadapter->Release();
        dxgiadapter = nullptr;
    }
}

[[nodiscard]] bool DXGI::setDisplayAdapter() noexcept {
#if _DEBUG
    ID3D12Debug* debug;
    if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debug)))) {
        debug->EnableDebugLayer();
    }
#endif
    {
        UINT createFactoryFlags = 0;
#if _DEBUG
        createFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
#endif
        const auto hr = CreateDXGIFactory2(createFactoryFlags, IID_PPV_ARGS(&dxgifactory));
        if (FAILED(hr)) {
            assert(false && "DXGIファクトリーの作成に失敗");
            return false;
        }
    }
    {
        auto           select = 0;
        IDXGIAdapter1* dxgiAdapter{};

        while (dxgifactory->EnumAdapters1(select, &dxgiAdapter) != DXGI_ERROR_NOT_FOUND) {

            DXGI_ADAPTER_DESC1 desc{};
            dxgiAdapter->GetDesc1(&desc);

            select++;

            if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) {
                dxgiAdapter->Release();
                continue;
            }
            if (FAILED(D3D12CreateDevice(dxgiAdapter, D3D_FEATURE_LEVEL_11_0, _uuidof(ID3D12Device), nullptr))) {
                dxgiAdapter->Release();
                continue;
            }

            dxgiadapter = dxgiAdapter;
            break;
        }

        if (!dxgiadapter) {
            assert(false && "アダプタの取得に失敗");
            return false;
        }
    }

    return true;
}

[[nodiscard]] IDXGIFactory4* DXGI::factory() const noexcept {
    if (!dxgifactory) {
        assert(false && "DXGIファクトリーが未作成です");
    }

    return dxgifactory;
}

[[nodiscard]] IDXGIAdapter1* DXGI::displayAdapter() const noexcept {
    if (!dxgiadapter) {
        assert(false && "ディスプレイアダプターが未作成です");
    }

    return dxgiadapter;
}
