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
1������һ��Actorģ��,��setbodypart
2�����Բ�ͬ��������£������Ƿ����
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
		GetFiles("../x52_test_resources/��������������Դ/��ɫ��װBPT�ļ�/114001001");
	}

	virtual void CleanUpScene()
	{
		m_pScene->CleanScene();
		Hippo_GetSceneManager()->DelScene(m_pScene);
	}

	virtual bool AutoRunExec()
	{

		if(bAutoEnd) //ʲô��������10s�Ӻ��л�����һ��״̬
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


	//�ж��Ƿ���һ��bpt�ļ�
	bool IsBpt(string path)
	{
		int index = path.find_last_of(".");
		if (-1 == index)
		{
			return false;
		}
		
		return path.substr(index+1,3) == string("BPT");

	}

	//����ָ����Ŀ¼�������ļ����ļ��У������dml�ļ��򱣴���dmlPathList��
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

		//���������Ŀ¼���ļ���·�����ַ�����ʹ��ͨ�����*��  
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
				// ���������ʾ������Ŀ¼���ϼ�Ŀ¼�ġ�.���͡�..����  
				if(lstrcmp(FindFileData.cFileName,TEXT("."))==0||  
					lstrcmp(FindFileData.cFileName,TEXT(".."))==0)  
				{  
					continue;  
				}  
				FileName = FindFileData.cFileName;

				////�����ȫ·��  
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



