#include "../../HippoTestManager/HippoTestManager.h"
#include "../CommonCode/HippoUtil/ActorUtil.h"
#include "../CommonCode/HippoUtil/HippoGlobal.h"
#include "../CommonCode/HippoUtil/HippoScene.h"
#include "engine_interface_internal.h"
#include "../CommonCode/HippoUtil/FileOperation.h"
#include "EffectCore_dx.h"
#include "idMath/dMathHeader.h"
#include "../../FunctionalTest/stress_test/HippoTestConfig.h"

using namespace  std;


/*测试逻辑
使用引擎：
使用引擎加载1个actor
准备1套服装，同步加载，完成后渲染5s
释放该人物，不要clearmatlib
再创建人物，穿同样的衣服
循环
*/
class StressTest_Engine_actorBpt:public HippoTestCase_RefCompare
{
public: 
	StressTest_Engine_actorBpt(const char* casename):HippoTestCase_RefCompare(casename){}

	virtual void InitScene()
	{
		m_pScene = Hippo_GetSceneManager()->CreateEmptyLevel();
		m_pActor = 0;
		CreatActor();
	}

	virtual void CreatActor()
	{
		if (m_pActor)
		{
			m_pScene->DelActor(m_pActor);
		}
		m_pActor = m_pScene->CreateActor(false);
		//load bpt
		m_pActor->SetBodyPart(Make_X52Resources_Path("art/role/bodypart/female/socks/117013001/117013001.bpt").c_str());
		m_pActor->SetBodyPart(Make_X52Resources_Path("art/role/bodypart/female/body/114001001/114001001.BPT").c_str());
		m_pActor->SetBodyPart(Make_X52Resources_Path("art/role/bodypart/female/face/112002001/112002001.BPT").c_str());
		m_pActor->SetBodyPart(Make_X52Resources_Path("art/role/bodypart/female/hair/113004001/113004001.BPT").c_str());
		m_pActor->SetBodyPart(Make_X52Resources_Path("art/role/bodypart/female/hand/115002001/115002001.bpt").c_str());
		m_pActor->SetBodyPart(Make_X52Resources_Path("art/role/bodypart/female/shoe/118006001/118006001.bpt").c_str());
		m_pActor->SetBodyPart(Make_X52Resources_Path("art/role/bodypart/female/suit/119005001/1193005001/1193005001.bpt").c_str());
		m_pActor->SetBodyPart(Make_X52Resources_Path("art/role/bodypart/female/trousers/116004001/116004001.bpt").c_str());
	}

	virtual bool ManualRunExec()
	{
		if (Spin_N_Ms(5000))
		{
			CreatActor();
		}
		return false;
	}

	virtual bool AutoRunExec()
	{
		return ManualRunExec();
	}
	virtual void CleanUpScene()
	{
		if (m_pScene)
		{
			m_pScene->CleanScene();
			Hippo_GetSceneManager()->DelScene(m_pScene);
		}
	}
private:
	static HippoTestCaseBase* const testcase_;

public:
	HippoScene* m_pScene;
	H3DI::IActor* m_pActor;
};
ADD_TESTCASE("STRSS_TRST",StressTest_Engine_actorBpt);

/*测试逻辑
使用引擎加载1个actor
准备1套挂件，同步加载，完成后渲染5s
释放该人物，不要clearmatlib
再创建人物，穿同样的挂件
循环
*/
class StressTest_Engine_actorAdorment:public StressTest_Engine_actorBpt
{
public:
	StressTest_Engine_actorAdorment(const char* casename):StressTest_Engine_actorBpt(casename){}

	virtual void CreatActor()
	{
		if (m_pActor)
		{
			m_pScene->DelActor(m_pActor);
		}
		m_pActor = m_pScene->CreateActor(false);

		m_pActor->SetAdornment((H3DI::EActorAdornmentPosition)0,Make_X52Resources_Path("art/role/link/female/4110/4110001001/4110001001.spe").c_str());
		m_pActor->SetAdornment((H3DI::EActorAdornmentPosition)1,Make_X52Resources_Path("art/role/link/female/4111/4111006001/4111006001.spe").c_str());
		m_pActor->SetAdornment((H3DI::EActorAdornmentPosition)2,Make_X52Resources_Path("art/role/link/female/4112/4112001001/4112001001.spe").c_str());
		m_pActor->SetAdornment((H3DI::EActorAdornmentPosition)3,Make_X52Resources_Path("art/role/link/female/4113/4113006002/4113006002.spe").c_str());
		m_pActor->SetAdornment((H3DI::EActorAdornmentPosition)4,Make_X52Resources_Path("art/role/link/female/4114/4114004003/4114004003.spe").c_str());
		m_pActor->SetAdornment((H3DI::EActorAdornmentPosition)5,Make_X52Resources_Path("art/role/link/female/4115/4115004001/4115004001.spe").c_str());
		m_pActor->SetAdornment((H3DI::EActorAdornmentPosition)6,Make_X52Resources_Path("art/role/link/female/4116/4116009002/4116009002.spe").c_str());
		m_pActor->SetAdornment((H3DI::EActorAdornmentPosition)7,Make_X52Resources_Path("art/role/link/female/4117/4117004001/4117004001.spe").c_str());
		m_pActor->SetAdornment((H3DI::EActorAdornmentPosition)8,Make_X52Resources_Path("art/role/link/female/4118/4118006001/4118006001.spe").c_str());
		m_pActor->SetAdornment((H3DI::EActorAdornmentPosition)9,Make_X52Resources_Path("art/role/link/female/4119/4119007001/4119007001.spe").c_str());
		m_pActor->SetAdornment((H3DI::EActorAdornmentPosition)10,Make_X52Resources_Path("art/role/link/female/4120/4120007001/4120007001.spe").c_str());
		m_pActor->SetAdornment((H3DI::EActorAdornmentPosition)11,Make_X52Resources_Path("art/role/link/female/4121/4121007001/4121007001.spe").c_str());
	}
private:
	static HippoTestCaseBase* const testcase_;
};
ADD_TESTCASE("STRESS_TEST",StressTest_Engine_actorAdorment);