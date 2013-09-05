#include "../../HippoTestManager/HippoTestManager.h"
#include "../CommonCode/HippoUtil/ActorUtil.h"
#include "../CommonCode/HippoUtil/HippoScene.h"
#include "../CommonCode/HippoUtil/HippoGlobal.h"
#include "../CommonCode/HippoUtil/RefCountCompare.h"
#include "engine_interface.h"
#include "engine_interface_internal.h"
#include "EffectCore_dx.h"
#include "../idMath/dMathHeader.h"
#include <algorithm>
using namespace  std;

/*�����߼�
��lod0����1��actor1
��lod1����1��actor2
��actor1�첽����һ���װ��task1��
��actor2�첽����ͬһ���װ��task2��
Task1������ɺ����ø�Actor1
ִ��clearmatlib
Task2����������ø�actor2
��Ⱦ5s���۲�Ч��
�л�actor1��lod1
��actor1�첽����ͬһ���װ��task3��
Task3ִ��2s���ͷ�actor2��clearmatlib
��task3���������ø�actor1
��Ⱦ5s���۲�Ч��
�ͷ��������ִ��clearmatlib
*/

/*��ע
�ò�����������ͨ������
����㣺 mat tpl tex ǰ��һ��
*/


class AsynTestCase21 : public HippoTestCase_RefCompare
{
public:
	AsynTestCase21(const char* casename):HippoTestCase_RefCompare(casename){}
	HippoLevelScene* m_pScene;

	H3DI::IActor* m_pActor1_lod0;
	H3DI::IActor* m_pActor2_lod1;

	H3DI::sCreateOp createop0;
	H3DI::sCreateOp createop1;
	
	const char* bptName;
	unsigned int task1,task2,task3;

	enum ExeState
	{
		INIT,
		//��lod0����1��actor1 ��lod1����1��actor2 	��actor1�첽����һ���װ��task1��	��actor2�첽����ͬһ���װ��task2��
		CREATE_TWOACTOR_ASYN_BPT_TASK,
		//Task1������ɺ����ø�Actor1 ִ��clearmatlib
		AFTER_TASK1_NO_SET,
		//Task2����������ø�actor2
		AFTER_TASK2_SET_ACTOR2,
		//��Ⱦ5s���۲�Ч��
		SPIN_5E_FIRST,
		//�л�actor1��lod1
		ACTOR1_SET_LOD1,
		//��actor1�첽����ͬһ���װ��task3��
		ACTOR1_ASYN_BPT,
		//Task3ִ��2s���ͷ�actor2��clearmatlib
		RELEASE_ACTOR2,
		//��task3���������ø�actor1
		TASK3_LOADED_SET_ACTOR1,
		//��Ⱦ5s���۲�Ч��
		SPIN_5S_SECOND,
		//�ͷ��������ִ��clearmatlib
		RELESE_ACTORS,
		TEST_OVER
	};
	ExeState m_exeState;
private:
	static HippoTestCaseBase* const testcase_;
public:
 
	int GetMatLod1(){return 1;}
	int GetMatLod0(){return 0;}
	virtual void InitScene()
	{
		m_pScene = Hippo_GetSceneManager()->CreateEmptyLevel();
		bptName = "../resources/art/role/bodypart/female/body/114089002/114089002.bpt";
		m_exeState = INIT;
	}

	virtual void CleanUpScene()
	{
		m_pScene->CleanScene();
		Hippo_GetSceneManager()->DelScene(m_pScene);
	}

	virtual bool AutoRunExec()
	{
		if (m_exeState = TEST_OVER)
		{
	    	return true;
		}
		return ManualRunExec();
	}
	void NextState()
	{
		int* p = (int*)(&m_exeState);
		++(*p);
	}

