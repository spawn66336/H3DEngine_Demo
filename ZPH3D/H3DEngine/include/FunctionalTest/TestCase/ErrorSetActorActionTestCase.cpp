#include "../HippoTestManager/HippoTestManager.h"
#include "../CommonCode/HippoUtil/HippoScene.h"
#include "../CommonCode/HippoUtil/RefCountCompare.h"
#include "../CommonCode/HippoUtil/HippoGlobal.h"
#include <iostream>
#include "idMath/dMathHeader.h"
#include "engine_interface.h"
#include "../CommonCode/HippoUtil/ActorUtil.h"
#include "log/HippoLog.h"
#include <list>
using namespace std;



/*
�����߼���
1������actorģ��
2�����Բ�ͬ��������£������Ƿ����
3.����������actor�����Ĳ���
*/

class ErrorSetActorActionTestCase : public HippoTestCaseBase
{
public:
	ErrorSetActorActionTestCase(const char* case_name):HippoTestCaseBase(case_name){}
	HippoLevelScene* m_pScene;
	H3DI::IAnimationChannel* channel;
	H3DI::IActor* m_pActor;
	bool bAutoEnd;

private:
	static HippoTestCaseBase* const testcase_;

public:
	virtual void InitScene()
	{
		bAutoEnd = false;
		//����ж���Ҫ��Դ�������---�����б����---
		Hippo_WriteConsoleAndLog(CC_WHITE,"�����б��������");
		if (!Hippo_GetIRender()->OpenActionLib("../resources/art/role/actions/role.xml"))
		{
			Hippo_WriteConsoleAndLog(CC_WHITE,"����role.xml�ļ�ʧ��");
			m_state=CASE_END_EXECING;
		}
		m_pScene = Hippo_GetSceneManager()->CreateEmptyLevel();
		m_pActor = m_pScene->CreateActor(false);


		//������Ե��ǣ������ļ������ڣ������ļ����ݴ��󣬶����ļ���һ��fig�����
		//ֻ��Ҫ����һ��chrʵ���������ʵ������3����ͬ�Ķ�������3�������ֱ��Ӧ����3�д���

		Hippo_WriteConsoleAndLog(CC_WHITE,"������Ե��ǣ������ļ������ڣ������ļ����ݴ��󣬶����ļ���һ��fig----begin");

		Hippo_GetIRender()->OpenActionLib("../x52_test_resources/��������������Դ/���ý�ɫ����/�����ļ�����/�����ļ�������_���ݲ���ȷ_ֻ��һ��fig/4115001001/role.xml");  
		channel = m_pActor->GetAnmChannel(0);
		channel->SetTransitionTimeFromCurrent(0.0f);
		Hippo_WriteConsoleAndLog(CC_WHITE,"�����ļ�������");
		channel->SetAction("SD_122BPM_huh005_01_1",true);
		Hippo_WriteConsoleAndLog(CC_WHITE,"�����ļ����ݴ���");
		channel->SetAction("SD_122BPM_huh005_02_1",true);
		Hippo_WriteConsoleAndLog(CC_WHITE,"�����ļ���һ��fig");
		channel->SetAction("SD_122BPM_huh005_03_1",true);
		Hippo_WriteConsoleAndLog(CC_WHITE,"������Ե��ǣ������ļ������ڣ������ļ����ݴ��󣬶����ļ���һ��fig----end");

		/* 
		 �����ǶԹ����ļ�����Ĳ��ԣ�����Ҫ����3��role�ļ���û����һ�ζ�������һ��chrʵ��
		 Ȼ������ʵ��SetAction���� ��ʹ�õ���3�������ļ���Ӧ��3��fig�ļ�
		 �ֱ�fig�����ڣ�fig��ʽ����ȷ��fig�������淶
		*/
		
		Hippo_WriteConsoleAndLog(CC_WHITE,"������Ե��ǣ�fig�����ڣ�fig��ʽ����ȷ��fig�������淶----begin");


		TestFigFile("../x52_test_resources/��������������Դ/���ý�ɫ����/�����ļ�����/NodeName���������Ӧ/�����ļ�������/4115001001/role.xml"
			,m_pActor
			,"tpose_hand");

		TestFigFile("../x52_test_resources/��������������Դ/���ý�ɫ����/�����ļ�����/NodeName���������Ӧ/�ļ���ʽ����ȷ/4115001001/role.xml"
			,m_pActor
			,"tpose_hand");

		TestFigFile("../x52_test_resources/��������������Դ/���ý�ɫ����/�����ļ�����/NodeName���������Ӧ/NodeName�������淶/4115001001/role.xml"
			,m_pActor
			,"tpose_hand");

		Hippo_WriteConsoleAndLog(CC_WHITE,"������Ե��ǣ�fig�����ڣ�fig��ʽ����ȷ��fig�������淶----end");

		m_pScene->RestructScene();
		bAutoEnd = true;
	}

	bool TestFigFile(string rolePaht,H3DI::IActor* m_pActor,string actionId)
	{

		Hippo_GetIRender()->OpenActionLib(rolePaht.c_str()); 
		channel = m_pActor->GetAnmChannel(0); 
		channel->SetTransitionTimeFromCurrent(0.0f);
		channel->SetAction(actionId.c_str(),true);

		return true;
	}

	virtual void CleanUpScene()
	{
		m_pScene->CleanScene();
		Hippo_GetSceneManager()->DelScene(m_pScene);
	}

	virtual bool AutoRunExec()
	{
		if (bAutoEnd)
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

};

ADD_TESTCASE("ERROR_RESOURCE_TEST",ErrorSetActorActionTestCase);
