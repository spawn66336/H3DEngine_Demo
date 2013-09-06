include "../HippoTestManager/HippoTestManager.h"
#include "../CommonCode/HippoUtil/ActorUtil.h"
#include "../CommonCode/HippoUtil/HippoScene.h"
#include "../CommonCode/HippoUtil/HippoGlobal.h"
#include "../CommonCode/HippoUtil/RefCountCompare.h"
#include "engine_interface.h"
#include "EffectCore_dx.h"

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

打开景深渲染5s
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

	enum ExeState
	{
		INIT,
		//打开后处理渲染，打开设置景深
		OPEN_POSTRENDER_FOCUSDEPTH,
		DML_LOAD_LOD0,
		DML_LOD0_SPIN,
		//拿到该dml的前向渲染的渲染状态，深度检测一定=true，深度写=false。Shader中应该是有gama
		GET_RENDERSTATE_AND_COM_FIRST,
		//关闭景深
		FOCUSDEPTH_CLOSE,
		//渲染5s
		AFTER_FOCUSDEPTH_CLOSE_SPIN,
		//拿到该dml的前向渲染的渲染状态，深度检测一定=false，深度写=true。Shader中应该是有gama
		GET_RENDERSTATE_AND_COM_SECOND,
		//打开景深
		FOCUSDEPTH_OPEN,
		SPIN_5S,//渲染5s
		//关闭后处理
		POSTRENDER_CLOSE,
		AFTER_POSTRENDER_CLOSE_SPIN,
		//拿到该dml的前向渲染的渲染状态，深度检测一定=false，深度写=true。Shader中应该是没有gama
		GET_RENDERSTATE_AND_COM_THIRD,
		TEST_OVER
	};
	ExeState m_exeState;
	
private:
	static HippoTestCaseBase* const testcase_;
public:
	virtual void InitScene()
	{
		m_pScene = Hippo_GetSceneManager()->CreateEmptyLevel();
		m_exeState =  INIT;
	}

	virtual void CleanUpScene()
	{
		m_pScene->CleanScene();
		Hippo_GetSceneManager()->DelScene(m_pScene);
	}
	void NextState()
	{
		int* p = (int*)(&m_exeState);
		++(*p);
	}
	virtual bool AutoRunExec()
	{
		if (m_exeState ==  TEST_OVER)
		{
			return true;
		}
		return ManualRunExec();
	}

	void PreRenderState_Test()
	{
		//******************************
		//等到接口实现后 根据接口实现该函数
	}
	virtual bool ManualRunExec()
	{
		switch(m_exeState)
		{
		case INIT:
			NextState();
			break;
		case OPEN_POSTRENDER_FOCUSDEPTH: //打开后处理渲染，打开设置景深
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

				Hippo_WriteConsole(CC_WHITE,"设置后处理:%s","..\\data\\product\\postprocess\\ue3_uber.xml");
				NextState();
				break;
			}
		case DML_LOAD_LOD0: //以lod0载入该dml
			{
				//该dml已使用编辑器编辑，满足 使用深度写，不使用深度检测，并且在景深之后绘制
				m_pDml = m_pScene->CreateDml("../x52_test_resources/景深后绘制测试用例资源/4110001002/4110001.dml",0); 
				Hippo_WriteConsole(CC_WHITE,"载入Dml完成");
				NextState();
				break;
			}
		case DML_LOD0_SPIN: //渲染5s
			if (Spin_N_Ms(5000))
			{
				NextState();
			}
			break;
		case GET_RENDERSTATE_AND_COM_FIRST: //拿到该dml的前向渲染的渲染状态，深度检测一定=true，深度写=false。Shader中应该是有gama[前向渲染状态的接口]
			PreRenderState_Test();
			NextState();
			break;
		case FOCUSDEPTH_CLOSE: //关闭景深
			Hippo_GetIRender()->OnCommandLine("SetEngineConfig usePostProcess false");
			Hippo_WriteConsole(CC_WHITE,"关闭景深完成");
			NextState();
			break;
		case AFTER_FOCUSDEPTH_CLOSE_SPIN: //渲染5s
			if (Spin_N_Ms(5000))
			{
				NextState();
			}
			break;
		case GET_RENDERSTATE_AND_COM_SECOND: //拿到该dml的前向渲染的渲染状态，深度检测一定=false，深度写=true。Shader中应该是有gama
			PreRenderState_Test();
			NextState();
			break;
		case FOCUSDEPTH_OPEN: //打开景深渲染5s
			{
				Hippo_GetIRender()->OnCommandLine("SetEngineConfig usePostProcess true");
				Hippo_WriteConsole(CC_WHITE,"打开景深");
				NextState();
			}
			break;
		case  SPIN_5S:
			if (Spin_N_Ms(5000))
			{
				NextState();
			}
			break;
		case POSTRENDER_CLOSE: //关闭后处理
			m_pScene->GetH3DScene()->DelPostProcess("../data/product/postprocess/UE3_Uber.xml");
			Hippo_WriteConsole(CC_WHITE,"关闭后处理");
			NextState();
			break;
		case AFTER_POSTRENDER_CLOSE_SPIN: //渲染5s
			if (Spin_N_Ms(5000))
			{
				NextState();
			}
			break;
		case GET_RENDERSTATE_AND_COM_THIRD: //拿到该dml的前向渲染的渲染状态，深度检测一定=false，深度写=true。Shader中应该是没有gama
			PreRenderState_Test();
			NextState();
			break;
		}
		return false;
	}

	virtual int OnKeyDown(unsigned int key)
	{
		if (HippoTestManager::GetInstance()->GetCurrentRunMode() == MANUAL_RUN)
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