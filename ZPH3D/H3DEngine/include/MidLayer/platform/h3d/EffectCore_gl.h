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
///��Ҫ������setActor(),unSetActor(),update(),render()
class ISpecialEffect : public H3DI::IMoveObject{
public:
	/**
	*���ļ�����һ����Ч
	*/
	virtual bool  load(const char* filename) = 0;

	/// ��õ�ǰ��Ч�Ķ�Ӧ�ļ�����
	virtual const char*  getName() = 0;
	/**
	*�趨��Ч�ķ����ߡ����ж�������ߣ����ϼ���
	*/
	virtual void  setActor(EffectActor eActor[], int actorCount, int type = 0) = 0;

	virtual void  setObject(H3DI::ISkeletonModel* actor) = 0;

	///
	///�����Ч�ķ����ߣ��������Խ�����Чֱ����������������
	virtual void  unsetActor() = 0;

	/// 
	/// ʹ����Ч��������һ��Ч
	virtual void  attachTo(ISpecialEffect* pEffect, bool isLink = false, int index = 1) = 0;

	/// 
	/// ȡ����ǰ��Ч�����ӡ��統ǰ��Чδ�������κ���Ч��ִ�пղ�����
	virtual void  detach() = 0;

	/// 
	/// �õ���Ч��״̬
	virtual int  getState() = 0;

	/// 
	/// �õ���Ч�ĵ�ǰʱ��
	virtual float  getCurrentTime() = 0;

	/// 
	///������Ч��״̬
	virtual void  setState(int state) = 0;

	///
	/// �õ���Ч����Ծ���(������actor����Ч���þ���Ϊ�������)
	//virtual const float*  getLocationMatrix() = 0;

	/// 
	///������Ч����Ծ���(������actor����Ч���þ���Ϊ�������
	//virtual void  setLocationMatrix(const float* mat) = 0;

	/// 
	/// �õ�����Ч�İ�Χ��
	//virtual const float*  getAABB() = 0;

	/// 
	/// ����������Ч��͸����
	virtual void  setTransparence(float trans) = 0;

	///
	///��������Ч����Ч������
	virtual void  release() = 0;

	///
	///�ж���Ч�Ƿ���actor
	virtual bool  hasActor() = 0;

	///
	///�õ���Ч���ܳ��ȣ�����TIME_MAX��ʾѭ����Ч
	virtual float  getAnimationLength() = 0;

	///
	///������Ч��Ĭ�ϸ���ʱ��Ϊ���ʱ��
	virtual void  update(float interval, bool isinterval = true) = 0;

	///
	///��Ⱦ��Ч
	virtual void  render() = 0;
	virtual void  NewRender(int pipeType) = 0;
	/// 
	/// ������Ч״̬
	virtual void  reset() = 0;

	/// ��õ�ǰ��Ч�и��ִ�IMoveObject�̳е��ӽڵ�
	virtual void  getMoveObjectArray(H3DI::IMoveObject** pLightArray, 
		H3DI::IMoveObject** pSoundArray, int* pLightSize, int* pSoundSize) = 0;

	// ��ʼ����ǰ��Ч
	virtual bool  Initialize() = 0;



	//////////////////////////////////////////////////////////////////////////
	////////////////   ���½ӿ���Ҫ���༭��ʹ��   ////////////////////////////
	////////////////   ����ʱ������ã��������� ////////////////////////////

	///
	///������Ч���ļ�(��Ҫ���༭��ʹ��,����ʱ������ã���������)
	virtual bool  save(const char* filename) = 0;

	///
	///�õ�������Ч�ڵ������(��Ҫ���༭��ʹ��,����ʱ������ã���������)
	virtual IForest*  getForest() = 0;

	/// �õ���ǰ��Ч�ӽڵ����Ŀ
	virtual int  getForestChildCount() = 0;

	///
	///�õ���ЧԪ�ع�����(��Ҫ���༭��ʹ��,����ʱ������ã���������)
	//hide this interface

	///
	///����һ����Ч��������actor��Ϣ���������Խ�����Ч���Ʋ���������actor
	virtual ISpecialEffect*  clone(bool soundSample = true) = 0;

	///
	///ɾ����Ч���ͷ��ڴ�ռ�(��Ҫ���༭��ʹ��,����ʱ������ã���������)
	virtual void  destroy() = 0;

	/// for editor only, add a shader
	virtual bool  addShader(const char* shaderName) = 0;

	/// 
	/// for editor only
	//virtual IAnimationTrack*  createAnimationTrack() = 0;

	/// ����һ���ƹ���Դ
	virtual ILightSource*  createLightSource() = 0;

	/** �ӱ���Ч�п�����Ӧ�Ľڵ㵽ָ������Ч
	@remark
	�����ڱ༭��Ӧ����
	@par
	pDstEffect: ָ����Ч��ָ��
	dstNodeAsParent: ָ����Ч�е�ĳ���ӽڵ������
	srcNodeName: ��ǰ��Ч��Ҫ�������ӽڵ������
	*/
	virtual const char*  copyNodeTo(ISpecialEffect* pDstEffect, const char* dstNodeAsParent, const char* srcNodeName) = 0;

