/**
* @file level_entity.h
* 这个文件的主要功能是：声明引擎层的ILevel类的包装类CLevelEntity
* 
* @author 王恒(Wangheng@h3d.com.cn)
* 
* 更改日志
* ----------------------------------------------------
* ver 1.0.0
*       by 王恒2012.2.28
*/

#ifndef _LEVEL_ENTITY_H_
#define _LEVEL_ENTITY_H_

#include "game_graphic_header.h"

class CGraphicsResMng;
class CCameraEntity;
class C3DEntity;
enum LOD_LEVEL;

class CLevelEntity : public CSceneEntity
{
public:
	CLevelEntity(CGraphicsResMng* res_mng, H3DI::ILevel* level);
	virtual ~CLevelEntity();

	void	set_ILevel(H3DI::ILevel* ilevel);
	H3DI::ILevel*	get_ILevel(){return m_level;};

private:

	H3DI::ILevel*		m_level;

	FORBIDDEN_EVIL_CONSTRUCTOR(CLevelEntity);
};
#endif 
