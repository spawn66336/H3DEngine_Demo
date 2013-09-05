#include "../HippoTestManager/HippoTestManager.h"
#include "../CommonCode/HippoUtil/HippoScene.h"
#include "../CommonCode/HippoUtil/RefCountCompare.h"
#include "../CommonCode/HippoUtil/HippoGlobal.h"
#include <iostream>
#include "idMath/dMathHeader.h"
#include "engine_interface.h"
#include "../CommonCode/HippoUtil/ActorUtil.h"

using namespace std;

/*
测试逻辑：
actor 反复 unload reload，效果正确，无内存泄漏
播动作连续
*/
class ActorUnloadResourceTest:public HippoTestCaseBase
{
public:
	HippoLevelScene* m_pScene;
	H3DI::IActor* m_pActor;
	H3DI::IAvatarSkeletonModel* m_pPet;
	bool m_bActorSex;

	ActorUnloadResourceTest(const char* case_name) : HippoTestCaseBase(case_name){}

	//
	virtual void InitScene()
	{
		m_pScene=Hippo_GetSceneManager()->CreateEmptyLevel();

		//create actor
		Hippo_GetIRender()->OpenActionLib("../resources/art/role/actions/role.xml");
		ActorUtil::InitActorDress("../resources/config/shared/item/dress.xml");
		ActorUtil::InitPetDress("../resources/config/shared/item/petdress.xml");
		m_bActorSex=false;
		m_pActor=m_pScene->CreateActor(m_bActorSex);

		m_pPet = m_pScene->CreatePet(m_bActorSex);
		
		m_pPet->SetPosition(H3DVec3(2.0f, 0.f, 0.f));

		Hippo_GetIRender()->LoadAction("SD_130BPM_Loveis086_01_2", H3DI::ACTOR_HUMAN, false);
		Hippo_GetIRender()->LoadAction("SD_128BPM_bopeepbopeep059_02_2", H3DI::ACTOR_HUMAN, false);
		Hippo_GetIRender()->LoadAction("SD_132BPM_saywhat045_03_2", H3DI::ACTOR_HUMAN, false);
		Hippo_GetIRender()->LoadAction("pet_ingame_enter_03_f", H3DI::ACTOR_PET, false);
		H3DI::IAnimationChannel* pAnimChannel = m_pActor->GetAnmChannel(0);
		pAnimChannel->SetAction("SD_130BPM_Loveis086_01_2", true);

		pAnimChannel = m_pPet->GetAnmChannel(0);
		pAnimChannel->SetAction("pet_ingame_enter_03_f", true);

		ActorUtil::RandomBodypart(m_pActor,m_bActorSex);
		ActorUtil::RandomLink(m_pActor,m_bActorSex);

		ActorUtil::RandomBodypart(m_pPet,m_bActorSex);
		ActorUtil::RandomLink(m_pPet,m_bActorSex);

		m_pScene->RestructScene();

		m_state=CASE_EXECING;
	}

	void CleanUpScene()
	{
		m_pScene->CleanScene();
		Hippo_GetSceneManager()->DelScene(m_pScene);
	}

	virtual bool AutoRunExec()
	{
		//if(Spin(10000))
		//{
		//	m_state=CASE_END_EXECING;
		//	return true;
		//}
		return true;
	}


	virtual bool ManualRunExec()
	{

		//while (1)
		//{
		//	H3DI::sCreateOp op;
		//	op.mat_lod=0;
		//	H3DI::IActor* pActor = (H3DI::IActor*)Hippo_GetIRender()->CreateActor(op, "asdffff", false, H3DI::ACTION_UPDATE_X52);

		//	H3DI::IAvatarSkeletonModel* pPet = Hippo_GetIRender()->CreateAvatarSkeletonModel(op, "ewrwexcv", false, H3DI::ACTION_UPDATE_X52);

		//	pActor->UnloadResource();

		//	//pActor->ReloadResource();

		//	pPet->UnloadResource();

		//	//pPet->ReloadResource();

		//	pActor->Release();

		//	pPet->Release();
		//}

		return false;
	}


	void DoTest()
	{
		H3DI::sCreateOp op;
		op.geo_lod = 0;
		op.mat_lod = 0;
		H3DI::IActor* pActor = (H3DI::IActor*)Hippo_GetIRender()->CreateActor(op, "actor1", m_bActorSex, H3DI::ACTION_UPDATE_X52);
		H3DI::IAvatarSkeletonModel* pPet = Hippo_GetIRender()->CreateAvatarSkeletonModel(op, "pet1", m_bActorSex, H3DI::ACTION_UPDATE_X52);

		ActorUtil::RandomBodypart(pActor,m_bActorSex);
		ActorUtil::RandomLink(pActor,m_bActorSex);

		ActorUtil::RandomBodypart(pPet,m_bActorSex);
		ActorUtil::RandomLink(pPet,m_bActorSex);

		pActor->Release();
		pPet->Release();
		 
		Hippo_GetIRender()->ClearMaterialLib();
	}

