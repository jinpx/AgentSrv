/////////////////////////////////////////////////////////////////////////////////
#include "IniFile.h"
#include <vector> 
#include <map> 
#include <algorithm> 
#include <string.h> 

/////////////////////////////////////////////////////////////////////////////////
typedef map<string, string, less<string> >		strMap; 
typedef strMap::iterator						strMapIt; 

using namespace std; 

// 解析数据
const char * const MIDDLESTRING = "____****____"; 

/////////////////////////////////////////////////////////////////////////////////
// INI分析结构体
struct IniAnalyze 
{
	// 数据参数
	string	strSect;
	strMap	*pMap; 

	// 分析构造
	IniAnalyze(strMap & strMap): pMap(&strMap) {}; 

	//重载() 
	void operator()(const string &sIni) 
	{
		// 段落参数
		unsigned int first = sIni.find('['); 
		unsigned int last = sIni.rfind(']'); 
		
		// 数据遍历
		if( first != string::npos && last != string::npos && first != last + 1) { 
			strSect = sIni.substr(first + 1, last - first - 1); 
			return; 
		} 

		// 段落为空
		if(strSect.empty()) { 
			return; 
		} 

		// 表达示
		if((first = sIni.find('=')) == string::npos) { 
			return; 
		} 

		// 参数数据
		string strtmp1 = sIni.substr(0, first); 
		string strtmp2 = sIni.substr(first + 1, string::npos); 
		first= strtmp1.find_first_not_of(" \t"); 
		last = strtmp1.find_last_not_of(" \t"); 

		// 字符指针
		if(first == string::npos || last == string::npos) { 
			return ; 
		} 

		// 获得KEY值
		string strkey = strtmp1.substr(first, last - first + 1); 
		first = strtmp2.find_first_not_of(" \t"); 

		// 过滤特殊字符
		if(((last = strtmp2.find("\t#", first)) != string::npos) || 
			((last = strtmp2.find(" #", first)) != string::npos) || 
			((last = strtmp2.find("\t;",first)) != string::npos) || 
			((last = strtmp2.find(" ;", first)) != string::npos) || 
			((last = strtmp2.find("\t//", first)) != string::npos)|| 
			((last = strtmp2.find(" //",  first)) != string::npos)||
			((last = strtmp2.find("\r",  first)) != string::npos) ||
			((last = strtmp2.find("\n",  first)) != string::npos)) { 
			strtmp2 = strtmp2.substr(0, last - first + 1); 
		} 

		// 是否结束
		last = strtmp2.find_last_not_of(" \t"); 
		if(first == string::npos || last == string::npos) { 
			return ; 
		} 

		// 存入所有值
		string value = strtmp2.substr(first, last - first + 1); 
		string mapkey = strSect + MIDDLESTRING + strkey; 
		(*pMap)[mapkey] = value; 
	} 
}; 

/////////////////////////////////////////////////////////////////////////////////
// INI加载

// 构造函数
CIniFile::CIniFile(const char* iniFile) 
{ 
	// 打开文件
	ini_file_name_ = iniFile;
	m_isOpenFile = OpenIni(iniFile); 
} 

// 析构函数
CIniFile::~CIniFile() 
{ 
} 

// 更新数据
void CIniFile::Update() { 
	WriteIni(ini_file_name_.c_str()); 
} 

/////////////////////////////////////////////////////////////////////////////////
// 字符串系例

// 获取字串
const char* CIniFile::GetString(const char* sect, const char* key, const char* defaultValue) 
{
	string mapkey = sect; 
	mapkey += MIDDLESTRING; 
	mapkey += key; 

	strMapIt it = ini_map_.find(mapkey); 
	if(it == ini_map_.end()) { 
		return defaultValue; 
	} 

	return it->second.c_str(); 
} 

// 设置字符
void CIniFile::SetString(const char* sect, const char* key, const char* value) 
{ 
	string mapkey = sect; 
	mapkey += MIDDLESTRING; 
	mapkey += key; 

	strMapIt it = ini_map_.find(mapkey); 
	if(it != ini_map_.end()) { 
		it->second = value; 
		return ; 
	} 

	ini_map_[mapkey] = value; 
} 

/////////////////////////////////////////////////////////////////////////////////
// 整型系例

// 获取整型
int CIniFile::GetInteger(const char* sect, const char* key, int iValue ) 
{ 
	const char *value = GetString(sect, key, NULL); 
	if (NULL == value) { 
		return iValue; 
	} 
	return atoi(value); 
} 

