#pragma once

#include <cstdint>


int getHostEndianness();

bool isEndiannessCorrect(int p_endianness);

bool shouldConvertEndianness(int p_fileEndianness, int p_hostEndianness);

uint16_t convertEndianness(uint16_t p_toConvert);

uint32_t convertEndianness(uint32_t p_toConvert);

uint64_t convertEndianness(uint64_t p_toConvert);

int64_t convertEndianness(int64_t p_toConvert);
