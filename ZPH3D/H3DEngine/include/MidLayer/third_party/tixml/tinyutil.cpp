#include "tinyutil.h"

const char* BiXmlUtility::get_value(const char* key, BiXmlElement *item)
{
	if(BiXmlElement* subitem = item->FirstChildElement(key))
	{
		return BiXmlUtility::get_value(subitem);
	}
	else
		return 0;
}

const char* BiXmlUtility::get_value(BiXmlElement *item)
{
	return item->GetText();
	/*BiXmlNode *childNode = item->FirstChild();
	if ( childNode == NULL )
		return 0;
	BiXmlText *childText = childNode->ToText();
	if ( childText == NULL )
		return 0;

	return childText->Value();*/
}

const char* BiXmlUtility::get_attribute(BiXmlNode* node, const char* key)
{
	BiXmlElement* ele = node->ToElement();
	if(ele)
	{
		return ele->Attribute(key);
	}
	else
		return 0;
}
//16进制字符串转换成整数
static unsigned char a16ToByte(const char* str)
{
	if (!str) return 0;

	//只要有一个字符为字符串结束符号时，返回0
	if (!str[0]) return 0;
	if (!str[1]) return 0;

	unsigned char result =  0;
	//获得高位的值
	if (str[0] >= '0' && str[0] <= '9')
	{
		result = ((str[0] - '0') << 4) & 0xF0;
	}
	else if (str[0] >= 'A' && str[0] <= 'F')
	{
		result = ((10 + str[0] - 'A') << 4) & 0xF0;
	}
	else if (str[0] >= 'a' && str[0] <= 'f')
	{
		result = ((10 + str[0] - 'a') << 4) & 0xF0;
	}
	else
	{
		return 0;
	}
	//获得低位的值
	if (str[1] >= '0' && str[1] <= '9')
	{
		result = result | ((str[1] - '0') & 0x0F);
	}
	else if (str[1] >= 'A' && str[1] <= 'F')
	{
		result = result | ((10 + str[1] - 'A') & 0x0F);
	}
	else if (str[1] >= 'a' && str[1] <= 'f')
	{
		result = result | ((10 + str[1] - 'a') & 0x0F);
	}
	else
	{
		return 0;
	}

	return result;
}
//内部不检测str的允许长度，只管写，长度由外部保证。
//str正常情况下至少3个字节，否则会出现越界读写
//每次函数调用会在str[0],str[1],内写2个字符，然后在str[2]写入'\0'
static bool byteToa16(unsigned char c, char* str)
{
	if (!str) return false;

	str[0] = '0';
	str[1] = '0';
	str[2] = 0;

	unsigned char ch = (c >> 4) & 0x0F;
	unsigned char cl = c & 0x0F;
	if (ch >= 0 && ch <= 9)
	{
		str[0] = '0' + ch;
	}
	else if (ch >= 10 && ch <= 15)
	{
		str[0] = 'A' + (ch - 10);
	}

	if (cl >= 0 && cl <= 9)
	{
		str[1] = '0' + cl;
	}
	else if (cl >= 10 && cl <= 15)
	{
		str[1] = 'A' + (cl - 10);
	}
	return true;
}
static const char* DataLenAttrName = "Len";
static const char* DataNodeName = "Data";
bool BiXmlUtility::insertBinaryData(BiXmlElement* parent, const char* keyname, int len, const unsigned char* data)
{
	if (!parent) return false;
	if (!keyname) return false;
	if (!data) return false;
	if (len <= 0) return false;

	//插入节点
	BiXmlElement* keynode = parent->InsertEndChild(keyname)->ToElement(); 
	//写入长度属性
	//BiXmlElement keynode(keyname);
	keynode->SetAttribute(DataLenAttrName, len);
	//分配中间内存
	char tempbuf[256 + 2] = {0};			
	char* tempp = tempbuf;
	int   tempacc = 0;
	//写入每行2进制数据内容
	for (int i = 0; i < len; i++)
	{							
		byteToa16(data[i], tempp);
		tempp += 2;
		tempacc += 2;
		if (tempacc >= 256)
		{				
			tempbuf[tempacc] = '\0';
			tempp = tempbuf;
			tempacc = 0;	

			/*BiXmlElement datanode(DataNodeName);

			BiXmlText textnode(tempbuf);
			datanode.InsertEndChild(textnode);

			keynode.InsertEndChild(datanode);*/
			BiXmlElement* datanode = keynode->InsertEndChild(DataNodeName)->ToElement();
			datanode->SetText(tempbuf);
		}
	}		
	//写入剩余2进制数据内容
	if (tempacc > 0)
	{
		tempbuf[tempacc] = '\0';
		tempp = tempbuf;
		tempacc = 0;

		/*BiXmlElement datanode(DataNodeName);

		BiXmlText textnode(tempbuf);
		datanode.InsertEndChild(textnode);

		keynode.InsertEndChild(datanode);*/
		BiXmlElement* datanode = keynode->InsertEndChild(DataNodeName)->ToElement();
		datanode->SetText(tempbuf);
	}
	//插入节点
	//parent->InsertEndChild(keynode);

	return true;
}
int BiXmlUtility::getBinaryDataLen(BiXmlNode* node)
{
	if (!node) return 0;
	BiXmlElement* ele = node->ToElement();
	if(!ele) return 0;

	int result = 0;
	ele->Attribute(DataLenAttrName, &result);
	return result;
}
bool BiXmlUtility::getBinaryData(BiXmlNode* node, int len, unsigned char* data)
{
	if (!node) return false;
	if (!data) return false;
	if (len <= 0) return false;
	//暂时不对传入len进行检测。
	//分行读取
	int tempacc = 0;
	BiXmlNode* datanode = node->FirstChild(DataNodeName);
	for (; datanode; datanode = datanode->NextSibling(DataNodeName))
	{
		BiXmlElement* ele = datanode->ToElement();
		if (!ele) continue;

		if(const char* v = get_value(ele))
		{
			const char* temp = v;
			//检测2进制数据的2位是否为字符串结束符号
			//检测累积记数是否超过允许长度
			while (temp[0] && temp[1] && tempacc < len)
			{
				data[tempacc] = a16ToByte(temp);
				tempacc++;
				temp += 2;
			}							
		}						
	}

	return true;
}
