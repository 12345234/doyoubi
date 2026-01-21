#pragma once

#include"DXGI.h"
#include<d3d12.h>
#include<wrl/client.h>
class device
{
public:
	device() = default;

	~device() = default;
	static device& instance()
	{
		static device instance;
		return instance;
	}

	[[nodiscard]] bool create() noexcept;

	[[nodiscard]] ID3D12Device* get() const noexcept;

	[[nodiscard]] const DXGI& dxgi()const noexcept;
private:
	
	device(const device& i) = delete;
	device& operator=(const device& r) = delete;
	device(device&& r) = delete;
	device& operator=(device&& r) = delete;


private:
	DXGI                                 dxgiInstance;
	Microsoft::WRL::ComPtr<ID3D12Device> device_;
};

