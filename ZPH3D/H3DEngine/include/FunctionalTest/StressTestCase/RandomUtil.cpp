#include <time.h>
#include "RandomUtil.h"
//获取随机整数
int GetRandomInt(int min,int max)
{
	if (min>max)
		return min;
	return (rand()%(max-min+1)+min);
}
//获取随机浮点数
float GetRandomFloat(float min,float max)
{
	if (max<=min)
		return min;
	return (((float)rand()/(float)RAND_MAX*(max-min)) +min);
}