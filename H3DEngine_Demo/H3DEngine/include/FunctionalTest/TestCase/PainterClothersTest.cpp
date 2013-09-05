/********************************************************************
	created:	2013/03/18
	created:	18:3:2013   16:43
	filename: 	H:\EngineCode\MultiThread\X52_Community_Version\TestCode\FunctionalTest\TestCase\PainterClothersTest.cpp
	file path:	H:\EngineCode\MultiThread\X52_Community_Version\TestCode\FunctionalTest\TestCase
	file base:	PainterClothersTest
	file ext:	cpp
	author:		lh
	
	purpose:	
*********************************************************************/
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
测试逻辑： 服装染色测试用例
1、基本的功能，指定给某一个服装、部位染色，效果正确
2、已经染色的部位，更换了新的服装或者挂件
3、已经染色的部位，重置了颜色，并更换了新的服装或者挂件


*/

class PainterClothersTest:public HippoTestCaseBase
{
public:
	HippoLevelScene* m_pLevelScene;

	H3DI::IActor* pactor;
	H3DI::IModel* gModel;
	ISpecialEffect* gSpe;
	PainterClothersTest(const char* case_name) : HippoTestCaseBase(case_name){}
	//
	virtual void InitScene()
	{
		Hippo_GetIRender()->OpenActionLib("../resources/art/role/actions/role.xml");
		m_pLevelScene=Hippo_GetSceneManager()->CreateEmptyLevel();
		pactor=m_pLevelScene->CreateActor(false,0);
		ActorUtil::InitActorDress("../resources/config/shared/item/dress.xml");

		float color0[4] = {0.0f, 2.0f, 1.0f, 1.0f};
		float colro1[4] = {0.0f, .0f, .0f, 1.f};
		pactor->SetDiffuseColorFactor(color0, 4, colro1, 4, 3);
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
		float color0[4] = {rand()%100*0.02f, 0.0f, rand()%100*0.02f, 1.0f};
		float colro1[4] = {0.0f, .0f, .0f, 1.f};

		switch( key )
		{
		case '1':
			{
				pactor->SetDiffuseColorFactor(color0, 4, colro1, 4, 3);
				pactor->SetDiffuseColorFactor(color0, 4, colro1, 4, H3DI::BODYPART_NUM+4);
				pactor->SetDiffuseColorFactor(color0, 4, colro1, 4, H3DI::BODYPART_NUM+5);
				return 1;
			}
		case '2':
			{
				pactor->SetDiffuseColorFactor(color0, 4, colro1, 4, 3);
				pactor->SetDiffuseColorFactor(color0, 4, colro1, 4, H3DI::BODYPART_NUM+4, true);
				pactor->SetDiffuseColorFactor(color0, 4, colro1, 4, H3DI::BODYPART_NUM+5, true);
				return 1;
			}
		case '4':
			{
				pactor->SetBodyPart("../resources/art/role/bodypart/female/hair/113005001/113005001.BPT");
				pactor->SetAdornment((H3DI::EActorAdornmentPosition)4,"../resources/art/role/link/female/4114/4114004003/4114004003.spe");
				pactor->SetAdornment((H3DI::EActorAdornmentPosition)5,"../resources/art/role/link/female/4115/4115004001/4115004001.spe");
				return 1;
			}
		case '3':
			{
				pactor->SetBodyPart("../resources/art/role/bodypart/female/hair/113069003/113069003.BPT");
				pactor->SetAdornment((H3DI::EActorAdornmentPosition)4,"../resources/art/role/link/female/4114/4114001001/4114001001.spe");
				pactor->SetAdornment((H3DI::EActorAdornmentPosition)5,"../resources/art/role/link/female/4115/4115001001/4115001001.spe");
				return 1;
			}
		default:
			return 0;
		}

	}

private:
	static HippoTestCaseBase* const testcase_;
};

ADD_TESTCASE("PainterClothersTest",PainterClothersTest)

