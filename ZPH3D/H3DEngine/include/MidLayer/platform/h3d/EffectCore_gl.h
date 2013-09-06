#ifndef __EFFECTCORE_H__
#define __EFFECTCORE_H__
//#include "IPrerequisites.h"

#include "RenderCore.h"

namespace H3DI {
	class ISoundSource;
	class I_CameraManager;
}
const float TIME_MAX = 999998.f;

enum EFFECT_STATE{
	EFFECT_NEEDACTOR = (1<<1),
	EFFECT_HASACTOR = (1<<2),
	EFFECT_USEMATRIX = (1<<4),
	EFFECT_EDITING = (1<<5),
	EFFECT_CONCAT_PARENT = (1<<6),
	EFFECT_CONCAT_CHILD = (1<<7),
	EFFECT_ACTIVATED = (1<<8),
};

enum NODE_STATE{
	NODE_UNACTIVATED = 1,
	NODE_PLAYING,
	NODE_FINISHED,
};

enum ACTOR_TYPE
{
	EFFECT_ACTOR,
	CHAIN_ACTOR,
};

#define NameArrayWidth 1024
#define NameArrayLength 128


class IParticleSystemManager;
class IParticleEffect;
class ISoundManager;
class ILightSource;
class IAnimationTrack;
class IEModel;
class IForest;



struct ObjManipulator
{
	virtual void SetPosition(float p[]) = 0;//x,y,z
	virtual void SetDirection(float d[]) = 0;//x,y,z
	virtual void GetPosition(float p[]) = 0;//x,y,z
	virtual void GetDirection(float d[]) = 0;//x,y,z
};

struct EffectActor
{
	H3DI::IModel* actor;
	ObjManipulator* obj;
};

//////////////////////////////////////////////////////////////////////////
///主要函数：setActor(),unSetActor(),update(),render()
class ISpecialEffect : public H3DI::IMoveObject{
public:
	/**
	*从文件读入一个特效
	*/
	virtual bool  load(const char* filename) = 0;

	/// 获得当前特效的对应文件名称
	virtual const char*  getName() = 0;
	/**
	*设定特效的发动者。可有多个发动者（联合技）
	*/
	virtual void  setActor(EffectActor eActor[], int actorCount, int type = 0) = 0;

	virtual void  setObject(H3DI::ISkeletonModel* actor) = 0;

	///
	///清除特效的发动者，这样可以将该特效直接用于其他发动者
	virtual void  unsetActor() = 0;

	/// 
	/// 使该特效连接至另一特效
	virtual void  attachTo(ISpecialEffect* pEffect, bool isLink = false, int index = 1) = 0;

	/// 
	/// 取消当前特效的连接。如当前特效未连接至任何特效，执行空操作。
	virtual void  detach() = 0;

	/// 
	/// 得到特效的状态
	virtual int  getState() = 0;

	/// 
	/// 得到特效的当前时间
	virtual float  getCurrentTime() = 0;

	/// 
	///设置特效的状态
	virtual void  setState(int state) = 0;

	///
	/// 得到特效的相对矩阵(对于无actor的特效，该矩阵即为世界矩阵)
	//virtual const float*  getLocationMatrix() = 0;

	/// 
	///设置特效的相对矩阵(对于无actor的特效，该矩阵即为世界矩阵）
	//virtual void  setLocationMatrix(const float* mat) = 0;

	/// 
	/// 得到该特效的包围盒
	//virtual const float*  getAABB() = 0;

	/// 
	/// 设置整个特效的透明度
	virtual void  setTransparence(float trans) = 0;

	///
	///交还该特效给特效管理器
	virtual void  release() = 0;

	///
	///判断特效是否有actor
	virtual bool  hasActor() = 0;

	///
	///得到特效的总长度，返回TIME_MAX表示循环特效
	virtual float  getAnimationLength() = 0;

	///
	///更新特效，默认更新时间为间隔时间
	virtual void  update(float interval, bool isinterval = true) = 0;

	///
	///渲染特效
	virtual void  render() = 0;
	virtual void  NewRender(int pipeType) = 0;
	/// 
	/// 重设特效状态
	virtual void  reset() = 0;

	/// 获得当前特效中各种从IMoveObject继承的子节点
	virtual void  getMoveObjectArray(H3DI::IMoveObject** pLightArray, 
		H3DI::IMoveObject** pSoundArray, int* pLightSize, int* pSoundSize) = 0;

	// 初始化当前特效
	virtual bool  Initialize() = 0;



	//////////////////////////////////////////////////////////////////////////
	////////////////   以下接口主要供编辑器使用   ////////////////////////////
	////////////////   运行时请勿调用，否则会出错 ////////////////////////////

	///
	///保存特效到文件(主要供编辑器使用,运行时请勿调用，否则会出错)
	virtual bool  save(const char* filename) = 0;

	///
	///得到保存特效节点的容器(主要供编辑器使用,运行时请勿调用，否则会出错)
	virtual IForest*  getForest() = 0;

	/// 得到当前特效子节点的数目
	virtual int  getForestChildCount() = 0;

	///
	///得到特效元素管理器(主要供编辑器使用,运行时请勿调用，否则会出错)
	//hide this interface

	///
	///复制一个特效，不包括actor信息，这样可以将该特效复制并用于其他actor
	virtual ISpecialEffect*  clone(bool soundSample = true) = 0;

	///
	///删除特效并释放内存空间(主要供编辑器使用,运行时请勿调用，否则会出错)
	virtual void  destroy() = 0;

