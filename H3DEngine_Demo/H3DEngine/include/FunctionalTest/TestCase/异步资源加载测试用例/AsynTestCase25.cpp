#include "../../HippoTestManager/HippoTestManager.h"
#include "../CommonCode/HippoUtil/ActorUtil.h"
#include "../CommonCode/HippoUtil/HippoScene.h"
#include "../CommonCode/HippoUtil/HippoGlobal.h"
#include "../CommonCode/HippoUtil/RefCountCompare.h"
#include "engine_interface.h"
#include "engine_interface_internal.h"
#include "EffectCore_dx.h"
#include "../idMath/dMathHeader.h"
#include <vector>
#include <algorithm>
using namespace  std;

/*测试逻辑
以lod0加载一个actor0
以异步的方式给actor0，加载FABCDDEEF件上衣(ABCDEF分别表示不同的几件上衣)，分别是task0，1，2，3，4，5，6，7 8
Sleep 100ms，cancel掉task 0。
等待所有task完成，把task8设置给actor0
渲染5s观察效果
Cancel掉所有的task（除了task0和task8）
释放角色
*/

/*备注
该测试用例能够通过断言
*/

class AsynTestCase25 : public HippoTestCase_RefCompare
{
public:
public:
	AsynTestCase25(const char* casename):HippoTestCase_RefCompare(casename){}
	HippoLevelScene* m_pScene;

	H3DI::IActor* m_pActor0;

	const char* bptName;
	unsigned int task0,task1,task2,task3,task4,task5,task6,task7,task8;
	vector<unsigned int> m_all_task;
	enum ExeState
	{
		INIT,
		//以lod0加载一个actor0  以异步的方式给actor0，加载FABCDDEEF件上衣(ABCDEF分别表示不同的几件上衣)，分别是task0，1，2，3，4，5，6，7 8
		CREATE_ACTOR_ASYN_BPT,
		//Sleep 100ms，cancel掉task 0。
		SLEEP_AND_CANCEL_TASK0,
		//等待所有task完成，把task8设置给actor0
		ALLTASK_LOADED,
		//渲染5s观察效果
		SPIN_5S,
		//Cancel掉所有的task（除了task0和task8）
		CANCEL_ALL_TASK,
		//释放角色
		RELEASE_ACTOR,
		TEST_OVER
	};
	ExeState m_exeState;

private:
	static HippoTestCaseBase* const testcase_;

public:
	int GetMatLod0()
	{
		return 0;
	}
	virtual void InitScene()
	{
		m_pScene = Hippo_GetSceneManager()->CreateEmptyLevel();
		m_exeState = INIT;
	}
	bool IaAllTaskLoaded()
	{
		unsigned int  res;
		auto  it = m_all_task.begin();
		while (it != m_all_task.end())
		{
			res =  Hippo_GetIRender()->QueryTaskState(*it);
			if (res == H3DI::TASK_WORKING)
			{
				return false;
			}
			++it;
		}
		return true;
	}
	void CancelAllTask()
	{
		auto itr = m_all_task.begin();
		while (itr != m_all_task.end())
		{
			Hippo_GetIRender()->CancelTask(*itr);
			itr = m_all_task.erase(itr);
		}
	}
	virtual void CleanUpScene()
	{
		m_pScene->CleanScene();
		Hippo_GetSceneManager()->DelScene(m_pScene);
	}

