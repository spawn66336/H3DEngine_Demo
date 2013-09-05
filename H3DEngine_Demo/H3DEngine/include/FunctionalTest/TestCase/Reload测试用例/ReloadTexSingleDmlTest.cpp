#include "../../HippoTestManager/HippoTestManager.h"
#include "../CommonCode/HippoUtil/ActorUtil.h"
#include "../CommonCode/HippoUtil/HippoScene.h"
#include "../CommonCode/HippoUtil/HippoGlobal.h"
#include "../CommonCode/HippoUtil/RefCountCompare.h"
#include "engine_interface.h"
#include "engine_interface_internal.h"
#include "../CommonCode/HippoUtil/HippoStateMachine.h"
#include "../CommonCode/HippoUtil/HippoTaskVisitor.h"
#include "EffectCore_dx.h"
#include <algorithm>
#include <stdio.h>
#include <Windows.h>
#include "../../FunctionalTest/functional_test/HippoTestConfig.h"
using namespace  std;

/*�����߼�
׼��һ��dml����ʹ���˴�����ͼ�Ĳ���ʵ��lod0
��lod0��ʽ���ش�dml��������5s
���ü�������s1
�ڲ���������ͨ�������޸�dml�Ĳ���ʵ�����ݣ����������ʵ������ͼ��һ��
	---ע������4110003001����Դ���滻��ͼ��������ͼ�滻������ͼ
����ģ�͵�reload Texturen��������Ⱦ5s
�鿴Ч���仯
ִ��clearmatlib
���ü�������s2
s1��2�Ĳ��Ӧ����һ����ͼ�Ĳ��
*/

 
class ReloadTexture1 : public HippoTestCase_RefCompare
{
public:
	ReloadTexture1(const char* casename):HippoTestCase_RefCompare(casename){}
	HippoLevelScene* m_pScene;
	H3DI::IModel* m_pDml;
	Hippo_StateMachine stateMachine;
	HippoResStateSnapShot* s1Tex;
	HippoResStateSnapShot* s1Mat;
	HippoResStateSnapShot* s1Tpl;

	HippoResStateSnapShot* s2Tex;
	HippoResStateSnapShot* s2Mat;
	HippoResStateSnapShot* s2Tpl;
	bool bAutoEnd;
private:
	static HippoTestCaseBase* const testcase_;
public:
	virtual int GetMatLod(){return 0;}

	virtual void CopyOrDel(bool bCopy)
	{
		if (bCopy)
		{//���������� ���Ա�֤ÿ��ʹ�õ�mat�ļ�������ȷ��
			CopyFile("../x52_test_resources/Reload����������Դ/Reload��ͼ/4110003001/back/4110003001.mat","../x52_test_resources/Reload����������Դ/Reload��ͼ/4110003001/4110003001.mat",TRUE);
			CopyFile("../x52_test_resources/Reload����������Դ/Reload��ͼ/4110003001/back/__4110003001.mat","../x52_test_resources/Reload����������Դ/Reload��ͼ/4110003001/__4110003001.mat",TRUE);
		}
		else
		{
			//ÿ�γ����˳�ǰ ���ʹ�õ�mat�ļ�
			DeleteFile("../x52_test_resources/Reload����������Դ/Reload��ͼ/4110003001/4110003001.mat");
			DeleteFile("../x52_test_resources/Reload����������Դ/Reload��ͼ/4110003001/__4110003001.mat");
			DeleteFile("../x52_test_resources/Reload����������Դ/Reload��ͼ/4110003001/back.mat");
		}
	}
	virtual void ChangeNameFirst()
	{
		rename("../x52_test_resources/Reload����������Դ/Reload��ͼ/4110003001/4110003001.mat","../x52_test_resources/Reload����������Դ/Reload��ͼ/4110003001/back.mat");
		rename("../x52_test_resources/Reload����������Դ/Reload��ͼ/4110003001/__4110003001.mat","../x52_test_resources/Reload����������Դ/Reload��ͼ/4110003001/4110003001.mat");
	}
	
	virtual void Createdml()
	{
		CopyOrDel(true);
		m_pDml=m_pScene->CreateDml("../x52_test_resources/Reload����������Դ/Reload��ͼ/4110003001/4110003.dml",GetMatLod());
	}

	 SCSTATE OnState0()
	{
		Hippo_WriteConsole(CC_WHITE,"CreateDml()");
	    Createdml();
		return STATE_CHANGE;
	}
	SCSTATE OnState1()
	{
		
		if (Spin_N_Ms(5000))
		{
			Hippo_WriteConsole(CC_WHITE,"��Ⱦ5s����");
			return STATE_CHANGE;
		}
		return STATE_NOT_CHANGE;
	}
	SCSTATE OnState2()
	{
		Hippo_WriteConsole(CC_WHITE,"���ü�������s1");
		s1Tex = RefCountCompare::GetResState(HIPPO_TEX_RES);
		s1Mat = RefCountCompare::GetResState(HIPPO_MAT_RES);
		s1Tpl = RefCountCompare::GetResState(HIPPO_TPL_RES);
		return STATE_CHANGE;
	}



