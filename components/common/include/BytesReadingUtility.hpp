#pragma once

#include <istream>
#include <cstring>


template <typename T>
void readBytesFromFile(T& p_toFill, int p_offset, std::istream* p_fileStream)
{
    p_fileStream->seekg(p_offset);
    unsigned char l_buffer[sizeof(T)];
    p_fileStream->read(reinterpret_cast<char*>(l_buffer), sizeof(T));
    std::memcpy(&p_toFill, l_buffer, sizeof(T));
}
