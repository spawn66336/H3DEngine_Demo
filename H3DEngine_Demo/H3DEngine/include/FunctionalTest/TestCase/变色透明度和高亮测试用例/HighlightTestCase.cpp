#include "../../HippoTestManager/HippoTestManager.h"
#include "../CommonCode/HippoUtil/ActorUtil.h"
#include "../CommonCode/HippoUtil/HippoGlobal.h"
#include "../CommonCode/HippoUtil/HippoScene.h"
#include "engine_interface_internal.h"
#include "EffectCore_dx.h"
#include "idMath/dMathHeader.h"

using namespace  std;
/*测试逻辑
准备1个角色（lod0），全身服装挂件都穿满
渲染5s
设置highlight color=红色  SetMaterialColorFactor
渲染5s
得到该角色所有的renderable，检查shader中HightLightC0，C1是否是红色
切换角色到lod1
渲染5s
得到该角色所有的renderable，检查shader中HightLightC0，C1是否是红色
切换角色到lod2
渲染5s
得到该角色所有的renderable，检查shader中HightLightC0，C1是否是红色
 */

class HiLightTestCase : public HippoTestCaseBase
{
public:
	HiLightTestCase(const char* casename):HippoTestCaseBase(casename){}
	HippoScene* m_pScene;
	H3DI::IActor* m_pActor;

	enum ExeState
	{
		INIT,
		CREATE_ACTOR,
		SPIN_5S_FIRST,
		SET_HILIGHT,
		SPIN_5S_SECOND,
		GET_REDERABLE_FIRST,
		CHAGE_ACTOR_LOD1,
		SPIN_5S_THIRD,
		GET_REDERABLE_SECOND,
		CHAGE_ACTOR_LOD2,
		SPIN_5S_FOURTH,
		GET_REDERABLE_THIRD,
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
		m_exeState = INIT;
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

	void CreateActor()
	{
		m_pActor = m_pScene->CreateActor(false,0);

		m_pActor->SetAdornment((H3DI::EActorAdornmentPosition)1,"../resources/art/role/link/female/4110/4110001001/4110001001.spe");
		m_pActor->SetAdornment((H3DI::EActorAdornmentPosition)2,"../resources/art/role/link/female/4112/4112001001/4112001001.spe");
		m_pActor->SetAdornment((H3DI::EActorAdornmentPosition)3,"../resources/art/role/link/female/4113/4113006002/4113006002.spe");
		m_pActor->SetAdornment((H3DI::EActorAdornmentPosition)4,"../resources/art/role/link/female/4114/4114004003/4114004003.spe");

		m_pActor->SetAdornment((H3DI::EActorAdornmentPosition)5,"../resources/art/role/link/female/4115/4115028002/4115028002.spe");
		m_pActor->SetAdornment((H3DI::EActorAdornmentPosition)6,"../resources/art/role/link/female/4116/4116009002/4116009002.spe");
		m_pActor->SetAdornment((H3DI::EActorAdornmentPosition)7,"../resources/art/role/link/female/4117/4117004001/4117004001.spe");
		m_pActor->SetAdornment((H3DI::EActorAdornmentPosition)8,"../resources/art/role/link/female/4118/4118006001/4118006001.spe");
		m_pActor->SetAdornment((H3DI::EActorAdornmentPosition)9,"../resources/art/role/link/female/4119/4119007001/4119007001.spe");
		m_pActor->SetAdornment((H3DI::EActorAdornmentPosition)10,"../resources/art/role/link/female/4120/4120007001/4120007001.spe");
		m_pActor->SetAdornment((H3DI::EActorAdornmentPosition)11,"../resources/art/role/link/female/4121/4121007001/4121007001.spe");
		m_pActor->SetAdornment((H3DI::EActorAdornmentPosition)0,"../resources/art/role/link/female/4111/4111006001/4111006001.spe");

		m_pActor->SetBodyPart("../resources/art/role/bodypart/female/socks/117013001/117013001.bpt");
		m_pActor->SetBodyPart("../resources/art/role/bodypart/female/socks/117013001/117013001.bpt");
		m_pActor->SetBodyPart("../resources/art/role/bodypart/female/body/114001001/114001001.BPT");
		m_pActor->SetBodyPart("../resources/art/role/bodypart/female/face/112002001/112002001.BPT");
		m_pActor->SetBodyPart("../resources/art/role/bodypart/female/hair/113004001/113004001.BPT");
		m_pActor->SetBodyPart("../resources/art/role/bodypart/female/hand/115002001/115002001.bpt");
		m_pActor->SetBodyPart("../resources/art/role/bodypart/female/shoe/118006001/118006001.bpt");
		m_pActor->SetBodyPart("../resources/art/role/bodypart/female/strippedbody/11a001.BPT");
		m_pActor->SetBodyPart("../resources/art/role/bodypart/female/suit/119005001/1193005001/1193005001.bpt");
		m_pActor->SetBodyPart("../resources/art/role/bodypart/female/trousers/116004001/116004001.bpt");
		Hippo_WriteConsole(CC_WHITE,"创建人物完成");

	}
	virtual bool ManualRunExec()
	{
		switch (m_exeState)
		{
		case INIT:
			NextState();
			break;
		case CREATE_ACTOR:
			CreateActor();
			NextState();
			break;

		case SPIN_5S_FIRST:
			if (Spin_N_Ms(5000))
			{
				NextState();
			}
			break;
		case SET_HILIGHT:
			{
				Hippo_WriteConsole(CC_WHITE,"设置highlight color=红色");
				float red[] = {1.0f,0.f,0.f,1.0f};
				m_pActor->SetMaterialColorFactor(red,4);
				NextState();
				break;
			}
			
		case SPIN_5S_SECOND:
			if (Spin_N_Ms(5000))
			{
				NextState();
			}
			break;

		case GET_REDERABLE_FIRST:
			//**************
			//do something here
			NextState();
			break;
		case CHAGE_ACTOR_LOD1:
			Hippo_WriteConsole(CC_WHITE,"切换角色到lod1");
			m_pActor->SetMaterialLod(1);
			NextState();
			break;
		case SPIN_5S_THIRD:
			if (Spin_N_Ms(5000))
			{
				NextState();
			}
			break;
		case GET_REDERABLE_SECOND:
			//*************
			//do something here
			NextState();
			break;
		case CHAGE_ACTOR_LOD2:
			Hippo_WriteConsole(CC_WHITE,"切换角色到lod2");
			m_pActor->SetMaterialLod(2);
			NextState();
			break;
		case SPIN_5S_FOURTH:
			if (Spin_N_Ms(5000))
			{
				NextState();
			}
			break;
		case GET_REDERABLE_THIRD:
			//*******************
			//do something here
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
ADD_TESTCASE("COLOR_TRANSPARENT_HILIGHT",HiLightTestCase);