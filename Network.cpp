#include "CircuitLock.h"
#include "Session.h"
#include "Network.h"

// 构造函数
NetworkObject::NetworkObject()
{
	m_pSession = NULL;
}

// 析构函数
NetworkObject::~NetworkObject()
{
}

// 关闭连接
void NetworkObject::Disconnect( BOOL bGracefulDisconnect )
{
	if( m_pSession ) {
		m_pSession->Disconnect( bGracefulDisconnect );
	}
}

// 发送数据
BOOL NetworkObject::Send( BYTE *pMsg, WORD wSize )
{
	if(!m_pSession || m_pSession->HasDisconnectOrdered() || m_pSession->ShouldBeRemoved()) {
		return FALSE;
	}

	return m_pSession->Send( pMsg, wSize );
}

// 发送数据
BOOL NetworkObject::SendEx( DWORD dwNumberOfMessages, BYTE **ppMsg, WORD *pwSize )
{
	if(!m_pSession || m_pSession->HasDisconnectOrdered() || m_pSession->ShouldBeRemoved()) {
		return FALSE;
	}

	return m_pSession->SendEx( dwNumberOfMessages, ppMsg, pwSize );
}

// 绑定参数
void NetworkObject::Redirect( NetworkObject *pNetworkObject )
{
	assert( pNetworkObject != NULL && "NULL Redirect"  );
	assert( m_pSession != NULL );

	m_pSession->BindNetworkObject( pNetworkObject );
}

// 获取地址
std::string NetworkObject::GetIP()
{
	if( m_pSession ) {
		return m_pSession->GetIP();
	}
	else {
		return "";
	}
}

// 获取地址
SOCKADDR_IN* NetworkObject::GetSockAddr()
{
	if( m_pSession ) {
		return m_pSession->GetSockAddr();
	}
	else {
		return NULL;
	}
}
