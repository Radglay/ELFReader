#pragma once

#include <vector>
#include <iostream>


class ElfPart;


std::vector<ElfPart> readElfPartsFromFile(std::istream*);