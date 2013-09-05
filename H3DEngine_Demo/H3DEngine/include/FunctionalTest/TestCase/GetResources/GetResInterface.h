#pragma once

#include <memory>
#include "tinystr.h"
#include "tinyxml.h"
#include <string>
using namespace std;

class ResUtil
{
public:
static std::string GetX52Resources_Path(const char* filename);
static std::string GetHippoTestResource_Path(const char* filename);
static std::string GetProjectHResources_Path(const char* filename);
static bool   GetResFromXml(void);
static void  CheckSlash(string& filename);
private: 
	
	static std::string s_testPath;
	static std::string s_x52Path;
	static std::string s_projHPath;
};



