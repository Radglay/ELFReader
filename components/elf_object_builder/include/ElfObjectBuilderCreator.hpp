#pragma once

#include "IElfObjectBuilder.hpp"
#include <istream>
#include <memory>


std::unique_ptr<IElfObjectBuilder> createElfObjectBuilder(std::istream* p_fileStream);
