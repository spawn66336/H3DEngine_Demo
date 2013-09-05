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
测试逻辑：
1、创建一个Actor模型,并setbodypart
2、测试不同错误情况下，程序是否崩溃
*/

class ErrorActorBptTestCase : public HippoTestCaseBase
{
public: 
	ErrorActorBptTestCase(const char* case_name) : HippoTestCaseBase(case_name){}
	HippoLevelScene* m_pScene;
	list<string> bptPathList;
	H3DI::IActor* m_pActor;
	bool m_bActorSex;
	bool bAutoEnd;

private:
	static HippoTestCaseBase* const testcase_;

public:
	 virtual void InitScene()
	 {
		 bAutoEnd = false;
		Init();
		m_pScene = Hippo_GetSceneManager()->CreateEmptyLevel();
		Hippo_GetIRender()->OpenActionLib("../resources/art/role/actions/role.xml");
		ActorUtil::InitActorDress("../resources/config/shared/item/dress.xml");
		
		m_pActor=m_pScene->CreateActor(m_bActorSex);

		for (list<string>::iterator it = bptPathList.begin(); it != bptPathList.end(); ++it)
		{
			Hippo_WriteConsoleAndLog(CC_WHITE,"::% s",(*it).c_str());
			m_pActor->SetBodyPart((*it).c_str());
			
		}
		m_pScene->RestructScene();
		bAutoEnd = true;
        }

	void Init()
	{
		m_bActorSex=false;
		GetFiles("../x52_test_resources/错误用例测试资源/角色服装BPT文件/114001001");
	}

	virtual void CleanUpScene()
	{
		m_pScene->CleanScene();
		Hippo_GetSceneManager()->DelScene(m_pScene);
	}

	virtual bool AutoRunExec()
	{

		if(bAutoEnd) //什么都不做，10s钟后切换到下一个状态
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


	//判断是否是一个bpt文件
	bool IsBpt(string path)
	{
		int index = path.find_last_of(".");
		if (-1 == index)
		{
			return false;
		}
		
		return path.substr(index+1,3) == string("BPT");

	}

	//遍历指定的目录内所有文件和文件夹，如果是dml文件则保存在dmlPathList中
	void GetFiles(string filePath)
	{

		//Hippo_WriteConsoleAndLog(CC_WHITE,"% s",filePath.c_str());

		WIN32_FIND_DATA FindFileData;  
		HANDLE hListFile;  
		CHAR szFilePath[MAX_PATH]; 
		CHAR szFullPath[MAX_PATH];  
		string FileName;
		string name =  "//*";
		string path = "";

		//构造代表子目录和文件夹路径的字符串，使用通配符“*”  
		lstrcpy(szFilePath, filePath.c_str());      
		lstrcat(szFilePath, name.c_str());  
		hListFile = FindFirstFile(szFilePath,&FindFileData);  
		if(hListFile==INVALID_HANDLE_VALUE)  
		{  
			Hippo_WriteConsoleAndLog(CC_WHITE,"% s","GetFiles failed");
			return ;  
		}  
		else  
		{   
			
			do  
			{  
				// 如果不想显示代表本级目录和上级目录的“.”和“..”，  
				if(lstrcmp(FindFileData.cFileName,TEXT("."))==0||  
					lstrcmp(FindFileData.cFileName,TEXT(".."))==0)  
				{  
					continue;  
				}  
				FileName = FindFileData.cFileName;

				////构造成全路径  
				sprintf(szFullPath,"%s/%s",filePath.c_str(),FileName.c_str());  

				if (IsBpt(FileName))
				{  
					path = filePath+"/"+FileName;
					bptPathList.push_back(path);
				}

				if(FindFileData.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)  
				{  
					GetFiles(szFullPath);  
				} 

			}  
			while(FindNextFile(hListFile, &FindFileData));  
		}  
	}

};

ADD_TESTCASE("ERROR_RESOURCE_TEST",ErrorActorBptTestCase)



