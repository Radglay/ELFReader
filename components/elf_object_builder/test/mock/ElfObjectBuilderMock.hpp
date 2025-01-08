#include <gmock/gmock.h>
#include "IElfObjectBuilder.hpp"
#include "ElfObjectTraits.hpp"
#include "ElfStructureInfoTraits.hpp"


template <typename T, typename U, typename ElfStructureInfoTraits = elf_structure_info_traits<U>, typename ElfObjectTraits = elf_object_traits<T>>
class ElfObjectBuilderMock : public IElfObjectBuilder<T, U, ElfStructureInfoTraits, ElfObjectTraits>
{
public:
    MOCK_METHOD(void, reset, (), (override));
    MOCK_METHOD(void, buildElfStructureInfo, (), (override));
    MOCK_METHOD(void, buildSymbolSections, (), (override));
    MOCK_METHOD(void, buildNoteSections, (), (override));
    MOCK_METHOD(void, buildRelocationSections, (), (override));
    MOCK_METHOD(void, buildRelocationWithAddendSections, (), (override));
    MOCK_METHOD(void, buildStringTableSections, (), (override));
    MOCK_METHOD(void, buildProgbitsSections, (), (override));
    MOCK_METHOD(void, buildNobitsSections, (), (override));
    MOCK_METHOD(T*, getResult, (), (override));
};
