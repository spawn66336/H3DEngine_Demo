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


class RenderTargetTest:public HippoTestCase_RefCompare
{
public:
	HippoLevelScene* m_pLevelScene;

	H3DI::IActor* pactor;
	H3DI::IModel* gModel;
	ISpecialEffect* gSpe;
	H3DI::IRenderTarget* ptarget;

	RenderTargetTest(const char* case_name) : HippoTestCase_RefCompare(case_name){}
	//
	virtual void InitScene()
	{
		Hippo_GetIRender()->OpenActionLib("../resources/art/role/actions/role.xml");
		m_pLevelScene=Hippo_GetSceneManager()->CreateEmptyLevel();
		//pactor=m_pLevelScene->CreateActor(false);
		pactor=m_pLevelScene->CreateActor(false,0);
		ActorUtil::InitActorDress("../resources/config/shared/item/dress.xml");

		unsigned int w=512;
		unsigned int h=512;
		ptarget=Hippo_GetIRender()->CreateRenderTarget(w,h,H3DI::FMT_RGBA8,H3DI::RT_COLOR);
		//ptarget->Activate(0);
		//ptarget->Deactivate();

		//ActorUtil::RandomLink(pactor,false);
		//pactor->SetAdornment((H3DI::EActorAdornmentPosition)5,"../resources/art/role/link/female/4115/4115012002/4115012002.spe");
		//gSpe=m_pLevelScene->CreateDml("../resources/art/stage/snowhouse001/model/snowhouse001_ground001.dml");

	}

	void CleanUpScene()
	{
		ptarget->Release();
		m_pLevelScene->CleanScene();
		Hippo_GetSceneManager()->DelScene(m_pLevelScene);

	}

	virtual void BeforeRender()
	{
		ptarget->Activate(0);
		Hippo_GetIRender()->ClearScreen();
	}

	virtual void AfterRender()
	{
		ptarget->Deactivate();
	}

	bool ManualRunExec()
	{
		bool res=false;
		
		if(Spin_N_Frame(150))
		{
			ptarget->SaveToFile("./Test_rt.bmp");
			ptarget->SaveToFile("./test_rt.pNg");
			ptarget->SaveToFile("./test_rt.jpG");
			ptarget->SaveToFile("./test_rt.DDS");
			ptarget->SaveToFile("./test_rt.tga");
			ptarget->SaveToFile("./test_rt.abc");
			m_state=CASE_END_EXECING;
			res=true;
		}
		
		return res;
	}

	virtual bool AutoRunExec()
	{
		return ManualRunExec();
	}


private:
	static HippoTestCaseBase* const testcase_;
};

ADD_TESTCASE("RenderTargetTest",RenderTargetTest)
