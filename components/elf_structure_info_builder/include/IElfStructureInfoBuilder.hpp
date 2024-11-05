#pragma once


template <typename T, typename Traits>
class IElfStructureInfoBuilder
{
public:
    virtual void reset() = 0;
    virtual void buildFileHeader() = 0;
    virtual void buildSectionHeaders() = 0;
    virtual void buildProgramHeaders() = 0;
    virtual T* getResult() = 0;

    virtual ~IElfStructureInfoBuilder() = default;
};
