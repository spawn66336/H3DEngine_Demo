#include "../HippoTestManager/HippoTestManager.h"
#include "../CommonCode/HippoUtil/HippoScene.h"
#include "../CommonCode/HippoUtil/RefCountCompare.h"
#include "../CommonCode/HippoUtil/HippoGlobal.h"
#include "engine_interface.h"
#include "engine_interface_internal.h"
#include "EffectCore_dx.h"
#include "../CommonCode/HippoUtil/ActorUtil.h"
#include <iostream>
#include "../CommonCode/HippoUtil/HippoStateMachine.h"
#include "../CommonCode/HippoUtil/HippoTaskVisitor.h"
#include "dMathHeader.h"
using namespace std;

/*
测试逻辑：
载入一个actor
加上服装，挂件
创建1个特效
渲染5s
特效绑定到人物身上

渲染人物时应该绘制出绑定的特效
*/

class ActorEffectTest:public HippoTestCase_RefCompare
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
	H3DI::IMatProxy* pShaderDec;
	ISpecialEffect* pspe;
	inner_state m_inner_state;

	ActorEffectTest(const char* case_name) : HippoTestCase_RefCompare(case_name){}
	//
	virtual void InitScene()
	{
		pspe=0;
		Hippo_GetIRender()->OnCommandLine("SetEngineConfig EnableLpp ture1");
		
		m_pLevelScene=Hippo_GetSceneManager()->CreateEmptyLevel();
		ActorUtil::InitActorDress("../resources/config/shared/item/dress.xml");
		
		pactor=m_pLevelScene->CreateActor(true);
		//ActorUtil::RandomBodypart(pactor,true);
		//ActorUtil::RandomLink(pactor,true);
		HippoResStateSnapShot* m_pSt=RefCountCompare::GetResState(HIPPO_MAT_RES);
		pspe=Hippo_GetIRender()->CreateSpe("../resources/art/effect/mode_srw/mode_srw_xin5.spe");
		unsigned int f=pspe->getState();
		pspe->setState(f|EFFECT_ACTIVATED);
		pspe->update(0);

		m_inner_state=STATE0;

	}

	void CleanUpScene()
	{
		m_pLevelScene->CleanScene();
		Hippo_GetSceneManager()->DelScene(m_pLevelScene);
	}


	bool ManualRunExec()
	{
		if(pspe)
			pspe->update(0.001f * Hippo_GetFrameTime());

		if(m_inner_state==STATE0)
		{
			if(Spin_N_Frame(150)) //什么都不做，render 1s
			{
				m_inner_state=STATE1;
				EffectActor a[1];
				a[0].actor=pactor;
				pspe->setActor(a,1);
			}
		}
		else if (m_inner_state==STATE1)
		{
			if(Spin_N_Frame(150)) //什么都不做，render 1s
			{
				pspe->unsetActor();
				m_inner_state=STATE2;
			}
			
		}
		else if (m_inner_state==STATE2)
		{
			if(Spin_N_Frame(150)) //什么都不做，render 1s
			{
				pspe->Release();
				pspe=0;
				m_pLevelScene->DelActor(pactor);
				Hippo_GetIRender()->ClearMaterialLib();
				m_inner_state=STATE3;
			}

		}
		else if(m_inner_state==STATE3)
		{
			if(Spin_N_Frame(150)) //什么都不做，render 1s
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

ADD_TESTCASE("ActorEffectTest",ActorEffectTest)



class ActorEffectTest2:public HippoTestCase_RefCompare
{
public:

	HippoLevelScene* m_pLevelScene;


	HippoTaskVistor m_task_query;
	Hippo_StateMachine sm;
	std::vector<ISpecialEffect*> all_spe;
	std::vector<H3DI::IActor*> all_actor;
	ActorEffectTest2(const char* case_name) : HippoTestCase_RefCompare(case_name){}


	SCSTATE OnState0()
	{
		if(Spin_N_Frame(150)) //什么都不做，render 1s
		{
			for (size_t i=0;i<all_spe.size();++i)
			{
				H3DI::IActor* pactor=all_actor.at(i);
				EffectActor a[1];
				a[0].actor=pactor;
				all_spe.at(i)->setActor(a,1);
			}
			return STATE_CHANGE;
		}
		return STATE_NOT_CHANGE;
	}

	SCSTATE OnState1()
	{
		if(Spin_N_Frame(150)) //什么都不做，render 1s
		{
			for (size_t i=0;i<all_spe.size();++i)
			{
				all_spe.at(i)->unsetActor();
			}
			return STATE_CHANGE;
		}
		return STATE_NOT_CHANGE;
	}

	SCSTATE OnState2()
	{
		if(Spin_N_Frame(150)) //什么都不做，render 1s
		{
			for (size_t i=0;i<all_spe.size();++i)
			{
				all_spe.at(i)->Release();
				m_pLevelScene->DelActor(all_actor.at(i));
				Hippo_GetIRender()->ClearMaterialLib();
				
			}
			all_spe.clear();
			return STATE_CHANGE;
		}
		return STATE_NOT_CHANGE;
	}

	virtual void InitScene()
	{
		sm.AddState(0,std::tr1::bind(&ActorEffectTest2::OnState0,this));
		sm.AddState(1,std::tr1::bind(&ActorEffectTest2::OnState1,this));
		sm.AddState(2,std::tr1::bind(&ActorEffectTest2::OnState2,this));
		
		m_pLevelScene=Hippo_GetSceneManager()->CreateEmptyLevel();
		

		all_spe.push_back(Hippo_GetIRender()->CreateSpe("../resources/art/effect/mode_srw/mode_srw_xin5.spe"));
		all_spe.push_back(Hippo_GetIRender()->CreateSpe("../resources/art/effect/mode_srw/mode_srw_xin5.spe"));
		all_spe.push_back(Hippo_GetIRender()->CreateSpe("../resources/art/effect/mode_srw/mode_srw_xin5.spe"));
		for (size_t i=0;i<all_spe.size();++i)
		{
			ISpecialEffect* p=all_spe.at(i);
			unsigned int f=p->getState();
			p->setState(f|EFFECT_ACTIVATED);
			p->update(0);
		}

		H3DI::IActor* a1=m_pLevelScene->CreateActor(false,0);
		H3DI::IActor* a2=m_pLevelScene->CreateActor(false,0);
		H3DI::IActor* a3=m_pLevelScene->CreateActor(false,0);
		a1->SetPosition(H3DVec3(-2,0,0));
		a2->SetPosition(H3DVec3(0,0,0));
		a3->SetPosition(H3DVec3(2,0,0));
		all_actor.push_back(a1);
		all_actor.push_back(a2);
		all_actor.push_back(a3);
	}

	void CleanUpScene()
	{
		m_pLevelScene->CleanScene();
		Hippo_GetSceneManager()->DelScene(m_pLevelScene);
	}

	bool ManualRunExec()
	{
		for (size_t i=0;i<all_spe.size();++i)
		{
			ISpecialEffect* pspe=all_spe.at(i);
			pspe->update(0.001f * Hippo_GetFrameTime());
		}

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

ADD_TESTCASE("ActorEffectTest2",ActorEffectTest2)

