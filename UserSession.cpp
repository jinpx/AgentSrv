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
	strcpy_s(m_pszID, "");
}

/////////////////////////////////////////////////////////////////////////////////
// 释放参数
VOID CUserSession::Release()
{
}

/////////////////////////////////////////////////////////////////////////////////
// 初始化
VOID CUserSession::UnGameServer()
{
	m_pGameServer = NULL;
}