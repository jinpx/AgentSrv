#include "UserSession.h"

/////////////////////////////////////////////////////////////////////////////////
// 构造函数
CUserSession::CUserSession()
{
}

/////////////////////////////////////////////////////////////////////////////////
// 析构函数
CUserSession::~CUserSession()
{
}

/////////////////////////////////////////////////////////////////////////////////
// 初始化
VOID CUserSession::Init()
{
	UnGameServer();
	m_dwUserId		= 0;
	m_dwUserKey		= 0;
	wcscpy( m_pszID, L"" );
}

/////////////////////////////////////////////////////////////////////////////////
// 释放参数
VOID CUserSession::Release()
{
}

/////////////////////////////////////////////////////////////////////////////////
// 连接数据
VOID CUserSession::OnDisconnect()
{

}

/////////////////////////////////////////////////////////////////////////////////
// 初始化
VOID CUserSession::UnGameServer()
{
	m_pGameServer = NULL;
}

/////////////////////////////////////////////////////////////////////////////////
// 打印日志
VOID CUserSession::OnLogString( char *pszLog )
{

}
