#include "../HippoTestManager/HippoTestManager.h"
#include "../CommonCode/HippoUtil/HippoScene.h"
#include "../CommonCode/HippoUtil/RefCountCompare.h"
#include "../CommonCode/HippoUtil/HippoGlobal.h"
#include "../CommonCode/HippoUtil/ActorUtil.h"
#include "engine_interface.h"
#include "EffectCore_dx.h"
#include <iostream>

using namespace std;

/*
测试逻辑：
挂件的隐藏和显示
加载四个部位的挂件，隐藏左右手的挂件，其他两个位置的挂件不受影响
*/

class ShowHideAdorment:public HippoTestCaseBase
{
public:
	HippoLevelScene* m_pLevelScene;

	H3DI::IActor* pactor;
	H3DI::IModel* gModel;
	ISpecialEffect* gSpe;
	ShowHideAdorment(const char* case_name) : HippoTestCaseBase(case_name){}
	//
	virtual void InitScene()
	{
		Hippo_GetIRender()->OpenActionLib("../resources/art/role/actions/role.xml");
		m_pLevelScene=Hippo_GetSceneManager()->CreateEmptyLevel();

		pactor=m_pLevelScene->CreateActor(false,0);

		pactor->SetAdornment(H3DI::ACTOR_ADORNMENT_TAIL, "../resources/art/role/link/female/4112/4112001001/4112001001.spe");
		pactor->SetAdornment(H3DI::ACTOR_ADORNMENT_LEFTHAND, "../resources/art/role/link/female/4113/4113002001/4113002001.spe");
		pactor->SetAdornment(H3DI::ACTOR_ADORNMENT_RIGHTHAND, "../resources/art/role/link/female/4114/4114001001/4114001001.spe");
		pactor->SetAdornment(H3DI::ACTOR_ADORNMENT_BACK, "../resources/art/role/link/female/4115/4115001001/4115001001.spe");

		//ActorUtil::RandomLink(pactor,false);
		//pactor->SetAdornment((H3DI::EActorAdornmentPosition)5,"../resources/art/role/link/female/4115/4115012002/4115012002.spe");
		//gSpe=m_pLevelScene->CreateDml("../resources/art/stage/snowhouse001/model/snowhouse001_ground001.dml");

	}

	void CleanUpScene()
	{
		m_pLevelScene->CleanScene();
		Hippo_GetSceneManager()->DelScene(m_pLevelScene);

	}

	bool ManualRunExec()
	{
		return false;
	}

	virtual bool AutoRunExec()
	{
		if(Spin_N_Frame(150)) //什么都不做，10s钟后切换到下一个状态
		{
			m_state=CASE_END_EXECING;
			return true;
		}
		return false;
	}

	virtual int  OnKeyDown(unsigned int key)
	{

		switch( key )
		{
		case '1':
			{
				//pactor->SetAdornmentVisibility(H3DI::ACTOR_ADORNMENT_TAIL, false);
				pactor->SetAdornmentVisibility(H3DI::ACTOR_ADORNMENT_LEFTHAND, false);
				pactor->SetAdornmentVisibility(H3DI::ACTOR_ADORNMENT_RIGHTHAND, false);
				//pactor->SetAdornmentVisibility(H3DI::ACTOR_ADORNMENT_BACK, false);
				return 1;
			}
		case '2':
			{
				pactor->SetAdornmentVisibility(H3DI::ACTOR_ADORNMENT_LEFTHAND, true);
				pactor->SetAdornmentVisibility(H3DI::ACTOR_ADORNMENT_RIGHTHAND, true);
				return 1;
			}
		case '3':
			{
				return 1;
			}
		default:
			return 0;
		}

	}

private:
	static HippoTestCaseBase* const testcase_;
};

ADD_TESTCASE("ShowHideAdorment",ShowHideAdorment)

