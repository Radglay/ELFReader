#pragma once

#include "IElfSection.hpp"
#include <vector>
#include <memory>
#include "ElfPart.hpp"
#include "IElfPartFromSectionVisitor.hpp"



template <typename SectionHeader, typename Relocation>
class RelocationSection : public IElfSection<SectionHeader>
{
public:
    RelocationSection(std::shared_ptr<SectionHeader> p_sectionHeader, std::vector<Relocation> p_relocationHeaders)
        : m_sectionHeader { p_sectionHeader }
        , m_relocationHeaders { p_relocationHeaders }
    {}

    ElfPart acceptElfPartAssembler(IElfPartFromSectionVisitor& p_visitor, const std::string& p_sectionName) override
    {
        // return p_visitor.assembleElfPartFromSection(*this);
    }

    std::shared_ptr<SectionHeader> getSectionHeader() const override
    {
        return m_sectionHeader;
    }

    std::vector<Relocation> getRelocationHeaders() const
    {
        return m_relocationHeaders;
    }

private:
    std::shared_ptr<SectionHeader> m_sectionHeader;
    std::vector<Relocation> m_relocationHeaders;
};
