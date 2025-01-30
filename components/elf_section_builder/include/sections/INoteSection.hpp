#pragma once

#include "IElfSection.hpp"


template <typename SectionHeader, typename NoteHeader>
class INoteSection : public IElfSection<SectionHeader>
{
public:
    virtual NoteHeader getNoteHeader() const = 0;
    virtual std::string getNamespace() const = 0;
};
