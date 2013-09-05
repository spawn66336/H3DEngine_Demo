#ifndef __H_EXTEND_ENTITY_CREATOR_H__
#define __H_EXTEND_ENTITY_CREATOR_H__

#include "media_end_entity_type.h"

class CMediaEndEntity;

class IEntityCreator
{
public:
	virtual CMediaEndEntity* create_entity(CGraphicsResMng* mng, const char* id) = 0;
	virtual void release_entity(CMediaEndEntity* entity) = 0;
	virtual int type()const = 0;
};

class CExtendEntityCreator: public IEntityCreator
{
public:
	virtual ~CExtendEntityCreator(){};
	virtual CMediaEndEntity* create_entity(CGraphicsResMng* mng, const char* id)
	{
		return NULL;
	}

	virtual void release_entity(CMediaEndEntity* entity)
	{
	}

	virtual int type()const{return 0;};
};



#endif