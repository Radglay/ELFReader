#pragma once

#include "ElfFileParserException.hpp"


namespace reader
{

class WrongTargetEndianessException : public ElfFileParserException
{};

}
