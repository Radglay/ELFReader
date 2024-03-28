#include <plog/Log.h>
#include "plog/Initializers/RollingFileInitializer.h"

#include <ctime>
#include <string>


void initLogger()
{
    std::string fileName = "logs/" + std::to_string(std::time(nullptr)) + "_log.txt";
    plog::init(plog::debug, fileName.c_str(), 1, 5);
}