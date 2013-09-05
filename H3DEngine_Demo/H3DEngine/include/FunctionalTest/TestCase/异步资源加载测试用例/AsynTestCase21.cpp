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

/*测试逻辑
以lod0创建1个actor1
以lod1创建1个actor2
给actor1异步加载一身服装（task1）
给actor2异步设置同一身服装（task2）
Task1加载完成后不设置给Actor1
执行clearmatlib
Task2加载完后设置给actor2
渲染5s，观察效果
切换actor1到lod1
给actor1异步加载同一身服装（task3）
Task3执行2s后，释放actor2，clearmatlib
等task3加载完设置给actor1
渲染5s，观察效果
释放所有人物，执行clearmatlib
*/

/*备注
该测试用例不能通过断言
出错点： mat tpl tex 前后不一致
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
		//以lod0创建1个actor1 以lod1创建1个actor2 	给actor1异步加载一身服装（task1）	给actor2异步加载同一身服装（task2）
		CREATE_TWOACTOR_ASYN_BPT_TASK,
		//Task1加载完成后不设置给Actor1 执行clearmatlib
		AFTER_TASK1_NO_SET,
		//Task2加载完后设置给actor2
		AFTER_TASK2_SET_ACTOR2,
		//渲染5s，观察效果
		SPIN_5E_FIRST,
		//切换actor1到lod1
		ACTOR1_SET_LOD1,
		//给actor1异步加载同一身服装（task3）
		ACTOR1_ASYN_BPT,
		//Task3执行2s后，释放actor2，clearmatlib
		RELEASE_ACTOR2,
		//等task3加载完设置给actor1
		TASK3_LOADED_SET_ACTOR1,
		//渲染5s，观察效果
		SPIN_5S_SECOND,
		//释放所有人物，执行clearmatlib
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

		//以lod0创建1个actor1 以lod1创建1个actor2
		H3DVec3 pos1(-1.5f,0.f,0.f);
		H3DVec3 pos2(1.5f,0.f,0.f);

		m_pActor1_lod0 = m_pScene->CreateActor(false,GetMatLod0());
		m_pActor2_lod1 = m_pScene->CreateActor(false,GetMatLod1());

		m_pActor1_lod0->SetPosition(pos1);
		m_pActor2_lod1->SetPosition(pos2);

		//给actor1异步加载一身服装（task1）
		task1 = Hippo_GetIRender()->LoadBodypartAsyn(bptName,H3DI::ACTOR_HUMAN,createop0,0);
		//给actor2异步设置同一身服装（task2）
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
				
		case CREATE_TWOACTOR_ASYN_BPT_TASK://以lod0创建1个actor1 以lod1创建1个actor2 	给actor1异步加载一身服装（task1）	给actor2异步加载同一身服装（task2）
			 CreateTwoActorAndAsyn();
			 NextState();
			 break;
		case AFTER_TASK1_NO_SET://Task1加载完成后不设置给Actor1 执行clearmatlib
			{ 

				unsigned int res = Hippo_GetIRender()->QueryTaskState(task1);
				
				if (res == H3DI::TASK_OK ||res == H3DI::TASK_ERROR)
				{
					Hippo_GetIRender()->ClearMaterialLib();
					NextState();
				}
			}
			 break;
				
		case AFTER_TASK2_SET_ACTOR2://Task2加载完后设置给actor2
			{
				unsigned int res = Hippo_GetIRender()->QueryTaskState(task2);
				if (res == H3DI::TASK_OK ||res == H3DI::TASK_ERROR)
				{
					m_pActor2_lod1->SetBodyPartAndFinishTask(task2);
					NextState();
				}
			}
			break;
				
		case SPIN_5E_FIRST://渲染5s，观察效果
			if (Spin_N_Ms(5000))
			{
				NextState();
			}
			break;
				
		case ACTOR1_SET_LOD1://切换actor1到lod1
			m_pActor1_lod0->SetMaterialLod(GetMatLod1());
			NextState();
			break;
				
		case ACTOR1_ASYN_BPT://给actor1异步加载同一身服装（task3）
			{
				task3 = Hippo_GetIRender()->LoadBodypartAsyn(bptName,H3DI::ACTOR_HUMAN,createop0,0);
				NextState();
			}
			break;
				
		case RELEASE_ACTOR2://Task3执行2s后，释放actor2，clearmatlib
			if (Spin_N_Ms(2000))
			{
				m_pScene->DelActor(m_pActor2_lod1);
				Hippo_GetIRender()->ClearMaterialLib();
				NextState();
			}
			break;
				
		case TASK3_LOADED_SET_ACTOR1://等task3加载完设置给actor1
			{
				unsigned int res = Hippo_GetIRender()->QueryTaskState(task3);
				if (res == H3DI::TASK_OK ||res == H3DI::TASK_ERROR)
				{
					m_pActor1_lod0->SetBodyPartAndFinishTask(task3);
					NextState();
				}
			}
			break;
				
		case SPIN_5S_SECOND://渲染5s，观察效果
			if (Spin_N_Ms(5000))
			{
				NextState();
			}
			break;
		case RELESE_ACTORS://释放所有人物，执行clearmatlib
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