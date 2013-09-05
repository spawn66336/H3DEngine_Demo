#include "../HippoTestManager/HippoTestManager.h"
#include "../CommonCode/HippoUtil/HippoScene.h"
#include "../CommonCode/HippoUtil/RefCountCompare.h"
#include "../CommonCode/HippoUtil/HippoGlobal.h"
#include <iostream>
#include "idMath/dMathHeader.h"
#include "engine_interface.h"
#include "../CommonCode/HippoUtil/ActorUtil.h"

using namespace std;

/*
�����߼���
����һ������
��һ��->��ִ��һ�������װ
��һ��<-��ִ��һ��������Ҽ�
*/
class ActorChangeDressTest:public HippoTestCaseBase
{
public:
	HippoLevelScene* m_pScene;
	H3DI::IActor* m_pActor;
	bool m_bActorSex;

	ActorChangeDressTest(const char* case_name) : HippoTestCaseBase(case_name){}

	//
	virtual void InitScene()
	{
		m_pScene=Hippo_GetSceneManager()->CreateEmptyLevel();

		//create actor
		Hippo_GetIRender()->OpenActionLib("../resources/art/role/actions/role.xml");
		ActorUtil::InitActorDress("../resources/config/shared/item/dress.xml");
		m_bActorSex=false;
		m_pActor=m_pScene->CreateActor(m_bActorSex);
		//m_pActor->SetAdornment((H3DI::EActorAdornmentPosition)5,"../resources/art/role/link/female/4115/4115012002/4115012002.spe");
		m_pScene->RestructScene();

	}

	void CleanUpScene()
	{
		m_pScene->CleanScene();
		Hippo_GetSceneManager()->DelScene(m_pScene);
	}

	virtual bool AutoRunExec()
	{
		if(Spin_N_Frame(150))
		{
			return true;
		}
		return false;
	}

	virtual bool ManualRunExec()
	{
		return false;
	}
	//!�Զ���ļ�����Ϣ������������0��ʾ�Ը���Ϣ������Ȥ������1��ʾ�Ѿ���������ҪĬ����Ϣ�������ٴ���
	virtual int OnKeyDown(unsigned int key)
	{
		switch( key )
		{
		case VK_RETURN:
			{
				m_state=CASE_END_EXECING;
				return 1;
			}
		case VK_LEFT:
			{
				ActorUtil::RandomBodypart(m_pActor,m_bActorSex);
				return 1;
			}
		case VK_RIGHT:
			{
				//m_pActor->SetAdornment((H3DI::EActorAdornmentPosition)5,"../resources/art/role/link/female/4115/4115012002/4115012002.spe");
				ActorUtil::RandomLink(m_pActor,m_bActorSex);
				return 1;
			}
		}
		return 0;
	}

private:
	static HippoTestCaseBase* const testcase_;
};

ADD_TESTCASE("ACTOR_TEST",ActorChangeDressTest)


/*
����Ŀ�ģ�20121116������ֻҪ�ǻ�װǰ�����������ӣ���װ��������¶�������ͻ�����Ҳ�����ͼ������

���Է�����
1����ʼ��װ����¶������
2������clear mat lib
3������������һ��¶�����ӵĿ���
*/

class ActorSockTest2 : public HippoTestCaseBase
{
public:
	ActorSockTest2(const char* casename):HippoTestCaseBase(casename){}
	H3DI::IActor* m_pActor;
	HippoScene* m_pScene;

	enum ExeState
	{
		INIT,
		CREATE_ACTOR,
		SPIN_5S_FIRST,
		CHAGE_BPT,
		SPIN_5S_SECOND,
		TEST_POINT,
		TEST_OVER
	};
	ExeState m_exeState;

public:

	virtual void InitScene()
	{
		m_pScene = Hippo_GetSceneManager()->CreateEmptyLevel();
		Hippo_GetIRender()->OpenActionLib("../resources/art/role/actions/role.xml");
		m_exeState = INIT;
	}

	virtual void CleanUpScene()
	{
		m_pScene->CleanScene();
		Hippo_GetSceneManager()->DelScene(m_pScene);
	}

	void NextState()
	{
		int* p = (int*)(&m_exeState);
		++(*p);
	}

	void TestPoint()
	{
		//*******************
		//���������û��ʵ��
	}
	void CreateActor()
	{
		m_pActor = m_pScene->CreateActor(false,2);
		m_pActor->SetBodyPart("../resources/art/role/bodypart/female/trousers/116008002/116008002.bpt");
		m_pActor->SetBodyPart("../resources/art/role/bodypart/female/shoe/118017001/118017001.bpt");

		m_pActor->SetBodyPart("../resources/art/role/bodypart/female/socks/117025003/117025003.bpt");

		float red[] = {1.f,0.f,0.f,1.0f};
		float colro1[4] = {0.0f, .0f, .0f, 1.f};

		m_pActor->SetDiffuseColorFactor(red, 4, colro1, 4, 7);
		Hippo_WriteConsole(CC_WHITE,"�����������--���� ��ѥ");
	}
	virtual bool ManualRunExec()
	{
		switch (m_exeState)
		{
		case INIT:
			NextState();
			break;
		case CREATE_ACTOR:
			CreateActor();
			NextState();
			break;
		case SPIN_5S_FIRST:
			if (Spin_N_Ms(5000))
			{
				Hippo_GetIRender()->ClearMaterialLib();
				NextState();
			}
			break;
		case CHAGE_BPT:
			//�̿�Ͷ�ѥ
			Hippo_WriteConsole(CC_WHITE,"���ɶ̿��ѥ");
			m_pActor->SetBodyPart("../resources/art/role/bodypart/female/trousers/116034002/116034002.bpt");
			m_pActor->SetBodyPart("../resources/art/role/bodypart/female/shoe/118114003/118114003.bpt");
			NextState();
			break;
		case SPIN_5S_SECOND:
			if (Spin_N_Ms(5000))
			{
				Hippo_GetIRender()->ClearMaterialLib();
				NextState();
			}
			break;
		case TEST_POINT:
			TestPoint();
			NextState();
			break;
		}
		return false;
	}

	virtual bool AutoRunExec()
	{
		if (m_exeState == TEST_OVER)
		{
			return true;
		}
		return ManualRunExec();
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
private:
	static HippoTestCaseBase* const testcase_;
};
ADD_TESTCASE("COLOR_TRANSPARENT_HILIGHT",ActorSockTest2);