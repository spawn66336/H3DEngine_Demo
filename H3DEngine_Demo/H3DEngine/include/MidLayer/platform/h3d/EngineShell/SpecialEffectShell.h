#ifndef SPECIALEFFECTSHELL
#define SPECIALEFFECTSHELL
#include "../EffectCore_dx.h"
#include "instance_pool.h"

namespace H3DI
{
	//////////////////////////////////////////////////////////////////////////
	///主要函数：setActor(),unSetActor(),update(),render()
	class CSpecialEffect : public ISpecialEffect{
	public:

		virtual H3DI::IMoveObject* GetModelInTreeNode(const char* inTreeNodeName) { return 0;}

		//OBJECT
		virtual void Release()
		{
			EnginePool::m_effect_pool->releaseObj(this);
		};
		virtual bool					IsValid()const{return true;};

		virtual void                    SetUserPtr(void*){};
		virtual void*                  GetUserPtr(void)const {return 0;};
		//MOVE OBJECT
		virtual MODEL_TYPE				Type(){return OBJECT_SPECIALEFFECT;};
		virtual unsigned int			GetID(){static int moveObjNumber=0; return moveObjNumber++;};
		virtual unsigned int			PrepareDraw(){return 0;};
		virtual unsigned int			Draw(){return 0;};//排序之后，透明OBJECT必须绘制自己！
		virtual void*					GetAllInside(int n){return 0;};
		//运动控制
		virtual IAnimation*				GetAnimTrack(){return 0;};
		virtual void					SetAnimTrack(IAnimation* pAnim){};
		virtual float					GetCurAnimTime(){return 0;};
		virtual void					RunAnimation(bool bRun,float fStartTime){};
		virtual bool					IsAnimationRun(){return 0;};

		virtual void					Update(){};
		virtual void					Update(float fIntervalTime){};

		virtual void                    GetAABB(float*){};
		virtual void					SetAABB(const float* boxWorld){};
		virtual float					Picking(const float* Dir,const float* Point,float* vecIntPoint=0){return 0;};

		virtual void                    GetLocalAABB(float*){};
		virtual const H3DMat4& GetLocationMatrix()const{static H3DMat4 localMat4; localMat4.Identity();return localMat4;};
		virtual const H3DMat4& GetOrgMatrix()const{ static H3DMat4 orgMat4; orgMat4.Identity();return orgMat4;};
		virtual void					SetLocationMatrix(const H3DMat4& mat){};//设置物体状态矩阵后，Z轴方向旋转角度自动清0
		virtual void					SetOrgMatrix(const H3DMat4& mat){};//设置物体原始状态矩阵后
		virtual void					SetPosition(const H3DVec3&){};
		virtual void		            GetPosition(H3DVec3&)const{};

		virtual void					SetZRotation(float degree){};
		virtual float					GetZRotation()const {return 0;};
		virtual void					SetScale(const H3DVec3& scale){};
		virtual void	                GetScale(H3DVec3&)const{};
		virtual void					SetDirection(const H3DVec3& dir){};
		virtual void                    GetDirection(H3DVec3&)const{};

		virtual void					SetColor(const float* col, bool bUseColor){};
		virtual const float*			GetColor()const{static float ca[4]={0};return ca;};

		//层次关系控制，上升到IMoveObject
		virtual unsigned int			GetNumChild()const{return 0;};
		virtual IMoveObject*			GetChild(unsigned int n){return 0;};
		virtual void					AddChild(IMoveObject* pMoveObj){};
		virtual IMoveObject*			GetParent(){return 0;};
		virtual void					SetParent(IMoveObject* pMoveObj){};

		virtual void					EnableSort(){};
		virtual void					DisableSort(){};
		virtual bool					IsSortEnabled(){return 0;};
		virtual void					SetLOD(size_t nLOD){};
		virtual size_t					GetLOD()const{return 0;};
		virtual size_t					GetLODNum(){return 0;};
		virtual float					GetLodDist(size_t nLod)const{return 0;};

		virtual void					EnableChildrenUpdate(bool bEnable){};

		virtual IShader*				GetShader(){return 0;};
		virtual void					SetShader(IShader* pIShader){};
		virtual void					SetEffectShader(IShader* pShader){};
		///重新载入所有,返回1表示成功，0表示失败。
		virtual int						ReloadAll()	{return 1;};

		/*参数：
		0：等同于ReloadAll
		1：Reload Geometry
		2：Reload 材质（包括贴图）
		3：Reload 贴图

		返回值-1为失败，大于0的数表示reload了几张贴图
		*/
		//virtual int						ReloadFromDisk(int reloadContent){return 0;};
		///获得本物体使用的显存
		//virtual unsigned int GetVideoMemUse(){return 0;};

