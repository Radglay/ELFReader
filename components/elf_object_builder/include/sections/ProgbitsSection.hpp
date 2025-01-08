#pragma once

#include "IElfSection.hpp"
#include <vector>
#include <memory>
#include "ElfPart.hpp"
#include "IElfPartAssembler.hpp"


template <typename SectionHeader>
class ProgbitsSection : public IElfSection<SectionHeader>
{
public:
    ProgbitsSection(std::shared_ptr<SectionHeader> p_sectionHeader, std::vector<unsigned char>& p_bytes)
        : m_sectionHeader { p_sectionHeader }
        , m_bytes { p_bytes }
    {}

    ElfPart acceptElfPartAssembler(IElfPartAssembler& p_visitor, const std::string& p_sectionName) override
    {
        // return p_visitor.assembleElfPartFromSection(*this);
    }

    std::shared_ptr<SectionHeader> getSectionHeader() const override
    {
        return m_sectionHeader;
    }

    std::vector<unsigned char> getBytes() const
    {
        return m_bytes;
    }

private:
    std::shared_ptr<SectionHeader> m_sectionHeader;
    std::vector<unsigned char> m_bytes;
};