	void CreateTwoActorAndAsyn()
	{
		createop0.mat_lod =GetMatLod0();
		createop0.geo_lod =GetMatLod0();

		createop1.mat_lod = GetMatLod1();
		createop1.geo_lod = GetMatLod1();

		//��lod0����1��actor1 ��lod1����1��actor2
		H3DVec3 pos1(-1.5f,0.f,0.f);
		H3DVec3 pos2(1.5f,0.f,0.f);

		m_pActor1_lod0 = m_pScene->CreateActor(false,GetMatLod0());
		m_pActor2_lod1 = m_pScene->CreateActor(false,GetMatLod1());

		m_pActor1_lod0->SetPosition(pos1);
		m_pActor2_lod1->SetPosition(pos2);

		//��actor1�첽����һ���װ��task1��
		task1 = Hippo_GetIRender()->LoadBodypartAsyn(bptName,H3DI::ACTOR_HUMAN,createop0,0);
		//��actor2�첽����ͬһ���װ��task2��
		task2 = Hippo_GetIRender()->LoadBodypartAsyn(bptName,H3DI::ACTOR_HUMAN,createop1,0);
	}

	virtual bool ManualRunExec()
	{
		Hippo_GetIRender()->UpdateEngineThread();
 		switch (m_exeState)
 		{
		case INIT:
			NextState();
			break;
				
		case CREATE_TWOACTOR_ASYN_BPT_TASK://��lod0����1��actor1 ��lod1����1��actor2 	��actor1�첽����һ���װ��task1��	��actor2�첽����ͬһ���װ��task2��
			 CreateTwoActorAndAsyn();
			 NextState();
			 break;
		case AFTER_TASK1_NO_SET://Task1������ɺ����ø�Actor1 ִ��clearmatlib
			{ 

				unsigned int res = Hippo_GetIRender()->QueryTaskState(task1);
				
				if (res == H3DI::TASK_OK ||res == H3DI::TASK_ERROR)
				{
					Hippo_GetIRender()->ClearMaterialLib();
					NextState();
				}
			}
			 break;
				
		case AFTER_TASK2_SET_ACTOR2://Task2����������ø�actor2
			{
				unsigned int res = Hippo_GetIRender()->QueryTaskState(task2);
				if (res == H3DI::TASK_OK ||res == H3DI::TASK_ERROR)
				{
					m_pActor2_lod1->SetBodyPartAndFinishTask(task2);
					NextState();
				}
			}
			break;
				
		case SPIN_5E_FIRST://��Ⱦ5s���۲�Ч��
			if (Spin_N_Ms(5000))
			{
				NextState();
			}
			break;
				
		case ACTOR1_SET_LOD1://�л�actor1��lod1
			m_pActor1_lod0->SetMaterialLod(GetMatLod1());
			NextState();
			break;
				
		case ACTOR1_ASYN_BPT://��actor1�첽����ͬһ���װ��task3��
			{
				task3 = Hippo_GetIRender()->LoadBodypartAsyn(bptName,H3DI::ACTOR_HUMAN,createop0,0);
				NextState();
			}
			break;
				
		case RELEASE_ACTOR2://Task3ִ��2s���ͷ�actor2��clearmatlib
			if (Spin_N_Ms(2000))
			{
				m_pScene->DelActor(m_pActor2_lod1);
				Hippo_GetIRender()->ClearMaterialLib();
				NextState();
			}
			break;
				
		case TASK3_LOADED_SET_ACTOR1://��task3���������ø�actor1
			{
				unsigned int res = Hippo_GetIRender()->QueryTaskState(task3);
				if (res == H3DI::TASK_OK ||res == H3DI::TASK_ERROR)
				{
					m_pActor1_lod0->SetBodyPartAndFinishTask(task3);
					NextState();
				}
			}
			break;
				
		case SPIN_5S_SECOND://��Ⱦ5s���۲�Ч��
			if (Spin_N_Ms(5000))
			{
				NextState();
			}
			break;
		case RELESE_ACTORS://�ͷ��������ִ��clearmatlib
			{
				m_pScene->DelActor(m_pActor1_lod0);
				NextState();
			}
 			break;
 		}
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
ADD_TESTCASE("ASYN_LOAD_TEST",AsynTestCase21);