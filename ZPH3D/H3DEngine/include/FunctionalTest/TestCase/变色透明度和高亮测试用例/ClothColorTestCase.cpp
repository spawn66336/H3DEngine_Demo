#include "../../HippoTestManager/HippoTestManager.h"
#include "../CommonCode/HippoUtil/ActorUtil.h"
#include "../CommonCode/HippoUtil/HippoGlobal.h"
#include "../CommonCode/HippoUtil/HippoScene.h"
#include "engine_interface_internal.h"
#include "EffectCore_dx.h"
#include "idMath/dMathHeader.h"

using namespace  std;
/*测试逻辑
以lod0，lod1，lod2的方式分别载入一个actor（actor0，actor1，actor2），给这3个人穿上同样的一件上衣。
Actor0、actor1，actor2的服装染成红色
渲染5s
执行检查点1。
渲染10帧以后，Actor0切换成lod1，actor1切换成lod2，actor2切换成lod0。
渲染5s
执行检查点1。
渲染10帧以后，Actor0切换成lod2，actor1切换成lod0，actor2切换成lod1。
渲染5s
执行检查点1。
检查点1：
　　检查actor的各个renderable的shader中的DiffuseColorFactorC0和DiffuseColorFactorC1的shaderconst的值是红色。
*/
class ClothColor1 : public HippoTestCaseBase
{
public:
	ClothColor1(const char* casename):HippoTestCaseBase(casename){}
	H3DI::IActor* m_pActor0_lod0;
	H3DI::IActor* m_pActor1_lod1;
	H3DI::IActor* m_pActor2_lod2;

	HippoScene* m_pScene;

	enum ExeState
	{
		INIT,
		CREATACTOR,
		SPIN_5S_FIRST,
		TEST_FIRST,
		SPIN_10F_FIRST,
		ACTOR_SET_LOD_FIRST,
		SPIN_5S_SECOND,
		TEST_SECOND,
		SPIN_10F_SECOND,
		ACTOR_SET_LOD_SECOND,
		SPIN_5S_THIRD,
		TEST_THIRD,
		TEST_OVER
	};
	ExeState m_exeState;
private:
	static HippoTestCaseBase* const testcase_;
public:
	virtual void InitScene()
	{
		m_pScene = Hippo_GetSceneManager()->CreateEmptyLevel();
		Hippo_GetIRender()->OpenActionLib("../resources/art/role/actions/role.xml");
		m_exeState  =  INIT;
	}

	virtual void CleanUpScene()
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

	void CreatActor()
	{
		H3DVec3 pos0(-3.f,0.f,0.f);
		H3DVec3 pos2(3.f,0.f,0.f);

		m_pActor0_lod0 = m_pScene->CreateActor(false,0);
		m_pActor1_lod1 = m_pScene->CreateActor(false,1);
		m_pActor2_lod2 = m_pScene->CreateActor(false,2);

		m_pActor0_lod0->SetPosition(pos0);
		m_pActor2_lod2->SetPosition(pos2);

		SetBptOrAdorment();
	}

	virtual void SetBptOrAdorment()
	{
		m_pActor0_lod0->SetBodyPart("../resources/art/role/bodypart/female/body/114001001/114001001.BPT");
		m_pActor1_lod1->SetBodyPart("../resources/art/role/bodypart/female/body/114001001/114001001.BPT");
		m_pActor2_lod2->SetBodyPart("../resources/art/role/bodypart/female/body/114001001/114001001.BPT");

		float red[] = {1.f,0.f,.0f,1.0f};
		float colro1[4] = {0.0f, .0f, .0f, 1.f};

		//m_pActor0_lod0->SetDiffuseColorFactor(red,4,colro1,4,4);
		m_pActor1_lod1->SetDiffuseColorFactor(red,4,colro1,4,4);
		m_pActor2_lod2->SetDiffuseColorFactor(red,4,colro1,4,4);
		Hippo_WriteConsole(CC_WHITE,"创建人物完成");
	}

