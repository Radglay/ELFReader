#pragma once


struct TargetMachineInfo
{
    unsigned char magicNumber[4];
    unsigned char bitVersion;
    unsigned char endianness;
    unsigned char elfVersion;
    unsigned char abi;
    unsigned char abiVersion;
    unsigned char padding[7];
};
