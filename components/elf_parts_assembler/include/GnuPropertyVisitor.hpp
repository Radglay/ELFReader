#pragma once

#include "IGnuPropertyVisitor.hpp"


class GnuPropertyVisitor : public IGnuPropertyVisitor
{
public:
    ElfField assembleElfFieldFromGnuProperty(const GnuPropertyX86Features&) override;
    ElfField assembleElfFieldFromGnuProperty(const GnuPropertyX86InstructionSet&) override;
};
