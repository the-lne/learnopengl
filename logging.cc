#include <iostream>
#include "logging.h"

#ifdef LOGGING
static int error_iterator = 0;

void Log_Error(const char* file, int line, const char* function) 
{
    std::cout << "Error " << error_iterator << " Begin!" << std::endl;
    std::cout << "\tFile: " << file << std::endl;
    std::cout << "\tLine: " << line << std::endl;
    std::cout<< "\tFunction: " << function << std::endl;
    std::cout << "Error " << error_iterator << " End!" << std::endl;
    error_iterator++;
}
#else
void Log_Error(const char* file, int line, const char* function) { return; }
#endif