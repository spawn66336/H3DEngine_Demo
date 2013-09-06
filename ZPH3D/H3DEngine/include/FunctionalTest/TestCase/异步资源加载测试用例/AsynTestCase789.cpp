#include "../../HippoTestManager/HippoTestManager.h"
#include "../CommonCode/HippoUtil/ActorUtil.h"
#include "../CommonCode/HippoUtil/HippoScene.h"
#include "../CommonCode/HippoUtil/HippoGlobal.h"
#include "../CommonCode/HippoUtil/RefCountCompare.h"
#include "engine_interface.h"
#include "engine_interface_internal.h"
#include "EffectCore_dx.h"
#include <algorithm>
using namespace  std;

/*测试逻辑
以lod0创建1个actor
异步加载一件服装
加载完成后不设置给Actor
cancel掉该加载任务，等到引擎加载队列数目为0，执行下一步。
执行clearmatlib
*/


/*备注
该测试用例能够通过断言
*/

class AsynTestCase7 : public HippoTestCase_RefCompare
{
public:
	AsynTestCase7(const char* casename):HippoTestCase_RefCompare(casename){}
	HippoLevelScene* m_pScene;

	H3DI::IActor* m_pActor;
	std::vector<unsigned int> m_all_task;

	bool bAutoEnd; //自动运行结束标志

private:
	static HippoTestCaseBase* const testcase_;
public:
	virtual int GetMatLod() {return 0;}
	virtual void InitScene()
	{
		m_pScene = Hippo_GetSceneManager()->CreateEmptyLevel();
		//创建1个actor 异步加载一件服装
		m_pActor = m_pScene->CreateActor(false, GetMatLod());

		H3DI::sCreateOp createOp;
		createOp.mat_lod =  GetMatLod();
		createOp.geo_lod =  GetMatLod();
		m_all_task.push_back(Hippo_GetIRender()->LoadBodypartAsyn("../resources/art/role/bodypart/female/hand/115002001/115002001.bpt",H3DI::ACTOR_HUMAN, createOp,0)) ;

		bAutoEnd = false;
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
		ManualRunExec();
		return false;
	}

	virtual bool ManualRunExec()
	{
		Hippo_GetIRender()->UpdateEngineThread();
		//加载完成后不设置给Actor cancel掉该加载任务，等到引擎加载队列数目为0，执行下一步。 执行clearmatlib

		auto itr=m_all_task.begin();
		while (itr!=m_all_task.end())
		{
			unsigned int taskId=*itr;
			unsigned int  res = Hippo_GetIRender()->QueryTaskState(taskId);
			if (res == H3DI::TASK_OK || res == H3DI::TASK_WORKING)
			{
				Hippo_GetIRender()->CancelTask(*itr);
				itr=m_all_task.erase(itr);
				continue;
			}
			++itr;
		}

		//等到引擎加载队列数目为0，执行ClearMaterialLib,
		if (0 == Hippo_GetIRender()->GetWaitingTaskCount())
		{
			Hippo_GetIRender()->ClearMaterialLib();
			bAutoEnd = true;
		}
		return false;
	}

	virtual int OnKeyDown(unsigned int key)
	{
		if(HippoTestManager::GetInstance()->GetCurrentRunMode() )
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
ADD_TESTCASE("ASYN_LOAD_TEST",AsynTestCase7);

/*
测试逻辑
使用lod1创建人物，其他逻辑和AsynTestCase7相同
*/

class AsynTestCase8 : public AsynTestCase7
{
public:
	AsynTestCase8(const char* casename):AsynTestCase7(casename){}
	virtual int GetMatLod(){return 1;}
private:
	static HippoTestCaseBase* const testcase_;
};

ADD_TESTCASE("ASYN_LOAD_TEST",AsynTestCase8);


/*
测试逻辑
使用lod2创建人物，其他逻辑和AsynTestCase9相同
*/
class AsynTestCase9 : public AsynTestCase7
{
public:
	AsynTestCase9(const char* casename):AsynTestCase7(casename){}
	virtual int GetMatLod(){return 2;}
private:
	static HippoTestCaseBase* const testcase_;
};

ADD_TESTCASE("ASYN_LOAD_TEST",AsynTestCase9);
