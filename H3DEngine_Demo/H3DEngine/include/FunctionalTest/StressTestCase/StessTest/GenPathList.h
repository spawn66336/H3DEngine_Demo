#include "tinystr.h"
#include "tinyxml.h"
#include "../CommonCode/HippoUtil/FileOperation.h"
#include "../CommonCode/HippoUtil/HippoGlobal.h"
#include <vector>
using namespace std;
//!����ļ���Ҫ��������Դ�б�
//����һ�μ��ص�ʱ��û����Դ�б��ļ���ʱ�ᴴ��һ���б��ļ�
//��֮�������е�ʱ�򶼿���ֱ�Ӽ��ش����õ��б��ļ�
bool GenPathList( vector<std::string>& m_pDmlPathList,std::string ErgodicDir);