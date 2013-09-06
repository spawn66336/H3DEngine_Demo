#ifndef BINYXML_UTIL
#define BINYXML_UTIL

//#include "tinyxml.h"
#include "IXMLNode.h"

/*
#t tinyxml

star_cl使用tinyxml进行xml文件的读取，可以使用BiXmlUtility或BiXmlHelper和tiny_parse_xml_value来减少文件读取的代码量．
tinyxml使用的是多字节编码方式，如果需要变为游戏内部能够接受的字符串，需要进行编码形式的转换．

*/

// tiny xml读取帮助类
class BiXmlUtility
{
public:
	// 获得给定key的节点的值，如果没有返回0
	static const char* get_value(const char* key, BiXmlElement *item);
	// 获得节点的值，如果没有返回0
	static const char* get_value(BiXmlElement *item);
	// 获得节点的对应的key的attribute值，如果没有返回0
	static const char* get_attribute(BiXmlNode* node, const char* key);
	// 存储2进制数据内容，存入xml的并非真正的2进制数据，而是经过一定规则转换的数据
	// 当数据量小的时候使用这种方法。超大量的应考虑其他途径存储
	// 存储后的格式为
	// <ParentName>
	//	   <KeyName Len="len">
	//		   <Data>FFEEBBAA0099788734382934</Data>
	//		   <Data>123842389478923AABBCCDDa</Data>
	//		   <Data>FFEE23949023840BBAA90809</Data>
	//	   </KeyName>
	// </ParentName>
	// 二进制的长度会作为属性来保存
	// 二进制数据会拆成最多256个字符每行，多行保存
	// 内部不负责对传入2进制buffer长度的检测，只按照给定长度来存储
	// 参数：parent父节点，keyname想要存放2进制数据的节点名称，len 2进制buffer的长度，data 2进制buffer内容
	// 返回false表示没有插入节点
	static bool insertBinaryData(BiXmlElement* parent, const char* keyname, int len, const unsigned char* data);
	// 读取2进制数据内容，先找到长度，然后按照长度将转换后的数据存入传入的2进制buffer
	// 内部不负责对传入字符串长度的检测，只按照给定长度来存储
	// 参数：node存放2进制数据的节点指针，len返回的2进制数据的长度
	// 返回0表示该节点没有对应长度或者就不是2进制数据的节点
	static int getBinaryDataLen(BiXmlNode* node);
	// 参数：node存放2进制数据的节点指针，len data的buffer的长度，data内部向其中写入2进制的数据。
	// data指向的buffer由外部分配，长度由getBinaryDataLen获得，内部只负责写入内容
	// 返回false表示没有读出
	static bool getBinaryData(BiXmlNode* node, int len, unsigned char* data);
};


#endif // BINYXML_UTIL

