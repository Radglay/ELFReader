#pragma once

#include <istream>
#include <cstring>
#include <string>
#include <vector>


template <typename T>
void readBytesFromFile(T& p_toFill, int p_offset, std::istream* p_fileStream)
{
    p_fileStream->seekg(p_offset);
    unsigned char l_buffer[sizeof(T)];
    p_fileStream->read(reinterpret_cast<char*>(l_buffer), sizeof(T));
    std::memcpy(&p_toFill, l_buffer, sizeof(T));
}

inline void readNullTerminatedStringFromFile(std::string& p_name, int p_offset, std::istream* p_fileStream)
{
    p_fileStream->seekg(p_offset);
    char l_buffer[100] {};
    p_fileStream->getline(l_buffer, 100, '\0');

    p_name = std::string { l_buffer };
}

inline void readBytesFromFileToVector(std::vector<unsigned char>& p_toFill, int p_offset, int p_size, std::istream* p_fileStream)
{
    p_fileStream->seekg(p_offset);
    unsigned char l_buffer[p_size];
    p_fileStream->read(reinterpret_cast<char*>(l_buffer), p_size);
    p_toFill.assign(&l_buffer[0], &l_buffer[p_size]);
}


inline int alignOffset(int p_value, int p_alignment)
{
    auto l_remainder { p_value % p_alignment };
    return l_remainder ? p_value + (p_alignment - l_remainder) : p_value;
}
