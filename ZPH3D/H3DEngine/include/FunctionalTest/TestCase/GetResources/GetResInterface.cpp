#include "GetResInterface.h"

std::string ResUtil::s_testPath = "test";
std::string ResUtil::s_x52Path = "x52";
std::string ResUtil::s_projHPath ="projh";

void ResUtil::CheckSlash(string& filename)
{
	if (filename.length() != 0)
	{
		char& slash=filename.at(filename.length()-1);
		if(slash!='/' && slash!='\\')
		{
			filename.append("/");
		}
	}
}


bool ResUtil::GetResFromXml(void)
{

	TiXmlDocument xmlDoc;
	if (!xmlDoc.LoadFile("../HippoTestConfig/ProjResPath.xml"))
	{
		return false;
	}
	TiXmlElement* pRoot = xmlDoc.RootElement();
	TiXmlElement* pResX52 = pRoot->FirstChildElement("X52_Resource");
	if (pResX52)
	{
		s_x52Path= pResX52->Attribute("resPath");
		CheckSlash(s_x52Path);
	}
	TiXmlElement* pResTest = pRoot->FirstChildElement("HippoTest_Resource");
	if (pResTest)
	{
		s_testPath= pResTest->Attribute("resPath");
		CheckSlash(s_testPath);
	}
	TiXmlElement* pResProj = pRoot->FirstChildElement("ProjH_Resource ");
	if (pResProj)
	{
		s_projHPath= pResProj->Attribute("resPath");
		CheckSlash(s_projHPath);
	}
	return true;
}

std::string ResUtil::GetX52Resources_Path(const char* filename)
{
	return s_x52Path+filename;
}
std::string ResUtil::GetHippoTestResource_Path(const char* filename)
{
	return s_testPath+filename;
}
 std::string ResUtil::GetProjectHResources_Path(const char* filename)
{
	return s_projHPath+filename;
}
