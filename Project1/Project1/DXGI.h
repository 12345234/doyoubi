#pragma once
#include <d3d12.h>
#include<dxgi1_4.h>
class DXGI
{
public:
	DXGI() = default;

	~DXGI();

	[[nodiscard]] bool setdisplayAdapter() noexcept;


	[[nodiscard]] IDXGIFactory4* factory() const noexcept;

	[[nodiscard]] IDXGIAdapter1* displayAdapter() const noexcept;

private:
	IDXGIFactory4* dxgiFactory{}; 
	IDXGIAdapter1* dxgiAdapter{};
};

