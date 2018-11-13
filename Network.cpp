#include "ThreadLock.h"
#include "Session.h"
#include "Network.h"

// ���캯��
CNetworkObject::CNetworkObject()
{
	m_pSession = NULL;
}

// ��������
CNetworkObject::~CNetworkObject()
{
}

// �ر�����
void CNetworkObject::Disconnect( BOOL bGracefulDisconnect )
{
	if( m_pSession ) {
		m_pSession->Disconnect( bGracefulDisconnect );
	}
}

// ��������
BOOL CNetworkObject::Send( BYTE *pMsg, WORD wSize )
{
	if(!m_pSession || m_pSession->HasDisconnectOrdered() || m_pSession->ShouldBeRemoved()) {
		return FALSE;
	}

	return m_pSession->Send( pMsg, wSize );
}

// ��������
BOOL CNetworkObject::SendEx( DWORD dwNumberOfMessages, BYTE **ppMsg, WORD *pwSize )
{
	if(!m_pSession || m_pSession->HasDisconnectOrdered() || m_pSession->ShouldBeRemoved()) {
		return FALSE;
	}

	return m_pSession->SendEx( dwNumberOfMessages, ppMsg, pwSize );
}

// �󶨲���
void CNetworkObject::Redirect( CNetworkObject *pNetworkObject )
{
	assert( pNetworkObject != NULL && "NULL Redirect"  );
	assert( m_pSession != NULL );

	m_pSession->BindNetworkObject( pNetworkObject );
}

// ��ȡ��ַ
std::string CNetworkObject::GetIP()
{
	if( m_pSession ) {
		return m_pSession->GetIP();
	}
	else {
		return "";
	}
}

// ��ȡ��ַ
SOCKADDR_IN* CNetworkObject::GetSockAddr()
{
	if( m_pSession ) {
		return m_pSession->GetSockAddr();
	}
	else {
		return NULL;
	}
}
