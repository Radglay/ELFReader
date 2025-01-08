#pragma once

#include "IElfSection.hpp"
#include <vector>
#include <memory>
#include "ElfPart.hpp"
#include "IElfPartAssembler.hpp"


template <typename SectionHeader, typename NoteHeader>
class NoteSection : public IElfSection<SectionHeader>
{
public:
    NoteSection(std::shared_ptr<SectionHeader> p_sectionHeader, NoteHeader p_noteHeader,
                std::string p_namespace, std::vector<unsigned char> p_description)
        : m_sectionHeader { p_sectionHeader }
        , m_noteHeader { p_noteHeader }
        , m_namespace { p_namespace }
        , m_description { p_description }
    {}

    std::shared_ptr<SectionHeader> getSectionHeader() const override
    {
        return m_sectionHeader;
    }

    ElfPart acceptElfPartAssembler(IElfPartAssembler& p_visitor, const std::string& p_sectionName) override
    {
        // return p_visitor.assembleElfPartFromSection(*this);
    }

    NoteHeader getNoteHeader() const
    {
        return m_noteHeader;
    }

    std::string getNamespace() const
    {
        return m_namespace;
    }

private:
    std::shared_ptr<SectionHeader> m_sectionHeader;
    NoteHeader m_noteHeader;
    std::string m_namespace;
    std::vector<unsigned char> m_description { nullptr };
};
