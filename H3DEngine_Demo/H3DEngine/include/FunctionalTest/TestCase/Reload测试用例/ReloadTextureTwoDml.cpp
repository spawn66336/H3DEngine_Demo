#include "../../HippoTestManager/HippoTestManager.h"
#include "../CommonCode/HippoUtil/ActorUtil.h"
#include "../CommonCode/HippoUtil/HippoScene.h"
#include "../CommonCode/HippoUtil/HippoGlobal.h"
#include "../CommonCode/HippoUtil/RefCountCompare.h"
#include "engine_interface.h"
#include "engine_interface_internal.h"
#include "../CommonCode/HippoUtil/HippoStateMachine.h"
#include "../idMath/dMathHeader.h"
#include "EffectCore_dx.h"
#include <algorithm>
#include <stdio.h>
using namespace  std;

/*�����߼�
�й�ͬ��ͼ��ʵ���Ĳ��ԣ�
����4��������TestCase_RefCompare����
׼������DML��dml1��dml2��������dml��LOD0�Ĳ�����������һ����ͼ��ͬ��Tex0��--ע���������Դֻ�й���ɫ��ͼ��ͬ��
��LOD0�ķ�ʽ����������DML��������5s��
�޸Ĵ�����Tex0�ļ������ݡ�
Reload dml1��
Ч����dml1��dml2Ӧ��һ��仯��
*/

class ReloadTextureTwoDml1 :public HippoTestCase_RefCompare
{
public: 
	ReloadTextureTwoDml1(const char* casename):HippoTestCase_RefCompare(casename){}
	H3DI::IModel* m_pDml1;
	H3DI::IModel* m_pDml2;
	HippoLevelScene* m_pScene;
	Hippo_StateMachine stateMachine;
	bool bAutoEnd;
private:
	static HippoTestCaseBase* const testcase_;
public:

	virtual int GetMatLod(){return 0;}
	virtual void ChangeDiskTex0First()
	{
		rename("../x52_test_resources/Reload����������Դ/Reload��ͼ/�й�ͬ��ͼ��ʵ������/4110003001/Texture/4110003001_d.dds",
				"../x52_test_resources/Reload����������Դ/Reload��ͼ/�й�ͬ��ͼ��ʵ������/4110003001/Texture/back.dds");
		rename("../x52_test_resources/Reload����������Դ/Reload��ͼ/�й�ͬ��ͼ��ʵ������/4110003001/Texture/4117007002_d.dds",
			    "../x52_test_resources/Reload����������Դ/Reload��ͼ/�й�ͬ��ͼ��ʵ������/4110003001/Texture/4110003001_d.dds");
	}
	virtual void ChangeDiskTex0Second()
	{
		rename("../x52_test_resources/Reload����������Դ/Reload��ͼ/�й�ͬ��ͼ��ʵ������/4110003001/Texture/4110003001_d.dds",
			"../x52_test_resources/Reload����������Դ/Reload��ͼ/�й�ͬ��ͼ��ʵ������/4110003001/Texture/4117007002_d.dds");
		rename("../x52_test_resources/Reload����������Դ/Reload��ͼ/�й�ͬ��ͼ��ʵ������/4110003001/Texture/back.dds",
			"../x52_test_resources/Reload����������Դ/Reload��ͼ/�й�ͬ��ͼ��ʵ������/4110003001/Texture/4110003001_d.dds");
	}
	virtual void CreateModle()
	{
		m_pDml1=m_pScene->CreateDml("../x52_test_resources/Reload����������Դ/Reload��ͼ/�й�ͬ��ͼ��ʵ������/4110003001/4110003.dml",GetMatLod());
		m_pDml2=m_pScene->CreateDml("../x52_test_resources/Reload����������Դ/Reload��ͼ/�й�ͬ��ͼ��ʵ������/4110003002/4110003.dml",GetMatLod());
		m_pDml1->SetPosition(H3DVec3(-0.5f,0.f,0.f));
		m_pDml2->SetPosition(H3DVec3(0.5f,0.f,0.f));
	}

	//��LOD0�ķ�ʽ����������DMl
	SCSTATE OnState0()
	{
		Hippo_WriteConsole(CC_WHITE,"CreateModle()");
		CreateModle();
		return STATE_CHANGE;
	}

	//����5s��
	SCSTATE OnState1()
	{
		if (Spin_N_Ms(5000))
		{
			Hippo_WriteConsole(CC_WHITE,"��Ⱦ5s");
			return STATE_CHANGE;
		}
		return STATE_NOT_CHANGE;
	}

