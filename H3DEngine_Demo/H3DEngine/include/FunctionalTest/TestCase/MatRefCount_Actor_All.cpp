#include "../HippoTestManager/HippoTestManager.h"
#include "../CommonCode/HippoUtil/ActorUtil.h"
#include "../CommonCode/HippoUtil/HippoGlobal.h"
#include "../CommonCode/HippoUtil/HippoScene.h"
#include "../CommonCode/HippoUtil/RefCountCompare.h"
#include "engine_interface.h"
#include "EffectCore_dx.h"

using namespace  std;

/*测试逻辑
 1.以lod0创建actor并穿上全部挂件+服装，渲染五秒钟后释放，比较前后引用计数
 2.以lod1创建actor并穿上全部挂件+服装，渲染五秒钟后释放，比较前后引用计数
 3.以lod2创建actor并穿上全部挂件+服装，渲染五秒钟后释放，比较前后引用计数
*/

class MatRetCountActorAll : public HippoTestCaseBase
{
public:
	MatRetCountActorAll(const char* casename):HippoTestCaseBase(casename){}

	HippoLevelScene* m_pScene;
	H3DI::IActor* m_pActor;
	bool m_bActorSex;

	struct sSnapShot
	{
		HippoResStateSnapShot* SnapShot_Mat;
		HippoResStateSnapShot* SnapShot_Tpl;
		HippoResStateSnapShot* SnapShot_Tex;
	};

	struct sSnapShot beforeSnapShot;
	struct sSnapShot afterSnapShot;

	enum ExeState
	{
		INIT,

		ACTOR_LOD0,
		ACTOR_LOD0_ALL,
		ACTOR_LOD0_SPIN,
		ACTOR_LOD0_DEL,

		SNAPSHOT_LOD0_COM,

		ACTOR_LOD1,
		ACTOR_LOD1_ALL,
		ACTOR_LOD1_SPIN,
		ACTOR_LOD1_DEL,

		SNAPSHOT_LOD1_COM,

		ACTOR_LOD2,
		ACTOR_LOD2_ALL,
		ACTOR_LOD2_SPIN,
		ACTOR_LOD2_DEL,

		SNAPSHOT_LOD2_COM,

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
		ActorUtil::InitActorDress("../resources/config/shared/item/dress.xml");
		m_bActorSex = false;
		m_exeState = INIT;

		m_pActor = 0;

		m_pScene->RestructScene();
	}

	virtual void CleanUpScene()
	{
		m_pScene->CleanScene();
		Hippo_GetSceneManager()->DelScene(m_pScene);
	}

	void CreateActor(int matlod)
	{
		m_pActor = m_pScene->CreateActor(m_bActorSex,matlod);
	}

	void SetAllAdomentAndBodypart()
	{
		m_pActor->SetAdornment((H3DI::EActorAdornmentPosition)1,"../resources/art/role/link/female/4110/4110001001/4110001001.spe");
		m_pActor->SetAdornment((H3DI::EActorAdornmentPosition)2,"../resources/art/role/link/female/4112/4112001001/4112001001.spe");
		m_pActor->SetAdornment((H3DI::EActorAdornmentPosition)3,"../resources/art/role/link/female/4113/4113006002/4113006002.spe");
		m_pActor->SetAdornment((H3DI::EActorAdornmentPosition)4,"../resources/art/role/link/female/4114/4114004003/4114004003.spe");

		m_pActor->SetAdornment((H3DI::EActorAdornmentPosition)5,"../resources/art/role/link/female/4115/4115004001/4115004001.spe");
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
	}
	void DelActor()
	{
		m_pScene->DelActor(m_pActor);
		Hippo_GetIRender()->ClearMaterialLib();
	}

