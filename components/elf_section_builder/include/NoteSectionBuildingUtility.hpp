#pragma once

#include <string>
#include <istream>
#include <memory>
#include <vector>


struct AbiTagInformation;
struct GnuPropertyX86Features;
struct GnuPropertyX86InstructionSet;
struct IGnuProperty;
struct TargetMachineInfo;


AbiTagInformation readAbiTagInformation(std::istream* p_fileStream, int p_targetMachineEndianness, int p_offset);

std::vector<unsigned char> readBuildIdInformation(std::istream* p_fileStream, int p_targetMachineEndianness, int p_offset, int p_size);

GnuPropertyX86Features readGnuPropertyX86Features(std::istream* p_fileStream, int p_targetMachineEndianness, int p_offset);

GnuPropertyX86InstructionSet readGnuPropertyX86InstructionSet(std::istream* p_fileStream, int p_targetMachineEndianness, int p_offset);

std::vector<std::shared_ptr<IGnuProperty>> readGnuPropertyInformation(std::istream* p_fileStream, TargetMachineInfo& p_targetMachineInfo, int p_offset, int p_size);
