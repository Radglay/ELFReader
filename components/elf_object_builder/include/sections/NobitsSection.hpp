#pragma once

#include "IElfSection.hpp"
#include <vector>
#include <memory>
#include "ElfPart.hpp"
#include "IElfPartAssembler.hpp"


template <typename SectionHeader>
class NobitsSection : public IElfSection<SectionHeader>
{
public:
    NobitsSection(std::shared_ptr<SectionHeader> p_sectionHeader)
        : m_sectionHeader { p_sectionHeader }
    {}

    ElfPart acceptElfPartAssembler(IElfPartAssembler& p_visitor, const std::string& p_sectionName) override
    {
        // return p_visitor.assembleElfPartFromSection(*this);
    }

    std::shared_ptr<SectionHeader> getSectionHeader() const override
    {
        return m_sectionHeader;
    }

private:
    std::shared_ptr<SectionHeader> m_sectionHeader;
};
