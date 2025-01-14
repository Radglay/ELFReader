#pragma once

#include <QString>
#include <vector>


struct ElfField
{
    ElfField() = default;

    ElfField(QString p_name, QString p_type, QString p_value, QString p_description, std::vector<ElfField> p_fields = std::vector<ElfField>())
        : name { p_name }
        , type { p_type }
        , value { p_value }
        , description { p_description }
        , fields { p_fields }
    {}

    QString name;
    QString type;
    QString value;
    QString description;
    std::vector<ElfField> fields;
};

enum class ElfPartType
{
    FileHeader,
    SectionHeader,
    ProgramHeader,
    Section
};

struct ElfPart
{
    ElfPart() = default;

    ElfPart(QString p_name, ElfPartType p_elfPartType, int p_offset, int p_size, QString p_description = "", std::vector<ElfField> p_fields = std::vector<ElfField>())
        : name { p_name }
        , type { p_elfPartType }
        , offset { p_offset }
        , size { p_size }
        , description { p_description }
        , fields { p_fields }
    {}

    QString name;
    ElfPartType type;
    int offset;
    int size;
    QString description;
    std::vector<ElfField> fields;
};


