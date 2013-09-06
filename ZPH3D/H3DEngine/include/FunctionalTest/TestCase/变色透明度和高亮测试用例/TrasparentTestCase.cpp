#include "../../HippoTestManager/HippoTestManager.h"
#include "../CommonCode/HippoUtil/ActorUtil.h"
#include "../CommonCode/HippoUtil/HippoGlobal.h"
#include "../CommonCode/HippoUtil/HippoScene.h"
#include "engine_interface_internal.h"
#include "EffectCore_dx.h"
#include "idMath/dMathHeader.h"

using namespace  std;
/*�����߼�
׼��1��dml��Ҫ���dml��͸���������Զ�lod��
׼��1��dml��Ҫ���dmlʹ��͸�����ʣ�����͸����dstfactor=one�������Զ�lod��
׼��1��dml��Ҫ���dmlʹ���οղ��ʣ���͸���������Զ�lod��
��lod0��������3��dml
��Ⱦ5s
ȡ��Щdml��shader�����ǵ�TmpalphaӦ��=1
��������3��dml��͸����=0.5
��Ⱦ5s
ȡ��Щdml��shader�����ǵ�TmpalphaӦ��=0.5
3��ģ��ȫ���л���lod1
��Ⱦ5s
ȡ��Щdml��shader�����ǵ�TmpalphaӦ��=0.5
3��ģ��ȫ���л���lod2
��Ⱦ5s
ȡ��Щdml��shader�����ǵ�TmpalphaӦ��=0.5
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
		m_pDml1 = m_pScene->CreateDml("../x52_test_resources/����͸���Ȳ���/dml��͸���������Զ�lod/Teapot_Notransparency.dml");
		m_pDml2 = m_pScene->CreateDml("../x52_test_resources/����͸���Ȳ���/dmlʹ��͸�����ʣ�����͸����dstfactor=one�������Զ�lod/Teapot_transparency.dml");
		m_pDml3 = m_pScene->CreateDml("../x52_test_resources/����͸���Ȳ���/dmlʹ���οղ��ʣ���͸���������Զ�lod/Teapot_loukong.dml");

		H3DVec3 pos1(-3.f,0.f,0.f);
		H3DVec3 pos3(3.f,0.f,0.f);

		m_pDml1->SetPosition(pos1);
		m_pDml3->SetPosition(pos3);
		Hippo_WriteConsole(CC_WHITE,"����ģ�����");
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
			Hippo_WriteConsole(CC_WHITE,"����͸����Ϊ0.5f");
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
			Hippo_WriteConsole(CC_WHITE,"����ģ��Ϊlod1");
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
			Hippo_WriteConsole(CC_WHITE,"����ģ��Ϊlod2");
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