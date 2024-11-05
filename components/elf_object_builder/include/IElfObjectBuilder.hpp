#pragma once


template <typename T, typename U, typename ElfStructureInfoTraits, typename ElfObjectTraits>
class IElfObjectBuilder
{
public:
    virtual void reset() = 0;
    virtual void buildElfStructureInfo() = 0;
    virtual void buildSymbols() = 0;

    virtual ~IElfObjectBuilder() = default;
};
