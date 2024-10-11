#pragma once



class IElfObjectBuilder
{
public:
    virtual void reset() = 0;
    virtual void buildElfStructureInfo() = 0;
    // virtual void resetSections() = 0;
    // virtual void resetPrograms() = 0;

    virtual ~IElfObjectBuilder() = default;
};
