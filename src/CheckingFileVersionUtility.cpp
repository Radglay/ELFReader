#include <fstream>


namespace
{
    int getBitVersionFlag(std::istream& p_fileStream)
    {
        p_fileStream.seekg(4);
        int l_systemVersion{ p_fileStream.peek() };
        p_fileStream.seekg(0);

        return l_systemVersion;
    }
}

namespace parser
{

bool is32BitVersion(std::istream& p_fileStream)
{
    return getBitVersionFlag(p_fileStream) == 1;
}

bool is64BitVersion(std::istream& p_fileStream)
{
    return getBitVersionFlag(p_fileStream) == 2;
}


}