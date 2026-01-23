#include "input.h"

[[nodiscard]] bool input::getkey(uint16_t sKey) const noexcept {
    return (keystate[sKey] & 0x80) != 0;
}

void input::updateKeystate(void* pState) noexcept {
    memcpy_s(keystate, sizeof(keystate), pState, sizeof(keystate));
}