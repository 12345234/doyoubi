#pragma once

#include <Windows.h>
#include <cstdint>
class input {
public:

    static input& instance() noexcept {
        static input instance;
        return instance;
    }
    [[nodiscard]] bool getkey(uint16_t sKey) const noexcept;

    [[nodiscard]] bool getTrigger(uint16_t sKey) const noexcept;

    void updateKeystate(void* pState) noexcept;


private:
    input() = default;

    ~input() = default;

    byte keystate[256];
};
