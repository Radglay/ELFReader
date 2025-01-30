#pragma once

#include "INoteSection.hpp"
#include <memory>
#include "ElfPart.hpp"
#include "IElfPartFromSectionVisitor.hpp"


template <typename SectionHeader, typename NoteHeader, typename Descriptor>
class NoteSection : public INoteSection<SectionHeader, NoteHeader>
{
public:
    NoteSection(std::shared_ptr<SectionHeader> p_sectionHeader, NoteHeader p_noteHeader,
                std::string p_namespace, Descriptor p_descriptor)
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

    Descriptor getDescriptor() const
    {
        return m_descriptor;
    }

private:
    std::shared_ptr<SectionHeader> m_sectionHeader;
    NoteHeader m_noteHeader;
    std::string m_namespace;
    Descriptor m_descriptor;
};
