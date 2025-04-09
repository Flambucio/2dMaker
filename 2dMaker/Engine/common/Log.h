#pragma once

#ifndef STR_INCLUDED
#include <string>
#define STR_INCLUDED
#endif

#ifdef _DEBUG
#define TRACE(x) std::cout << "[TRACE] " << x << std::endl;
#else
#define TRACE(x)
#endif

#define ERROR(x) std::cout << "[ERROR] " << x << std::endl;