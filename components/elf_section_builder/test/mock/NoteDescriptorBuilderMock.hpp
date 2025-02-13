#pragma once

#include <gmock/gmock.h>
#include "INoteDescriptorBuilder.hpp"
#include "AbiTagInformation.hpp"
#include "IGnuProperty.hpp"


class NoteDescriptorBuilderMock : public INoteDescriptorBuilder
{
public:
    MOCK_METHOD(AbiTagInformation, buildAbiTagInformation, (int), (override));
    MOCK_METHOD(std::vector<unsigned char>, buildBuildIdInformation, (int, int), (override));
    MOCK_METHOD(std::vector<std::shared_ptr<IGnuProperty>>, buildGnuPropertyInformation, (int, int),  (override));
};