	void GetNowRefCount(sSnapShot & snapshot)
	{
		snapshot.SnapShot_Mat =  RefCountCompare::GetResState(HIPPO_MAT_RES);
		snapshot.SnapShot_Tex = RefCountCompare::GetResState(HIPPO_TEX_RES);
		snapshot.SnapShot_Tpl = RefCountCompare::GetResState(HIPPO_TPL_RES);
	}

	void SnapShotCom(sSnapShot& before,sSnapShot& after)
	{
		HIPPO_EXPECT_EQ(RefCountCompare::SnapShotEqual(before.SnapShot_Mat,after.SnapShot_Mat),true);
		HIPPO_EXPECT_EQ(RefCountCompare::SnapShotEqual(before.SnapShot_Tex,after.SnapShot_Tex),true);
		HIPPO_EXPECT_EQ(RefCountCompare::SnapShotEqual(before.SnapShot_Tpl,after.SnapShot_Tpl),true);
		before.SnapShot_Mat->Release();
		before.SnapShot_Tex->Release();
		before.SnapShot_Tpl->Release();
		after.SnapShot_Mat->Release();
		after.SnapShot_Tex->Release();
		after.SnapShot_Tpl->Release();
	}

	void NextState()
	{
		int* p = (int*)(&m_exeState);
		++(*p);
	}

	bool ExeCuting()
	{
		switch(m_exeState)
		{
		case INIT:
			GetNowRefCount(beforeSnapShot);
			NextState();
			break;
		case ACTOR_LOD0:
			CreateActor(0);
			NextState();
			break;
		case ACTOR_LOD0_ALL:
			SetAllAdomentAndBodypart();
			NextState();
			break;
		case  ACTOR_LOD0_SPIN:
			if (Spin_N_Frame(150))
			{
				NextState();
			}
			break;
		case ACTOR_LOD0_DEL:
			DelActor();
			GetNowRefCount(afterSnapShot);
			NextState();
			break;
		case  SNAPSHOT_LOD0_COM:
			SnapShotCom(beforeSnapShot,afterSnapShot);
			NextState();
			break;
		case ACTOR_LOD1:
			GetNowRefCount(beforeSnapShot);
			CreateActor(1);
			NextState();
			break;
		case ACTOR_LOD1_ALL:
			SetAllAdomentAndBodypart();
			NextState();
			break;
		case ACTOR_LOD1_SPIN:
			if (Spin_N_Frame(150))
			{
				NextState();
			}
			break;
		case ACTOR_LOD1_DEL:
			DelActor();
			GetNowRefCount(afterSnapShot);
			NextState();
			break;
		case  SNAPSHOT_LOD1_COM:
			SnapShotCom(beforeSnapShot,afterSnapShot);
			NextState();
			break;
		case ACTOR_LOD2:
			GetNowRefCount(beforeSnapShot);
			CreateActor(2);
			NextState();
			break;
		case ACTOR_LOD2_ALL:
			SetAllAdomentAndBodypart();
			NextState();
			break;
		case  ACTOR_LOD2_SPIN:
			if (Spin_N_Frame(150))
			{
				NextState();
			}
			break;
		case ACTOR_LOD2_DEL:
			DelActor();
			GetNowRefCount(afterSnapShot);
			NextState();
			break;
		case  SNAPSHOT_LOD2_COM:
			SnapShotCom(beforeSnapShot,afterSnapShot);
			NextState();
			break;
		}
		return false;
	}

	virtual bool AutoRunExec()
	{
		if (m_exeState == TEST_OVER )
		{
			return true;
		}
		return ExeCuting();
	}
	virtual bool ManualRunExec()
	{
		return ExeCuting();
	}

	virtual int OnKeyDown(unsigned int key)
	{
		if (HippoTestManager::GetInstance()->GetCurrentRunMode())
		{
			switch(key)
			{
			case   VK_RETURN:
				m_state = CASE_END_EXECING;
				return 1;
			}
		}
		return 0;

	}
};

ADD_TESTCASE("MATERIAL_TEST",MatRetCountActorAll);