		///获得性能数据
		//virtual void* GetPerfData(MODEL_PERFDATA_TYPE t){return 0;};

		///从内存设置一个材质实例给模型的一个网格，返回1表示成功，0表示找不到该网格
		//virtual int SetMeshMaterialTemplateFromMemByName(const char* meshName,void* pXmlDoc){};

		//用AABB测试是否和某个射线相交。point和dir都是x、y、z三个分量
		//返回值为1表示相交
		virtual int RayHitTestWithAABB(const H3DVec3& point,const H3DVec3& dir){return 0;};

		//设置颜色系数
		//f是颜色系数的数组，arraysize是颜色系数数组长度
		//返回值=1表示成功，返回值小于0表示不成功，不成功的原因是材质中没有定义颜色系数
		virtual int SetMaterialColorFactor(float* f,int arraysize){return 1;};

		/// 设置固有色颜色系数,要求传入两个float[4]，影响方法： (Mat_d*C0.rgb)+C1.rgb，fC0的Alpha用于饱和度计算，fC1的Alpha用于亮度计算
		/// meshPart表示模型接受变色的网格的ID（对于一般模型可以是MeshID），
		///     如果是角色，表示服装的BodypartID或挂件ID(BODYPART_NUM+挂件ID)，当值为-1时，将变色此模型所有的部分。
		/// bUpdateChildren为true时，将变色此模型的子节点（例如挂件上的特效）
		virtual int SetDiffuseColorFactor(float* fC0,int arraysizeC0, float* fC1, int arraysizeC1, int meshPart=-1, bool bUpdateChildren=true){return 0;};

		//为模型设置渲染参数。参数是EEntityFlag的按位运算的结果
		virtual unsigned int			GetFlags(){return 0;};
		virtual void					SetFlags(unsigned int nFlags){};

		///设置材质lod,nLod{}即为最高细节，nLod=2为最简单细节
		virtual void SetMaterialLod(int nLod){};
		virtual int GetMaterialLod(){return 0;};

		//special effect
		/**
		*从文件读入一个特效
		*/
		virtual bool  load(const char* filename) {return true;};

		/// 获得当前特效的对应文件名称
		virtual const char*  getName() {return "";};
		/**
		*设定特效的发动者。可有多个发动者（联合技）
		*/
		virtual void  setActor(EffectActor eActor[], int actorCount, int type =0) {};

		virtual void  setObject(H3DI::ISkeletonModel* actor) {};

		///
		///清除特效的发动者，这样可以将该特效直接用于其他发动者
		virtual void  unsetActor() {};

		/// 
		/// 使该特效连接至另一特效
		virtual void  attachTo(ISpecialEffect* pEffect, bool isLink = false, int index = 1) {};

		/// 
		/// 取消当前特效的连接。如当前特效未连接至任何特效，执行空操作。
		virtual void  detach() {};

		/// 
		/// 得到特效的状态
		virtual int  getState() {return 0;};

		/// 
		/// 得到特效的当前时间
		virtual float  getCurrentTime() {return 0;};

		/// 
		///设置特效的状态
		virtual void  setState(int state) {};

		///
		/// 得到特效的相对矩阵(对于无actor的特效，该矩阵即为世界矩阵)
		//virtual const float*  getLocationMatrix() {};

		/// 
		///设置特效的相对矩阵(对于无actor的特效，该矩阵即为世界矩阵）
		//virtual void  setLocationMatrix(const float* mat) {};

		/// 
		/// 得到该特效的包围盒
		//virtual const float*  getAABB() {};

		/// 
		/// 设置整个特效的透明度
		virtual void  setTransparence(float trans) {};

		///
		/// 设置特效是否投射阴影
		virtual void setCastShadow(bool bCastShadow) {};

		///
		///交还该特效给特效管理器
		virtual void  release() {};

		///
		///判断特效是否有actor
		virtual bool  hasActor() {return 0;};

		///
		///得到特效的总长度，返回TIME_MAX表示循环特效
		virtual float  getAnimationLength() {return 0;};

		///
		///更新特效，默认更新时间为间隔时间
		virtual void  update(float interval, bool isinterval = true) {};

		///
		///渲染特效
		virtual void  render() {};
		virtual void  NewRender(int pipeType) {};
		/// 
		/// 重设特效状态
		virtual void  reset() {};