// 设置整型
void CIniFile::SetInteger(const char* sect, const char* key, int iValue) 
{ 
	char szValue[25] = {0}; 
	sprintf(szValue, "%d", iValue); 
	SetString(sect, key, szValue); 
} 

/////////////////////////////////////////////////////////////////////////////////
// 浮点型系例

// 获取浮点型
double CIniFile::GetDouble(const char* sect, const char* key, double dValue ) 
{ 
	const char *value = GetString(sect, key, NULL); 
	if (NULL == value) { 
		return dValue; 
	} 
	return atof(value); 
} 

// 设置浮点型
void CIniFile::SetDouble(const char* sect, const char* key, double dValue) 
{ 
	char szValue[25] = {0}; 
	sprintf(szValue, "%f", dValue); 
	SetString(sect, key, szValue); 
} 

/////////////////////////////////////////////////////////////////////////////////
// 布尔型系例

// 获取布尔型
bool CIniFile::GetBool(const char* sect, const char* key, bool bValue ) 
{ 
	const char *value = GetString(sect, key, NULL); 
	if (NULL == value) { 
		return bValue; 
	} 
	return strcmp(value, "1") == 0? true : false; 
} 

// 设置布尔型
void CIniFile::SetBool(const char* sect, const char* key, bool bValue) 
{ 
	char szValue[2] = {0}; 
	szValue[0] = '0'; 
	if (bValue) szValue[0] = '1'; 
	SetString(sect, key, szValue); 
} 

/////////////////////////////////////////////////////////////////////////////////
// 删除键值

// 删除键值
void CIniFile::DeleteKey(const char* sect, const char* key) 
{ 
	string mapkey = sect; 
	mapkey += MIDDLESTRING; 
	mapkey += key; 

	strMapIt it = ini_map_.find(mapkey); 
	if(it != ini_map_.end()) { 
		ini_map_.erase(it); 
	} 
} 

// 删除段值
void CIniFile::DeleteSession(const char* sect) 
{ 
	strMapIt it; 
	string sSess, strSect; 
	unsigned int uPos = 0; 
	for (it = ini_map_.begin(); it != ini_map_.end(); ++it) 
	{ 
		strSect = it->first; 
		uPos = strSect.find(MIDDLESTRING); 
		sSess = strSect.substr(0, uPos); 
		if (sSess == sect) { 
			ini_map_.erase(it); 
		} 
	} 
} 

/////////////////////////////////////////////////////////////////////////////////
// 打开文件
bool CIniFile::OpenIni(const char* iniFile) 
{ 
	FILE* fp = fopen(iniFile, "rb");

	if(fp == NULL) { 
		return false; 
	} 

	vector<string> strVect; 

	char strLine[512]; 
	while(!feof(fp)) { 
		char *s = fgets(strLine, 512, fp);		
		if ( s != NULL ) {
			strVect.push_back(strLine); 
		}
	}
	fclose(fp); 

	if(strVect.empty()) { 
		return false; 
	} 

	for_each(strVect.begin(), strVect.end(), IniAnalyze(ini_map_)); 
	return !ini_map_.empty(); 
} 

/////////////////////////////////////////////////////////////////////////////////
// 写入文件
bool CIniFile::WriteIni(const char* iniFile) 
{ 
	if (ini_map_.empty()) 
	{ 
		return false; 
	} 

	FILE* fp = fopen(iniFile, "wb");
	if (fp == NULL) 
	{ 
		return false; 
	} 

	strMapIt it; 
	int iMID_LEN = strlen(MIDDLESTRING); 
	string sSessSave = "", sSess, sKey, strSect; 
	unsigned int uPos = 0; 
	for (it = ini_map_.begin(); it != ini_map_.end(); ++it) 
	{ 
		strSect = it->first; 
		uPos = strSect.find(MIDDLESTRING); 
		sSess = strSect.substr(0, uPos); 
		if (sSessSave != sSess) 
		{ 
			sSessSave = sSess; 			
			fprintf(fp, "[%s]\n", sSess.c_str());
		} 

		sKey = strSect.substr(uPos + iMID_LEN, strlen(strSect.c_str()) - uPos - iMID_LEN); 		
		fprintf(fp, "%s = %s\n", sKey.c_str(), it->second.c_str());
	} 

	fclose(fp); 
	return true; 
}
