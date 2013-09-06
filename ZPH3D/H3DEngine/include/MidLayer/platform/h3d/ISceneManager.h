#ifndef __ISCENEMANAGER_H__
#define __ISCENEMANAGER_H__

class ICamera;

namespace H3DI
{
	class IRender;

}
namespace H3D
{ 
	class IRenderScene;

	class  ISceneManager 
	{
	public:
		virtual ~ISceneManager(){}
		virtual void Init(void) = 0;
		virtual void Release(void) = 0;

		virtual IRenderScene* CreateScene(void) = 0;
		virtual void ReleaseScene(IRenderScene*) = 0;


		virtual void  SetRenderer(H3DI::IRender* pRenderer) = 0;
		virtual void  SetScreenInfo(unsigned int width, unsigned int height) = 0;

		virtual void SetMute(bool mute) = 0;
	};
}

H3D::ISceneManager*   CreateSceneManager();

float*  GetGraphicsCounter();

int  GetFramePolygon();

#endif