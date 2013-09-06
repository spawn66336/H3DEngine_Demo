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

/*测试逻辑
以lod0创建actor0，actor1，actor2，actor3
给actor0异步加载一身服装（task0）
任务开始后2s钟，cancel掉task0，等到引擎加载队列数目为0，执行下一步。
执行clearmatlib
给actor1异步加载同样的一身服装（task1）
给actor3异步加载同样的一身服装（task2）
等task1加载完成后不要设置给设置给actor1，cancel掉task1
执行clearmatlib
等task2加载完成后不要设置给设置给actor3，
执行clearmatlib
给actor2同步设置同样的一身衣服
渲染5s，看效果对不对
释放4个actor
*/


/*备注
测试用例没有通过
错误点：在比较引用计数的时候，三种资源信息 前后数量不一致
可能原因：
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
		//以lod0创建actor0，actor1，actor2，actor3 给actor0异步加载一件服装（task0）
		CREATE_ACTOR_ASYN_BPT_TASK0,
		//任务开始后0.5s钟，cancel掉task0
		AFTER_5S_CANCEL_TASK0,
		//等到引擎加载队列数目为0，执行下一步。
		WAIT_QUEUE_IS_ZERO,
		//给actor1异步加载同样的一身服装（task1）,
		ASYN_ACTOR1_TASK1,
		//给actor3异步加载同样的一身服装（task2）
		ASYN_ACTOR3_TASK2,
		//等task1加载完成后不要设置给设置给actor1，cancel掉task1 执行clearmatlib
		TASK1_LOADED_THEN_CANCEL,
		//等task2加载完成后不要设置给设置给actor3，执行clearmatlib
		TASK2_LOADED,
		

		//给actor2同步设置同样的一身衣服
		SETBPT_ACTOR2,
		//渲染5s，看效果对不对
		SPIN_5S,
		//释放4个actor
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
		case CREATE_ACTOR_ASYN_BPT_TASK0://以lod0创建actor0，actor1，actor2，actor3 给actor0异步加载一件服装（task0）
			 CreateActorsAsynBpt();
			 NextState();
			 break;
		case AFTER_5S_CANCEL_TASK0://任务开始后0.5s钟，cancel掉task0
			if (Spin_N_Ms(500))
			{
				Hippo_GetIRender()->CancelTask(task0);
				NextState();
			}
			break;
				
		case WAIT_QUEUE_IS_ZERO://等到引擎加载队列数目为0，执行下一步
			if (0 == Hippo_GetIRender()->GetWaitingTaskCount())
			{
				NextState();
			}	
			break;
		case ASYN_ACTOR1_TASK1://给actor1异步加载同样的一身服装（task1）,
			{
				task1 = Hippo_GetIRender()->LoadBodypartAsyn(bptName,H3DI::ACTOR_HUMAN,createop,0);
				NextState();
				break;
			}
				
		case ASYN_ACTOR3_TASK2://给actor3异步加载同样的一身服装（task2）
			{
				task2 = Hippo_GetIRender()->LoadBodypartAsyn(bptName,H3DI::ACTOR_HUMAN,createop,0);
				NextState();
				break;
			}
				
		case TASK1_LOADED_THEN_CANCEL://等task1加载完成后不要设置给设置给actor1，cancel掉task1 执行clearmatlib
									 
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
		case TASK2_LOADED://等task2加载完成后不要设置给设置给actor3，执行clearmatlib
			{
				unsigned int  res = Hippo_GetIRender()->QueryTaskState(task2);
				if (res == H3DI::TASK_OK || res == H3DI::TASK_ERROR)
				{
					Hippo_GetIRender()->ClearMaterialLib();
					NextState();
				}
				break;
			}

		case SETBPT_ACTOR2://给actor2同步设置同样的一身衣服
			{
				m_pActor2->SetBodyPart(bptName);
				NextState();
				break;
			}
			
		case SPIN_5S://渲染5s，看效果对不对
			if (Spin_N_Ms(5000))
			{
				NextState();
			}
			break;
		case RELEASE_ACTORS://释放4个actor
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
