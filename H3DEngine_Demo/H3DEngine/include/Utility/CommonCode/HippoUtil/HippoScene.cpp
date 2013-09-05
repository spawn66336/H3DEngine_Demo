#include "HippoScene.h"
#include "engine_interface.h"
#include "dMathHeader.h"
#include "EffectCore_dx.h"
#include "HippoGlobal.h"
#include "ActorUtil.h"
#include <algorithm>
#include "log/HippoLog.h"
HippoScene::HippoScene()
{

}

HippoScene::~HippoScene()
{
	CleanScene();
}

void HippoScene::DetachAll()
{
	Hippo_WriteConsoleAndLog(CC_WHITE,"HippoScene::DetachAll Begin");
	{
		IActorConItr itr=m_IActorCon.begin();
		while (itr!=m_IActorCon.end())
		{
			GetH3DScene()->DetachModel((*itr) );
			++itr;
		}
	}
	{
		IAvatarSkeletonModelConItr itr=m_IAvatarSkeletonModelCon.begin();
		while (itr!=m_IAvatarSkeletonModelCon.end())
		{
			GetH3DScene()->DetachModel((*itr) );
			++itr;
		}
	}

	{
		IModelConItr itr=m_IModelCon.begin();
		while (itr!=m_IModelCon.end())
		{
			GetH3DScene()->DetachModel((*itr) );
			++itr;
		}
	}
	{
		ISkeletonModelConItr itr=m_ISkeletonModelCon.begin();
		while (itr!=m_ISkeletonModelCon.end())
		{
			GetH3DScene()->DetachModel((*itr) );
			++itr;
		}
	}

	{
		ISpecialEffectConItr itr=m_ISpecialEffectCon.begin();
		while (itr!=m_ISpecialEffectCon.end())
		{
			GetH3DScene()->DetachModel((*itr) );
			++itr;
		}
	}
	for(int i=0;i<H3DI::LIGHTAFFECTPARAM_END;++i)
	{
		IPrePassLightConItr itr=m_DirCon[i].begin();
		while (itr!=m_DirCon[i].end())
		{

			H3DI::IPrePassLight* plight=*itr;
			GetH3DScene()->DetachModel(plight);
			++itr;
		}
	}
	for(int i=0;i<H3DI::LIGHTAFFECTPARAM_END;++i)
	{
		IPrePassLightConItr itr=m_PointCon[i].begin();
		while (itr!=m_PointCon[i].end())
		{
			H3DI::IPrePassLight* plight=*itr;
			GetH3DScene()->DetachModel(plight);
			++itr;
		}
	}
	for(int i=0;i<H3DI::LIGHTAFFECTPARAM_END;++i)
	{
		IPrePassLightConItr itr=m_SpotCon[i].begin();
		while (itr!=m_SpotCon[i].end())
		{
			H3DI::IPrePassLight* plight=*itr;
			GetH3DScene()->DetachModel(plight);
			++itr;
		}
	}
	Hippo_WriteConsoleAndLog(CC_WHITE,"HippoScene::DetachAll End");
}
void HippoScene::CleanScene()
{
	Hippo_WriteConsoleAndLog(CC_WHITE,"HippoScene::CleanScene Begin");
	{
		IActorConItr itr=m_IActorCon.begin();
		while (itr!=m_IActorCon.end())
		{
			(*itr)->Release();
			++itr;
		}
		m_IActorCon.clear();
	}
	{
		IAvatarSkeletonModelConItr itr=m_IAvatarSkeletonModelCon.begin();
		while (itr!=m_IAvatarSkeletonModelCon.end())
		{
			(*itr)->Release();
			++itr;
		}
		m_IAvatarSkeletonModelCon.clear();
	}

	{
		IModelConItr itr=m_IModelCon.begin();
		while (itr!=m_IModelCon.end())
		{
			(*itr)->Release();
			++itr;
		}
		m_IModelCon.clear();
	}
	{
		ISkeletonModelConItr itr=m_ISkeletonModelCon.begin();
		while (itr!=m_ISkeletonModelCon.end())
		{
			(*itr)->Release();
			++itr;
		}
		m_ISkeletonModelCon.clear();
	}

	{
		ISpecialEffectConItr itr=m_ISpecialEffectCon.begin();
		while (itr!=m_ISpecialEffectCon.end())
		{
			(*itr)->Release();
			++itr;
		}
		m_ISpecialEffectCon.clear();
	}
	for(int i=0;i<H3DI::LIGHTAFFECTPARAM_END;++i)
	{
		IPrePassLightConItr itr=m_DirCon[i].begin();
		while (itr!=m_DirCon[i].end())
		{

			H3DI::IPrePassLight* plight=*itr;
			plight->Release();
			++itr;
		}
		m_DirCon[i].clear();
	}
	for(int i=0;i<H3DI::LIGHTAFFECTPARAM_END;++i)
	{
		IPrePassLightConItr itr=m_PointCon[i].begin();
		while (itr!=m_PointCon[i].end())
		{
			H3DI::IPrePassLight* plight=*itr;
			plight->Release();
			++itr;
		}
		m_PointCon[i].clear();
	}
	for(int i=0;i<H3DI::LIGHTAFFECTPARAM_END;++i)
	{
		IPrePassLightConItr itr=m_SpotCon[i].begin();
		while (itr!=m_SpotCon[i].end())
		{
			H3DI::IPrePassLight* plight=*itr;
			plight->Release();
			++itr;
		}
		m_SpotCon[i].clear();
	}

	H3DI::IRender* pRender=Hippo_GetIRender();
	pRender->ClearMaterialLib();

	Hippo_WriteConsoleAndLog(CC_WHITE,"HippoScene::CleanScene End");
}

