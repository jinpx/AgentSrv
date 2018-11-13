#include "ServerInfo.h"
#include "ServerSession.h"
#include "IniFile.h"

/////////////////////////////////////////////////////////////////////////////////
CServerInfo g_ServerInfo;

/////////////////////////////////////////////////////////////////////////////////
CServerInfo::CServerInfo(void)
{
	pIniFile = NULL;
	//Init(".\\ServerInfo.ini");
}

/////////////////////////////////////////////////////////////////////////////////
CServerInfo::~CServerInfo(void)
{
	SAFE_DELETE(pIniFile);
}

/////////////////////////////////////////////////////////////////////////////////
//初始化
BOOL CServerInfo::Init(char *strFileName)
{
	pIniFile = new IniFile(strFileName);
	if (!pIniFile) {
		return FALSE;
	}

	if (!pIniFile->m_isOpenFile){
		return FALSE;
	}

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////////
// 取得字符串
BOOL CServerInfo::GetString( char * strSection, char * szName, char * szDefault, char *pszValue)
{
	char szBuffer[256];
	const char *pChar = NULL;
	
	pChar = pIniFile->GetString(strSection, szName, szDefault);
	if(!pChar) {
		return FALSE;
	}

	memset(szBuffer, 0x0, sizeof(szBuffer));
	strncpy(szBuffer, pChar, min( 256, strlen(pChar) ) );
	
	int nLen = (int)strlen(szBuffer); 
	if ( nLen > 0 ) {
		strcpy(pszValue, szBuffer);
		return TRUE;
	}
	return FALSE;
}

INT CServerInfo::GetInt( char * strSection, char * szName, char * szDefault   )
{
	int retInt = 0;
	retInt = pIniFile->GetInteger(strSection, szName, -1);
	return retInt;
}

//获取服务器端口信息
SERVERINFO_EX CServerInfo::GetServerInfo(eSERVER_TYPE type)
{
	std::map<eSERVER_TYPE, std::string> mapSrvNameList;
	mapSrvNameList[LOGON_SERVER]		= "LOGON_SERVER";//登录服务器
	mapSrvNameList[GAME_SERVER]			= "GAME_SERVER";//游戏服务器
	mapSrvNameList[DB_SERVER]			= "DB_SERVER";//数据库服务

	SERVERINFO_EX sInfo;
	std::map<eSERVER_TYPE, std::string>::iterator itSrv = mapSrvNameList.find(type);
	if (itSrv == mapSrvNameList.end()) {
		assert(false);
		return sInfo;
	}
	
	char szName[32] = {0};
	strcpy(szName, itSrv->second.c_str());
	GetString(szName, "IP", "", sInfo.m_strIp);
	sInfo.m_dwPort = GetInt(szName, "PORT", "0");

	assert(0 != sInfo.m_dwPort);
	assert(0 != strlen(sInfo.m_strIp));
	return sInfo;