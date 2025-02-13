#pragma once

#include <memory>
#include <vector>


struct AbiTagInformation;
struct IGnuProperty;


class INoteDescriptorBuilder
{
public:
    virtual AbiTagInformation buildAbiTagInformation(int p_offset) = 0;
    virtual std::vector<unsigned char> buildBuildIdInformation(int p_offset, int p_size) = 0;
    virtual std::vector<std::shared_ptr<IGnuProperty>> buildGnuPropertyInformation(int p_offset, int p_size) = 0;

    virtual ~INoteDescriptorBuilder() = default;
};