H3DI::IModel*				HippoScene::CreateDml(const char* fn, int matlod)
{
	HippoFunLogObj("HippoScene::CreateDml");
	//Hippo_WriteConsoleAndLog(CC_WHITE,"HippoScene::CreateDml Begin");
	H3DI::IRender* pRender=Hippo_GetIRender();
	H3DI::sCreateOp op;
	op.mat_lod=matlod;
	H3DI::IModel* p=pRender->CreateModel(op,fn);
	if(!p)
	{
		Hippo_WriteConsoleAndLog(CC_RED,"¶ÁÈ¡dmlÊ§°Ü:%s",fn);
		return 0;
	}
	p->Update(0);
	m_IModelCon.push_back(p);

	GetH3DScene()->AttachModel((H3DI::IMoveObject*)p,H3DI::SL_DetailObj);
	
	return p;
}


ISpecialEffect*		HippoScene::CreateSpe(const char* fn,int matlod)
{
	HippoFunLogObj("HippoScene::CreateSpe");
	H3DI::sCreateOp op;
	op.mat_lod=matlod;
	ISpecialEffect* p=Hippo_GetSpeManager()->getSpecialEffect(op,fn);
	if(!p)
	{
		Hippo_WriteConsoleAndLog(CC_RED,"¶ÁÈ¡speÊ§°Ü:%s",fn);
		return 0;
	}
	unsigned int f=p->getState();
	p->setState(f|EFFECT_ACTIVATED);
	p->update(0);

	m_ISpecialEffectCon.push_back(p);
	GetH3DScene()->AttachModel((H3DI::IMoveObject*)p,H3DI::SL_SpecialEffect);
	return p;
}

H3DI::INewShader*			HippoScene::CreateShader(const char* fn,const char* matn)
{
	HippoFunLogObj("HippoScene::CreateShader");
	H3DI::INewShader* p=Hippo_GetIRender()->GetNewShader(fn,matn);
	if(!p)
	{
		Hippo_WriteConsoleAndLog(CC_RED,"¶ÁÈ¡ShaderÊ§°Ü:%s,%s",fn,matn);
		return 0;
	}

	return p;
}

H3DI::ISkeletonModel*		HippoScene::CreateChr(const char* fn,int matlod)
{
	HippoFunLogObj("HippoScene::CreateChr");
	H3DI::IRender* pRender=Hippo_GetIRender();
	H3DI::sCreateOp op;
	op.mat_lod=matlod;
	H3DI::ISkeletonModel* p=(H3DI::ISkeletonModel*)pRender->CreateModel(op,fn);
	if(!p)
	{
		Hippo_WriteConsoleAndLog(CC_RED,"¶ÁÈ¡chrÊ§°Ü:%s",fn);
		return 0;
	}
	p->Update(0);
	m_ISkeletonModelCon.push_back(p);

	GetH3DScene()->AttachModel((H3DI::IMoveObject*)p,H3DI::SL_DetailObj);

	return p;
}