	virtual void BeforeRender()
	{
		Hippo_GetIRender()->UpdateCpuSkin();
		Hippo_GetIRender()->ForceSyncData();
		Hippo_GetIRender()->UpdatePhx();

		//int numReloaded = 0;
		//while(1)
		//{
		//	DoTest();
		//	//m_pActor->UnloadResource();
		//	//m_pPet->UnloadResource();

		//	////m_pActor->ReloadResource();
		//	////m_pPet->ReloadResource();

		//	//ActorUtil::RandomBodypart(m_pActor,m_bActorSex);
		//	//ActorUtil::RandomLink(m_pActor,m_bActorSex);

		//	//ActorUtil::RandomBodypart(m_pPet,m_bActorSex);
		//	//ActorUtil::RandomLink(m_pPet,m_bActorSex);

		//	Hippo_WriteConsole(CC_WHITE,"reloaded %d", ++numReloaded);
		//}

	}




	virtual int OnKeyDown(unsigned int key)
	{
		switch( key )
		{
		case VK_RETURN:
			{
				m_state=CASE_END_EXECING;
				return 1;
			}
		case VK_LEFT:
			{
				static bool bUnload = true;

				if (bUnload)
				{
					m_pActor->UnloadResource();
					m_pPet->UnloadResource();

					m_pActor->SetTransparence(0.2f);
					//H3DI::IAnimationChannel* pAnimChannel = m_pActor->GetAnmChannel(0);
					//pAnimChannel->SetAction("SD_132BPM_saywhat045_03_2", true);

					//ActorUtil::RandomBodypart(m_pActor,m_bActorSex);

					Hippo_GetIRender()->ClearMaterialLib();
				}
				else
				{
					m_pActor->ReloadResource();
					m_pPet->ReloadResource();

					ActorUtil::RandomBodypart(m_pActor,m_bActorSex);
					ActorUtil::RandomLink(m_pActor,m_bActorSex);

					ActorUtil::RandomBodypart(m_pPet,m_bActorSex);
					ActorUtil::RandomLink(m_pPet,m_bActorSex);

					//m_pActor->SetAdornment((H3DI::EActorAdornmentPosition)5,"../resources/art/role/link/female/4115/4115012002/4115012002.spe");
					//ActorUtil::RandomLink(m_pActor,m_bActorSex);
				}

				bUnload = !bUnload;

				return 1;
			}
		case VK_RIGHT:
			{
				static bool bHideAdorment = false;

				m_pActor->SetAdornmentVisibility(H3DI::ACTOR_ADORNMENT_LEFTWRIST, bHideAdorment);
				m_pActor->SetAdornmentVisibility(H3DI::ACTOR_ADORNMENT_RIGHTWRIST, bHideAdorment);
				m_pActor->SetAdornmentVisibility(H3DI::ACTOR_ADORNMENT_TAIL, bHideAdorment);
				m_pActor->SetAdornmentVisibility(H3DI::ACTOR_ADORNMENT_LEFTHAND, bHideAdorment);
				m_pActor->SetAdornmentVisibility(H3DI::ACTOR_ADORNMENT_RIGHTHAND, bHideAdorment);

				m_pPet->SetAdornmentVisibility(H3DI::PET_ADORNMENT_BACK, bHideAdorment);
				m_pPet->SetAdornmentVisibility(H3DI::PET_ADORNMENT_ANKLETS, bHideAdorment);
				m_pPet->SetAdornmentVisibility(H3DI::PET_ADORNMENT_LEFTHAND, bHideAdorment);
				m_pPet->SetAdornmentVisibility(H3DI::PET_ADORNMENT_RIGHTHAND, bHideAdorment);

				bHideAdorment = !bHideAdorment;

				return 1;
			}
		case VK_DOWN:
			{
				m_pScene->CleanScene();
			}
		case '1':
			{
				// test for error detect
				m_pActor->UnloadResource();

				// unload之后不应有的调用
				m_pActor->SetBodyPart("../resources/art/role/bodypart/female/hair/113069003/113069003.BPT");
				m_pActor->SetAdornment(H3DI::ACTOR_ADORNMENT_TAIL, "../resources/art/role/link/female/4112/4112001001/4112001001.spe");

				// 不应再次调用unload
				m_pActor->UnloadResource();

				m_pActor->ReloadResource();
				// 不应再次调用reload
				m_pActor->ReloadResource();
				return 1;
			}
		case '2':
			{
				// unload之后高跟鞋的偏移也应该复位
				
				// 执行以下过程actor的位置应该不变（错误的情况是actor的位置会越来越高）
				m_pActor->SetBodyPart("../resources/art/role/bodypart/female/shoe/118008001/118008001.BPT");
				m_pActor->UnloadResource();
				m_pActor->ReloadResource();
				m_pActor->SetBodyPart("../resources/art/role/bodypart/female/shoe/118008001/118008001.BPT");

				break;
			}
		}
		return 0;
	}

	//!自定义的键盘消息处理函数，返回0表示对该消息不感兴趣，返回1表示已经处理，不需要默认消息处理函数再处理
	virtual int ProcessInput(unsigned int nChar, bool bKeyDown, bool bAltDown,bool bCrtlDown,bool bShiftDown, void* pUserContext )
	{
		return 0;
	}

private:
	static HippoTestCaseBase* const testcase_;
};

ADD_TESTCASE("ActorUnloadResourceTest",ActorUnloadResourceTest)

