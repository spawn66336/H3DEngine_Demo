#pragma	   once
/********************************************************************
	created:	2005/01/11
	created:	11:1:2005   19:13
	filename: 	d:\LastOrder_Final2\SDK\include\RenderCore\H3DDisplaySetup.h
	file path:	d:\LastOrder_Final2\SDK\include\RenderCore
	file base:	H3DDisplaySetup
	file ext:	h
	author:		姚勇 Yaoyong
	
	purpose:	显示效果调整
				
*********************************************************************/

namespace H3D
{
	enum //windows's ver
	{
		SERVER2003,
		WINXP,
		WIN2000,
		WINNT,
		WIN95,
		WIN95OSR2,
		WIN98,
		WIN98SE,
		WINME,
		VISTA,
		WINSHIT,
	};
	enum
	{
		HWCAP_PBUFFER = 1<<0,
		HWCAP_RENDER_TO_TEXTURE  =1<<1,
		HWCAP_RECTANGLE_TEXTURE  = 1<<2,
		HWCAP_PBUFFER_RECTANGLE_TEXTURE  = 1<<3,
		HWCAP_PS = 1<<4,
		HWCAP_VS = 1<<5,
	};

	struct tPerformanceTuning
	{
		tPerformanceTuning()
		{
			nReflect=nRefract=nGlow=nShadow=1;
			nReflectSize=512;
			nShadowSize=256;
			nTerrainTextureSize=1024;
			nTerrainTextureLev = 0;
			nTextureLevel=1;
			nTerrainGlobalTextureDist=4000;
			nHDR=0;
			nTerrainErrPixel = 40;
			nTerrainFarDist=768;
			nDynamicLight=1;
			nGeometryDist=100;
			nGeometryLod=30;
			nVideoRam=64000000;
			nReflectFreq=30;
			nTerrainLodBegin=50;
			nTexTriFilter = 0;
			nTexAnti = 0;
			nMultiSample = 0;
			nPhysicMem=512000000;
			nWinVer=WINXP;
			nHorizonCulling=0;
			nHorizonTest=40;
			nShowFPS=0;
			nGlowSize=nShadowSize;
			nHwPipe=0;
			nCurPipe=0;
			nHwCap2=nHwCap3=0;
			nHwCap =0;
			pReserve=pReserve1=pReserve2=0;
			nReflectRefractHeight=256;
			nTexMipmapBias=0;
			nReserve1=0;nReserve2=0;nReserve3=0;
		}
		int	nReflect,nRefract,nGlow,nShadow,nHDR,nReflectFreq;
		int	nReflectSize,nShadowSize,nGlowSize,nTerrainTextureSize,nTextureLevel;
		int	nTerrainErrPixel ,nTerrainTextureLev,nTerrainFarDist,nTerrainLodBegin,nTerrainGlobalTextureDist;
		int nDynamicLight,nGeometryDist,nGeometryLod;
		int nVideoRam;
		int nTexTriFilter,nTexAnti,nMultiSample;
		double nTexMipmapBias;
		int nHorizonCulling;
		int nHorizonTest;
		unsigned int nPhysicMem;
		int nWinVer,nShowFPS,nHwPipe,nCurPipe;
		unsigned int nHwCap,nHwCap2,nHwCap3;
		void* pReserve,*pReserve1,*pReserve2;
		unsigned int nReserve1,nReserve2,nReserve3;
		int nReflectRefractHeight;
	};
}