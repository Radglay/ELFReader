#pragma once

#include <gmock/gmock.h>
#include "IGnuProperty.hpp"


class GnuPropertyMock : public IGnuProperty
{
public:
    MOCK_METHOD(ElfField, acceptGnuPropertyVisitor, (IGnuPropertyVisitor&), (override));
};
