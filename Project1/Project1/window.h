#pragma once

#include<Windows.h>
#include<string>
using namespace std;
class window
{
public:
	static window& instance() noexcept {
		static window instance;
		return instance;
	}

	[[nodiscard]] HRESULT create(HINSTANCE instance, int width, int height, std::string_view name) noexcept;

	[[nodiscard]] bool messageLoop() const noexcept;

	[[nodiscard]] HWND handle() const noexcept;

	[[nodiscard]] pair<int, int> size() const noexcept;

private:
	window() = default;

	~window() = default;

	window(window& r) = delete;
	window& operator=(window& r) = delete;
	window(window&& r) = delete;
	window& operator=(window&& r) =delete;

	HWND handle_{};  
	int  witdh_{};   
	int  height_{};
};