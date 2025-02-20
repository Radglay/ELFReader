#pragma once

struct ElfField;
struct GnuPropertyX86Features;
struct GnuPropertyX86InstructionSet;


class IGnuPropertyVisitor
{
public:
    virtual ElfField assembleElfFieldFromGnuProperty(const GnuPropertyX86Features&) = 0;
    virtual ElfField assembleElfFieldFromGnuProperty(const GnuPropertyX86InstructionSet&) = 0;

    virtual ~IGnuPropertyVisitor() = default;
};
