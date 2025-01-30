#pragma once

#include <string>
#include <istream>
#include "INoteSection.hpp"
#include <memory>
#include <elf.h>


template <typename SectionHeader, typename NoteHeader>
std::shared_ptr<INoteSection<SectionHeader, NoteHeader>> createNoteSection(std::istream*, int, std::shared_ptr<SectionHeader>, NoteHeader&, std::string);
