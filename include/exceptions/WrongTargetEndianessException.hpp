#pragma once

#include "ElfFileParserException.hpp"


namespace parser
{

class WrongTargetEndianessException : public ElfFileParserException
{};

}
