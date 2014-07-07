#include "TypeConver.h"
#include "moonSugar.h"
#include <cstring>

NS_MS_BEGIN
	char * TypeConver::ConstCharToChar(const char * constChar)
	{
		size_t len = strlen(constChar);
		char * newChar = new char[len + 1];
		strcpy(newChar, constChar);
		return newChar;
	}

	char * TypeConver::StringToChar(std::string * str)
	{ 
		return ConstCharToChar(str->c_str());
	}

	std::string * TypeConver::CharToString(char * charStr)
	{
		std::string * str = new std::string(charStr);
		return str;
	}

	char * TypeConver::combinationChar(char * char1, char * char2)
	{
		size_t len = strlen(char1) + strlen(char2);
		char * valueChar = new char[len + 1];
		strcpy(valueChar, char1);
		strcat(valueChar, char2);
		return valueChar;
	}

	int TypeConver::CharToInt(const char * s)
	{
		int sign = 1;
		if (*s == '-')
		{
			sign = -1;
			s ++;
		}
		int num = 0;
		while(*s)
		{
			num = ((*s) - '0') + num * 10;
			s ++;
		}
		return num * sign;
	}

int TypeConver::charToInt(const char s)
{
    if (s == '0') return 0;
    if (s == '1') return 1;
    if (s == '2') return 2;
    if (s == '3') return 3;
    if (s == '4') return 4;
    if (s == '5') return 5;
    if (s == '6') return 6;
    if (s == '7') return 7;
    if (s == '8') return 8;
    if (s == '9') return 9;
    return 0;
}
    
    std::string TypeConver::intToString(int intValue)
    {
        return cocos2d::StringUtils::toString(intValue);
    }

std::string TypeConver::filterStr(int num, int bit)
{
    std::string strNum = TypeConver::intToString(num);
    int n = bit - strNum.size();
    for (int i = 0; i < n; i ++)
    {
        strNum = "0" + strNum;
    }
    return strNum;
}

std::vector<std::string> TypeConver::splitStr(std::string strValue, std::string tagValue)
{
    std::vector<std::string> res;
    char *dStr = new char[strValue.size() + 1];
    strcpy(dStr, strValue.c_str());
    char *p = std::strtok(dStr, tagValue.c_str());
    while(p != NULL)
    {
        res.push_back(p);
        p = std::strtok(NULL, tagValue.c_str());
    }
    return res;
}
NS_MS_END;
