#pragma once

#include <memory>
#include <string>
#include "ElfPart.hpp"


class IElfPartAssembler;
class ElfPart;

template <typename SectionHeader>
class IElfSection
{
public:
    virtual std::shared_ptr<SectionHeader> getSectionHeader() const = 0;
    virtual ElfPart acceptElfPartAssembler(IElfPartAssembler&, const std::string&) = 0;
    virtual ~IElfSection() = default;
};
