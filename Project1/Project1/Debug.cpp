void Dx12::EnableDebugLayer() {
#if defined(_DEBUG)
    ID3D12Debug* debugController;
    if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)))) {
       
        debugController->EnableDebugLayer();

        ID3D12Debug1* debugController1;
        if (SUCCEEDED(debugController->QueryInterface(IID_PPV_ARGS(&debugController1)))) {
            debugController1->SetEnableGPUBasedValidation(TRUE);
        }
    }
#endif
}