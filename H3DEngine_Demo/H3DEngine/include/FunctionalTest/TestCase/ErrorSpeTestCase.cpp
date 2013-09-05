#include "../HippoTestManager/HippoTestManager.h"
#include "../CommonCode/HippoUtil/HippoScene.h"
#include "../CommonCode/HippoUtil/RefCountCompare.h"
#include "../CommonCode/HippoUtil/HippoGlobal.h"
#include <iostream>
#include "idMath/dMathHeader.h"
#include "engine_interface.h"
#include "../CommonCode/HippoUtil/ActorUtil.h"
#include "log/HippoLog.h"

#include <Windows.h>
#include <list>
using namespace std;

/*
�����߼���
1����������ӿ�����speʵ��
2�����Բ�ͬ��������£������Ƿ����
*/

class ErrorSpeTestCase :public HippoTestCaseBase
{
public:
	ErrorSpeTestCase(const char* case_name):HippoTestCaseBase(case_name){}
	HippoLevelScene* m_pScene;
	list<string> spePathList;
	ISpecialEffect* gSpe;
	bool bAutoEnd;

private: 
	static HippoTestCaseBase* const testcase_;
	
public:
	virtual void InitScene()
	{
		bAutoEnd = false;
		Init();
		m_pScene=Hippo_GetSceneManager()->CreateEmptyLevel();
		Hippo_GetIRender()->OpenActionLib("../resources/art/role/actions/role.xml");
		for(list<string>::iterator it = spePathList.begin();it!= spePathList.end();it++)
		{
			Hippo_WriteConsoleAndLog(CC_WHITE,"%s",(*it).c_str());
			gSpe = m_pScene->CreateSpe((*it).c_str());
		}
		m_pScene->RestructScene();
		bAutoEnd = true;
	}

	bool Init()
	{
		GetSpeFiles("../x52_test_resources/��������������Դ/��Ч�Ҽ�SPE�ļ�/�ļ�������");
		return true;
	}

	virtual bool AutoRunExec()
	{
		if(bAutoEnd)
		{
			return true;
		}
		return false;
	}
	virtual bool ManualRunExec()
	{
		return false;
	}


	virtual int OnKeyDown(unsigned int key)
	{
		if (HippoTestManager::GetInstance()->GetCurrentRunMode())
		{
			switch(key)
			{
			case   VK_RETURN:
				m_state = CASE_END_EXECING;
				return 1;
			}
		}
		return 0;
	}

	virtual void CleanUpScene()
	{
		m_pScene->CleanScene();
		Hippo_GetSceneManager()->DelScene(m_pScene);
	}

	bool IsSpe(string path)
	{

		int idx = path.find_last_of(".");
		if(-1 == idx)
		{
			return false;
		}
		
		return path.substr(idx+1,3) == string("spe");
	}

	void GetSpeFiles(string filePath)
	{
		WIN32_FIND_DATA FindFileData;
		HANDLE hListFile;
		CHAR szFilePath[MAX_PATH];
		CHAR szFullPath[MAX_PATH];
		string FileName;
		string name = "//*";
		string path = "";

		lstrcpy(szFilePath,filePath.c_str());
		lstrcat(szFilePath,name.c_str());

		hListFile = FindFirstFile(szFilePath,&FindFileData);
		if(INVALID_HANDLE_VALUE == hListFile)
		{
			Hippo_WriteConsoleAndLog(CC_WHITE,"%s","GetSpeFiles failed!");
			return ;
		}
		else
		{
			do
			{
				/*if(lstrcmp(FindFileData.cFileName,TEXT(".") == 0)||
				lstrcmp(FindFileData.cFileName,TEXT(".."))==0)*/
				if(lstrcmp(FindFileData.cFileName,TEXT("."))==0||  
					lstrcmp(FindFileData.cFileName,TEXT(".."))==0)  
				{
					continue;
				}

				FileName = FindFileData.cFileName;
				sprintf(szFullPath,"%s/%s",filePath.c_str(),FileName.c_str());

				if(IsSpe(FileName))
				{
					path = filePath + "/" + FileName;
					spePathList.push_back(path);
				}

				if(FindFileData.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)
				{
					GetSpeFiles(szFullPath);
				}

			}while(FindNextFile(hListFile,&FindFileData));

		}
	}
};

ADD_TESTCASE("ERROR_RESOURCE_TEST",ErrorSpeTestCase);
