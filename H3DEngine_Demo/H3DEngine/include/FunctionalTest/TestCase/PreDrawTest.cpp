#include "../HippoTestManager/HippoTestManager.h"
#include "../CommonCode/HippoUtil/HippoScene.h"
#include "../CommonCode/HippoUtil/RefCountCompare.h"
#include "../CommonCode/HippoUtil/HippoGlobal.h"
#include "engine_interface.h"
#include "engine_interface_internal.h"
#include "EffectCore_dx.h"
#include "../CommonCode/HippoUtil/ActorUtil.h"
#include <iostream>

using namespace std;

/*
测试逻辑：
加载一个人物，先绘制几帧
然后执行预绘制接口，看是不是有fx重新生成，如果有新的生成不通过

*/

class PreDrawTest:public HippoTestCaseBase
{
public:
	enum InnerState
	{
		INIT,
		//素体lod0
		CREATE_STRIPACTOR_LOD0,
		//素体lod0空转渲染
		EMPTY_SPIN_STRIPACTOR_LOD0,
		//挂接在actor上的特效
		ADD_SPE,
		ADD_SPE_SPIN,
		SET_SPE_ACTOR,
		PRERENDER_SPE,
		EMPTY_SPIN_SPE,

		//素体lod0预渲染
		PRERENDER_STRIPACTOR_LOAD0,
		//服装lod0
		ADD_BODYPART_LOD0,
		//服装lod0空转渲染
		EMPTY_SPIN_BODYPART_LOD0,
		//服装lod0预渲染
		PRERENDER_BODYPART_LOD0,
		//挂件lod0
		ADD_LINK_LOD0,
		//挂件lod0空转渲染
		EMPTY_SPIN_LINK_LOD0,
		//挂件lod0预渲染
		PRERENDER_LINK_LOD0,

		//清空lod0
		RESET_LOD0,


		INNER_END

	};

	InnerState m_inner_state;
	HippoLevelScene* m_pLevelScene;

	H3DI::IActor* pactor;
	H3DI::IModel* gModel;
	H3DI::IGlobalEnvironment* pGlobalEnv;
	bool m_bActorSex;
	ISpecialEffect* spe;
	PreDrawTest(const char* case_name) : HippoTestCaseBase(case_name){}
	//Hippo_GetIRender()->OnCommandLine("SetEngineConfig usePostProcess ture");
	//Hippo_GetIRender()->OnCommandLine("SetEngineConfig usePostProcess false");
	virtual bool InitScene()
	{
		Hippo_GetIRender()->OnCommandLine("SetEngineConfig EnableLpp ture1");
		Hippo_GetIRender()->OpenActionLib("../resources/art/role/actions/role.xml");
		m_pLevelScene=Hippo_GetSceneManager()->CreateEmptyLevel();
		m_inner_state=INIT;
		pactor=0;
		pGlobalEnv=Hippo_GetIProxyFactory()->GetGlobalEnv();
		ActorUtil::InitActorDress("../resources/config/shared/item/dress.xml");
		m_bActorSex=false;
		m_state=CASE_EXECING;
		return true;
	}
	void CreateActor(int lod)
	{
		pactor=m_pLevelScene->CreateActor(m_bActorSex,lod);
	}
	bool CleanUpScene()
	{
		m_pLevelScene->CleanScene();
		Hippo_GetSceneManager()->DelScene(m_pLevelScene);
		return true;
	}

	void InnerStateIncr()
	{
		int* p=(int*)(&m_inner_state);
		++(*p);
	}

	void PreRender(int lod)
	{
		unsigned int n1=pGlobalEnv->GetLastGenFxTimeStamp(4);
		pactor->SetFakeRender(lod==1);
		unsigned int n2=pGlobalEnv->GetLastGenFxTimeStamp(4);
		//这里确定没有fx生成
		HIPPO_EXPECT_EQ(n1,n2);
	}

	void SetBp()
	{
		ActorUtil::RandomBodypart(pactor,m_bActorSex);
		//pactor->SetBodyPart("../resources/art/role/bodypart/female/body/114030003/114030003.bpt");
	}

	void SetLink()
	{
		ActorUtil::RandomLink(pactor,m_bActorSex);
	}

	void AddSpe()
	{
		spe=m_pLevelScene->CreateSpe("../resources/art/effect/mode_srw/mode_srw_xin1.spe");
		
	}

	void SetSpe()
	{
		EffectActor a[1];
		a[0].actor=pactor;
		spe->setActor(a,1);
	}
	bool TestLogic()
	{
		switch(m_inner_state)
		{
		case INIT:
			InnerStateIncr();
			break;
		case CREATE_STRIPACTOR_LOD0:
			CreateActor(0);
			InnerStateIncr();
			break;
		case EMPTY_SPIN_STRIPACTOR_LOD0:
			if(Spin_N_Frame(150))
			{
				PreRender(1);
				InnerStateIncr();
			}
			break;
		case ADD_SPE:
			AddSpe();
			InnerStateIncr();
			break;
		case ADD_SPE_SPIN:
			if(Spin_N_Frame(150))
			{
				InnerStateIncr();
			}
			break;
		case SET_SPE_ACTOR:
			SetSpe();
			InnerStateIncr();
			break;
		case PRERENDER_SPE:
			//PreRender(1);
			if(Spin_N_Frame(150))
			{
				PreRender(0);
				InnerStateIncr();
			}
			break;
		case EMPTY_SPIN_SPE:
			if(Spin_N_Frame(150))
			{
				InnerStateIncr();
			}
			break;
		case PRERENDER_STRIPACTOR_LOAD0:
			
			InnerStateIncr();
			break;
		case ADD_BODYPART_LOD0:
			SetBp();
			InnerStateIncr();
			break;
		case EMPTY_SPIN_BODYPART_LOD0:
			if(Spin_N_Frame(150))
			{
				InnerStateIncr();
			}
			break;
		case PRERENDER_BODYPART_LOD0:
			PreRender(1);
			if(Spin_N_Frame(150))
			{
				PreRender(0);
				InnerStateIncr();
			}

			break;
		case ADD_LINK_LOD0:
			SetLink();
			InnerStateIncr();
			break;
		case EMPTY_SPIN_LINK_LOD0:
			if(Spin_N_Frame(150))
			{
				InnerStateIncr();
			}
			break;
		case PRERENDER_LINK_LOD0:
			PreRender(1);
			if(Spin_N_Frame(150))
			{
				PreRender(0);
				InnerStateIncr();
			}
			break;
		case RESET_LOD0:
			m_pLevelScene->CleanScene();
			Hippo_GetIRender()->ClearMaterialLib();
			InnerStateIncr();
			break;

		default:
			return true;
		}
		return false;
	}

	bool ManualRunExec()
	{

		return TestLogic();
	}

	virtual bool AutoRunExec()
	{

		return TestLogic();
	}




private:
	static HippoTestCaseBase* const testcase_;
};

ADD_TESTCASE("PreDrawTest",PreDrawTest)