	//�޸Ĵ�����Tex0�ļ������ݡ�
	SCSTATE OnState2()
	{
		Hippo_WriteConsole(CC_WHITE,"�޸Ĵ�����Tex0�ļ�������");
		ChangeDiskTex0First();
		return STATE_CHANGE;
	}

	//Reload dml1
	SCSTATE OnState3()
	{
		Hippo_WriteConsole(CC_WHITE,"Reload dml1");
		int res = m_pDml1->ReloadFromDisk(3);

		ChangeDiskTex0Second();
		bAutoEnd = true;

		return STATE_CHANGE;
	}

	virtual void InitScene()
	{
		bAutoEnd = false;
		m_pScene = Hippo_GetSceneManager()->CreateEmptyLevel();
		stateMachine.AddState(0,std::tr1::bind(&ReloadTextureTwoDml1::OnState0,this));
		stateMachine.AddState(1,std::tr1::bind(&ReloadTextureTwoDml1::OnState1,this));
		stateMachine.AddState(2,std::tr1::bind(&ReloadTextureTwoDml1::OnState2,this));
		stateMachine.AddState(3,std::tr1::bind(&ReloadTextureTwoDml1::OnState3,this));

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
		if (HippoTestManager::GetInstance()->GetCurrentRunMode() == MANUAL_RUN)
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
ADD_TESTCASE("RELOAD_TESTCASE",ReloadTextureTwoDml1)

/*�����߼�
	�߼�ͬ����1����lod1�ķ�ʽ����
*/

class ReloadTextureTwoDml2 : public ReloadTextureTwoDml1
{
public:
	ReloadTextureTwoDml2(const char* casename):ReloadTextureTwoDml1(casename){}
	virtual int GetMatLod(){return 1;};
private:
	static HippoTestCaseBase* const testcase_;
};
ADD_TESTCASE("RELOAD_TESTCASE",ReloadTextureTwoDml2);

/*�����߼�
	�߼�ͬ����1����lod2�ķ�ʽ����
*/

class ReloadTextureTwoDml3: public ReloadTextureTwoDml1
{
public:
	ReloadTextureTwoDml3(const char* casename):ReloadTextureTwoDml1(casename){}
	virtual int GetMatLod(){return 2;};


	//lod2�����ʹ�õ��Ǵ���׺_lod2����ͼ
	virtual void ChangeDiskTex0First()
	{
		rename("../x52_test_resources/Reload����������Դ/Reload��ͼ/�й�ͬ��ͼ��ʵ������/4110003001/Texture/4110003001_d_LOD2.dds",
			"../x52_test_resources/Reload����������Դ/Reload��ͼ/�й�ͬ��ͼ��ʵ������/4110003001/Texture/back.dds");
		rename("../x52_test_resources/Reload����������Դ/Reload��ͼ/�й�ͬ��ͼ��ʵ������/4110003001/Texture/4117007002_d.dds",
			"../x52_test_resources/Reload����������Դ/Reload��ͼ/�й�ͬ��ͼ��ʵ������/4110003001/Texture/4110003001_d_LOD2.dds");
	}
	virtual void ChangeDiskTex0Second()
	{
		rename("../x52_test_resources/Reload����������Դ/Reload��ͼ/�й�ͬ��ͼ��ʵ������/4110003001/Texture/4110003001_d_LOD2.dds",
			"../x52_test_resources/Reload����������Դ/Reload��ͼ/�й�ͬ��ͼ��ʵ������/4110003001/Texture/4117007002_d.dds");
		rename("../x52_test_resources/Reload����������Դ/Reload��ͼ/�й�ͬ��ͼ��ʵ������/4110003001/Texture/back.dds",
			"../x52_test_resources/Reload����������Դ/Reload��ͼ/�й�ͬ��ͼ��ʵ������/4110003001/Texture/4110003001_d_LOD2.dds");
	}

private:
	static HippoTestCaseBase* const testcase_;
};
ADD_TESTCASE("RELOAD_TESTCASE",ReloadTextureTwoDml3);


/*�����߼�
	�߼�ͬ����1����lod0�ķ�ʽ����--ע���������Դ����mask���� �� ����ɫ��ͼ �߹���ͼ�ͷ�����ͼ����ͬ
	ʵ�����޸ĵ��ǹ���ɫ��ͼ��ʹ����һ�Ź���ɫ��ͼ�����滻
*/

class ReloadTextureTwoDml4 : public ReloadTextureTwoDml1
{
public:
	ReloadTextureTwoDml4(const char* casename):ReloadTextureTwoDml1(casename){}
	virtual void ChangeDiskTex0First()
	{
		rename("../x52_test_resources/Reload����������Դ/Reload��ͼ/�й�ͬ��ͼ��ʵ������/��mask����/4015012001/Texture/4015012001_D.dds",
			"../x52_test_resources/Reload����������Դ/Reload��ͼ/�й�ͬ��ͼ��ʵ������/��mask����/4015012001/Texture/back.dds");
		rename("../x52_test_resources/Reload����������Դ/Reload��ͼ/�й�ͬ��ͼ��ʵ������/��mask����/4015012001/Texture/4110003001_d.dds",
			"../x52_test_resources/Reload����������Դ/Reload��ͼ/�й�ͬ��ͼ��ʵ������/��mask����/4015012001/Texture/4015012001_D.dds");
	}
	virtual void ChangeDiskTex0Second()
	{
		rename("../x52_test_resources/Reload����������Դ/Reload��ͼ/�й�ͬ��ͼ��ʵ������/��mask����/4015012001/Texture/4015012001_D.dds",
			"../x52_test_resources/Reload����������Դ/Reload��ͼ/�й�ͬ��ͼ��ʵ������/��mask����/4015012001/Texture/4110003001_d.dds");
		rename("../x52_test_resources/Reload����������Դ/Reload��ͼ/�й�ͬ��ͼ��ʵ������/��mask����/4015012001/Texture/back.dds",
			"../x52_test_resources/Reload����������Դ/Reload��ͼ/�й�ͬ��ͼ��ʵ������/��mask����/4015012001/Texture/4015012001_D.dds");
	}
	virtual void CreateModle()
	{
		m_pDml1=m_pScene->CreateChr("../x52_test_resources/Reload����������Դ/Reload��ͼ/�й�ͬ��ͼ��ʵ������/��mask����/4015012001/4015012.dml",GetMatLod());
		m_pDml2=m_pScene->CreateChr("../x52_test_resources/Reload����������Դ/Reload��ͼ/�й�ͬ��ͼ��ʵ������/��mask����/4015012002/4015012.dml",GetMatLod());
		m_pDml1->SetPosition(H3DVec3(-1.5f,0.f,0.f));
		m_pDml2->SetPosition(H3DVec3(1.5f,0.f,0.f));
	}

private:
	static HippoTestCaseBase* const testcase_;
};
ADD_TESTCASE("RELOAD_TESTCASE",ReloadTextureTwoDml4);


/*�����߼�
	�߼�ͬ����4����lod1�ķ�ʽ����--ע���������Դ����mask���� �� ����ɫ��ͼ �߹���ͼ�ͷ�����ͼ����ͬ
	ʵ�����޸ĵ��ǹ���ɫ��ͼ��ʹ����һ�Ź���ɫ��ͼ�����滻
*/

class ReloadTextureTwoDml5 : public ReloadTextureTwoDml4
{
public:
	ReloadTextureTwoDml5(const char* casename):ReloadTextureTwoDml4(casename){}
	virtual int GetMatLod(){return 1;}

private:
	static HippoTestCaseBase* const testcase_;
};
ADD_TESTCASE("RELOAD_TESTCASE",ReloadTextureTwoDml5);


/*�����߼�
	�߼�ͬ����4����lod2�ķ�ʽ����--ע���������Դ����mask���� �� ����ɫ��ͼ �߹���ͼ�ͷ�����ͼ����ͬ
	ʵ�����޸ĵ��ǹ���ɫ��ͼ��ʹ����һ�Ź���ɫ��ͼ�����滻
*/

class ReloadTextureTwoDml6 : public ReloadTextureTwoDml4
{
public:
	ReloadTextureTwoDml6(const char* casename):ReloadTextureTwoDml4(casename){}
	virtual int GetMatLod(){return 2;}

	virtual void ChangeDiskTex0First()
	{
		rename("../x52_test_resources/Reload����������Դ/Reload��ͼ/�й�ͬ��ͼ��ʵ������/��mask����/4015012001/Texture/4015012001_d_LOD2.dds",
			"../x52_test_resources/Reload����������Դ/Reload��ͼ/�й�ͬ��ͼ��ʵ������/��mask����/4015012001/Texture/back.dds");
		rename("../x52_test_resources/Reload����������Դ/Reload��ͼ/�й�ͬ��ͼ��ʵ������/��mask����/4015012001/Texture/4110003001_d.dds",
			"../x52_test_resources/Reload����������Դ/Reload��ͼ/�й�ͬ��ͼ��ʵ������/��mask����/4015012001/Texture/4015012001_d_LOD2.dds");
	}
	virtual void ChangeDiskTex0Second()
	{
		rename("../x52_test_resources/Reload����������Դ/Reload��ͼ/�й�ͬ��ͼ��ʵ������/��mask����/4015012001/Texture/4015012001_d_LOD2.dds",
			"../x52_test_resources/Reload����������Դ/Reload��ͼ/�й�ͬ��ͼ��ʵ������/��mask����/4015012001/Texture/4110003001_d.dds");
		rename("../x52_test_resources/Reload����������Դ/Reload��ͼ/�й�ͬ��ͼ��ʵ������/��mask����/4015012001/Texture/back.dds",
			"../x52_test_resources/Reload����������Դ/Reload��ͼ/�й�ͬ��ͼ��ʵ������/��mask����/4015012001/Texture/4015012001_d_LOD2.dds");
	}

private:
	static HippoTestCaseBase* const testcase_;
};
ADD_TESTCASE("RELOAD_TESTCASE",ReloadTextureTwoDml6);


/*�����߼�
	�߼�ͬ����4����lod0�ķ�ʽ����--ע���������Դ����mask���� ��mask��ͼ ����ɫ��ͼ �߹���ͼ�ͷ�����ͼ����ͬ
	ʵ�����޸ĵ���mask��ͼ��ʹ����һ��mask��ͼ�����滻
*/

class ReloadTextureTwoDml7 : public ReloadTextureTwoDml4
{
public:
	ReloadTextureTwoDml7(const char* casename):ReloadTextureTwoDml4(casename){}

	virtual void ChangeDiskTex0First()
	{
		rename("../x52_test_resources/Reload����������Դ/Reload��ͼ/�й�ͬ��ͼ��ʵ������/��mask����/4015012001/Texture/4015012001_M.dds",
			"../x52_test_resources/Reload����������Դ/Reload��ͼ/�й�ͬ��ͼ��ʵ������/��mask����/4015012001/Texture/back.dds");
		rename("../x52_test_resources/Reload����������Դ/Reload��ͼ/�й�ͬ��ͼ��ʵ������/��mask����/4015012001/Texture/114006001_M.dds",
			"../x52_test_resources/Reload����������Դ/Reload��ͼ/�й�ͬ��ͼ��ʵ������/��mask����/4015012001/Texture/4015012001_M.dds");
	}
	virtual void ChangeDiskTex0Second()
	{
		rename("../x52_test_resources/Reload����������Դ/Reload��ͼ/�й�ͬ��ͼ��ʵ������/��mask����/4015012001/Texture/4015012001_M.dds",
			"../x52_test_resources/Reload����������Դ/Reload��ͼ/�й�ͬ��ͼ��ʵ������/��mask����/4015012001/Texture/114006001_M.dds");
		rename("../x52_test_resources/Reload����������Դ/Reload��ͼ/�й�ͬ��ͼ��ʵ������/��mask����/4015012001/Texture/back.dds",
			"../x52_test_resources/Reload����������Դ/Reload��ͼ/�й�ͬ��ͼ��ʵ������/��mask����/4015012001/Texture/4015012001_M.dds");
	}

private:
	static HippoTestCaseBase* const testcase_;
};
ADD_TESTCASE("RELOAD_TESTCASE",ReloadTextureTwoDml7);


/*�����߼�
	�߼�ͬ����7����lod1�ķ�ʽ����--ע���������Դ����mask���� ��mask��ͼ ����ɫ��ͼ �߹���ͼ�ͷ�����ͼ����ͬ
	ʵ�����޸ĵ���mask��ͼ��ʹ����һ��mask��ͼ�����滻
*/

class ReloadTextureTwoDml8 : public ReloadTextureTwoDml7
{
public:
	ReloadTextureTwoDml8(const char* casename):ReloadTextureTwoDml7(casename){}

	virtual int GetMatLod(){return 1;}

private:
	static HippoTestCaseBase* const testcase_;
};
ADD_TESTCASE("RELOAD_TESTCASE",ReloadTextureTwoDml8);

/*�����߼�
	�߼�ͬ����7����lod2�ķ�ʽ����--ע���������Դ����mask���� ��mask��ͼ ����ɫ��ͼ �߹���ͼ�ͷ�����ͼ����ͬ
	ʵ�����޸ĵ���mask��ͼ��ʹ����һ��mask��ͼ�����滻
*/

class ReloadTextureTwoDml9 : public ReloadTextureTwoDml7
{
public:
	ReloadTextureTwoDml9(const char* casename):ReloadTextureTwoDml7(casename){}

	virtual int GetMatLod(){return 2;}

private:
	static HippoTestCaseBase* const testcase_;
};
ADD_TESTCASE("RELOAD_TESTCASE",ReloadTextureTwoDml9);