	virtual bool AutoRunExec()
	{
		if (m_exeState == TEST_OVER)
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

	void CreateAcotrAndBpt()
	{
		//以lod0加载一个actor0
		m_pActor0 = m_pScene->CreateActor(false,GetMatLod0());

		const char* nameF = "../resources/art/role/bodypart/female/body/114001001/114001001.BPT";
		const char* nameA = "../resources/art/role/bodypart/female/body/114004001/114004001.BPT";
		const char* nameB = "../resources/art/role/bodypart/female/body/114012001/114012001.BPT";
		const char* nameC = "../resources/art/role/bodypart/female/body/114013003/114013003.BPT";
		const char* nameD = "../resources/art/role/bodypart/female/body/114018004/114018004.BPT";
		const char* nameE = "../resources/art/role/bodypart/female/body/114023004/114023004.BPT";

		H3DI::sCreateOp op;
		op.mat_lod = GetMatLod0();
		op.geo_lod = GetMatLod0();

		//a以异步的方式给actor0，加载FABCDDEEF件上衣(ABCDEF分别表示不同的几件上衣)，
		task0 = Hippo_GetIRender()->LoadBodypartAsyn(nameF,H3DI::ACTOR_HUMAN,op,0);
		m_all_task.push_back(task0);
		task1 = Hippo_GetIRender()->LoadBodypartAsyn(nameA,H3DI::ACTOR_HUMAN,op,0);
		m_all_task.push_back(task1);
		task2 = Hippo_GetIRender()->LoadBodypartAsyn(nameB,H3DI::ACTOR_HUMAN,op,0);
		m_all_task.push_back(task2);
		task3 = Hippo_GetIRender()->LoadBodypartAsyn(nameC,H3DI::ACTOR_HUMAN,op,0);
		m_all_task.push_back(task3);
		task4 = Hippo_GetIRender()->LoadBodypartAsyn(nameD,H3DI::ACTOR_HUMAN,op,0);
		m_all_task.push_back(task4);
		task5 = Hippo_GetIRender()->LoadBodypartAsyn(nameD,H3DI::ACTOR_HUMAN,op,0);
		m_all_task.push_back(task5);
		task6 = Hippo_GetIRender()->LoadBodypartAsyn(nameE,H3DI::ACTOR_HUMAN,op,0);
		m_all_task.push_back(task6);
		task7 = Hippo_GetIRender()->LoadBodypartAsyn(nameE,H3DI::ACTOR_HUMAN,op,0);
		m_all_task.push_back(task7);
		task8 = Hippo_GetIRender()->LoadBodypartAsyn(nameF,H3DI::ACTOR_HUMAN,op,0);
		m_all_task.push_back(task8);
	}
	virtual bool ManualRunExec()
	{
		Hippo_GetIRender()->UpdateEngineThread();
		switch (m_exeState)
		{
		case INIT:
			NextState();
			break;

		//以lod0加载一个actor0  以异步的方式给actor0，加载FABCDDEEF件上衣(ABCDEF分别表示不同的几件上衣)，分别是task0，1，2，3，4，5，6，7 8
		case CREATE_ACTOR_ASYN_BPT:
			CreateAcotrAndBpt();
			NextState();
			break;
				
		case SLEEP_AND_CANCEL_TASK0://Sleep 100ms，cancel掉task 0。
			{
			Sleep(100);
			Hippo_GetIRender()->CancelTask(task0);
			vector<unsigned int>::iterator itr = find(m_all_task.begin(),m_all_task.end(),task0);
			if(itr != m_all_task.end())
				m_all_task.erase(itr);
			NextState();
			}
			break;
				
		case ALLTASK_LOADED://等待所有task完成，把task8设置给actor0
			if (IaAllTaskLoaded())
			{
				m_pActor0->SetBodyPartAndFinishTask(task8);
				vector<unsigned int>::iterator itr = find(m_all_task.begin(),m_all_task.end(),task8);
				if(itr != m_all_task.end())
					m_all_task.erase(itr);
				NextState();
			}
			break;
				
		case SPIN_5S://渲染5s观察效果
			if (Spin_N_Ms(5000))
			{
				NextState();
			}
			break;
				
		case CANCEL_ALL_TASK://Cancel掉所有的task（除了task0和task8）
			CancelAllTask();
			NextState();
			break;
				
		case RELEASE_ACTOR://释放角色
			m_pScene->DelActor(m_pActor0);
			NextState();
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
ADD_TESTCASE("ASYN_LOAD_TEST",AsynTestCase25);