unsigned int HippoScene::CreateChrAysn(const char* fn, int matlod)
{
	HippoFunLogObj("HippoScene::CreateChrAysn");
	H3DI::IRender* pRender=Hippo_GetIRender();
	H3DI::sCreateOp op;
	op.mat_lod=matlod;

	return pRender->CreateModelAsyn(fn, op, 0);
}

std::string MakeAutoName(const char* prefix)
{
	static size_t idx=0;
	std::string res=prefix;
	++idx;
	char buff[32];
	_itoa_s(idx,buff,32,10);
	res+=buff;
	return res;
}

H3DI::IAvatarSkeletonModel* HippoScene::CreatePet(bool bmale,int matlod)
{
	HippoFunLogObj("HippoScene::CreatePet");
	std::string n=MakeAutoName("pet");
	H3DI::IRender* pRender=Hippo_GetIRender();
	H3DI::sCreateOp op;
	op.mat_lod=matlod;
	H3DI::IAvatarSkeletonModel* p=(H3DI::IAvatarSkeletonModel*)pRender->CreateAvatarSkeletonModel(op,n.c_str(),bmale,H3DI::ACTION_UPDATE_PH);
	if(!p)
	{
		Hippo_WriteConsoleAndLog(CC_RED,"Create PetÊ§°Ü");
		return 0;
	}
	p->Update(0);
	m_IAvatarSkeletonModelCon.push_back(p);

	GetH3DScene()->AttachModel((H3DI::IMoveObject*)p,H3DI::SL_Actors);

	return p;
}

H3DI::IActor* HippoScene::CreateActor(bool bmale,int matlod)
{
	HippoFunLogObj("HippoScene::CreateActor");
	std::string n=MakeAutoName("ac");
	H3DI::IRender* pRender=Hippo_GetIRender();
	H3DI::sCreateOp op;
	op.mat_lod=matlod;
	H3DI::IActor* p=(H3DI::IActor*)pRender->CreateActor(op,n.c_str(),bmale,H3DI::ACTION_UPDATE_PH);
	if(!p)
	{
		Hippo_WriteConsoleAndLog(CC_RED,"Create ActorÊ§°Ü");
		return 0;
	}
	p->Update(0);
	m_IActorCon.push_back(p);

	GetH3DScene()->AttachModel((H3DI::IMoveObject*)p,H3DI::SL_Actors);

	return p;
}

H3DI::IActor* HippoScene::CreateActor_RandomBodyPart(bool bmale,int matlod)
{
	HippoFunLogObj("HippoScene::CreateActor_RandomBodyPart");
	H3DI::IActor* p=CreateActor(bmale,matlod);
	ActorUtil::RandomBodypart(p,bmale);
	return p;
}

H3DI::IActor* HippoScene::CreateActor_RandomAllDress(bool bmale,int matlod)
{
	HippoFunLogObj("HippoScene::CreateActor_RandomAllDress");
	H3DI::IActor* p=CreateActor(bmale,matlod);
	ActorUtil::RandomBodypart(p,bmale);
	ActorUtil::RandomLink(p,bmale);
	return p;
}


H3DI::IPrePassLight* HippoScene::CreateLight(H3DI::LightAffectParam e,H3DI::LIGHT_TYPE t)
{
	HippoFunLogObj("HippoScene::CreateLight");
	H3DI::IRender* pRender=Hippo_GetIRender();
	H3DI::IPrePassLight* p=pRender->CreatePrePassLight(t);
	p->SetLightAffectParam(e);
	GetLightCon(p)->push_back(p);
	GetH3DScene()->AttachModel((H3DI::IMoveObject*)p,H3DI::SL_Lights);
	return p;
}


