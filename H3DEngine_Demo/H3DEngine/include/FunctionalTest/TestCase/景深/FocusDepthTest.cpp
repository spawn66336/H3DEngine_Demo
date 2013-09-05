#include "../../HippoTestManager/HippoTestManager.h"
#include "../CommonCode/HippoUtil/ActorUtil.h"
#include "../CommonCode/HippoUtil/HippoScene.h"
#include "../CommonCode/HippoUtil/HippoGlobal.h"
#include "../CommonCode/HippoUtil/RefCountCompare.h"
#include "../CommonCode/HippoUtil/HippoStateMachine.h"
#include "engine_interface.h"
#include "EffectCore_dx.h"
#include "../../FunctionalTest/functional_test/HippoTestConfig.h"

using namespace  std;

/*测试逻辑
准备1个dml，要求材质中使用深度写，不使用深度检测，并且在景深之后绘制
打开后处理渲染，打开设置景深
以lod0载入该dml
渲染5s
拿到该dml的前向渲染的渲染状态，深度检测一定=true，深度写=false。Shader中应该是有gama[前向渲染状态的接口]
关闭景深
渲染5s
拿到该dml的前向渲染的渲染状态，深度检测一定=false，深度写=true。Shader中应该是有gama

打开景深
渲染5s
关闭后处理
渲染5s
拿到该dml的前向渲染的渲染状态，深度检测一定=false，深度写=true。Shader中应该是没有gama
*/

class FocusDepthTest : public HippoTestCaseBase
{
public:
	FocusDepthTest(const char* casename):HippoTestCaseBase(casename){}
	HippoLevelScene* m_pScene;
	H3DI::IModel* m_pDml;
	Hippo_StateMachine stateMachine;
	bool bAutoEnd;
	
private:
	static HippoTestCaseBase* const testcase_;
public:


	SCSTATE OnState0()//打开后处理渲染，打开设置景深	以lod0载入该dml
	{
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
		SET_POSTPROCESS_PARAM("DOF_Inner",1000.0f);
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

		Hippo_WriteConsole(CC_WHITE,"设置后处理:%s","..\\data\\product\\postprocess\\ue3_uber.xml");
		m_pDml = m_pScene->CreateDml(Make_HippoTestResource_Path("景深后绘制测试用例资源/4110001002/4110001.dml").c_str(),0); 
		Hippo_WriteConsole(CC_WHITE,"载入Dml完成");
		return STATE_CHANGE;
	}
	SCSTATE OnState1()
	{
		if (Spin_N_Ms(5000))
		{
			return STATE_CHANGE;
			Hippo_WriteConsole(CC_WHITE,"渲染5s完成");
		}
		return STATE_NOT_CHANGE;
	}
	
	SCSTATE OnState2()
	{
		//拿到该dml的前向渲染的渲染状态，深度检测一定=true，深度写=false。Shader中应该是有gama[前向渲染状态的接口]
		return STATE_CHANGE;
	}
	SCSTATE OnState3()//关闭景深
	{
		m_pScene->GetH3DScene()->DelPostProcess("..\\data\\product\\postprocess\\ue3_uber.xml");
		Hippo_WriteConsole(CC_WHITE,"关闭景深完成");
		return STATE_CHANGE;
	}
	SCSTATE OnState4()
	{
		//拿到该dml的前向渲染的渲染状态，深度检测一定=false，深度写=true。Shader中应该是有gama
		return STATE_CHANGE;
	}
	SCSTATE OnState5()//打开景深
	{
		m_pScene->GetH3DScene()->AddPostProcess("..\\data\\product\\postprocess\\ue3_uber.xml");
		Hippo_WriteConsole(CC_WHITE,"打开景深完成");
		return STATE_CHANGE;
	}
	SCSTATE OnState6() //关闭后处理
	{
		Hippo_GetIRender()->OnCommandLine("SetEngineConfig usePostProcess false");
		Hippo_WriteConsole(CC_WHITE,"关闭后处理完成");
		return STATE_CHANGE;
	}
	SCSTATE OnState7()
	{
		//拿到该dml的前向渲染的渲染状态，深度检测一定=false，深度写=true。Shader中应该是没有gama
		bAutoEnd = true;
		Hippo_WriteConsole(CC_WHITE,"用例状态执行结束");
		return STATE_CHANGE;
	}
	virtual void InitScene()
	{
		bAutoEnd = false;
		m_pScene = Hippo_GetSceneManager()->CreateEmptyLevel();

		stateMachine.AddState(0,std::tr1::bind(&FocusDepthTest::OnState0,this));////打开后处理渲染，打开设置景深	以lod0载入该dml
		stateMachine.AddState(1,std::tr1::bind(&FocusDepthTest::OnState1,this));//渲染
		stateMachine.AddState(2,std::tr1::bind(&FocusDepthTest::OnState2,this));//拿到
		stateMachine.AddState(3,std::tr1::bind(&FocusDepthTest::OnState3,this));//关闭景深
		stateMachine.AddState(4,std::tr1::bind(&FocusDepthTest::OnState1,this));//render
		stateMachine.AddState(5,std::tr1::bind(&FocusDepthTest::OnState4,this));//get 
		stateMachine.AddState(6,std::tr1::bind(&FocusDepthTest::OnState5,this));//open dof
		stateMachine.AddState(7,std::tr1::bind(&FocusDepthTest::OnState1,this));//render
		stateMachine.AddState(8,std::tr1::bind(&FocusDepthTest::OnState6,this));//close postprocess
		stateMachine.AddState(9,std::tr1::bind(&FocusDepthTest::OnState1,this));//render
		stateMachine.AddState(10,std::tr1::bind(&FocusDepthTest::OnState7,this));//get 
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
		return ManualRunExec();
	}

	virtual bool ManualRunExec()
	{
		stateMachine.Run();
		return false;
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
ADD_TESTCASE("FOCUS_DEPTH_TEST",FocusDepthTest);