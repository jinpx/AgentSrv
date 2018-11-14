#include "ServerSession.h"
#include "LogonSession.h"
#include "PacketHandler.h"

/////////////////////////////////////////////////////////////////////////////////
// 构造函数
CLogonSession::CLogonSession(void)
{
}

/////////////////////////////////////////////////////////////////////////////////
// 析构函数
CLogonSession::~CLogonSession(void)
{
}

/////////////////////////////////////////////////////////////////////////////////
// 初始数据
VOID CLogonSession::Init()
{
}

/////////////////////////////////////////////////////////////////////////////////
// 接收数据
VOID CLogonSession::OnRecv( BYTE *pMsg, WORD wSize )
{
	g_PacketHander.ParsePacket_AL( this, (MSG_BASE *)pMsg, wSize );
}

/////////////////////////////////////////////////////////////////////////////////
// 断开连接
VOID CLogonSession::OnDisconnect()
{
	CServerSession::OnDisconnect();
}

/////////////////////////////////////////////////////////////////////////////////
// 断开连接
VOID CLogonSession::OnLogString( char *pszLog )
{ 
}
