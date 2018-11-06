#ifndef _IniFile_Headfile_
#define _IniFile_Headfile_

/////////////////////////////////////////////////////////////////////////////////
#include <map>
#include <string>

using namespace std;

/////////////////////////////////////////////////////////////////////////////////
// INI加载

class CIniFile
{
	// 基本函数
public:
	// 构造函数
	CIniFile(const char * pIniFile);
	// 析构函数
	~CIniFile();

	// 最新函数
public:
	// 更新数据
	void Update():
	// 获取字串
	const char * GetString(const char * sect, const char * key, const char * defaultValue = "");
	// 设置字符
	void SetString(const char * sect, const char * key, const char * value;

	// 获取整型
    int GetInteger(const char* sect, const char* key, int defaultValue = 0);
	// 设置整型
	void SetInteger(const char* sect, const char* key, int value);

	// 获取浮点型
    double GetDouble(const char* sect, const char* key, double defaultValue = 0);
    // 设置浮点型
	void SetDouble(const char* sect, const char* key, double value);
    
	// 获取布尔型
	bool GetBool(const char* sect, const char* key, bool defaultValue = false);
	// 设置布尔型
	void SetBool(const char* sect, const char* key, bool value);
    
	// 删除键值
	void DeleteKey(const char* sect, const char* key);
    // 删除段值
	void DeleteSession(const char* sect);

	// 文件已经打开
	bool m_isOpenFile;

	// 变量名称
private: 
	// 文件名称和路径
    string									ini_file_name_; 
    // 文件键值
	map<string, string, less<string> >		ini_map_; 

private:
	// 文件名称
	CIniFile(void) {
		m_isOpenFile = false; 
	};
	
	// 打开文件
    bool OpenIni(const char* iniFile) ;
    // 写入文件
	bool WriteIni(const char* iniFile) ;
}; 


/////////////////////////////////////////////////////////////////////////////////
#endif	// _IniFile_Headfile_