		/// 获得当前特效中各种从IMoveObject继承的子节点
		virtual void  getMoveObjectArray(H3DI::IMoveObject** pLightArray, 
			H3DI::IMoveObject** pSoundArray, int* pLightSize, int* pSoundSize) {};

		// 初始化当前特效
		virtual bool  Initialize() {return true;};

		// 设置3个轴的缩放系数
		virtual void _stdcall setScaleInfo(float x, float y, float z) {};



		//////////////////////////////////////////////////////////////////////////
		////////////////   以下接口主要供编辑器使用   ////////////////////////////
		////////////////   运行时请勿调用，否则会出错 ////////////////////////////

		///
		///保存特效到文件(主要供编辑器使用,运行时请勿调用，否则会出错)
		virtual bool  save(const char* filename) {return 0;};

		///
		///得到保存特效节点的容器(主要供编辑器使用,运行时请勿调用，否则会出错)
		virtual IForest*  getForest() {return 0;};

		/// 得到当前特效子节点的数目
		virtual int  getForestChildCount() {return 0;};

		///
		///得到特效元素管理器(主要供编辑器使用,运行时请勿调用，否则会出错)
		//hide this interface

		///
		///复制一个特效，不包括actor信息，这样可以将该特效复制并用于其他actor
		virtual ISpecialEffect*  clone(bool soundSample = true) {return 0;};

		///
		///删除特效并释放内存空间(主要供编辑器使用,运行时请勿调用，否则会出错)
		virtual void  destroy() {};

		/// for editor only, add a shader
		virtual bool  addShader(const char* shaderName) {return 0;};

		/// 
		/// for editor only
		//virtual IAnimationTrack*  createAnimationTrack() {};

		/// 创建一个灯光资源
		virtual ILightSource*  createLightSource() {return 0;};

		/** 从本特效中拷贝对应的节点到指定的特效
		@remark
		仅用于编辑器应用中
		@par
		pDstEffect: 指定特效的指针
		dstNodeAsParent: 指定特效中的某个子节点的名称
		srcNodeName: 当前特效中要拷贝的子节点的名称
		*/
		virtual const char*  copyNodeTo(ISpecialEffect* pDstEffect, const char* dstNodeAsParent, const char* srcNodeName) {return "";};

		/// 获取当前打开的所有shaderlib的数目
		virtual int  getShaderCount() {return 0;};
		/// 利用index获取shaderlib的文件名称
		virtual const char*  getShaderURL(int index) {return "";};

		/////////////////////add in 7.28 for IModel
		/// 获取当前特效中的模型指针
		virtual int  getIModelArray(H3DI::IModel** modelArray, int max_count) {return 0;};
		/// 设置特效中的粒子节点是否产生新的粒子
		virtual void  ParticleCreateNewOne(bool create) {};
		/// 动态改变模型的shader
		virtual void  changeWeaponShader(const char* shaderLibName) {};
		/// 强制设置产生的粒子的高度
		virtual void  ForceParticleHeight(float height) {};
		/// 获取特效中所有模型的名称（编辑器
		virtual int  getModelName(char nameArray[NameArrayLength][NameArrayWidth]) {return 0;};
		/// 导出为2进制文件
		virtual void  exportBinFile(const char* fileName) {};
		/// get the sound source array
		virtual int  getSoundSourceArray(H3DI::ISoundSource** soundArray, int maxCount) {return 0;};

		// for multi thread 查询特效资源是否loading完毕
		virtual bool  isLoadingEnd() {return true;};

		/// disable sound node
		virtual void  disableSoundSourceNode() {};

		/// 设置方向和位置
		virtual void  setDirectionPosition(const H3DVec3& dir, const H3DVec3& pos, const H3DVec3& scale) {};

		// 彻底删除
		virtual void  clean() {};

		virtual void  cancelLoading() {};

		virtual void  setShaderLoadingLevel(int scale, int compress) {};

		virtual void  resetSkillEffect() {};

		virtual void  setEffectShader() {};
		//reload shader
		virtual void reloadShader() {};

		// 设置特效中粒子的连接点的数组
		virtual void  setParticleLinkPoints(float* array, int count) {};

		// 设置特效中粒子的大小
		virtual void  setParticleSize(float length, float width) {};

		///获得本物体使用的显存
		unsigned int GetVideoMemUse()
		{
			return 0;
		}
		///获得性能数据
		void* GetPerfData(H3DI::MODEL_PERFDATA_TYPE t)
		{
			return 0;
		}

