#ifndef _IRENDER_SCENE_H_
#define _IRENDER_SCENE_H_

#include "CartSceneDefine.h"


namespace H3DI
{
	class IRender;
}

class ICamera;
struct IGameLogic;

namespace H3D
{
	/**
	render scene is a container in which all entities need to be rendered are stored.
	 when the scene is released, all entities within are also released.
	 this implies that it is very dangerous to move an entity from one scene to the other( double delete when both the scenes containing the entity go out of scope)
	 another drawback of entity sharing is the additional programming cost to save/restore matrix.
	 Resource release must go through ReleaseEntity() method in that scene manager needs to be notify about the delete.
	*/
	enum
	{
		USE_INTERVAL_RENDER=1<<0,
	};

//#define SingleTexMask (1<<20)
//#define HwIndexBuffer (1<<21)
//#define CullingMask (1<<19)

	struct RenderSceneConfig 
	{
		RenderSceneConfig()
		{
			renderWorld = true;
			renderWorldTrans = true;
			renderObjects = true;
			renderObjectsCartoon = true;
			renderPlayers = true;
			renderKarts = true;
			renderEffects = true;
			renderIModels = true;
			nflags=0;
		}

		bool renderWorld;
		bool renderWorldTrans;
		bool renderObjects;
		bool renderObjectsCartoon;
		bool renderPlayers;
		bool renderKarts;
		bool renderEffects;
		bool renderIModels;
		unsigned int nflags;//add by puzzy, a int == 32 bool values.... use it
	};

	struct tAwardCamParams 
	{
		tAwardCamParams()
		{
			singleTargetOffset = -0.45f;
			singleCamVerticalOffset = 1.7f;
			singleCamHorizonalOffset = 12.3f;
			teamTargetOffset = -0.15f;
			teamCamVerticalOffset = 1.6f;
			teamCamHorizonalOffset = 12.3f;
		}
		float singleTargetOffset;
		float singleCamVerticalOffset;
		float singleCamHorizonalOffset;
		float teamTargetOffset;
		float teamCamVerticalOffset;
		float teamCamHorizonalOffset;
	};

	enum RenderSceneType
	{
		NORMAL,
		AWARD_SINGLE,
		AWARD_TEAM,
	};

	enum SoundType
	{
		SOUND_BACKGROUND = 0,
		SOUND_OTHER
	};

	class IRenderCart;
	class IRenderEntity;
	class IRenderEffect;
	class IRenderPlayer;
	class IRenderPet;
	class ISoundEffect;

	class IRenderScene
	{
	public:
		virtual void Init(void) = 0;
		virtual void ReleaseEntity(IRenderEntity*)  = 0;

		virtual ::ICamera* CreateCamera() = 0;
		virtual void  RemoveCamera(ICamera* p) = 0;
		virtual void SetCurrentCamera(ICamera* pcam) = 0;


		virtual void Render(bool renderShadow = true, RenderSceneType type = NORMAL) = 0;

		virtual void  RenderEntityPipe() = 0;
		virtual void  PushEntityToPipe(IRenderEntity* p, bool shadow = false, bool cartoon =true) = 0;
		virtual void  ClearEntityPipe() = 0;
 
		virtual IRenderCart*  CreateCart(const GFX_CREATE_CART_INFO& info, bool loadAllRes = false) = 0;
		virtual IRenderEffect*  CreateEffect(const GFX_CREATE_EFFECT_INFO& info) = 0;
		virtual IRenderPlayer*  CreateCartPlayer(const GFX_CREATE_PLAYER_INFO& info) = 0;
		
		virtual IRenderEntity*   CreateEntity(const GFX_CREATE_ENTITY_INFO& info) = 0;
		//virtual IRenderEntityPTR   CreateEntity(const GFX_CREATE_ENTITY_INFO& info) = 0;
		
		virtual bool   LoadScene(const GFX_CREATE_SCENE_INFO& info) = 0;

		virtual FindTriangleRequestBuffer&  GetFindTriangleRequestBuffer() = 0;

		virtual bool   LoadScene(const char* filename) = 0;
		virtual void  ClearScene() = 0;
		virtual IRenderCart*  CreateCart(const char* filename, bool loadAllRes = false) = 0;
		virtual IRenderPlayer*  CreateCartPlayer(const char* filename) = 0;
		virtual void SetFogParams(bool use, float begin, float end, int mode/*0:linear, 1:exp, 2:exp2*/, float r = 1.0f, float g = 1.0f, float b = 1.0f) = 0;

		virtual IRenderPet* CreatePet(const GFX_CREATE_PET_INFO& info) = 0;

		virtual IRenderPet* CreatePet(const char* filename) = 0;

		virtual void SetGameLogic( IGameLogic *gameLogic ) =  0;

		virtual void SetConfig(const RenderSceneConfig& config) = 0;

		virtual void SetBackGroundSoundVolume(float vol) = 0;

		virtual bool GetSingleAwardPosition(int id, float* fVec3) = 0; //id starts from 0

		virtual bool GetTeamAwardPosition(int id, float* fVec3) = 0; //id starts from 0

		virtual void SetAwardCamera(ICamera* pCam, bool single) = 0;

		virtual ISoundEffect* CreateSoundEffect(int ID, int type = SOUND_OTHER, bool is3D = false) = 0;
		virtual ISoundEffect* CreateSoundEffect(const char* filename/*.mp3, .wav, .ogg*/, int type = SOUND_OTHER, bool is3D = false) = 0;

		virtual void OnRaceBegin() = 0;
		virtual void OnRaceEnd() = 0;

		virtual void  SetSoundVolume(float volume,  int type=SOUND_OTHER, bool setDSound = true) = 0;

		virtual void ProfileEnd() = 0;

		virtual const RenderSceneConfig& GetConfig(void)const = 0;

		virtual void SetCullingCamera(ICamera* pcam) = 0;
		virtual ICamera* GetCullingCamera(void)const = 0;

		virtual void SetAwardParams(const tAwardCamParams& params) = 0;

		virtual const tAwardCamParams& GetAwardParams(void)const = 0;

		virtual void SetMute(bool mute) = 0;

	protected:
		virtual ~IRenderScene(){}
		virtual void Release() = 0;
	};
}
#endif