#pragma once

#include <istream>


namespace reader
{

bool is32BitVersion(std::istream& p_fileStream);
bool is64BitVersion(std::istream& p_fileStream);

}