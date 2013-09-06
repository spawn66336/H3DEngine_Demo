#ifndef __IRENDERENGINE_H__
#define __IRENDERENGINE_H__

//#include <string>

enum  OBJ_TYPE
{
	OBJ_CAMERA = 1,
	OBJ_LIGHT,
	OBJ_MODEL,
	OBJ_UNKNOWN,
};

enum  MOVE_DIRECTION
{
	MOVE_FRONT = 1,
	MOVE_BACK,
	MOVE_LEFT,
	MOVE_RIGHT,
	MOVE_UP,
	MOVE_DOWN,
};


class  IMovableObject
{
public:	
	virtual ~IMovableObject(){}
	virtual void setPosition(const float* pos) = 0;
	virtual const float* getPosition() const = 0;
	virtual void setLocationMatrix(const float* mat) = 0;
	virtual const float* getLocationMatrix() const = 0;
	virtual const float*  getFrontVec() const = 0;
	virtual const float* getUpVec() const = 0;
	virtual const float* getRightVec() const = 0;
	virtual void moveFront(float val) = 0;
	virtual void moveUp(float val) = 0;
	virtual void moveRight(float val) = 0;
	virtual void rotate(float* axis, float degree) = 0;
	virtual void rotateAroundFrontVec(float degree) = 0;
	virtual void rotateAroundUpVec(float degree) = 0;
	virtual void rotateAroundRightVec(float degree) = 0;
	virtual void getViewMatrix(float* buffer, bool rowMajor = true) = 0; // shader friendly
    virtual void getTransformMatrix(float* buffer, bool rowMajor = false) = 0;
	virtual void  applyViewProjection() = 0; 
	virtual void  getViewProjectionMatrix(float* buffer, bool rowMajor = true) = 0;
	virtual OBJ_TYPE getObjType() = 0;
	virtual void setFrontVec(float x, float y, float z) = 0;
	virtual void setUpVec(float x, float y, float z) = 0;
	virtual void setRightVec(float x, float y, float z) = 0;
	virtual void SetSpeed(float f) = 0;
	virtual float GetSpeed(void)const = 0;
	virtual void SetMoveDirection(MOVE_DIRECTION d ) = 0;
	virtual void Move(float time) = 0;
	virtual void Jump(float height) = 0;
};


class  ICamera : public virtual IMovableObject
{
public:
	virtual ~ICamera(){}
	virtual void setFov(float val = 90.0) = 0;
	virtual void setAspect(float val = 1.0) = 0;
	virtual void setZNear(float val = 0.1) = 0;
	virtual void setZFar(float val = 1000.0) = 0;
	virtual float getFov() = 0;
	virtual float getAspect() = 0;
	virtual float getNear() = 0;
	virtual float getFar() = 0;
    virtual const char* getName() = 0;
	virtual void GetScreenPos(const float* target3, float* result3) = 0;
};



#endif