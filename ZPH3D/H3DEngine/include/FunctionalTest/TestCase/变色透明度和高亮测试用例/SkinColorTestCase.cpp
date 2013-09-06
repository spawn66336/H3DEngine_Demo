#include "../../HippoTestManager/HippoTestManager.h"
#include "../CommonCode/HippoUtil/ActorUtil.h"
#include "../CommonCode/HippoUtil/HippoGlobal.h"
#include "../CommonCode/HippoUtil/HippoScene.h"
#include "engine_interface_internal.h"
#include "EffectCore_dx.h"
#include "idMath/dMathHeader.h"

using namespace  std;
/*测试逻辑
以lod0，lod1，lod2的方式分别载入一个actor（actor0，actor1，actor2），给这3个人穿上同样的一件上衣。
Actor0、actor1，actor2的皮肤染成红色 SetSkinColor
渲染5s
执行检查点1。
渲染10帧以后，Actor0切换成lod1，actor1切换成lod2，actor2切换成lod0。
渲染5s
执行检查点1。
渲染10帧以后，Actor0切换成lod2，actor1切换成lod0，actor2切换成lod1。
渲染5s
执行检查点1。
检查点1：
　　检查actor的各个renderable的shader中的SkinColor的shaderconst的值是红色。
*/
class SkinColorTestCase : public HippoTestCaseBase
{
public:
	SkinColorTestCase(const char* casename):HippoTestCaseBase(casename){}
	H3DI::IActor* m_pActor0_lod0;
	H3DI::IActor* m_pActor1_lod1;
	H3DI::IActor* m_pActor2_lod2;

	HippoScene* m_pScene;

	enum ExeState
	{
		INIT,
		CREATACTOR,
		SPIN_5S_FIRST,
		TEST_FIRST,
		SPIN_10F_FIRST,
		ACTOR_SET_LOD_FIRST,
		SPIN_5S_SECOND,
		TEST_SECOND,
		SPIN_10F_SECOND,
		ACTOR_SET_LOD_SECOND,
		SPIN_5S_THIRD,
		TEST_THIRD,
		TEST_OVER
	};
	ExeState m_exeState;
private:
	static HippoTestCaseBase* const testcase_;
public:
	virtual void InitScene()
	{
		m_pScene = Hippo_GetSceneManager()->CreateEmptyLevel();
		Hippo_GetIRender()->OpenActionLib("../resources/art/role/actions/role.xml");
		m_exeState  =  INIT;
	}

	virtual void CleanUpScene()
	{
		m_pScene->CleanScene();
		Hippo_GetSceneManager()->DelScene(m_pScene);
	}

	virtual bool AutoRunExec()
	{
		if (m_exeState == TEST_OVER)
		{
			return true;
		}
		return ManualRunExec();
	}

	void NextState()
	{
		int* p = (int*)(&m_exeState);
		++(*p);
	}

	void CreatActor()
	{
		H3DVec3 pos0(-2.f,0.f,0.f);
		H3DVec3 pos2(2.f,0.f,0.f);

		m_pActor0_lod0 = m_pScene->CreateActor(false,0);
		m_pActor1_lod1 = m_pScene->CreateActor(false,1);
		m_pActor2_lod2 = m_pScene->CreateActor(false,2);

		m_pActor0_lod0->SetPosition(pos0);
		m_pActor2_lod2->SetPosition(pos2);

		m_pActor0_lod0->SetBodyPart("");
		m_pActor1_lod1->SetBodyPart("");
		m_pActor2_lod2->SetBodyPart("");

		float red[] = {255,0,0,255};
		m_pActor0_lod0->SetSkinColor(red);
		m_pActor1_lod1->SetSkinColor(red);
		m_pActor2_lod2->SetSkinColor(red);
		Hippo_WriteConsole(CC_WHITE,"创建人物完成");
	}

	void TestPoint()
	{
		//**************************
		//这个函数还没有实现
	}
	virtual bool ManualRunExec()
	{
		switch (m_exeState)
		{
		case INIT:
			NextState();
			break;
		case CREATACTOR:
			CreatActor();
			NextState();
			break;
		case SPIN_5S_FIRST:
			if (Spin_N_Ms(5000))
			{
				NextState();
			}
			break;
		case  TEST_FIRST:
			TestPoint();
			NextState();
			break;
		case SPIN_10F_FIRST:
			if (Spin_N_Frame(10))
			{
				NextState();
			}
			break;
		case ACTOR_SET_LOD_FIRST:
			m_pActor0_lod0->SetMaterialLod(1);
			m_pActor1_lod1->SetMaterialLod(2);
			m_pActor2_lod2->SetMaterialLod(0);
			Hippo_WriteConsole(CC_WHITE,"第一次setlod完成");
			NextState();
			break;
		case SPIN_5S_SECOND:
			if (Spin_N_Ms(5000))
			{
				NextState();
			}
			break;
		case TEST_SECOND:
			TestPoint();
			NextState();
			break;
		case SPIN_10F_SECOND:
			if (Spin_N_Frame(10))
			{
				NextState();
			}
			break;
		case ACTOR_SET_LOD_SECOND:
			m_pActor0_lod0->SetMaterialLod(2);
			m_pActor1_lod1->SetMaterialLod(0);
			m_pActor2_lod2->SetMaterialLod(1);
			Hippo_WriteConsole(CC_WHITE,"第二次setlod完成");
			NextState();
			break;
		case SPIN_5S_THIRD:
			if (Spin_N_Ms(5000))
			{
				NextState();
			}
			break;
		case TEST_THIRD:
			TestPoint();
			NextState();
			break;
		}
		return false;
	}
	virtual int OnKeyDown(unsigned int key)
	{
		if (HippoTestManager::GetInstance()->GetCurrentRunMode())
		{
			switch (key)
			{
			case VK_RETURN:
				m_state = CASE_END_EXECING;
				return 0;
			}
		}
		return -1;
	}
};

ADD_TESTCASE("COLOR_TRANSPARENT_HILIGHT",SkinColorTestCase);