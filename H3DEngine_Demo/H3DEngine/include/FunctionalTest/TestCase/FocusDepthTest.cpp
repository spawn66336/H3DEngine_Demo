include "../HippoTestManager/HippoTestManager.h"
#include "../CommonCode/HippoUtil/ActorUtil.h"
#include "../CommonCode/HippoUtil/HippoScene.h"
#include "../CommonCode/HippoUtil/HippoGlobal.h"
#include "../CommonCode/HippoUtil/RefCountCompare.h"
#include "engine_interface.h"
#include "EffectCore_dx.h"

using namespace  std;

/*�����߼�
׼��1��dml��Ҫ�������ʹ�����д����ʹ����ȼ�⣬�����ھ���֮�����
�򿪺�����Ⱦ�������þ���
��lod0�����dml
��Ⱦ5s
�õ���dml��ǰ����Ⱦ����Ⱦ״̬����ȼ��һ��=true�����д=false��Shader��Ӧ������gama[ǰ����Ⱦ״̬�Ľӿ�]
�رվ���
��Ⱦ5s
�õ���dml��ǰ����Ⱦ����Ⱦ״̬����ȼ��һ��=false�����д=true��Shader��Ӧ������gama

�򿪾�����Ⱦ5s
�رպ���
��Ⱦ5s
�õ���dml��ǰ����Ⱦ����Ⱦ״̬����ȼ��һ��=false�����д=true��Shader��Ӧ����û��gama
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
		//�򿪺�����Ⱦ�������þ���
		OPEN_POSTRENDER_FOCUSDEPTH,
		DML_LOAD_LOD0,
		DML_LOD0_SPIN,
		//�õ���dml��ǰ����Ⱦ����Ⱦ״̬����ȼ��һ��=true�����д=false��Shader��Ӧ������gama
		GET_RENDERSTATE_AND_COM_FIRST,
		//�رվ���
		FOCUSDEPTH_CLOSE,
		//��Ⱦ5s
		AFTER_FOCUSDEPTH_CLOSE_SPIN,
		//�õ���dml��ǰ����Ⱦ����Ⱦ״̬����ȼ��һ��=false�����д=true��Shader��Ӧ������gama
		GET_RENDERSTATE_AND_COM_SECOND,
		//�򿪾���
		FOCUSDEPTH_OPEN,
		SPIN_5S,//��Ⱦ5s
		//�رպ���
		POSTRENDER_CLOSE,
		AFTER_POSTRENDER_CLOSE_SPIN,
		//�õ���dml��ǰ����Ⱦ����Ⱦ״̬����ȼ��һ��=false�����д=true��Shader��Ӧ����û��gama
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
		//�ȵ��ӿ�ʵ�ֺ� ���ݽӿ�ʵ�ָú���
	}
	virtual bool ManualRunExec()
	{
		switch(m_exeState)
		{
		case INIT:
			NextState();
			break;
		case OPEN_POSTRENDER_FOCUSDEPTH: //�򿪺�����Ⱦ�������þ���
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

				Hippo_WriteConsole(CC_WHITE,"���ú���:%s","..\\data\\product\\postprocess\\ue3_uber.xml");
				NextState();
				break;
			}
		case DML_LOAD_LOD0: //��lod0�����dml
			{
				//��dml��ʹ�ñ༭���༭������ ʹ�����д����ʹ����ȼ�⣬�����ھ���֮�����
				m_pDml = m_pScene->CreateDml("../x52_test_resources/�������Ʋ���������Դ/4110001002/4110001.dml",0); 
				Hippo_WriteConsole(CC_WHITE,"����Dml���");
				NextState();
				break;
			}
		case DML_LOD0_SPIN: //��Ⱦ5s
			if (Spin_N_Ms(5000))
			{
				NextState();
			}
			break;
		case GET_RENDERSTATE_AND_COM_FIRST: //�õ���dml��ǰ����Ⱦ����Ⱦ״̬����ȼ��һ��=true�����д=false��Shader��Ӧ������gama[ǰ����Ⱦ״̬�Ľӿ�]
			PreRenderState_Test();
			NextState();
			break;
		case FOCUSDEPTH_CLOSE: //�رվ���
			Hippo_GetIRender()->OnCommandLine("SetEngineConfig usePostProcess false");
			Hippo_WriteConsole(CC_WHITE,"�رվ������");
			NextState();
			break;
		case AFTER_FOCUSDEPTH_CLOSE_SPIN: //��Ⱦ5s
			if (Spin_N_Ms(5000))
			{
				NextState();
			}
			break;
		case GET_RENDERSTATE_AND_COM_SECOND: //�õ���dml��ǰ����Ⱦ����Ⱦ״̬����ȼ��һ��=false�����д=true��Shader��Ӧ������gama
			PreRenderState_Test();
			NextState();
			break;
		case FOCUSDEPTH_OPEN: //�򿪾�����Ⱦ5s
			{
				Hippo_GetIRender()->OnCommandLine("SetEngineConfig usePostProcess true");
				Hippo_WriteConsole(CC_WHITE,"�򿪾���");
				NextState();
			}
			break;
		case  SPIN_5S:
			if (Spin_N_Ms(5000))
			{
				NextState();
			}
			break;
		case POSTRENDER_CLOSE: //�رպ���
			m_pScene->GetH3DScene()->DelPostProcess("../data/product/postprocess/UE3_Uber.xml");
			Hippo_WriteConsole(CC_WHITE,"�رպ���");
			NextState();
			break;
		case AFTER_POSTRENDER_CLOSE_SPIN: //��Ⱦ5s
			if (Spin_N_Ms(5000))
			{
				NextState();
			}
			break;
		case GET_RENDERSTATE_AND_COM_THIRD: //�õ���dml��ǰ����Ⱦ����Ⱦ״̬����ȼ��һ��=false�����д=true��Shader��Ӧ����û��gama
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