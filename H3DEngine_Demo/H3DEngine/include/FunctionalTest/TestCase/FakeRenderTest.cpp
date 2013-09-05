#include "../HippoTestManager/HippoTestManager.h"
#include "../CommonCode/HippoUtil/HippoScene.h"
#include "../CommonCode/HippoUtil/RefCountCompare.h"
#include "../CommonCode/HippoUtil/HippoGlobal.h"
#include "engine_interface.h"
#include "engine_interface_internal.h"
#include "EffectCore_dx.h"
#include "../CommonCode/HippoUtil/ActorUtil.h"
#include "../CommonCode/HippoUtil/HippoStateMachine.h"
#include "../CommonCode/HippoUtil/HippoTaskVisitor.h"
#include <iostream>

using namespace std;

/*

测试逻辑：
载入一个actor，设置actor为fake render
异步加载8个服装
所有异步任务完成以后，把人物fake render 关闭
渲染5s
观察效果，所有的部位应该是一起出现在画面中

*/


class FakeRenderTest:public HippoTestCase_RefCompare
{
public:

	HippoLevelScene* m_pLevelScene;

	H3DI::IActor* pactor;
	HippoTaskVistor m_task_query;
	Hippo_StateMachine sm;

	FakeRenderTest(const char* case_name) : HippoTestCase_RefCompare(case_name){}

	virtual void SetPhy(){}
	
	int OnBodyPartLoadFinish(unsigned int taskid)
	{
		pactor->SetBodyPartAndFinishTask(taskid);
		return 1;
	}
	SCSTATE OnState0()
	{
		if(Spin_N_Ms(5000))
		{
			return STATE_CHANGE;
		}
		return STATE_NOT_CHANGE;
	}

	SCSTATE OnState1()
	{
		pactor->SetFakeRender(false);
		Hippo_WriteConsole(CC_WHITE,"Set Fake Render = false");
		return STATE_CHANGE;
	}

	SCSTATE OnState3()
	{
		pactor->SetFakeRender(true);
		Hippo_WriteConsole(CC_WHITE,"Set Fake Render = true");
		return STATE_CHANGE;
	}

	virtual void InitScene()
	{
		sm.AddState(0,std::tr1::bind(&FakeRenderTest::OnState0,this));
		sm.AddState(1,std::tr1::bind(&FakeRenderTest::OnState1,this));
		sm.AddState(2,std::tr1::bind(&FakeRenderTest::OnState0,this));
		sm.AddState(3,std::tr1::bind(&FakeRenderTest::OnState3,this));

		m_pLevelScene=Hippo_GetSceneManager()->CreateEmptyLevel();
		SetPhy();
		pactor=m_pLevelScene->CreateActor(false,0);
		pactor->SetFakeRender(true);
		Hippo_WriteConsole(CC_WHITE,"Set Fake Render = true");

		TaskFinishCallback cb=std::tr1::bind(&FakeRenderTest::OnBodyPartLoadFinish,this,std::placeholders::_1);
		m_task_query.PushLoadBodyPartTask("../resources/art/role/bodypart/female/hand/115002001/115002001.bpt",H3DI::ACTOR_HUMAN, 0,0,cb);
		m_task_query.PushLoadBodyPartTask("../resources/art/role/bodypart/female/trousers/116004001/116004001.bpt",H3DI::ACTOR_HUMAN, 0,0,cb) ;
		m_task_query.PushLoadBodyPartTask("../resources/art/role/bodypart/female/suit/119005001/1193005001/1193005001.bpt",H3DI::ACTOR_HUMAN, 0,0,cb) ;
		m_task_query.PushLoadBodyPartTask("../resources/art/role/bodypart/female/strippedbody/11a001.BPT",H3DI::ACTOR_HUMAN, 0,0,cb) ;
		m_task_query.PushLoadBodyPartTask("../resources/art/role/bodypart/female/shoe/118006001/118006001.bpt",H3DI::ACTOR_HUMAN, 0,0,cb) ;
		m_task_query.PushLoadBodyPartTask("../resources/art/role/bodypart/female/hair/113004001/113004001.BPT",H3DI::ACTOR_HUMAN, 0,0,cb) ;
		m_task_query.PushLoadBodyPartTask("../resources/art/role/bodypart/female/face/112002001/112002001.BPT",H3DI::ACTOR_HUMAN, 0,0,cb) ;
		m_task_query.PushLoadBodyPartTask("../resources/art/role/bodypart/female/body/114001001/114001001.BPT",H3DI::ACTOR_HUMAN, 0,0,cb) ;


	}

	void CleanUpScene()
	{
		m_pLevelScene->CleanScene();
		Hippo_GetSceneManager()->DelScene(m_pLevelScene);
	}

