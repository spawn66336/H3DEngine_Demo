#include "../HippoTestManager/HippoTestManager.h"
#include "../CommonCode/HippoUtil/HippoScene.h"
#include "../CommonCode/HippoUtil/RefCountCompare.h"
#include "../CommonCode/HippoUtil/HippoGlobal.h"
#include "engine_interface.h"
#include "EffectCore_dx.h"
#include <iostream>

using namespace std;

/*
异步加载bodypart黑白格的测试
一个bodypart异步加载完成，然后有异步加载（这时会克隆）
然后cancel第一个bodypart，clearmatlib，
这时资源可能被释放(因为克隆时没加资源引用计数）
*/

class BodypartCloneAndCancel:public HippoTestCaseBase
{
public:
	HippoLevelScene* m_pLevelScene;

	H3DI::IActor* pactor;
	H3DI::IModel* gModel;
	ISpecialEffect* gSpe;

	int m_TaskId;
	int m_TaskId2;

	BodypartCloneAndCancel(const char* case_name) : HippoTestCaseBase(case_name){}
	//
	
	virtual void InitScene()
	{
		Hippo_GetIRender()->OpenActionLib("../resources/art/role/actions/role.xml");
		m_pLevelScene=Hippo_GetSceneManager()->CreateEmptyLevel();
		pactor=m_pLevelScene->CreateActor(false);

		m_TaskId = 0;
		m_TaskId2 = 0;


		Hippo_GetIRender()->Exec(99, "a", 0, NULL, 0);
		//HIPPO_EXPECT_EQ(pactor!=0,true);

		//gSpe=m_pLevelScene->CreateDml("../resources/art/stage/snowhouse001/model/snowhouse001_ground001.dml");
		m_state=CASE_EXECING;
	}

	void CleanUpScene()
	{
		m_pLevelScene->CleanScene();
		Hippo_GetSceneManager()->DelScene(m_pLevelScene);
		
	}

	int CreateTask()
	{
		H3DI::sCreateOp createOp;
		createOp.mat_lod = 0;
		createOp.geo_lod = 0;
		int taskId = Hippo_GetIRender()->LoadBodypartAsyn("../resources/art/role/bodypart/female/hair/113004001/113004001.bpt",H3DI::ACTOR_HUMAN, createOp,0);

		return taskId;
	}

	virtual bool AutoRunExec()
	{
		//if(Spin(10000)) //什么都不做，10s钟后切换到下一个状态
		//{
		//	m_state=CASE_END_EXECING;
		//	return true;
		//}
		return true;
	}

	virtual bool ManualRunExec()
	{
		Hippo_GetIRender()->UpdateEngineThread();

		static int a = 0;

		if (m_TaskId != 0)
		{
			unsigned int  res = Hippo_GetIRender()->QueryTaskState(m_TaskId);
			if (res==H3DI::TASK_OK || res==H3DI::TASK_ERROR)
			{
				// clone
				m_TaskId2 = CreateTask();

				// cancel task
				//Hippo_GetIRender()->CancelTask(m_TaskId);
				a = m_TaskId;
				m_TaskId = 0;

			}
		}

		if (m_TaskId2 != 0)
		{
			unsigned int  res = Hippo_GetIRender()->QueryTaskState(m_TaskId2);
			if (res==H3DI::TASK_OK || res==H3DI::TASK_ERROR)
			{

				// release resource
				//Hippo_GetIRender()->ClearMaterialLib();
				Hippo_GetIRender()->CancelTask(a);

				Hippo_GetIRender()->Exec(99, "b", 0, NULL, 0);
				// error cannot find resource
				//pactor->SetBodyPartAndFinishTask(m_TaskId2);

				Hippo_GetIRender()->CancelTask(m_TaskId2);

				Hippo_GetIRender()->Exec(99, "c", 0, NULL, 0);

				m_TaskId2 = 0;
			}

		}

		return true;
	}


	virtual int  OnKeyDown(unsigned int key)
	{
		switch( key )
		{
		case VK_RETURN:
			{
				m_state=CASE_END_EXECING;
				return 1;
			}
		case '1':
			{
				m_TaskId = CreateTask();
				return 1;
			}
	
		case '3':
			{
				pactor->SetBodyPart("../resources/art/role/bodypart/female/hair/113069003/113069003.BPT");
				return 1;
			}
		default:
			return 0;
		}

	}
	//!自定义的键盘消息处理函数，返回0表示对该消息不感兴趣，返回1表示已经处理，不需要默认消息处理函数再处理
	virtual int ProcessInput(unsigned int nChar, bool bKeyDown, bool bAltDown,bool bCrtlDown,bool bShiftDown, void* pUserContext )
	{
		if( bKeyDown )
		{
			switch( nChar )
			{
			case VK_RETURN:
				{
					m_state=CASE_END_EXECING;
					return 1;
				}
			case '1':
				{
					static float f=1.0f;
					f-=0.1f;
					gSpe->setTransparence(f);
					return 1;
				}
			case '2':
				{
					gModel->SetMaterialLod(2);
					return 1;
				}
			case '3':
				{
					pactor->SetBodyPart("../resources/art/role/bodypart/female/hair/113069003/113069003.BPT");
					return 1;
				}
			}
		}
		return 0;
	}

private:
	static HippoTestCaseBase* const testcase_;
};

ADD_TESTCASE("BodypartCloneAndCancel",BodypartCloneAndCancel)

