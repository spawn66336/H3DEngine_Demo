#include <time.h>
#include "RandomUtil.h"
//��ȡ�������
int GetRandomInt(int min,int max)
{
	if (min>max)
		return min;
	return (rand()%(max-min+1)+min);
}
//��ȡ���������
float GetRandomFloat(float min,float max)
{
	if (max<=min)
		return min;
	return (((float)rand()/(float)RAND_MAX*(max-min)) +min);
}