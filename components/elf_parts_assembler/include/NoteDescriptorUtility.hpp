#pragma once

#include <vector>
#include <memory>


struct ElfField;
struct AbiTagInformation;
struct IGnuProperty;


ElfField assembleElfPartFromDescriptor(const AbiTagInformation&);
ElfField assembleElfPartFromDescriptor(const std::vector<unsigned char>&);
ElfField assembleElfPartFromDescriptor(const std::vector<std::shared_ptr<IGnuProperty>>&);
