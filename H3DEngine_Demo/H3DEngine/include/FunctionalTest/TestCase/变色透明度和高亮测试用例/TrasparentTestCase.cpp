#include "../../HippoTestManager/HippoTestManager.h"
#include "../CommonCode/HippoUtil/ActorUtil.h"
#include "../CommonCode/HippoUtil/HippoGlobal.h"
#include "../CommonCode/HippoUtil/HippoScene.h"
#include "engine_interface_internal.h"
#include "EffectCore_dx.h"
#include "idMath/dMathHeader.h"

using namespace  std;
/*测试逻辑
准备1个dml，要求该dml不透明，接受自动lod。
准备1个dml，要求该dml使用透明材质，并且透明的dstfactor=one，接受自动lod。
准备1个dml，要求该dml使用镂空材质，不透明，接受自动lod。
以lod0载入以上3个dml
渲染5s
取这些dml的shader，他们的Tmpalpha应该=1
设置以上3个dml的透明度=0.5
渲染5s
取这些dml的shader，他们的Tmpalpha应该=0.5
3个模型全部切换到lod1
渲染5s
取这些dml的shader，他们的Tmpalpha应该=0.5
3个模型全部切换到lod2
渲染5s
取这些dml的shader，他们的Tmpalpha应该=0.5
 */

class TrasparenceTestCase :public HippoTestCaseBase
{
public: 
	TrasparenceTestCase(const char* casename):HippoTestCaseBase(casename){}
	H3DI::IModel* m_pDml1;
	H3DI::IModel* m_pDml2;
	H3DI::IModel* m_pDml3;
	HippoScene* m_pScene;

	enum ExeState
	{
		INIT,
		CREATE_DML,
		SPIN_5S_FIRST,
		GET_SHADER_FIRST,
		SET_TRASPARENCE,
		SPIN_5S_SECOND,
		GET_SHADER_SECOND,
		SET_DML_LOD1,
		SPIN_5S_THIRD,
		GET_SHADER_THIRD,
		SET_DML_LOD2,
		SPIN_5S_FOURTH,
		GET_SHADER_FOURTH,
		TEST_OVER
	};
	ExeState m_exeState;

private:
	static HippoTestCaseBase* const testcase_;

public:
	virtual void InitScene()
	{
		m_pScene = Hippo_GetSceneManager()->CreateEmptyLevel();
		m_exeState = INIT;
	}

	virtual void  CleanUpScene()
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

	void CreateDml()
	{
		m_pDml1 = m_pScene->CreateDml("../x52_test_resources/设置透明度测试/dml不透明，接受自动lod/Teapot_Notransparency.dml");
		m_pDml2 = m_pScene->CreateDml("../x52_test_resources/设置透明度测试/dml使用透明材质，并且透明的dstfactor=one，接受自动lod/Teapot_transparency.dml");
		m_pDml3 = m_pScene->CreateDml("../x52_test_resources/设置透明度测试/dml使用镂空材质，不透明，接受自动lod/Teapot_loukong.dml");

		H3DVec3 pos1(-3.f,0.f,0.f);
		H3DVec3 pos3(3.f,0.f,0.f);

		m_pDml1->SetPosition(pos1);
		m_pDml3->SetPosition(pos3);
		Hippo_WriteConsole(CC_WHITE,"创建模型完成");
	}

	virtual bool ManualRunExec()
	{
		switch (m_exeState)
		{
		case INIT:
			NextState();
			break;
		case CREATE_DML:
			CreateDml();
			NextState();
			break;
		case SPIN_5S_FIRST:
			if (Spin_N_Ms(5000))
			{
				NextState();
			}
			break;
		case GET_SHADER_FIRST:
			//******************
			NextState();
			break;
		case SET_TRASPARENCE:
			Hippo_WriteConsole(CC_WHITE,"设置透明度为0.5f");
			m_pDml1->SetTransparence(0.5f);
			m_pDml2->SetTransparence(0.5f);
			m_pDml3->SetTransparence(0.5f);
			NextState();
			break;
		case SPIN_5S_SECOND:
			if (Spin_N_Ms(5000))
			{
				NextState();
			}
			break;
		case GET_SHADER_SECOND:
			//*****************
			 //do something here
			NextState();
			break;
		case SET_DML_LOD1:
			Hippo_WriteConsole(CC_WHITE,"设置模型为lod1");
			m_pDml1->SetMaterialLod(1);
			m_pDml2->SetMaterialLod(1);
			m_pDml3->SetMaterialLod(1);
			NextState();
			break;
		case SPIN_5S_THIRD:
			if (Spin_N_Ms(5000))
			{
				NextState();
			}
			break;
		case GET_SHADER_THIRD:
			//************************
			//do something here
			NextState();
			break;
		case SET_DML_LOD2:
			Hippo_WriteConsole(CC_WHITE,"设置模型为lod2");
			m_pDml1->SetMaterialLod(2);
			m_pDml2->SetMaterialLod(2);
			m_pDml3->SetMaterialLod(2);
			NextState();
			break;
		case SPIN_5S_FOURTH:
			if (Spin_N_Ms(5000))
			{
				NextState();
			}
			break;
		case GET_SHADER_FOURTH:
			//*****************
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
ADD_TESTCASE("COLOR_TRANSPARENT_HILIGHT",TrasparenceTestCase);