	//�ڲ���������ͨ�������޸�dml�Ĳ���ʵ�����ݣ����������ʵ������ͼ��һ��
	SCSTATE OnState3()
	{
		Hippo_WriteConsole(CC_WHITE,"���������ʵ������ͼ��һ��");
		
		ChangeNameFirst();
		
		return STATE_CHANGE;
	}
	//����ģ�͵�reload Texturen����
	SCSTATE OnState4()
	{
		Hippo_WriteConsole(CC_WHITE,"ReloadFromDisk()");
		int res = m_pDml->ReloadAll();
		CopyOrDel(false);
		return STATE_CHANGE;
	}
	SCSTATE OnState5()
	{
		Hippo_WriteConsole(CC_WHITE,"��¼���ü������Ƚ�");
		Hippo_GetIRender()->ClearMaterialLib();
		s2Tex = RefCountCompare::GetResState(HIPPO_TEX_RES);
		s2Mat = RefCountCompare::GetResState(HIPPO_MAT_RES);
		s2Tpl = RefCountCompare::GetResState(HIPPO_TPL_RES);

		HippoResStateSSDiff* diff = RefCountCompare::GetSnapShotDiff(s1Tex,s2Tex);
		vector<_DiffResState> diffvec = diff->m_DiffVec;
		vector<_DiffResState>::size_type size = diffvec.size();
		//������СӦ��Ϊ2����Ϊֻ��һ����ͼ����
 		HIPPO_EXPECT_EQ((size==2),true);
		vector<_DiffResState>::iterator itr = diffvec.begin();
		while (itr != diffvec.end())
		{
			Hippo_WriteConsoleAndLog(CC_WHITE,"%s   %d----%d",itr->m_name,itr->m_ref_diff,itr->m_position);
			bool flag = (*itr).m_position != BOTH; 
			HIPPO_EXPECT_EQ(flag,true);
			++itr;
		}
		
		HIPPO_EXPECT_EQ(RefCountCompare::SnapShotEqual(s1Mat,s2Mat),true);
		HIPPO_EXPECT_EQ(RefCountCompare::SnapShotEqual(s2Tpl,s1Tpl),true);
		diff->Release();
		s1Tex->Release();
		s1Mat->Release();
		s1Tpl->Release();

		s2Tex->Release();
		s2Mat->Release();
		s2Tpl->Release();

		bAutoEnd = true;
		
		return STATE_CHANGE;
	}

	void InitScene()
	{
		m_pScene = Hippo_GetSceneManager()->CreateEmptyLevel();

		stateMachine.AddState(0,std::tr1::bind(&ReloadTexture1::OnState0,this));
		stateMachine.AddState(1,std::tr1::bind(&ReloadTexture1::OnState1,this));
		stateMachine.AddState(2,std::tr1::bind(&ReloadTexture1::OnState2,this));
		stateMachine.AddState(3,std::tr1::bind(&ReloadTexture1::OnState3,this));
		stateMachine.AddState(4,std::tr1::bind(&ReloadTexture1::OnState4,this));
		stateMachine.AddState(5,std::tr1::bind(&ReloadTexture1::OnState1,this));
		stateMachine.AddState(6,std::tr1::bind(&ReloadTexture1::OnState5,this));

		bAutoEnd = false;
	}

	void CleanUpScene()
	{
		m_pScene->CleanScene();
		Hippo_GetSceneManager()->DelScene(m_pScene);
	}

	bool ManualRunExec()
	{

		stateMachine.Run();
		return false;
	}

