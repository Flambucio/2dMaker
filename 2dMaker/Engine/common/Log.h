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

#define PRINT_2D_ARRAY_STR_ADVANCED(x) for(int i=0;i<x.size();i++){\
std::string t="i:"+std::to_string(i);\
for(int j=0;j<x[i].size();j++){\
	t+="j:"+std::to_string(j)+" "+x[i][j]+" ";\
	}\
	TRACE(t);\
}

#define PRINT_U_SET_STR(x) std::string t="";\
for(auto y : x){t+=y+"|";}TRACE(t);

namespace D2Maker
{
	struct ConsoleShouldOpen
	{
	public:
		inline static bool value = false;
		inline static bool operator()()
		{
			return value;
		}
	};

	struct ConsoleOpen
	{
	public:
		inline static bool value = false;
		inline static bool operator()()
		{
			return value;
		}
	};

	struct ConsoleToRender
	{
	public:
		inline static std::vector<std::string> toRender = {};
		inline static uint32_t count = 0;
		static void Log(const std::string& logstr)
		{
			ConsoleToRender::toRender.push_back((std::to_string(ConsoleToRender::count) + ". " + logstr));
			ConsoleToRender::count++;
		}
	};
	#ifndef CONSOLELOG(x)
	#define CONSOLELOG(x) ConsoleToRender::Log(x);
	#endif
}
