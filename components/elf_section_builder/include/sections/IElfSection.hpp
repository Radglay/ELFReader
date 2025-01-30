#pragma once

#include <memory>
#include <string>


class IElfPartFromSectionVisitor;
class ElfPart;

template <typename SectionHeader>
class IElfSection
{
public:
    virtual std::shared_ptr<SectionHeader> getSectionHeader() const = 0;
    virtual ElfPart acceptElfPartAssembler(IElfPartFromSectionVisitor&, const std::string&) = 0;
    virtual ~IElfSection() = default;
};