		///从内存设置一个材质实例给模型的一个网格，返回1表示成功，0表示找不到该网格
		int SetMeshMaterialTemplateFromMemByName(const char* meshName, void* pXmlDoc)
		{
			return 1;
		}

		/*参数：
		0：等同于ReloadAll
		1：Reload Geometry
		2：Reload 材质（包括贴图）
		3：Reload 贴图

		返回值-1为失败，大于0的数表示reload了几张贴图
		*/
		int ReloadFromDisk(int reloadContent)
		{
			return 0;
		}

		/// 设置此特效为角色相关特效
		virtual void	SetIsActorEffect(bool b) {};

		//!预生成fx
		virtual int PreGenFx(H3DI::IFxInfo* p){return 0;};

		//////////////////////////////////////////////////////////////////////////
		// 设置和获取特效播放时间比率
		// 在特效Update的时候处理将传入的时间t处理为t*ratio
		//! 目前由特效编辑器编辑时根据美术的保存类型设置
		//! 同时在特效编辑器编辑状态下此参数并不起作用，只在客户端状态下才起作用
		virtual void SetSpeLengthRatio(float fRatio) {};
		virtual float GetSpeLengthRatio() {return 0;};
		//////////////////////////////////////////////////////////////////////////

	};
class SpecialEffectManager : public ISpecialEffectManager{
public:

	/// 预先读入xml文件
	virtual void  getSpecialEffectXMLRead(const char* name) { };
	//////////////////////////////////////////////////////////////////////////
	/// 根据名称获得一个特效
	virtual ISpecialEffect*  getSpecialEffect(const char* name, bool binary = false) { return dynamic_cast<CSpecialEffect*>(EnginePool::m_effect_pool->createObj(name)); };
	virtual ISpecialEffect*  getSpecialEffect(H3DI::sCreateOp& op,const char* name, bool binary = false) { return dynamic_cast<CSpecialEffect*>(EnginePool::m_effect_pool->createObj(name)); };
	/// for multi thread loading
	virtual ISpecialEffect*  getSpecialEffectThread(const char* name, bool binary = false, int scale =0, int compress =0) {  return dynamic_cast<CSpecialEffect*>(EnginePool::m_effect_pool->createObj(name));};

	//////////////////////////////////////////////////////////////////////////
	/// 得到粒子等效果的管理器
	virtual IParticleSystemManager*  getParticleManager() { return 0;};

	//////////////////////////////////////////////////////////////////////////
	/// get ISoundManager
	virtual ISoundManager*  getSoundManager() {return 0; };

	//////////////////////////////////////////////////////////////////////////
	/// 删除所有特效（不常用）
	virtual void  deleteAllSpecialEffects() { };

	/// 设置地形
	virtual void         setTerrain(H3DI::ITerrain*) { };

	/// 设置声音资源管理器
	virtual void 		setSoundManager(ISoundManager* sound) { };
	/// 创建子节点对应的各种资源（编辑器调用）
	virtual IEModel*  createEModel(int type, int createinfo, const char* url, const char* extrainfo =0) { return 0; };
	/// 创建时间Track
	virtual IAnimationTrack*  createNewAnimationTrack() {return 0; };
	/// 获得相机控制的管理器
	virtual H3DI::I_CameraManager*  getCameraManager() { return 0;};

	virtual H3DI::ILight*  getLightFirst() {return 0; };

	virtual H3DI::ILight*  getLightNext() {return 0; };

	virtual void  beginRenderParticle() { };

	virtual void  endRenderParticle() { };

	/// 设置特效内的声音是否起作用  true代表起作用,false代表不起作用
	virtual void  setSoundEffectState(bool state) { };

	virtual bool  getSoundEffectState() { return false;};

	virtual void  beginOpenShaderLib(int scale =0, int compress =0) { };

	virtual bool  isOpenShaderLibOK() { return false;};

	virtual void  cancelLoadingShaderLib() { };
	//由于特效没有移植完毕,所以需要兼容老版本的保存和加载
	virtual void  setEditModel(bool b){};

	// 仅供编辑器使用的读取接口，编辑器不需要缓存
	virtual ISpecialEffect* getEffectForEditor(const char* name, bool binary = false) {return 0; };

	// 供编辑器获取特效使用的材质信息
	// 传入特效文件名和一个指向有count个元素的数组的指针
	// 返回实际的元素个数，从0开始
	// 空间由调用者分配
	virtual int Editor_GetMatFromSPEFile(const char* filename, Editor_SPEMatInfo* matInfo, unsigned int count) { return 0;};


	//virtual ~SpecialEffectManager(){}	
};
}
#endif