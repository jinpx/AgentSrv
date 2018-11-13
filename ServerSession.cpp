#include "Utility.h"
#include "ServerSession.h"

/////////////////////////////////////////////////////////////////////////////////
// ���캯��
CServerSession::CServerSession()
{
}

/////////////////////////////////////////////////////////////////////////////////
// ��������
CServerSession::~CServerSession()
{
}

/////////////////////////////////////////////////////////////////////////////////
// ���캯��
VOID CServerSession::Clear()
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
VOID CServerSession::Init()
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
VOID CServerSession::Release()
{
	m_strConnectIP.clear();
}

/////////////////////////////////////////////////////////////////////////////////
// ��������
VOID CServerSession::Update()
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
			msg.m_byProtocol		= 1;
			Send( (BYTE*)&msg, sizeof(MSG_HEARTBEAT) );
		}
	}
}

/////////////////////////////////////////////////////////////////////////////////
// ��������
VOID CServerSession::OnConnect( BOOL bSuccess, DWORD dwNetworkIndex )
{
	if( bSuccess )
	{
		SetForConnect( TRUE );
		SetSessionIndex( dwNetworkIndex );
		// ServerSessionManager::Instance()->AddServer( this );
		m_bConnection = TRUE;
	}
}

/////////////////////////////////////////////////////////////////////////////////
// ��������
VOID CServerSession::OnAccept( DWORD dwNetworkIndex )
{
	SetForConnect( FALSE );
	SetSessionIndex( dwNetworkIndex );
	// ServerSessionManager::Instance()->AddServer( this );
	m_bConnection = TRUE;
}

/////////////////////////////////////////////////////////////////////////////////
// �Ͽ�����
VOID CServerSession::OnDisconnect()
{
	// ServerSessionManager::Instance()->RemoveServer( GetSessionIndex() );
	m_bConnection = FALSE;
}

/////////////////////////////////////////////////////////////////////////////////
// ��������
VOID CServerSession::SendServerType()
{
	MSG_SERVER_TYPE		msg;
	msg.m_byCategory	= 0;
	msg.m_byProtocol	= 0;
	msg.m_byServerType	= GetServerType();
	Send( (BYTE*)&msg, sizeof(MSG_SERVER_TYPE) );
}

/////////////////////////////////////////////////////////////////////////////////
// ���õ�ַ
VOID CServerSession::SetAddr( char *pszIP, WORD wPort )
{
	m_strConnectIP = pszIP;
	m_wConnectPort = wPort;
}

/////////////////////////////////////////////////////////////////////////////////
// ��������
VOID CServerSession::TryToConnect()
{
	if( m_strConnectIP.empty() ) {
		return;
	}

	// ��������
	// g_pAgentServer->ConnectToServer( this, (char*)m_strConnectIP.c_str(), m_wConnectPort );
}

VOID CServerSession::OnLogString( char *pszLog )
{
	// AddLogMsg( LOG_OUT, pszLog );
}