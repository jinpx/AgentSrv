#include <XML.h>
#include "ServerSession.h"
#include <LogMsg.h>
#include "AgentSrv.h"
#include "ServerSessionFactory.h"
#include "ServerSessionManager.h"

/////////////////////////////////////////////////////////////////////////////////
// 构造函数
ServerSession::ServerSession()
{
}

/////////////////////////////////////////////////////////////////////////////////
// 析构函数
ServerSession::~ServerSession()
{
}

/////////////////////////////////////////////////////////////////////////////////
// 构造函数
VOID ServerSession::Clear()
{
	m_dwSessionIndex			= 0;
	m_wConnectPort				= 0;
	m_bForConnect				= FALSE;
	m_dwLastHeartbeatTick		= GetTickCount();
	m_bConnection				= FALSE;
	m_strConnectIP.clear();
}

/////////////////////////////////////////////////////////////////////////////////
// 初始数据
VOID ServerSession::Init()
{
	m_dwSessionIndex			= 0;
	m_wConnectPort				= 0;
	m_bForConnect				= FALSE;
	m_dwLastHeartbeatTick		= GetTickCount();
	m_bConnection				= FALSE;
	m_strConnectIP.clear();
}

/////////////////////////////////////////////////////////////////////////////////
// 释放数据
VOID ServerSession::Release()
{
	m_strConnectIP.clear();
}

/////////////////////////////////////////////////////////////////////////////////
// 更新数据
VOID ServerSession::Update()
{
	if( IsForConnect() )
	{
		// heartbeat
		DWORD dwCurTick = GetTickCount();
		if( dwCurTick - m_dwLastHeartbeatTick > 10000 )
		{
			m_dwLastHeartbeatTick = dwCurTick;

			MSG_HEARTBEAT msg;
			msg.m_byCategory		= 0;
			msg.m_byProtocol		= SERVERCOMMON_HEARTBEAT;
			Send( (BYTE*)&msg, sizeof(MSG_HEARTBEAT) );
		}
	}
}

/////////////////////////////////////////////////////////////////////////////////
// 设置连接
VOID ServerSession::OnConnect( BOOL bSuccess, DWORD dwNetworkIndex )
{
	if( bSuccess )
	{
		SetForConnect( TRUE );
		SetSessionIndex( dwNetworkIndex );
		ServerSessionManager::Instance()->AddServer( this );
		m_bConnection = TRUE;
	}
}

/////////////////////////////////////////////////////////////////////////////////
// 监听网络
VOID ServerSession::OnAccept( DWORD dwNetworkIndex )
{
	SetForConnect( FALSE );
	SetSessionIndex( dwNetworkIndex );
	ServerSessionManager::Instance()->AddServer( this );
	m_bConnection = TRUE;
}

/////////////////////////////////////////////////////////////////////////////////
// 断开连接
VOID ServerSession::OnDisconnect()
{
	ServerSessionManager::Instance()->RemoveServer( GetSessionIndex() );
	m_bConnection = FALSE;
}

/////////////////////////////////////////////////////////////////////////////////
// 发送类型
VOID ServerSession::SendServerType()
{
	MSG_SERVER_TYPE		msg;
	msg.m_byCategory	= 0;
	msg.m_byProtocol	= 0;
	msg.m_byServerType	= GetServerType();
	Send( (BYTE*)&msg, sizeof(MSG_SERVER_TYPE) );
}

/////////////////////////////////////////////////////////////////////////////////
// 设置地址
VOID ServerSession::SetAddr( char *pszIP, WORD wPort )
{
	m_strConnectIP = pszIP;
	m_wConnectPort = wPort;
}

/////////////////////////////////////////////////////////////////////////////////
// 尝试连接
VOID ServerSession::TryToConnect()
{
	if( m_strConnectIP.empty() ) {
		return;
	}

	// 发送数据
	// g_pAgentServer->ConnectToServer( this, (char*)m_strConnectIP.c_str(), m_wConnectPort );
}

VOID ServerSession::OnLogString( char *pszLog )
{
	AddLogMsg( LOG_OUT, pszLog );
}