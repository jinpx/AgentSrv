#ifndef _ServerInfo_Headfile_
#define _ServerInfo_Headfile_

/////////////////////////////////////////////////////////////////////////////////
#include <Utility.h>
#include "IniFile.h"

/////////////////////////////////////////////////////////////////////////////////
// 服务器地址
struct SERVERINFO_EX
{
	char        m_strIp[IP_MAX_LEN]; //IP
	DWORD       m_dwPort;            //PORT

	SERVERINFO_EX()
	{
		memset(this, 0, sizeof(SERVERINFO_EX));
		strcpy(m_strIp, "");
		m_dwPort = 0;
	}
};

/////////////////////////////////////////////////////////////////////////////////
// 服务器信息
class CServerInfo
{
	// 参数变化
private:
	IniFile	*pIniFile;

	// 基础函数
public:
	// 构造函数
	CServerInfo(void);
	// 析构函数
	~CServerInfo(void);

	// 初始化
	BOOL Init(char *strFileName);
	// 获取数据
	BOOL GetString( char * strSection, char * szName, char * szDefault, char *pszValue);
	// 获取数据
	INT	 GetInt( char * strSection, char * szName, char * szDefault  );
	//获取服务器端口信息
	SERVERINFO_EX GetServerInfo(eSERVER_TYPE type);
};

/////////////////////////////////////////////////////////////////////////////////
#endif

/////////////////////////////////////////////////////////////////////////////////
// 全局变量
extern CServerInfo g_ServerInfo;
