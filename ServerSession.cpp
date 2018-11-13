#include <XML.h>
#include "ServerSession.h"
#include <LogMsg.h>
#include "AgentSrv.h"
#include "ServerSessionFactory.h"
#include "ServerSessionManager.h"

/////////////////////////////////////////////////////////////////////////////////
// ���캯��
ServerSession::ServerSession()
{
}

/////////////////////////////////////////////////////////////////////////////////
// ��������
ServerSession::~ServerSession()
{
}

/////////////////////////////////////////////////////////////////////////////////
// ���캯��
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
// ��ʼ����
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
// �ͷ�����
VOID ServerSession::Release()
{
	m_strConnectIP.clear();
}

/////////////////////////////////////////////////////////////////////////////////
// ��������
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
// ��������
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
// ��������
VOID ServerSession::OnAccept( DWORD dwNetworkIndex )
{
	SetForConnect( FALSE );
	SetSessionIndex( dwNetworkIndex );
	ServerSessionManager::Instance()->AddServer( this );
	m_bConnection = TRUE;
}

/////////////////////////////////////////////////////////////////////////////////
// �Ͽ�����
VOID ServerSession::OnDisconnect()
{
	ServerSessionManager::Instance()->RemoveServer( GetSessionIndex() );
	m_bConnection = FALSE;
}

/////////////////////////////////////////////////////////////////////////////////
// ��������
VOID ServerSession::SendServerType()
{
	MSG_SERVER_TYPE		msg;
	msg.m_byCategory	= 0;
	msg.m_byProtocol	= 0;
	msg.m_byServerType	= GetServerType();
	Send( (BYTE*)&msg, sizeof(MSG_SERVER_TYPE) );
}

/////////////////////////////////////////////////////////////////////////////////
// ���õ�ַ
VOID ServerSession::SetAddr( char *pszIP, WORD wPort )
{
	m_strConnectIP = pszIP;
	m_wConnectPort = wPort;
}

/////////////////////////////////////////////////////////////////////////////////
// ��������
VOID ServerSession::TryToConnect()
{
	if( m_strConnectIP.empty() ) {
		return;
	}

	// ��������
	// g_pAgentServer->ConnectToServer( this, (char*)m_strConnectIP.c_str(), m_wConnectPort );
}

VOID ServerSession::OnLogString( char *pszLog )
{
	AddLogMsg( LOG_OUT, pszLog );
}