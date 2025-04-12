#pragma once

#ifndef STR_INCLUDED
#include <string>
#define STR_INCLUDED
#endif

#ifdef _DEBUG
#define TRACE(x) std::cout << "[TRACE]: " << x << std::endl;
#define WARN(x) std::cout << "[WARN]: " << x << std::endl;
#else
#define TRACE(x)
#define WARN(x)
#endif

#define ERROR(x) std::cout << "[ERROR]: " << x << std::endl;