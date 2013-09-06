
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
1������chrģ��
2�����Բ�ͬ��������£������Ƿ����
*/


class ErrorChrReadTestCase : public HippoTestCaseBase
{
public:
	ErrorChrReadTestCase(const char* case_name):HippoTestCaseBase(case_name){}
	HippoLevelScene* m_pScene;
	list<string> chrPathList;
	int matLod;
	bool bAutoEnd;
private:
	static HippoTestCaseBase* const testcase_;

public:

	virtual void InitScene()
	{
		bAutoEnd = false;
		Init();
		Hippo_GetIRender()->OpenActionLib("../resources/art/role/actions/role.xml");
		m_pScene = Hippo_GetSceneManager()->CreateEmptyLevel();
		for (list<string>::iterator it = chrPathList.begin(); it != chrPathList.end(); ++it)
		{
			CreateChrCase((*it).c_str(),matLod);
		}
		m_pScene->RestructScene();
		bAutoEnd = true;
	}

	void Init()
	{
		matLod = 0;
		GetFiles("../x52_test_resources/��������������Դ/��ȡ��̬����CHR�ļ�����/093005001");
	}

	virtual void CleanUpScene()
	{
		m_pScene->CleanScene();
		Hippo_GetSceneManager()->DelScene(m_pScene);
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
	
	//����ģ��
	H3DI::ISkeletonModel* CreateChrCase(const char* fn, int matlod)
	{
		Hippo_WriteConsoleAndLog(CC_WHITE,"**CreateChrCase**% s",fn);
		H3DI::IRender* pRender=Hippo_GetIRender();
		H3DI::sCreateOp op;
		op.mat_lod=matlod;
		H3DI::ISkeletonModel* p=(H3DI::ISkeletonModel*)pRender->CreateModel(op,fn);
		if (NULL != p)
		{

			p->Update(0);
			m_pScene->GetH3DScene()->AttachModel((H3DI::IMoveObject*)p,H3DI::SL_DetailObj);
			Hippo_WriteConsoleAndLog(CC_WHITE,"% s","����Chr�ɹ�");
		}
		else
		{
			Hippo_WriteConsoleAndLog(CC_WHITE,"% s","����Chr����");
		}

		return p;
	}

	//�ж��Ƿ���һ��chr�ļ�
	bool IsChr(string path)
	{
		int index = path.find_last_of(".");
		if (-1 == index)
		{
			return false;
		}
		
		return path.substr(index+1,3) == string("chr");

	}

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

 		lstrcpy(szFilePath, filePath.c_str());      
		lstrcat(szFilePath, name.c_str());  
		hListFile = FindFirstFile(szFilePath,&FindFileData);  
		if(hListFile==INVALID_HANDLE_VALUE)  
		{  
			Hippo_WriteConsoleAndLog(CC_WHITE,"% s","getfiles failed");
			return ;  
		}  
		else  
		{   
			
			//HippoFunLogObj(filePath.c_str());
			do  
			{  
 				if(lstrcmp(FindFileData.cFileName,TEXT("."))==0||  
					lstrcmp(FindFileData.cFileName,TEXT(".."))==0)  
				{  
					continue;  
				}  
				FileName = FindFileData.cFileName;

				////�����ȫ·��  
				sprintf(szFullPath,"%s/%s",filePath.c_str(),FileName.c_str());  

				if (IsChr(FileName))
				{  
					path = filePath+"/"+FileName;
					chrPathList.push_back(path);
					//Hippo_WriteConsoleAndLog(CC_WHITE,"-----::% s",path.c_str());
				}

				//�����Ŀ¼����ݹ���� 
				if(FindFileData.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)  
				{  
					GetFiles(szFullPath);  
				} 

			}  
			while(FindNextFile(hListFile, &FindFileData));  
		}  
	}
};

ADD_TESTCASE("ERROR_RESOURCE_TEST",ErrorChrReadTestCase);