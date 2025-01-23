#pragma once

#include "IElfSection.hpp"
#include <vector>
#include <memory>
#include "ElfPart.hpp"
#include "IElfPartFromSectionVisitor.hpp"


template <typename SectionHeader, typename NoteHeader>
class NoteSection : public IElfSection<SectionHeader>
{
public:
    NoteSection(std::shared_ptr<SectionHeader> p_sectionHeader, NoteHeader p_noteHeader,
                std::string p_namespace, std::vector<unsigned char> p_descriptor)
        : m_sectionHeader { p_sectionHeader }
        , m_noteHeader { p_noteHeader }
        , m_namespace { p_namespace }
        , m_descriptor { p_descriptor }
    {}

    std::shared_ptr<SectionHeader> getSectionHeader() const override
    {
        return m_sectionHeader;
    }

    ElfPart acceptElfPartAssembler(IElfPartFromSectionVisitor& p_visitor, const std::string& p_sectionName) override
    {
        return p_visitor.assembleElfPartFromSection(*this, p_sectionName);
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
    std::vector<unsigned char> m_descriptor { nullptr };
};
