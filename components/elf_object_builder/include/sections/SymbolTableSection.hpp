#pragma once

#include "IElfSection.hpp"
#include <vector>
#include <memory>
#include "ElfPart.hpp"
#include "IElfPartFromSectionVisitor.hpp"


template <typename SectionHeader, typename SymbolHeader>
class SymbolTableSection : public IElfSection<SectionHeader>
{
public:
    SymbolTableSection(std::shared_ptr<SectionHeader> p_sectionHeader, std::vector<SymbolHeader> p_symbolHeaders)
        : m_sectionHeader { p_sectionHeader }
        , m_symbolHeaders { p_symbolHeaders }
    {}

    ElfPart acceptElfPartAssembler(IElfPartFromSectionVisitor& p_visitor, const std::string& p_sectionName) override
    {
        // return p_visitor.assembleElfPartFromSection(*this);
    }

    std::shared_ptr<SectionHeader> getSectionHeader() const override
    {
        return m_sectionHeader;
    }

    std::vector<SymbolHeader> getSymbolHeaders() const
    {
        return m_symbolHeaders;
    }

private:
    std::shared_ptr<SectionHeader> m_sectionHeader;
    std::vector<SymbolHeader> m_symbolHeaders;
};
