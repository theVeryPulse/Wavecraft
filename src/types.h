#pragma once
#ifndef TYPES_H
#define TYPES_H

#include <cstdint>

using PCM8 = int8_t;
using PCM16 = int16_t; // Pulse-code modulation 16-bit: [0, 65535], [-32768, 32767]
using PCM32 = float; // Pulse-code modulation: float

using i8 = int8_t;

using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;

#endif /* TYPES_H */