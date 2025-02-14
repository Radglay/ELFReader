#pragma once

#include "IElfSection.hpp"
#include <memory>
#include "ElfPart.hpp"
#include "IElfPartFromSectionVisitor.hpp"


template <typename SectionHeader>
class NullSection : public IElfSection<SectionHeader>
{
public:
    NullSection(std::shared_ptr<SectionHeader> p_sectionHeader)
        : m_sectionHeader { p_sectionHeader }
    {}

    ElfPart acceptElfPartAssembler(IElfPartFromSectionVisitor& p_visitor, const std::string& p_sectionName) override
    {
        return p_visitor.assembleElfPartFromSection(*this, p_sectionName);
    }

    std::shared_ptr<SectionHeader> getSectionHeader() const override
    {
        return m_sectionHeader;
    }

private:
    std::shared_ptr<SectionHeader> m_sectionHeader;
};
