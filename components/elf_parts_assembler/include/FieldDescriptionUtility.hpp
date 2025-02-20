#pragma once

#include <QStringList>
#include <cstdint>
#include <vector>


class QString;


QString getBinaryNumberString(uint64_t p_number, int p_length);
QString getHexNumberString(uint64_t p_number);
QString getDecimalNumberString(uint64_t p_number);
QString getBooleanString(bool p_flag);
QString getBytesString(const std::vector<unsigned char>& p_bytes);

QString getBitVersionHighLevelValue(int p_ident);
QString getEndiannessHighLevelValue(int p_endianness);
QString getELFVersionHighLevelValue(int p_version);
QString getABIHighLevelValue(int p_abi);
QString getABIVersionHighLevelValue(int p_version);
QString getObjectTypeHighLevelValue(int p_type);
QString getInstructionSetArchitectureHighLevelValue(int p_architecture);


QString getSegmentTypeHighLevelValue(int p_type);
QString getSegmentFlagsHighLevelValues(uint32_t p_flags);


QString getSectionTypeHighLevelValue(int p_type);
QString getSectionAtrributesHighLevelValues(uint64_t p_attributes);
QString getSectionInfoHighLevelValue(uint32_t p_sectionType, uint32_t p_info);
QString getSectionLinkHighLevelValue(uint32_t p_sectionType, uint32_t p_link);
