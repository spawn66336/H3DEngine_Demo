#include "../../HippoTestManager/HippoTestManager.h"
#include "../CommonCode/HippoUtil/ActorUtil.h"
#include "../CommonCode/HippoUtil/HippoGlobal.h"
#include "../CommonCode/HippoUtil/HippoScene.h"
#include "engine_interface_internal.h"
#include "EffectCore_dx.h"
#include "idMath/dMathHeader.h"

using namespace  std;
/*�����߼�
��lod0��lod1��lod2�ķ�ʽ�ֱ�����һ��actor��actor0��actor1��actor2��������3���˴���ͬ����һ�����¡�
Actor0��actor1��actor2��Ƥ��Ⱦ�ɺ�ɫ SetSkinColor
��Ⱦ5s
ִ�м���1��
��Ⱦ10֡�Ժ�Actor0�л���lod1��actor1�л���lod2��actor2�л���lod0��
��Ⱦ5s
ִ�м���1��
��Ⱦ10֡�Ժ�Actor0�л���lod2��actor1�л���lod0��actor2�л���lod1��
��Ⱦ5s
ִ�м���1��
����1��
�������actor�ĸ���renderable��shader�е�SkinColor��shaderconst��ֵ�Ǻ�ɫ��
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
		Hippo_WriteConsole(CC_WHITE,"�����������");
	}

	void TestPoint()
	{
		//**************************
		//���������û��ʵ��
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
			Hippo_WriteConsole(CC_WHITE,"��һ��setlod���");
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
			Hippo_WriteConsole(CC_WHITE,"�ڶ���setlod���");
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