	/// for editor only, add a shader
	virtual bool  addShader(const char* shaderName) = 0;

	/// 
	/// for editor only
	//virtual IAnimationTrack*  createAnimationTrack() = 0;

	/// 创建一个灯光资源
	virtual ILightSource*  createLightSource() = 0;

	/** 从本特效中拷贝对应的节点到指定的特效
	@remark
	仅用于编辑器应用中
	@par
	pDstEffect: 指定特效的指针
	dstNodeAsParent: 指定特效中的某个子节点的名称
	srcNodeName: 当前特效中要拷贝的子节点的名称
	*/
	virtual const char*  copyNodeTo(ISpecialEffect* pDstEffect, const char* dstNodeAsParent, const char* srcNodeName) = 0;

	/// 获取当前打开的所有shaderlib的数目
	virtual int  getShaderCount() = 0;
	/// 利用index获取shaderlib的文件名称
	virtual const char*  getShaderURL(int index) = 0;

	/////////////////////add in 7.28 for IModel
	/// 获取当前特效中的模型指针
	virtual int  getIModelArray(H3DI::IModel** modelArray, int max_count) = 0;
	/// 设置特效中的粒子节点是否产生新的粒子
	virtual void  ParticleCreateNewOne(bool create) = 0;
	/// 动态改变模型的shader
	virtual void  changeWeaponShader(const char* shaderLibName) = 0;
	/// 强制设置产生的粒子的高度
	virtual void  ForceParticleHeight(float height) = 0;
	/// 获取特效中所有模型的名称（编辑器
	virtual int  getModelName(char nameArray[NameArrayLength][NameArrayWidth]) = 0;
	/// 导出为2进制文件
	virtual void  exportBinFile(const char* fileName) = 0;
	/// get the sound source array
	virtual int  getSoundSourceArray(H3DI::ISoundSource** soundArray, int maxCount) = 0;

	// for multi thread 查询特效资源是否loading完毕
	virtual bool  isLoadingEnd() = 0;

	/// disable sound node
	virtual void  disableSoundSourceNode() = 0;

	/// 设置方向和位置
	virtual void  setDirectionPosition(const H3DVec3& dir, const H3DVec3& pos, const H3DVec3& scale) = 0;

	// 彻底删除
	virtual void  clean() = 0;

	virtual void  cancelLoading() = 0;

	virtual void  setShaderLoadingLevel(int scale, int compress) = 0;

	virtual void  resetSkillEffect() = 0;

	virtual void  setEffectShader() = 0;
	//reload shader
	virtual void reloadShader() = 0;

	// 设置特效中粒子的连接点的数组
	virtual void  setParticleLinkPoints(float* array, int count) = 0;

	// 设置特效中粒子的大小
	virtual void  setParticleSize(float length, float width) = 0;

	///获得本物体使用的显存
	unsigned int GetVideoMemUse()
	{
		return 0;
	}
};



//////////////////////////////////////////////////////////////////////////
///主要接口:getSpecialEffect()，所有特效都将通过该接口得到
class ISpecialEffectManager{
public:

	/// 预先读入xml文件
	virtual void  getSpecialEffectXMLRead(const char* name) = 0;
	//////////////////////////////////////////////////////////////////////////
	/// 根据名称获得一个特效
	virtual ISpecialEffect*  getSpecialEffect(const char* name, bool binary = false) = 0;

	/// for multi thread loading
	virtual ISpecialEffect*  getSpecialEffectThread(const char* name, bool binary = false, int scale = 0, int compress = 0) = 0;

	//////////////////////////////////////////////////////////////////////////
	/// 得到粒子等效果的管理器
	virtual IParticleSystemManager*  getParticleManager() = 0;

	//////////////////////////////////////////////////////////////////////////
	/// get ISoundManager
	virtual ISoundManager*  getSoundManager() = 0;

	//////////////////////////////////////////////////////////////////////////
	/// 删除所有特效（不常用）
	virtual void  deleteAllSpecialEffects() = 0;

	/// 设置地形
	virtual void         setTerrain(H3DI::ITerrain*) = 0;

	/// 设置声音资源管理器
	virtual void 		setSoundManager(ISoundManager* sound) = 0;
	/// 创建子节点对应的各种资源（编辑器调用）
	virtual IEModel*  createEModel(int type, int createinfo, const char* url, const char* extrainfo = 0) = 0;
	/// 创建时间Track
	virtual IAnimationTrack*  createNewAnimationTrack() = 0;
	/// 获得相机控制的管理器
	virtual H3DI::I_CameraManager*  getCameraManager() = 0;

	virtual H3DI::ILight*  getLightFirst() = 0;

	virtual H3DI::ILight*  getLightNext() = 0;

	virtual void  beginRenderParticle() = 0;

	virtual void  endRenderParticle() = 0;

	/// 设置特效内的声音是否起作用  true代表起作用,false代表不起作用
	virtual void  setSoundEffectState(bool state) = 0;

	virtual bool  getSoundEffectState() = 0;

	virtual void  beginOpenShaderLib(int scale = 0, int compress = 0) = 0;

	virtual bool  isOpenShaderLibOK() = 0;

	virtual void  cancelLoadingShaderLib() = 0;
	//由于特效没有移植完毕,所以需要兼容老版本的保存和加载
	virtual void  setEditModel(bool b)=0;

protected:
	virtual ~ISpecialEffectManager(){}	
};
#endif
