#ifndef __LOGGING_H__
#define __LOGGING_H__

void Log_Error(const char* file, int line, const char* function);

#ifdef _MSC_BUILD // visual c++ compiler
#define __DYNAMIC_FUNC_NAME__ __FUNCSIG__
#elif __GNUC__
#define __DYNAMIC_FUNC_NAME__ __PRETTY_FUNCTION__
#elif 
#define __DYNAMIC_FUNC_NAME__ __func__
#endif

#define LOG_ERROR Log_Error(__FILE__, __LINE__, __DYNAMIC_FUNC_NAME__)

#endif