	bool ManualRunExec()
	{
		m_task_query.Update();
		sm.Run();

		return false;
	}

	virtual bool AutoRunExec()
	{

		return ManualRunExec();
		return false;
	}

private:
	static HippoTestCaseBase* const testcase_;
};

ADD_TESTCASE("FakeRenderTest",FakeRenderTest)


/*

测试逻辑：
载入一个actor
渲染5s
设置actor为fake render，此时应该看不见人物
加载8个服装，各个部位的挂件，此时依然应该看不见
把人物fake render 关闭，此时应该能看见
渲染5s
*/
class FakeRenderTest2:public HippoTestCase_RefCompare
{
public:
	enum inner_state
	{
		STATE0,
		STATE1,
		STATE2,
		STATE3,
	};

	HippoLevelScene* m_pLevelScene;

	H3DI::IActor* pactor;
	H3DI::IModel* gModel;
	int m_inner_state;
	std::vector<unsigned int> m_all_task;


	FakeRenderTest2(const char* case_name) : HippoTestCase_RefCompare(case_name){}
	//
	virtual void InitScene()
	{
		m_pLevelScene=Hippo_GetSceneManager()->CreateEmptyLevel();

		pactor=m_pLevelScene->CreateActor(false,0);
		

		ActorUtil::InitActorDress("../resources/config/shared/item/dress.xml");
		H3DI::sCreateOp createOp;
		createOp.mat_lod = 0;
		createOp.geo_lod = 0;
// 		pactor->SetBodyPart("../resources/art/role/bodypart/female/hand/115002001/115002001.bpt") ;
// 		pactor->SetBodyPart("../resources/art/role/bodypart/female/trousers/116004001/116004001.bpt") ;
// 		pactor->SetBodyPart("../resources/art/role/bodypart/female/suit/119005001/1193005001/1193005001.bpt") ;
// 		pactor->SetBodyPart("../resources/art/role/bodypart/female/strippedbody/11a001.BPT") ;
// 		pactor->SetBodyPart("../resources/art/role/bodypart/female/shoe/118006001/118006001.bpt") ;
// 		pactor->SetBodyPart("../resources/art/role/bodypart/female/hair/113004001/113004001.BPT") ;
// 		pactor->SetBodyPart("../resources/art/role/bodypart/female/face/112002001/112002001.BPT") ;
// 		pactor->SetBodyPart("../resources/art/role/bodypart/female/body/114001001/114001001.BPT") ;

		ActorUtil::RandomLink(pactor,false);
		m_inner_state=STATE0;

	}

	void CleanUpScene()
	{
		m_pLevelScene->CleanScene();
		Hippo_GetSceneManager()->DelScene(m_pLevelScene);
	}


	void NextState()
	{
		(int)m_inner_state+=1;
	}

	bool ManualRunExec()
	{
		if(m_inner_state==STATE0)
		{
			if(Spin_N_Ms(5000))
			{
				NextState();
			}
		}
		else if(m_inner_state==STATE1)
		{
			if(m_all_task.empty())
			{
				pactor->SetFakeRender(true);
				Hippo_WriteConsole(CC_WHITE,"Set Fake Render = true");
				m_inner_state=STATE2;
			}
		}
		else if(m_inner_state==STATE2)
		{
			if(Spin_N_Ms(5000))
			{
				pactor->SetFakeRender(false);
				Hippo_WriteConsole(CC_WHITE,"Set Fake Render = false");
				NextState();
			}
		}
		else if(m_inner_state==STATE3)
		{
			if(Spin_N_Ms(5000))
			{
				return true;
			}
		}
		return false;
	}

	virtual bool AutoRunExec()
	{

		return ManualRunExec();
		return false;
	}

private:
	static HippoTestCaseBase* const testcase_;
};
ADD_TESTCASE("FakeRenderTest",FakeRenderTest2)


class FakeRenderTest3:public FakeRenderTest
{
public:
	bool bOrgPhyxOpen;
	FakeRenderTest3(const char* casename):FakeRenderTest(casename){}
	virtual void SetPhy()
	{
		bOrgPhyxOpen=Hippo_GetIGlobalEnv()->IsPhyXClothOpen();
		Hippo_GetIGlobalEnv()->SetPhyXClothOpen(false);
	}

	void CleanUpScene()
	{
		Hippo_GetIGlobalEnv()->SetPhyXClothOpen(bOrgPhyxOpen);
		FakeRenderTest::CleanUpScene();
	}

private:
	static HippoTestCaseBase* const testcase_;
};

ADD_TESTCASE("FakeRenderTest",FakeRenderTest3)