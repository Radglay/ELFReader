#include <gmock/gmock.h>
#include "IElfStructureInfoBuilder.hpp"
#include "ElfStructureInfoTraits.hpp"


template <typename T, typename Traits = elf_structure_info_traits<T>>
class ElfStructureInfoBuilderMock : public IElfStructureInfoBuilder<T, Traits>
{
public:
    MOCK_METHOD(void, reset, (), (override));
    MOCK_METHOD(void, buildFileHeader, (), (override));
    MOCK_METHOD(void, buildSectionHeaders, (), (override));
    MOCK_METHOD(void, buildProgramHeaders, (), (override));
    MOCK_METHOD(T*, getResult, (), (override));
};
