#pragma once

#include "IElfSection.hpp"
#include <vector>
#include <memory>


template <typename SectionHeader, typename NoteHeader>
class NoteSection : public IElfSection<SectionHeader>
{
public:
    NoteSection(std::shared_ptr<SectionHeader> p_sectionHeader, NoteHeader p_noteHeader)
        : m_sectionHeader { p_sectionHeader }
        , m_noteHeader { p_noteHeader }
    {}

    std::shared_ptr<SectionHeader> getSectionHeader() const
    {
        return m_sectionHeader;
    }

    NoteHeader getNoteHeader() const
    {
        return m_noteHeader;
    }

private:
    std::shared_ptr<SectionHeader> m_sectionHeader;
    NoteHeader m_noteHeader;
};
