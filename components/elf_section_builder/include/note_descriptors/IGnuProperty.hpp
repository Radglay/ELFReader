#pragma once

struct ElfField;
class IGnuPropertyVisitor;


class IGnuProperty
{
public:
    virtual ElfField acceptGnuPropertyVisitor(IGnuPropertyVisitor&) = 0;
    virtual ~IGnuProperty() = default;
};
