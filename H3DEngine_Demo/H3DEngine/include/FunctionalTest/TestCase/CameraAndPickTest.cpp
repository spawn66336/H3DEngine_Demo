#include "../HippoTestManager/HippoTestManager.h"
#include "../CommonCode/HippoUtil/HippoScene.h"
#include "../CommonCode/HippoUtil/RefCountCompare.h"
#include "../CommonCode/HippoUtil/HippoGlobal.h"
#include "engine_interface.h"
#include <iostream>
#include "dMathHeader.h"

using namespace std;

/*
测试逻辑：
1、加载一个模型，记录引擎当前引用计数
2、查看模型，按回车键删除模型，记录引擎当前引用计数
3、比较两个引用计数是否相等
*/

#define TEST_IN_VIEWPRT 0

class CameraAndPickTest:public HippoTestCaseBase
{
public:
	HippoLevelScene* m_pLevelScene;

	H3DI::IActor* pactor;

	unsigned short screenVp[4];
	unsigned short viewPort[4];

	H3DVec3 rayPos;
	H3DVec3 rayDir;

	CameraAndPickTest(const char* case_name) : HippoTestCaseBase(case_name){}
	//
	virtual void InitScene()
	{
		Hippo_GetIRender()->OpenActionLib("../resources/art/role/actions/role.xml");

		Hippo_GetIRender()->LoadCustomBoundingVolume("../resources/config/client/male_bv.xml");
		Hippo_GetIRender()->LoadCustomBoundingVolume("../resources/config/client/female_bv.xml");

		viewPort[0] = 100;
		viewPort[1] = 100;
		viewPort[2] = 300;
		viewPort[3] = 400;

		Hippo_GetIRender()->GetDynamicVB()->CreateBuffer(4096, 1024, 0, 0, NULL, NULL);

		m_pLevelScene=Hippo_GetSceneManager()->CreateEmptyLevel();

		//const char* path1="../resources/art/stage/snowhouse001/model/snowhouse001_ground001ao.dml";
		//m_pLevelScene->CreateDml(path1);

		pactor=m_pLevelScene->CreateActor(false);
		pactor->SetBodyPart("../resources/art/role/bodypart/female/shoe/118019002/118019002.bpt");
		pactor->SetBodyPart("../resources/art/role/bodypart/female/hair/113019003/113019003.BPT");
		pactor->SetBodyPart("../resources/art/role/bodypart/female/body/114030003/114030003.bpt");
		pactor->SetBodyPart("../resources/art/role/bodypart/female/trousers/116032002/116032002.bpt");

		float skinColor[] = {0.f, 0.f, 0.f, 0.f};
		pactor->SetSkinColor(skinColor);
	}

	void CleanUpScene()
	{
		m_pLevelScene->CleanScene();
		Hippo_GetSceneManager()->DelScene(m_pLevelScene);
	}

	H3DI::ILevel* GetILevel()
	{
		return (H3DI::ILevel*)m_pLevelScene->GetH3DScene();
	}

	virtual bool ManualRunExec()
	{

		return false;
	}
	virtual bool AutoRunExec()
	{
		if(Spin_N_Frame(150))
		{
			m_state=CASE_END_EXECING;
			return true;
		}
		return false;
	}

	virtual void BeforeRender()
	{
#if TEST_IN_VIEWPRT
		Hippo_GetIRender()->GetViewPort(screenVp);
		Hippo_GetIRender()->SetViewport(viewPort);
		Hippo_GetIRender()->BeginViewPort();
#endif
	}


	virtual void AfterRender()
	{
#if TEST_IN_VIEWPRT
		float color[] = {1.f, 1.f, 1.f, 1.f};
		H3DVec3 end = rayDir * 100.f;
		Hippo_GetIRender()->DrawSegment(rayPos + H3DVec3(0.f, 0.f, 0.001f), rayPos + end, color, 1.f);

		Hippo_GetIRender()->EndViewPort();
		Hippo_GetIRender()->SetViewport(screenVp);
#endif
	}



	int OnKeyDown(unsigned int key)
	{

		switch( key )
		{
		case VK_RETURN:
			{
				m_state=CASE_END_EXECING;
				return 1;
			}
		default:
			return 0;
		}
	}



	int OnLeftDown(HippoMouseEvent& e)
	{
		pactor->SetUseCustomBoundBox(true);

#if TEST_IN_VIEWPRT
		int xPosInViewport = xPos - viewPort[0];
		int yPosInViewport = yPos + viewPort[1] + viewPort[3] - screenVp[3];
		GetILevel()->GetMouseRay(xPosInViewport, yPosInViewport, viewPort[2], viewPort[3], &rayPos, &rayDir);
#else
		GetILevel()->GetMouseRay(e.PosX, e.PosY, 0, 0, &rayPos, &rayDir);
#endif
		//Hippo_GetIRender()->GetMouseRayDir(xPosInViewport, yPosInViewport, &rayPos, &rayDir);

		if (pactor->RayHitTestWithAABB(rayPos, rayDir))
		{
			float skinColor[] = {1.f, 0.f, 0.f, 0.f};
			pactor->SetSkinColor(skinColor);

		}
		else
		{
			float skinColor[] = {0.f, 0.f, 0.f, 0.f};
			pactor->SetSkinColor(skinColor);
		}

		pactor->SetUseCustomBoundBox(false);
		return 1;
	}

private:
	static HippoTestCaseBase* const testcase_;
};

ADD_TESTCASE("CameraAndPickTest",CameraAndPickTest)

