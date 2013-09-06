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
以lod0创建actor0，actor1
给actor0异步加载一身服装（task0）
任务开始后2s钟，cancel掉task0，等到引擎加载队列数目为0，执行下一步。
执行clearmatlib
给actor1异步加载同样的一身服装（task1）
等task1加载完成后设置给actor1
渲染5s，看效果对不对
释放2个actor
*/

/*备注
测试用例通过，前后引用计数相等
*/

class AsynTestCase19 : public HippoTestCase_RefCompare
{
	AsynTestCase19(const char* casename):HippoTestCase_RefCompare(casename){}

	HippoLevelScene* m_pScene;
	unsigned int task0 , task1;

	H3DI::IActor* m_pActor0;
	H3DI::IActor* m_pActor1;
	const char* bptName ;

	enum ExeState
	{
		INIT,
		//以lod0创建1个actor0，actor1 给actor0异步加载一件服装（task0）
		CREATE_ACTOR_ASYN_BPT_TASK0,
		//任务开始后0.5s钟，cancel掉task0
		AFTER_5S_CANCEL_TASK0,
		//等到引擎加载队列数目为0，执行下一步。
		WAIT_QUEUE_IS_ZERO,
		//执行clearmatlib 给actor1异步加载同样的一身服装（task1）
		ASYN_LOAD_BPT_ACTOR1,
		//等task1加载完成后设置给actor1。
		TASK1_LOADED_SET_ACTOR1,
		//渲染5s，看效果对不对
		SPIN_5S,
		//释放两个人物
		RELEASE_ACTORS,
		TEST_OVER
	};
	ExeState m_exeState;
	H3DI::sCreateOp createop;
private:
	static HippoTestCaseBase* const testcase_;

public:

    int GetMatlod() {return 0;}
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
	void CreateActorAndASynBpt()
	{
		//以lod0创建actor0，actor1 ,给actor0异步加载一身服装（task0）
		m_pActor0 = m_pScene->CreateActor(false,GetMatlod());
		H3DVec3 pos0(-2.f,0.f,0.f);
		m_pActor0->SetPosition(pos0);
		m_pActor1 = m_pScene->CreateActor(false,GetMatlod());
		H3DVec3 pos1(2.f,0.f,0.f);
		m_pActor1->SetPosition(pos1);

		createop.mat_lod = GetMatlod();
		createop.geo_lod = GetMatlod();
		task0 = Hippo_GetIRender()->LoadBodypartAsyn(bptName,H3DI::ACTOR_HUMAN,createop,0);
	}
 

	virtual bool ManualRunExec()
	{
		Hippo_GetIRender()->UpdateEngineThread();
		switch (m_exeState)
		{
		case INIT:
			NextState();
			break;
		case CREATE_ACTOR_ASYN_BPT_TASK0://以lod0创建1个actor0，actor1 给actor0异步加载一件服装（task0）
			CreateActorAndASynBpt();
			NextState();
			break;
		case  AFTER_5S_CANCEL_TASK0://任务开始后0.5s钟，cancel掉task0
			if (Spin_N_Ms(500))
			{
				Hippo_GetIRender()->CancelTask(task0);
				NextState();
			}
			break;
		case  WAIT_QUEUE_IS_ZERO://等到引擎加载队列数目为0，执行下一步。
			if (0 == Hippo_GetIRender()->GetWaitingTaskCount())
			{
				NextState();
			}
			break;
		case ASYN_LOAD_BPT_ACTOR1://执行clearmatlib 给actor1异步加载同样的一身服装（task1）
			{
				Hippo_GetIRender()->ClearMaterialLib();
				task1 = Hippo_GetIRender()->LoadBodypartAsyn(bptName,H3DI::ACTOR_HUMAN,createop,0);
				NextState();
			}
			break;
		case TASK1_LOADED_SET_ACTOR1://等task1加载完成后设置给actor1。
			{
			unsigned int res = Hippo_GetIRender()->QueryTaskState(task1);
			if (res == H3DI::TASK_OK || res == H3DI::TASK_ERROR)
			{
				m_pActor1->SetBodyPartAndFinishTask(task1);
				NextState();
			}
			}
			break;
		case SPIN_5S://渲染5s，看效果对不对
			if (Spin_N_Ms(5000))
			{
				NextState();
			}	
			break;
		case RELEASE_ACTORS://释放两个人物
			{
				m_pScene->DelActor(m_pActor0);
				m_pScene->DelActor(m_pActor1);
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
ADD_TESTCASE("ASYN_LOAD_TEST",AsynTestCase19);
