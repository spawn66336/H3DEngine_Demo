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
			::MessageBox(0,"����Ŀ¼ʧ��","ERROR",0);
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
	if (!doc.LoadFile()) //����ʧ�ܣ����ȼ���Ƿ����ResListĿ¼
	{
		if ( _access("./ResList", 0) != 0)
			CreateResDir();

		vector<std::string> pathList;
		FindFileRecursion(&pathList,ErgodicDir.c_str(),"*.dml");

		int index = 0;
		while(index != pathList.size())
		{
			//����һ��Ԫ�ز����ӡ�
			TiXmlElement ResElement("ResList");
			ResElement.SetAttribute("dmlPath",pathList.at(index).c_str());
			doc.InsertEndChild(ResElement);
			index++;
		}
		if (!doc.SaveFile(filename.c_str()))//���浽�ļ�
		{
			Hippo_WriteConsole(CC_RED,"�����б��ļ�ʧ�ܣ������Ƿ����ResListĿ¼");
			return false;
		}
	}
	if (doc.LoadFile())
	{
		//���ļ�����
		TiXmlElement* pElem = doc.FirstChildElement("ResList");

		if (!pElem)
		{
			::MessageBox(0,"�ļ��б�Ϊ��","ERROR",0);
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