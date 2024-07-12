#pragma once

#include "ElfHeaderReaderException.hpp"


namespace reader
{

class WrongEndiannessException : public ElfHeaderReaderException
{};

}
