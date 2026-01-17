#pragma once

#include"DXGI.h"
#include<d3d12.h>
#include<wrl/client.h>
class device
{
public:
	static device& instance()
	{
		static device instance;
		return instance;
	}

	device() = default;

	~device() = default;

	[[nodiscard]] bool create() noexcept;

	[[nodiscard]] ID3D12Device* get() const noexcept;

private:
	DXGI   dxgiInstance;
	Microsoft::WRL::ComPtr<ID3D12Device> device_;
};

