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
测试逻辑：
发起很多异步task（确保有task在waiting队列），cancel掉waiting队列中的task，task会立刻被释放（且cancel成功）
canel完成队列中的task，task也会立刻被释放，（但是cancel失败）
*/
class TaskCancelTest : public HippoTestCaseBase
{
public:
	HippoLevelScene* m_pScene;

	vector<unsigned int> m_TaskIds;

	TaskCancelTest(const char* case_name) : HippoTestCaseBase(case_name){}

	virtual bool InitTestCase()
	{
		m_pScene=Hippo_GetSceneManager()->CreateEmptyLevel();

		//create actor
		Hippo_GetIRender()->OpenActionLib("../resources/art/role/actions/role.xml");
		//ActorUtil::InitActorDress("../resources/config/shared/item/dress.xml");
		//ActorUtil::InitPetDress("../resources/config/shared/item/petdress.xml");
		
		m_pScene->RestructScene();

		H3DI::sCreateOp op;
		op.geo_lod = 0;
		op.mat_lod = 0;

		for (int i = 0; i < 100; i++)
		{
			unsigned int id = Hippo_GetIRender()->LoadBodypartAsyn("../resources/art/role/bodypart/female/hair/113069003/113069003.BPT", H3DI::ACTOR_HUMAN, op, 0);
			m_TaskIds.push_back(id);
		}

		unsigned int id = m_TaskIds.back();
		int numWaiting = Hippo_GetIRender()->GetWaitingTaskCount();
		Hippo_GetIRender()->CancelTask(id);
		int numWaiting2 = Hippo_GetIRender()->GetWaitingTaskCount();

		HIPPO_EXPECT_EQ(numWaiting, numWaiting2 + 1);


		m_state=CASE_EXECING;
		return true;
	}

	virtual bool CleanUpTestCase()
	{
		return true;
	}

	//
	virtual void InitScene()
	{
	}

	void CleanUpScene()
	{
		m_pScene->CleanScene();
		Hippo_GetSceneManager()->DelScene(m_pScene);
	}

	virtual bool AutoRunExec()
	{
		//if(Spin(10000))
		//{
		//	m_state=CASE_END_EXECING;
		//	return true;
		//}
		return true;
	}


	virtual bool ManualRunExec()
	{
		return false;
	}


	virtual void BeforeRender()
	{
		Hippo_GetIRender()->UpdateCpuSkin();
		Hippo_GetIRender()->ForceSyncData();
		Hippo_GetIRender()->UpdatePhx();

		Hippo_GetIRender()->UpdateEngineThread();

		unsigned int id = m_TaskIds.front();

		int state = Hippo_GetIRender()->QueryTaskState(id);

		if (state == H3DI::TASK_OK)
		{
			// cancel task
			Hippo_GetIRender()->CancelTask(id);

		}
	}


	virtual int OnKeyDown(unsigned int key)
	{
		switch( key )
		{
		case VK_RETURN:
			{
				//m_state=CASE_END_EXECING;
				return 1;
			}
		case VK_LEFT:
			{

				return 1;
			}
		case VK_RIGHT:
			{

				return 1;
			}
		case VK_DOWN:
			{
				m_pScene->CleanScene();
			}
		}
		return 0;
	}

	//!自定义的键盘消息处理函数，返回0表示对该消息不感兴趣，返回1表示已经处理，不需要默认消息处理函数再处理
	virtual int ProcessInput(unsigned int nChar, bool bKeyDown, bool bAltDown,bool bCrtlDown,bool bShiftDown, void* pUserContext )
	{
		return 0;
	}

private:
	static HippoTestCaseBase* const testcase_;
};

ADD_TESTCASE("TaskCancelTest",TaskCancelTest)

