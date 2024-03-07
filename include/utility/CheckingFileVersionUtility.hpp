#pragma once

#include <fstream>


namespace parser
{

bool is32BitVersion(std::ifstream& p_fileStream);
bool is64BitVersion(std::ifstream& p_fileStream);

}