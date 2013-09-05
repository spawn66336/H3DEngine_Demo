#include "../../HippoTestManager/HippoTestManager.h"
#include "../CommonCode/HippoUtil/ActorUtil.h"
#include "../CommonCode/HippoUtil/HippoScene.h"
#include "../CommonCode/HippoUtil/HippoGlobal.h"
#include "../CommonCode/HippoUtil/RefCountCompare.h"
#include "../idMath/dMathHeader.h"
#include "engine_interface.h"
#include "EffectCore_dx.h"

using namespace  std;
 
class DOFTest : public HippoTestCaseBase
{
public:
	DOFTest(const char* casename):HippoTestCaseBase(casename){}
	HippoLevelScene* m_pScene;
	H3DI::IModel*  m_pDml1,*m_pDml2;
	bool bAutoEnd;
private:
	static HippoTestCaseBase* const testcase_;
public:
	virtual void InitScene()
	{
		bAutoEnd = false;
		m_pScene = Hippo_GetSceneManager()->CreateEmptyLevel();
		
		//打开景深
		Hippo_GetIRender()->OnCommandLine("SetEngineConfig usePostProcess true");
		m_pScene->GetH3DScene()->AddPostProcess("..\\data\\product\\postprocess\\ue3_uber.xml");

#define SET_POSTPROCESS_PARAM(Name, InValue)	{ FLOAT Value = InValue; \
	m_pScene->GetH3DScene() ->SetPostProcessParam(Name, &Value, sizeof(FLOAT)); }

#define SET_POSTPROCESS_PARAMS(Name, InValue) { FLOAT Value[4] = {InValue, InValue, InValue, InValue};	\
	m_pScene->GetH3DScene() ->SetPostProcessParam(Name, &Value, sizeof(FLOAT) * 4); }

		SET_POSTPROCESS_PARAMS("SceneMidTones",1.0f);
		SET_POSTPROCESS_PARAM("SceneDesaturation",1.0f);
		SET_POSTPROCESS_PARAMS("GammaOverlayColor",0.f);
		SET_POSTPROCESS_PARAMS("GammaColorScale",1.0f);
		SET_POSTPROCESS_PARAM("DOF_Dist",5.0f);
		SET_POSTPROCESS_PARAM("DOF_Range",5.0f);
		SET_POSTPROCESS_PARAM("DOF_Inner",10.0f);
		SET_POSTPROCESS_PARAM("DOFKernal",1.0f);
		SET_POSTPROCESS_PARAM("BloomKernal",1.0f);
		SET_POSTPROCESS_PARAM("BloomThreshold",1.0f);
		SET_POSTPROCESS_PARAM("BloomBlend",1.0f);
		SET_POSTPROCESS_PARAMS("BloomColor",1.0f);
		SET_POSTPROCESS_PARAMS("ScreenShadow",0.0f);
		SET_POSTPROCESS_PARAMS("ScreenHighLight",1.0f);
		SET_POSTPROCESS_PARAM("SoftFocalRange",1.0f);
		SET_POSTPROCESS_PARAM("SoftFocalBenld",0.0f);

#undef SET_POSTPROCESS_PARAMS
#undef SET_POSTPROCESS_PARAM

		//创建两个dml
		m_pDml1 = m_pScene->CreateDml("../resources/art/stage/Waitingroom002/model/waitingroom002_teatable001.dml");
		m_pDml1->SetPosition(H3DVec3(0.f,30.0f,0.f));
		m_pDml1->Update(0.0f);
		m_pDml2 = m_pScene->CreateDml("../resources/art/role/link/female/4111/4111002002/4111002.dml");
		m_pDml2->SetPosition(H3DVec3(0.f,0.0f,0.f));
		bAutoEnd = true;
		m_pScene->GetH3DScene()->RestructScene();
	}

	virtual void CleanUpScene()
	{
		m_pScene->CleanScene();
		Hippo_GetSceneManager()->DelScene(m_pScene);
	}

	virtual bool AutoRunExec()
	{
		if (bAutoEnd)
		{
			return true;
		}
		return false;
	}

	virtual bool ManualRunExec()
	{
		return false;
	}

	virtual int OnKeyDown(unsigned int key)
	{
		if (HippoTestManager::GetInstance()->GetCurrentRunMode())
		{
			switch(key)
			{
			case VK_RETURN:
				m_state = CASE_END_EXECING;
				return 0;
			}
		}
		return -1;
	}
};
ADD_TESTCASE("FOCUS_DEPTH_TEST",DOFTest);