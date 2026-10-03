#pragma once

#include "ElfObjectTraits.hpp"
#include "NullSection.hpp"
#include "SymbolTableSection.hpp"
#include "UnknownSection.hpp"
#include "RelocationSection.hpp"
#include "RelocationWithAddendSection.hpp"
#include "ProgbitsSection.hpp"
#include "NobitsSection.hpp"
#include "StringTableSection.hpp"
// #include "NoteSection.hpp"

#include <elf.h>
#include <variant>
#include <vector>


namespace elf::deserialization
{

// template <typename T>
// struct FileHeader
// {
//     static constexpr auto size = sizeof(T);

//     T value;
//     std::array<unsigned char, size> bytes;
// };


// template <typename T>
// struct ProgramHeader
// {
//     static constexpr auto size = sizeof(T);

//     T value;
//     std::array<unsigned char, size> bytes;
// };

// template <typename T>
// struct SectionHeader
// {
//     static constexpr auto size = sizeof(T);

//     T value;
//     std::array<unsigned char, size> bytes;
// };


// template <typename T, typename Traits = elf_object_traits<T>>
// struct basic_elf_object
// {
//     FileHeader<typename Traits::file_header_type> fileHeader;
//     std::vector<SectionHeader<typename Traits::section_header_type>> sectionHeaders;
//     std::vector<ProgramHeader<typename Traits::program_header_type>> programHeaders;
//     // deserialize ONLY value, do not use raw bytes...

//     std::vector<ElfSection<T>> sections;
// };

template <typename T>
using ElfSection = std::variant<NullSection<T>,
                                SymbolTableSection<T>,
                                UnknownSection<T>,
                                RelocationSection<T>,
                                RelocationWithAddendSection<T>,
                                ProgbitsSection<T>,
                                NobitsSection<T>,
                                StringTableSection<T>
                                >;

template <typename T, typename Traits = elf_object_traits<T>>
struct basic_elf_object
{
    typename Traits::file_header_type fileHeader;
    std::vector<typename Traits::section_header_type> sectionHeaders;
    std::vector<typename Traits::program_header_type> programHeaders;
    // deserialize ONLY value, do not use raw bytes...

    std::vector<ElfSection<T>> sections;
};

using ElfObjectX32_t = basic_elf_object<elf32_tag>;
using ElfObjectX64_t = basic_elf_object<elf64_tag>;

}
