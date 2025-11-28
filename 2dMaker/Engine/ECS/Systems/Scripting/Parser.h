#pragma once
#include "../../../common/Core.h"
namespace D2Maker
{




    static class Parser
    {
    private:
        static std::string ReadString(std::string filepath)
        {
            std::ifstream fs(filepath);
            std::stringstream content;
            content << fs.rdbuf();
            return content.str();

        }


        static std::vector<std::vector<std::string>> TokenizeString(std::string content)
        {
            std::vector<std::vector<std::string>> parsedStr;
            std::string token;
            parsedStr.push_back({});

            for (char c : content)
            {
                if (c == ' ')
                {
                    if (!token.empty()) {
                        parsedStr.back().push_back(token);
                    }
                    token.clear();
                }
                else if (c == ';')
                {
                    if (!token.empty()) {
                        parsedStr.back().push_back(token);

                    }
                    token.clear();
                    parsedStr.push_back({});
                }
                else if (c == '\n')
                {
                    continue;
                }
                else
                {
                    token += c;
                }
            }

            if (!token.empty()) {
                parsedStr.back().push_back(token);
            }

            return parsedStr;
        }




    public:
        static void ParseString(std::string filepath, std::vector<std::vector<std::string>> &parsedStr)
        {
            if (std::filesystem::exists(filepath) && std::filesystem::is_regular_file(filepath))
            {
                std::string content = ReadString(filepath);
                parsedStr = TokenizeString(content);

            }
            
        }

    };

}