bool HippoScene::DelActor(H3DI::IActor* p)
{
	HippoFunLogObj("HippoScene::DelActor");
	GetH3DScene()->DetachModel(p);
	p->Release();
	m_IActorCon.erase(std::find(m_IActorCon.begin(),m_IActorCon.end(),p));
	return true;
}
bool HippoScene::DelDml(H3DI::IModel* p)
{
	HippoFunLogObj("HippoScene::DelDml");
	GetH3DScene()->DetachModel(p);
	p->Release();
	m_IModelCon.erase(std::find(m_IModelCon.begin(),m_IModelCon.end(),p));
	return true;
}
bool HippoScene::DelSpe(ISpecialEffect* p)
{
	HippoFunLogObj("HippoScene::DelSpe");
	GetH3DScene()->DetachModel(p);
	p->Release();
	m_ISpecialEffectCon.erase(std::find(m_ISpecialEffectCon.begin(),m_ISpecialEffectCon.end(),p));
	return true;
}
bool HippoScene::DelShader(H3DI::INewShader* p)
{
	return true;
}
bool HippoScene::DelChr(H3DI::ISkeletonModel* p)
{
	HippoFunLogObj("HippoScene::DelChr");
	GetH3DScene()->DetachModel(p);
	p->Release();
	m_ISkeletonModelCon.erase(std::find(m_ISkeletonModelCon.begin(),m_ISkeletonModelCon.end(),p));
	return true;
}
bool HippoScene::DelPet(H3DI::IAvatarSkeletonModel* p)
{
	HippoFunLogObj("HippoScene::DelPet");
	GetH3DScene()->DetachModel(p);
	p->Release();
	m_IAvatarSkeletonModelCon.erase(std::find(m_IAvatarSkeletonModelCon.begin(),m_IAvatarSkeletonModelCon.end(),p));
	return true;
}

template<class Container,class Item>
bool DeleteItemFromVector(Container* pCon,Item* p)
{
	Container::iterator itr=std::find(pCon->begin(),pCon->end(),p);
	if(itr!=pCon->end())
	{
		pCon->erase(itr);
		return true;
	}
	return false;
}
IPrePassLightCon* HippoScene::GetLightCon(H3DI::IPrePassLight* p)
{
	H3DI::LightAffectParam e=p->GetLightAffectParam();
	H3DI::LIGHT_TYPE t=p->GetLightType();
	if(t==H3DI::LIGHT_DIR)
		return &m_DirCon[e];
	else if(t==H3DI::LIGHT_POINT)
		return &m_PointCon[e];
	else if(t==H3DI::LIGHT_PROJECT)
		return &m_SpotCon[e];

	return 0;
}




bool HippoScene::DelLight(H3DI::IPrePassLight* p)
{
	HippoFunLogObj("HippoScene::DelLight");
	GetH3DScene()->DetachModel(p);
	return DeleteItemFromVector(GetLightCon(p),p);
}




void HippoScene::Update(float escape)
{
	H3DI::IRender* pRender=Hippo_GetIRender();

	{
		IActorConItr itr=m_IActorCon.begin();
		while (itr!=m_IActorCon.end())
		{
			(*itr)->Update(escape);
			(*itr)->UpdateCpuSkin(escape);
			(*itr)->UpdateAdornment(escape);
			++itr;
		}
	}
	{
		IAvatarSkeletonModelConItr itr=m_IAvatarSkeletonModelCon.begin();
		while (itr!=m_IAvatarSkeletonModelCon.end())
		{
			(*itr)->UpdateCpuSkin(escape);
			(*itr)->Update(escape);
			(*itr)->UpdateAdornment(escape);
			++itr;
		}
	}
	
	{
		IModelConItr itr=m_IModelCon.begin();
		while (itr!=m_IModelCon.end())
		{
			(*itr)->Update(escape);
			++itr;
		}
	}
	{
		ISkeletonModelConItr itr=m_ISkeletonModelCon.begin();
		while (itr!=m_ISkeletonModelCon.end())
		{
			(*itr)->Update(escape);
			++itr;
		}
	}
	
	{
		ISpecialEffectConItr itr=m_ISpecialEffectCon.begin();
		while (itr!=m_ISpecialEffectCon.end())
		{
			(*itr)->update(escape);
			++itr;
		}
	}
	for(int i=0;i<H3DI::LIGHTAFFECTPARAM_END;++i)
	{
		IPrePassLightConItr itr=m_DirCon[i].begin();
		while (itr!=m_DirCon[i].end())
		{
			H3DI::IPrePassLight* plight=*itr;
			plight->Update(escape);
			++itr;
		}
	}

	for(int i=0;i<H3DI::LIGHTAFFECTPARAM_END;++i)
	{
		IPrePassLightConItr itr=m_PointCon[i].begin();
		while (itr!=m_PointCon[i].end())
		{
			H3DI::IPrePassLight* plight=*itr;
			plight->Update(escape);
			++itr;
		}
	}

	for(int i=0;i<H3DI::LIGHTAFFECTPARAM_END;++i)
	{
		IPrePassLightConItr itr=m_SpotCon[i].begin();
		while (itr!=m_SpotCon[i].end())
		{
			H3DI::IPrePassLight* plight=*itr;
			plight->Update(escape);
			++itr;
		}
	}



}