	bool AutoRunExec()
	{
		if (bAutoEnd)
		{
			return true;
		}
		return ManualRunExec();
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

ADD_TESTCASE("RELOAD_TESTCASE",ReloadTexture1);

/*�����߼�
ʹ��lod1����һ��dml������ͬ��
*/
class ReloadTexture2 : public ReloadTexture1
{
public:
	ReloadTexture2(const char* casename):ReloadTexture1(casename){}
	virtual int GetMatLod(){return 1;}
private:
	static HippoTestCaseBase* const testcase_;
};
ADD_TESTCASE("RELOAD_TESTCASE",ReloadTexture2);

/*�����߼�
ʹ��lod2����һ��dml������ͬ��
*/
class ReloadTexture3 : public ReloadTexture1
{
public:
	ReloadTexture3(const char* casename):ReloadTexture1(casename){}
	virtual int GetMatLod(){return 2;}
private:
	static HippoTestCaseBase* const testcase_;
};
ADD_TESTCASE("RELOAD_TESTCASE",ReloadTexture3);

/*�����߼�
ʹ��lod0����4115001001 �µ�dml������ͬ��
---ע������4115001001����Դ���滻��ͼ���÷�����ͼ�滻����ɫ��ͼ
*/
class ReloadTexture4 : public ReloadTexture1
{
public:
	ReloadTexture4(const char* casename):ReloadTexture1(casename){}
	virtual int GetMatLod(){return 0;}
	virtual void CopyOrDel(bool bCopy)
	{
		if (bCopy)
		{
			//���������� ���Ա�֤ÿ��ʹ�õ�mat�ļ�������ȷ��
			CopyFile("../x52_test_resources/Reload����������Դ/Reload��ͼ/4115001001/back/4115001001.mat","../x52_test_resources/Reload����������Դ/Reload��ͼ/4115001001/4115001001.mat",TRUE);
			CopyFile("../x52_test_resources/Reload����������Դ/Reload��ͼ/4115001001/back/__4115001001.mat","../x52_test_resources/Reload����������Դ/Reload��ͼ/4115001001/__4115001001.mat",TRUE);
		}
		else
		{
			//ÿ�γ����˳�ǰ ���ʹ�õ�mat�ļ�
			DeleteFile("../x52_test_resources/Reload����������Դ/Reload��ͼ/4115001001/4115001001.mat");
			DeleteFile("../x52_test_resources/Reload����������Դ/Reload��ͼ/4115001001/__4115001001.mat");
			DeleteFile("../x52_test_resources/Reload����������Դ/Reload��ͼ/4115001001/back.mat");
		}
	}

	virtual void ChangeNameFirst()
	{
		rename("../x52_test_resources/Reload����������Դ/Reload��ͼ/4115001001/4115001001.mat","../x52_test_resources/Reload����������Դ/Reload��ͼ/4115001001/back.mat");
		rename("../x52_test_resources/Reload����������Դ/Reload��ͼ/4115001001/__4115001001.mat","../x52_test_resources/Reload����������Դ/Reload��ͼ/4115001001/4115001001.mat");
	}

	virtual void Createdml()
	{
		CopyOrDel(true);
		m_pDml=m_pScene->CreateDml("../x52_test_resources/Reload����������Դ/Reload��ͼ/4115001001/4115001.dml",GetMatLod());
	}
private:
	static HippoTestCaseBase* const testcase_;
};
ADD_TESTCASE("RELOAD_TESTCASE",ReloadTexture4);


/*�����߼�
ʹ��lod1����4115001001 �µ�dml������ͬ��
*/
class ReloadTexture5 : public ReloadTexture4
{
public:
	ReloadTexture5(const char* casename):ReloadTexture4(casename){}
	virtual int GetMatLod(){return 1;}
private:
	static HippoTestCaseBase* const testcase_;
};
ADD_TESTCASE("RELOAD_TESTCASE",ReloadTexture5);


/*�����߼�
ʹ��lod2����4115001001 �µ�dml������ͬ��
*/
class ReloadTexture6 : public ReloadTexture4
{
public:
	ReloadTexture6(const char* casename):ReloadTexture4(casename){}
	virtual int GetMatLod(){return 2;}
private:
	static HippoTestCaseBase* const testcase_;
};
ADD_TESTCASE("RELOAD_TESTCASE",ReloadTexture6);


/*�����߼�
ʹ��lod0����4115ddd �µ�dml������ͬ��
---ע������4115ddd����Դ���滻��ͼ���ù���ɫ��ͼ�滻����ɫ��ͼ
*/
class ReloadTexture7 : public ReloadTexture1
{
public:
	ReloadTexture7(const char* casename):ReloadTexture1(casename){}
	virtual int GetMatLod(){return 0;}

	virtual void CopyOrDel(bool bCopy)
	{
		if (bCopy)
		{
			//���������� ���Ա�֤ÿ��ʹ�õ�mat�ļ�������ȷ��
			CopyFile("../x52_test_resources/Reload����������Դ/Reload��ͼ/4115ddd/back/4115001001.mat","../x52_test_resources/Reload����������Դ/Reload��ͼ/4115ddd/4115001001.mat",TRUE);
			CopyFile("../x52_test_resources/Reload����������Դ/Reload��ͼ/4115ddd/back/__4115001001.mat","../x52_test_resources/Reload����������Դ/Reload��ͼ/4115ddd/__4115001001.mat",TRUE);
		}
		else
		{
			//ÿ�γ����˳�ǰ ���ʹ�õ�mat�ļ�
			DeleteFile("../x52_test_resources/Reload����������Դ/Reload��ͼ/4115ddd/4115001001.mat");
			DeleteFile("../x52_test_resources/Reload����������Դ/Reload��ͼ/4115ddd/__4115001001.mat");
			DeleteFile("../x52_test_resources/Reload����������Դ/Reload��ͼ/4115ddd/back.mat");
		}
	}


	virtual void ChangeNameFirst()
	{
		rename("../x52_test_resources/Reload����������Դ/Reload��ͼ/4115ddd/4115001001.mat","../x52_test_resources/Reload����������Դ/Reload��ͼ/4115ddd/back.mat");
		rename("../x52_test_resources/Reload����������Դ/Reload��ͼ/4115ddd/__4115001001.mat","../x52_test_resources/Reload����������Դ/Reload��ͼ/4115ddd/4115001001.mat");
	}
	virtual void Createdml()
	{
		CopyOrDel(true);
		m_pDml=m_pScene->CreateDml("../x52_test_resources/Reload����������Դ/Reload��ͼ/4115ddd/4115001.dml",GetMatLod());
	}
private:
	static HippoTestCaseBase* const testcase_;
};
ADD_TESTCASE("RELOAD_TESTCASE",ReloadTexture7);


/*�����߼�
ʹ��lod1����4115ddd �µ�dml������ͬ��
*/
class ReloadTexture8 : public ReloadTexture7
{
public:
	ReloadTexture8(const char* casename):ReloadTexture7(casename){}
	virtual int GetMatLod(){return 1;}
private:
	static HippoTestCaseBase* const testcase_;
};
ADD_TESTCASE("RELOAD_TESTCASE",ReloadTexture8);


/*�����߼�
ʹ��lod2����4115ddd �µ�dml������ͬ��
*/
class ReloadTexture9 : public ReloadTexture7
{
public:
	ReloadTexture9(const char* casename):ReloadTexture7(casename){}
	virtual int GetMatLod(){return 2;}
private:
	static HippoTestCaseBase* const testcase_;
};
ADD_TESTCASE("RELOAD_TESTCASE",ReloadTexture9);

/*�����߼�
����4110001001��dml ����ʹ���� ��������ͼ 
ʹ��һ����������ͼchanged.dds �滻ԭ�е���������ͼhdr.dds
*/
class ReloadTexture10:public ReloadTexture1
{
public:
	ReloadTexture10(const char* casename):ReloadTexture1(casename){}
	virtual int GetMatLod(){return 0;}
	virtual void CopyOrDel(bool bCopy)
	{
		if (bCopy)
		{
			CopyFile(Make_HippoTestResource_Path("Reload����������Դ/Reload��ͼ/4110001001/back/hdr.dds").c_str(),
				Make_HippoTestResource_Path("Reload����������Դ/Reload��ͼ/4110001001/hdr.dds").c_str(),true);
			CopyFile(Make_HippoTestResource_Path("Reload����������Դ/Reload��ͼ/4110001001/back/changed.dds").c_str(),
				Make_HippoTestResource_Path("Reload����������Դ/Reload��ͼ/4110001001/changed.dds").c_str(),true);
		}
		else
		{
			DeleteFile(Make_HippoTestResource_Path("Reload����������Դ/Reload��ͼ/4110001001/hdr.dds").c_str());
			DeleteFile(Make_HippoTestResource_Path("Reload����������Դ/Reload��ͼ/4110001001/changed.dds").c_str());
			DeleteFile(Make_HippoTestResource_Path("Reload����������Դ/Reload��ͼ/4110001001/back.dds").c_str());
		}
	}
	virtual void  ChangeNameFirst()
	{
		rename(Make_HippoTestResource_Path("Reload����������Դ/Reload��ͼ/4110001001/hdr.dds").c_str(),
				Make_HippoTestResource_Path("Reload����������Դ/Reload��ͼ/4110001001/back.dds").c_str());
		rename(Make_HippoTestResource_Path("Reload����������Դ/Reload��ͼ/4110001001/changed.dds").c_str(),
			Make_HippoTestResource_Path("Reload����������Դ/Reload��ͼ/4110001001/hdr.dds").c_str());
	}
	virtual void Createdml()
	{
		CopyOrDel(true);
		m_pDml=m_pScene->CreateDml(Make_HippoTestResource_Path("Reload����������Դ/Reload��ͼ/4110001001/4110001.dml").c_str(),GetMatLod());
	}
private: 
	static HippoTestCaseBase* const testcase_;
};
ADD_TESTCASE("RELOAD_TESTCASE",ReloadTexture10);




