#include "../../HippoTestManager/HippoTestManager.h"
#include "../CommonCode/HippoUtil/ActorUtil.h"
#include "../CommonCode/HippoUtil/HippoScene.h"
#include "../CommonCode/HippoUtil/HippoGlobal.h"
#include "../CommonCode/HippoUtil/RefCountCompare.h"
#include "engine_interface.h"
#include "engine_interface_internal.h"
#include "EffectCore_dx.h"
#include <algorithm>
#include "../idMath/dMathHeader.h"
using namespace  std;

/*�����߼�
��lod0����actor0��actor1��actor2��actor3
��actor0�첽����һ���װ��task0��
����ʼ��2s�ӣ�cancel��task0���ȵ�������ض�����ĿΪ0��ִ����һ����
ִ��clearmatlib
��actor1�첽����ͬ����һ���װ��task1��
��actor3�첽����ͬ����һ���װ��task2��
��task1������ɺ�Ҫ���ø����ø�actor1��cancel��task1
ִ��clearmatlib
��task2������ɺ�Ҫ���ø����ø�actor3��
ִ��clearmatlib
��actor2ͬ������ͬ����һ���·�
��Ⱦ5s����Ч���Բ���
�ͷ�4��actor
*/


/*��ע
��������û��ͨ��
����㣺�ڱȽ����ü�����ʱ��������Դ��Ϣ ǰ��������һ��
����ԭ��
*/


class AsynTestCase20 : public HippoTestCase_RefCompare
{
public:
	AsynTestCase20(const char* casename):HippoTestCase_RefCompare(casename){}
	HippoLevelScene* m_pScene;

	H3DI::IActor* m_pActor0;
	H3DI::IActor* m_pActor1;
	H3DI::IActor* m_pActor2;
	H3DI::IActor* m_pActor3;

	unsigned int task0 ,task1,task2;
	const char* bptName;

	H3DI::sCreateOp createop;
	enum ExeState
	{
		INIT,
		//��lod0����actor0��actor1��actor2��actor3 ��actor0�첽����һ����װ��task0��
		CREATE_ACTOR_ASYN_BPT_TASK0,
		//����ʼ��0.5s�ӣ�cancel��task0
		AFTER_5S_CANCEL_TASK0,
		//�ȵ�������ض�����ĿΪ0��ִ����һ����
		WAIT_QUEUE_IS_ZERO,
		//��actor1�첽����ͬ����һ���װ��task1��,
		ASYN_ACTOR1_TASK1,
		//��actor3�첽����ͬ����һ���װ��task2��
		ASYN_ACTOR3_TASK2,
		//��task1������ɺ�Ҫ���ø����ø�actor1��cancel��task1 ִ��clearmatlib
		TASK1_LOADED_THEN_CANCEL,
		//��task2������ɺ�Ҫ���ø����ø�actor3��ִ��clearmatlib
		TASK2_LOADED,
		

