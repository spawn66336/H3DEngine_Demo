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

/*�����߼�
׼��1��dml��Ҫ�������ʹ�����д����ʹ����ȼ�⣬�����ھ���֮�����
�򿪺�����Ⱦ�������þ���
��lod0�����dml
��Ⱦ5s
�õ���dml��ǰ����Ⱦ����Ⱦ״̬����ȼ��һ��=true�����д=false��Shader��Ӧ������gama[ǰ����Ⱦ״̬�Ľӿ�]
�رվ���
��Ⱦ5s
�õ���dml��ǰ����Ⱦ����Ⱦ״̬����ȼ��һ��=false�����д=true��Shader��Ӧ������gama

�򿪾���
��Ⱦ5s
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
	Hippo_StateMachine stateMachine;
	bool bAutoEnd;
	
private:
	static HippoTestCaseBase* const testcase_;
public:


	SCSTATE OnState0()//�򿪺�����Ⱦ�������þ���	��lod0�����dml
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

		Hippo_WriteConsole(CC_WHITE,"���ú���:%s","..\\data\\product\\postprocess\\ue3_uber.xml");
		m_pDml = m_pScene->CreateDml(Make_HippoTestResource_Path("�������Ʋ���������Դ/4110001002/4110001.dml").c_str(),0); 
		Hippo_WriteConsole(CC_WHITE,"����Dml���");
		return STATE_CHANGE;
	}
	SCSTATE OnState1()
	{
		if (Spin_N_Ms(5000))
		{
			return STATE_CHANGE;
			Hippo_WriteConsole(CC_WHITE,"��Ⱦ5s���");
		}
		return STATE_NOT_CHANGE;
	}
	
	SCSTATE OnState2()
	{
		//�õ���dml��ǰ����Ⱦ����Ⱦ״̬����ȼ��һ��=true�����д=false��Shader��Ӧ������gama[ǰ����Ⱦ״̬�Ľӿ�]
		return STATE_CHANGE;
	}
	SCSTATE OnState3()//�رվ���
	{
		m_pScene->GetH3DScene()->DelPostProcess("..\\data\\product\\postprocess\\ue3_uber.xml");
		Hippo_WriteConsole(CC_WHITE,"�رվ������");
		return STATE_CHANGE;
	}
	SCSTATE OnState4()
	{
		//�õ���dml��ǰ����Ⱦ����Ⱦ״̬����ȼ��һ��=false�����д=true��Shader��Ӧ������gama
		return STATE_CHANGE;
	}
	SCSTATE OnState5()//�򿪾���
	{
		m_pScene->GetH3DScene()->AddPostProcess("..\\data\\product\\postprocess\\ue3_uber.xml");
		Hippo_WriteConsole(CC_WHITE,"�򿪾������");
		return STATE_CHANGE;
	}
	SCSTATE OnState6() //�رպ���
	{
		Hippo_GetIRender()->OnCommandLine("SetEngineConfig usePostProcess false");
		Hippo_WriteConsole(CC_WHITE,"�رպ������");
		return STATE_CHANGE;
	}
	SCSTATE OnState7()
	{
		//�õ���dml��ǰ����Ⱦ����Ⱦ״̬����ȼ��һ��=false�����д=true��Shader��Ӧ����û��gama
		bAutoEnd = true;
		Hippo_WriteConsole(CC_WHITE,"����״ִ̬�н���");
		return STATE_CHANGE;
	}
	virtual void InitScene()
	{
		bAutoEnd = false;
		m_pScene = Hippo_GetSceneManager()->CreateEmptyLevel();

		stateMachine.AddState(0,std::tr1::bind(&FocusDepthTest::OnState0,this));////�򿪺�����Ⱦ�������þ���	��lod0�����dml
		stateMachine.AddState(1,std::tr1::bind(&FocusDepthTest::OnState1,this));//��Ⱦ
		stateMachine.AddState(2,std::tr1::bind(&FocusDepthTest::OnState2,this));//�õ�
		stateMachine.AddState(3,std::tr1::bind(&FocusDepthTest::OnState3,this));//�رվ���
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