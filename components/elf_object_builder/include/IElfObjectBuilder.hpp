#pragma once


template <typename T, typename U, typename ElfStructureInfoTraits, typename ElfObjectTraits>
class IElfObjectBuilder
{
public:
    virtual void reset() = 0;
    virtual void buildElfStructureInfo() = 0;
    virtual void buildSymbolSections() = 0;
    virtual void buildNoteSections() = 0;
    virtual void buildRelocationSections() = 0;
    virtual void buildRelocationWithAddendSections() = 0;
    virtual T* getResult() = 0;

    virtual ~IElfObjectBuilder() = default;
};