	/// ��ȡ��ǰ�򿪵�����shaderlib����Ŀ
	virtual int  getShaderCount() = 0;
	/// ����index��ȡshaderlib���ļ�����
	virtual const char*  getShaderURL(int index) = 0;

	/////////////////////add in 7.28 for IModel
	/// ��ȡ��ǰ��Ч�е�ģ��ָ��
	virtual int  getIModelArray(H3DI::IModel** modelArray, int max_count) = 0;
	/// ������Ч�е����ӽڵ��Ƿ�����µ�����
	virtual void  ParticleCreateNewOne(bool create) = 0;
	/// ��̬�ı�ģ�͵�shader
	virtual void  changeWeaponShader(const char* shaderLibName) = 0;
	/// ǿ�����ò��������ӵĸ߶�
	virtual void  ForceParticleHeight(float height) = 0;
	/// ��ȡ��Ч������ģ�͵����ƣ��༭��
	virtual int  getModelName(char nameArray[NameArrayLength][NameArrayWidth]) = 0;
	/// ����Ϊ2�����ļ�
	virtual void  exportBinFile(const char* fileName) = 0;
	/// get the sound source array
	virtual int  getSoundSourceArray(H3DI::ISoundSource** soundArray, int maxCount) = 0;

	// for multi thread ��ѯ��Ч��Դ�Ƿ�loading���
	virtual bool  isLoadingEnd() = 0;

	/// disable sound node
	virtual void  disableSoundSourceNode() = 0;

	/// ���÷����λ��
	virtual void  setDirectionPosition(const H3DVec3& dir, const H3DVec3& pos, const H3DVec3& scale) = 0;

	// ����ɾ��
	virtual void  clean() = 0;

	virtual void  cancelLoading() = 0;

	virtual void  setShaderLoadingLevel(int scale, int compress) = 0;

	virtual void  resetSkillEffect() = 0;

	virtual void  setEffectShader() = 0;
	//reload shader
	virtual void reloadShader() = 0;

	// ������Ч�����ӵ����ӵ������
	virtual void  setParticleLinkPoints(float* array, int count) = 0;

	// ������Ч�����ӵĴ�С
	virtual void  setParticleSize(float length, float width) = 0;

	///��ñ�����ʹ�õ��Դ�
	unsigned int GetVideoMemUse()
	{
		return 0;
	}
};



//////////////////////////////////////////////////////////////////////////
///��Ҫ�ӿ�:getSpecialEffect()��������Ч����ͨ���ýӿڵõ�
class ISpecialEffectManager{
public:

	/// Ԥ�ȶ���xml�ļ�
	virtual void  getSpecialEffectXMLRead(const char* name) = 0;
	//////////////////////////////////////////////////////////////////////////
	/// �������ƻ��һ����Ч
	virtual ISpecialEffect*  getSpecialEffect(const char* name, bool binary = false) = 0;

	/// for multi thread loading
	virtual ISpecialEffect*  getSpecialEffectThread(const char* name, bool binary = false, int scale = 0, int compress = 0) = 0;

	//////////////////////////////////////////////////////////////////////////
	/// �õ����ӵ�Ч���Ĺ�����
	virtual IParticleSystemManager*  getParticleManager() = 0;

	//////////////////////////////////////////////////////////////////////////
	/// get ISoundManager
	virtual ISoundManager*  getSoundManager() = 0;

	//////////////////////////////////////////////////////////////////////////
	/// ɾ��������Ч�������ã�
	virtual void  deleteAllSpecialEffects() = 0;

	/// ���õ���
	virtual void         setTerrain(H3DI::ITerrain*) = 0;

	/// ����������Դ������
	virtual void 		setSoundManager(ISoundManager* sound) = 0;
	/// �����ӽڵ��Ӧ�ĸ�����Դ���༭�����ã�
	virtual IEModel*  createEModel(int type, int createinfo, const char* url, const char* extrainfo = 0) = 0;
	/// ����ʱ��Track
	virtual IAnimationTrack*  createNewAnimationTrack() = 0;
	/// ���������ƵĹ�����
	virtual H3DI::I_CameraManager*  getCameraManager() = 0;

	virtual H3DI::ILight*  getLightFirst() = 0;

	virtual H3DI::ILight*  getLightNext() = 0;

	virtual void  beginRenderParticle() = 0;

	virtual void  endRenderParticle() = 0;

	/// ������Ч�ڵ������Ƿ�������  true����������,false����������
	virtual void  setSoundEffectState(bool state) = 0;

	virtual bool  getSoundEffectState() = 0;

	virtual void  beginOpenShaderLib(int scale = 0, int compress = 0) = 0;

	virtual bool  isOpenShaderLibOK() = 0;

	virtual void  cancelLoadingShaderLib() = 0;
	//������Чû����ֲ���,������Ҫ�����ϰ汾�ı���ͼ���
	virtual void  setEditModel(bool b)=0;

protected:
	virtual ~ISpecialEffectManager(){}	
};
#endif