	void TestPoint()
	{
		//**************************
		//这个函数还没有实现
	}
	virtual bool ManualRunExec()
	{
		switch (m_exeState)
		{
		case INIT:
			NextState();
			break;
		case CREATACTOR:
			CreatActor();
			NextState();
			break;
		case SPIN_5S_FIRST:
			if (Spin_N_Ms(5000))
			{
				NextState();
			}
			break;
		case  TEST_FIRST:
			TestPoint();
			NextState();
			break;
		case SPIN_10F_FIRST:
			if (Spin_N_Frame(10))
			{
				NextState();
			}
			break;
		case ACTOR_SET_LOD_FIRST:
			m_pActor0_lod0->SetMaterialLod(1);
			m_pActor1_lod1->SetMaterialLod(2);
			m_pActor2_lod2->SetMaterialLod(0);
			Hippo_WriteConsole(CC_WHITE,"第一次setlod完成");
			NextState();
			break;
		case SPIN_5S_SECOND:
			if (Spin_N_Ms(5000))
			{
				NextState();
			}
			break;
		case TEST_SECOND:
			TestPoint();
			NextState();
			break;
		case SPIN_10F_SECOND:
			if (Spin_N_Frame(10))
			{
				NextState();
			}
			break;
		case ACTOR_SET_LOD_SECOND:
			m_pActor0_lod0->SetMaterialLod(2);
			m_pActor1_lod1->SetMaterialLod(0);
			m_pActor2_lod2->SetMaterialLod(1);
			
			Hippo_WriteConsole(CC_WHITE,"第二次setlod完成");
			NextState();
			break;
		case SPIN_5S_THIRD:
			if (Spin_N_Ms(5000))
			{
				NextState();
			}
			break;
		case TEST_THIRD:
			TestPoint();
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

ADD_TESTCASE("COLOR_TRANSPARENT_HILIGHT",ClothColor1);

/*测试逻辑
以lod0，lod1，lod2的方式分别载入一个actor（actor0，actor1，actor2），给这3个人穿上同样的一个挂件（翅膀）。
Actor0、actor1，actor2的服装染成红色
渲染5s
执行检查点1。
渲染10帧以后，Actor0切换成lod1，actor1切换成lod2，actor2切换成lod0。
渲染5s
执行检查点1。
渲染10帧以后，Actor0切换成lod2，actor1切换成lod0，actor2切换成lod1。
渲染5s
执行检查点1。
检查点1：
　　检查actor的各个renderable的shader中的DiffuseColorFactorC0和DiffuseColorFactorC1的shaderconst的值是红色。
  */

class ClothColor2 : public ClothColor1
{
public:
	ClothColor2(const char* casename):ClothColor1(casename){}
	virtual void SetBptOrAdorment()
	{
		m_pActor0_lod0->SetAdornment((H3DI::EActorAdornmentPosition)5,"../resources/art/role/link/female/4115/4115028002/4115028002.spe");
		m_pActor1_lod1->SetAdornment((H3DI::EActorAdornmentPosition)5,"../resources/art/role/link/female/4115/4115028002/4115028002.spe");
		m_pActor2_lod2->SetAdornment((H3DI::EActorAdornmentPosition)5,"../resources/art/role/link/female/4115/4115028002/4115028002.spe");
		
		float red[] = {1.f,0.f,0.f,1.0f};
		float colro1[4] = {0.0f, .0f, .0f, 1.f};

		m_pActor0_lod0->SetDiffuseColorFactor(red, 4, colro1, 4, H3DI::BODYPART_NUM+5);
		m_pActor1_lod1->SetDiffuseColorFactor(red, 4, colro1, 4, H3DI::BODYPART_NUM+5);
		m_pActor2_lod2->SetDiffuseColorFactor(red, 4, colro1, 4, H3DI::BODYPART_NUM+5);
		Hippo_WriteConsole(CC_WHITE,"创建人物完成");
	}

private:
	static HippoTestCaseBase* const testcase_;
};
ADD_TESTCASE("COLOR_TRANSPARENT_HILIGHT",ClothColor2);


/*测试逻辑
以lod0的方式载入一个actor（actor0）。
给actor0穿上长裤，长靴
给actor0的袜子染成红色  SetDiffuseColorFactor
渲染5s
给actor0换成短裤+短鞋。
渲染5s
执行检查点3
检查点3：
检查actor的袜子，的shader中的DiffuseColorFactorC0和DiffuseColorFactorC1的shaderconst的值是红色。
  */

class ClothColor3 : public HippoTestCaseBase
{
public:
	ClothColor3(const char* casename):HippoTestCaseBase(casename){}
	H3DI::IActor* m_pActor;
	HippoScene* m_pScene;

	enum ExeState
	{
		INIT,
		CREATE_ACTOR,
		SPIN_5S_FIRST,
		CHAGE_BPT,
		SPIN_5S_SECOND,
		TEST_POINT,
		TEST_OVER
	};
	ExeState m_exeState;

public:

	virtual void InitScene()
	{
		m_pScene = Hippo_GetSceneManager()->CreateEmptyLevel();
		Hippo_GetIRender()->OpenActionLib("../resources/art/role/actions/role.xml");
		m_exeState = INIT;
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

	void TestPoint()
	{
		//*******************
		//这个函数还没是实现
	}
	void CreateActor()
	{
		m_pActor = m_pScene->CreateActor(false);
		m_pActor->SetBodyPart("../resources/art/role/bodypart/female/trousers/116008002/116008002.bpt");
		m_pActor->SetBodyPart("../resources/art/role/bodypart/female/shoe/118017001/118017001.bpt");

		m_pActor->SetBodyPart("../resources/art/role/bodypart/female/socks/117025003/117025003.bpt");

		float red[] = {1.f,0.f,0.f,1.0f};
		float colro1[4] = {0.0f, .0f, .0f, 1.f};

		m_pActor->SetDiffuseColorFactor(red, 4, colro1, 4, 7);
		Hippo_WriteConsole(CC_WHITE,"创建人物完成--长裤 长靴");
	}
	virtual bool ManualRunExec()
	{
		switch (m_exeState)
		{
		case INIT:
			NextState();
			break;
		case CREATE_ACTOR:
			CreateActor();
			NextState();
			break;
		case SPIN_5S_FIRST:
			if (Spin_N_Ms(5000))
			{
				NextState();
			}
			break;
		case CHAGE_BPT:
			//短裤和短靴
			Hippo_WriteConsole(CC_WHITE,"换成短裤短靴");
			m_pActor->SetBodyPart("../resources/art/role/bodypart/female/trousers/116034002/116034002.bpt");
			m_pActor->SetBodyPart("../resources/art/role/bodypart/female/shoe/118114003/118114003.bpt");
			NextState();
			break;
		case SPIN_5S_SECOND:
			if (Spin_N_Ms(5000))
			{
				NextState();
			}
			break;
		case TEST_POINT:
			TestPoint();
			NextState();
			break;
		}
		return false;
	}

	virtual bool AutoRunExec()
	{
		if (m_exeState == TEST_OVER)
		{
			return true;
		}
		return ManualRunExec();
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
private:
	static HippoTestCaseBase* const testcase_;
};
ADD_TESTCASE("COLOR_TRANSPARENT_HILIGHT",ClothColor3);
