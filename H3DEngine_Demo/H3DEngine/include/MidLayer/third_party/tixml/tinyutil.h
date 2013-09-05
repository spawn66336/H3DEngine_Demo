#ifndef BINYXML_UTIL
#define BINYXML_UTIL

//#include "tinyxml.h"
#include "IXMLNode.h"

/*
#t tinyxml

star_clʹ��tinyxml����xml�ļ��Ķ�ȡ������ʹ��BiXmlUtility��BiXmlHelper��tiny_parse_xml_value�������ļ���ȡ�Ĵ�������
tinyxmlʹ�õ��Ƕ��ֽڱ��뷽ʽ�������Ҫ��Ϊ��Ϸ�ڲ��ܹ����ܵ��ַ�������Ҫ���б�����ʽ��ת����

*/

// tiny xml��ȡ������
class BiXmlUtility
{
public:
	// ��ø���key�Ľڵ��ֵ�����û�з���0
	static const char* get_value(const char* key, BiXmlElement *item);
	// ��ýڵ��ֵ�����û�з���0
	static const char* get_value(BiXmlElement *item);
	// ��ýڵ�Ķ�Ӧ��key��attributeֵ�����û�з���0
	static const char* get_attribute(BiXmlNode* node, const char* key);
	// �洢2�����������ݣ�����xml�Ĳ���������2�������ݣ����Ǿ���һ������ת��������
	// ��������С��ʱ��ʹ�����ַ�������������Ӧ��������;���洢
	// �洢��ĸ�ʽΪ
	// <ParentName>
	//	   <KeyName Len="len">
	//		   <Data>FFEEBBAA0099788734382934</Data>
	//		   <Data>123842389478923AABBCCDDa</Data>
	//		   <Data>FFEE23949023840BBAA90809</Data>
	//	   </KeyName>
	// </ParentName>
	// �����Ƶĳ��Ȼ���Ϊ����������
	// ���������ݻ������256���ַ�ÿ�У����б���
	// �ڲ�������Դ���2����buffer���ȵļ�⣬ֻ���ո����������洢
	// ������parent���ڵ㣬keyname��Ҫ���2�������ݵĽڵ����ƣ�len 2����buffer�ĳ��ȣ�data 2����buffer����
	// ����false��ʾû�в���ڵ�
	static bool insertBinaryData(BiXmlElement* parent, const char* keyname, int len, const unsigned char* data);
	// ��ȡ2�����������ݣ����ҵ����ȣ�Ȼ���ճ��Ƚ�ת��������ݴ��봫���2����buffer
	// �ڲ�������Դ����ַ������ȵļ�⣬ֻ���ո����������洢
	// ������node���2�������ݵĽڵ�ָ�룬len���ص�2�������ݵĳ���
	// ����0��ʾ�ýڵ�û�ж�Ӧ���Ȼ��߾Ͳ���2�������ݵĽڵ�
	static int getBinaryDataLen(BiXmlNode* node);
	// ������node���2�������ݵĽڵ�ָ�룬len data��buffer�ĳ��ȣ�data�ڲ�������д��2���Ƶ����ݡ�
	// dataָ���buffer���ⲿ���䣬������getBinaryDataLen��ã��ڲ�ֻ����д������
	// ����false��ʾû�ж���
	static bool getBinaryData(BiXmlNode* node, int len, unsigned char* data);
};


#endif // BINYXML_UTIL

