#pragma once

#include "ElfStructureInfoTraits.hpp"


template <typename T, typename U, typename Traits = ElfStructureInfoTraits<U>>
class IElfObjectBuilder
{
public:
    virtual void reset() = 0;
    virtual void buildElfStructureInfo() = 0;
    // virtual void resetSections() = 0;
    // virtual void resetPrograms() = 0;

    virtual ~IElfObjectBuilder() = default;
};
