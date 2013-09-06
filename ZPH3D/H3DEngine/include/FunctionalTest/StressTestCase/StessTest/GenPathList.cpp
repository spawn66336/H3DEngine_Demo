#include "GenPathList.h"
#include <io.h>

void CreateResDir()
{
	TCHAR szBuf[MAX_PATH];
	ZeroMemory(szBuf,MAX_PATH);
	if ( ::GetCurrentDirectory(MAX_PATH,szBuf) > 0)
	{
		sprintf(szBuf,"%s\\ResList",szBuf);
		if ( ! ::CreateDirectory(szBuf,NULL))
		{
			::MessageBox(0,"创建目录失败","ERROR",0);
			return ;
		}
	}
}

bool GenPathList( vector<std::string>& m_pDmlPathList,std::string ErgodicDir)
{

	unsigned int idx=ErgodicDir.find_last_of('/');
	std::string str= ErgodicDir.substr(idx+1);
	std::string filename= "./ResList/DmlResList_" + str +".xml";


	TiXmlDocument doc(filename.c_str());
	if (!doc.LoadFile()) //加载失败，首先检查是否存在ResList目录
	{
		if ( _access("./ResList", 0) != 0)
			CreateResDir();

		vector<std::string> pathList;
		FindFileRecursion(&pathList,ErgodicDir.c_str(),"*.dml");

		int index = 0;
		while(index != pathList.size())
		{
			//创建一个元素并连接。
			TiXmlElement ResElement("ResList");
			ResElement.SetAttribute("dmlPath",pathList.at(index).c_str());
			doc.InsertEndChild(ResElement);
			index++;
		}
		if (!doc.SaveFile(filename.c_str()))//保存到文件
		{
			Hippo_WriteConsole(CC_RED,"保存列表文件失败，请检查是否存在ResList目录");
			return false;
		}
	}
	if (doc.LoadFile())
	{
		//从文件加载
		TiXmlElement* pElem = doc.FirstChildElement("ResList");

		if (!pElem)
		{
			::MessageBox(0,"文件列表为空","ERROR",0);
			return false;
		}
		while(pElem)
		{
			string path= pElem->Attribute("dmlPath");
			m_pDmlPathList.push_back(path);
			pElem = pElem->NextSiblingElement("ResList");
		}
		return true;
	}
	return false;
}