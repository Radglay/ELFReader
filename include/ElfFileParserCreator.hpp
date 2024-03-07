#pragma once


namespace parser
{

class IElfFileParser;


class ElfFileParserCreator
{
public:
    static IElfFileParser* createElfFileParser(const char*);
};

}