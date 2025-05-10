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


#define PRINT_ARRAY(x) std::string t="";\
for (int j=0;j<x.size();j++)\
 {\
 t+=std::to_string(x[j])+"|";\
 }TRACE(t);

#define PRINT_2D_ARRAY(x) for(int i=0;i<x.size();i++){\
	PRINT_ARRAY(x[i]);\
}

#define PRINT_ARRAY_STR(x) std::string t="";\
for(int j=0;j<x.size();j++)\
{  t+=x[j]+"|";\
}TRACE(t);

#define PRINT_2D_ARRAY_STR(x) for(int i=0;i<x.size();i++){\
	PRINT_ARRAY_STR(x[i]);\
}

#define PRINT_U_SET_STR(x) std::string t="";\
for(auto y : x){t+=y+"|";}TRACE(t);
