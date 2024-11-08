#pragma once

#include <cstdint>


int getHostEndianness();

bool isEndiannessCorrect(int p_endianness);

bool shouldConvertEndianness(int p_fileEndianness);

void convertEndianness(uint16_t& p_toConvert);

void convertEndianness(uint32_t& p_toConvert);

void convertEndianness(uint64_t& p_toConvert);

void convertEndianness(int32_t& p_toConvert);

void convertEndianness(int64_t& p_toConvert);