		//��actor2ͬ������ͬ����һ���·�
		SETBPT_ACTOR2,
		//��Ⱦ5s����Ч���Բ���
		SPIN_5S,
		//�ͷ�4��actor
		RELEASE_ACTORS,
		TEST_OVER
	};
	ExeState m_exeState;
	
private:
	static HippoTestCaseBase* const testcase_;
public:
	int GetMatLod()
	{
		return 0;
	}
	virtual void InitScene()
	{
		m_pScene = Hippo_GetSceneManager()->CreateEmptyLevel();
		bptName = "../resources/art/role/bodypart/female/suit/119005001/1193005001/1193005001.bpt";
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
	void CreateActorsAsynBpt()
	{
		H3DVec3 pos0(-3.f,0.f,0.f);
		H3DVec3 pos1(-1.f,0.f,0.f);
		H3DVec3 pos2(1.f,0.f,0.f);
		H3DVec3 pos3(3.f,0.f,0.f);
		m_pActor0 = m_pScene->CreateActor(false, GetMatLod());
		m_pActor1 = m_pScene->CreateActor(false, GetMatLod());
		m_pActor2 = m_pScene->CreateActor(false, GetMatLod());
		m_pActor3 = m_pScene->CreateActor(false, GetMatLod());
		m_pActor0->SetPosition(pos0);
		m_pActor1->SetPosition(pos1);
		m_pActor2->SetPosition(pos2);
		m_pActor3->SetPosition(pos3);

		createop.mat_lod =  GetMatLod();
		createop.geo_lod =  GetMatLod();
		task0 = Hippo_GetIRender()->LoadBodypartAsyn(bptName,H3DI::ACTOR_HUMAN,createop, GetMatLod());
	}

	virtual bool ManualRunExec()
	{
		Hippo_GetIRender()->UpdateEngineThread();
 		switch (m_exeState)
 		{
		case INIT:
			 NextState();
			 break;
		case CREATE_ACTOR_ASYN_BPT_TASK0://��lod0����actor0��actor1��actor2��actor3 ��actor0�첽����һ����װ��task0��
			 CreateActorsAsynBpt();
			 NextState();
			 break;
		case AFTER_5S_CANCEL_TASK0://����ʼ��0.5s�ӣ�cancel��task0
			if (Spin_N_Ms(500))
			{
				Hippo_GetIRender()->CancelTask(task0);
				NextState();
			}
			break;
				
		case WAIT_QUEUE_IS_ZERO://�ȵ�������ض�����ĿΪ0��ִ����һ��
			if (0 == Hippo_GetIRender()->GetWaitingTaskCount())
			{
				NextState();
			}	
			break;
		case ASYN_ACTOR1_TASK1://��actor1�첽����ͬ����һ���װ��task1��,
			{
				task1 = Hippo_GetIRender()->LoadBodypartAsyn(bptName,H3DI::ACTOR_HUMAN,createop,0);
				NextState();
				break;
			}
				
		case ASYN_ACTOR3_TASK2://��actor3�첽����ͬ����һ���װ��task2��
			{
				task2 = Hippo_GetIRender()->LoadBodypartAsyn(bptName,H3DI::ACTOR_HUMAN,createop,0);
				NextState();
				break;
			}
				
		case TASK1_LOADED_THEN_CANCEL://��task1������ɺ�Ҫ���ø����ø�actor1��cancel��task1 ִ��clearmatlib
									 
			{
				unsigned int  res = Hippo_GetIRender()->QueryTaskState(task1);
				if (res == H3DI::TASK_OK || res == H3DI::TASK_ERROR)
				{
					Hippo_GetIRender()->CancelTask(task1);
					Hippo_GetIRender()->ClearMaterialLib();
					NextState();
				}
				break;
			}
		case TASK2_LOADED://��task2������ɺ�Ҫ���ø����ø�actor3��ִ��clearmatlib
			{
				unsigned int  res = Hippo_GetIRender()->QueryTaskState(task2);
				if (res == H3DI::TASK_OK || res == H3DI::TASK_ERROR)
				{
					Hippo_GetIRender()->ClearMaterialLib();
					NextState();
				}
				break;
			}

		case SETBPT_ACTOR2://��actor2ͬ������ͬ����һ���·�
			{
				m_pActor2->SetBodyPart(bptName);
				NextState();
				break;
			}
			
		case SPIN_5S://��Ⱦ5s����Ч���Բ���
			if (Spin_N_Ms(5000))
			{
				NextState();
			}
			break;
		case RELEASE_ACTORS://�ͷ�4��actor
			{
				m_pScene->DelActor(m_pActor0);
				m_pScene->DelActor(m_pActor1);
				m_pScene->DelActor(m_pActor2);
				m_pScene->DelActor(m_pActor3);
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
ADD_TESTCASE("ASYN_LOAD_TEST",AsynTestCase20);
