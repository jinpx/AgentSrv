#include "ThreadLock.h"
#include "Network.h"
#include "SocketOpt.h"
#include "RecvBuffer.h"
#include "SendBuffer.h"
#include "Session.h"
#include "ServerHandler.h"

/////////////////////////////////////////////////////////////////////////////////
// 构造函数
CSession::CSession( DWORD dwSendBufferSize, DWORD dwRecvBufferSize, DWORD dwMaxPacketSize, DWORD dwTimeOut )
{
	m_pSendBuffer = new CSendBuffer;
	m_pSendBuffer->Create( dwSendBufferSize, dwMaxPacketSize );

	m_pRecvBuffer = new CRecvBuffer;
	m_pRecvBuffer->Create( dwRecvBufferSize, dwMaxPacketSize );

	m_dwTimeOut			= dwTimeOut;
	m_socket			= INVALID_SOCKET;
	m_bAcceptSocket		= FALSE;
	m_bCanSend			= true;
}

/////////////////////////////////////////////////////////////////////////////////
// 析构函数
CSession::~CSession()
{
	CloseSocket();

	// 删除发送数据
	if( m_pSendBuffer ) {
		delete m_pSendBuffer;
	}

	// 删除接收数据
	if( m_pRecvBuffer ) {
		delete m_pRecvBuffer;
	}
}

/////////////////////////////////////////////////////////////////////////////////
// 发送数据
void CSession::Init()
{
	m_pSendBuffer->Clear();
	m_pRecvBuffer->Clear();

	ResetKillFlag();

	m_bDisconnectOrdered = FALSE;
	m_bCanSend			 = TRUE;
	
	m_dwTotalRecvBytes	= 0;
	m_dwTotalSendBytes	= 0;
}


/////////////////////////////////////////////////////////////////////////////////
// 发送数据
BOOL CSession::Send( BYTE *pMsg, WORD wSize )
{
	PACKET_HEADER header;
	header.size = wSize;
	if( m_pSendBuffer->Write( &header, pMsg ) == FALSE )
	{
		OnLogString( "m_pSendBuffer->Write fail. data length = %d, %d", m_pSendBuffer->GetLength(), wSize );
		Remove();
		return FALSE; 
	}

	m_dwTotalSendBytes	+= wSize;
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////////
// 发送数据
BOOL CSession::SendEx( DWORD dwNumberOfMessages, BYTE **ppMsg, WORD *pwSize )
{
	PACKET_HEADER header;
	header.size = 0;
	DWORD i;
	
	for( i = 0; i < dwNumberOfMessages; ++i ) {
		header.size += pwSize[i];
	}
	
	if( !m_pSendBuffer->Write( (BYTE*)&header, sizeof(PACKET_HEADER) ) ) {
		OnLogString( "m_pSendBuffer->Write fail. data length = %d, %d", m_pSendBuffer->GetLength(), sizeof(PACKET_HEADER) );
		Remove();
		return FALSE;
	}

	// (dwNumberOfMessages)
	for( i=0; i<dwNumberOfMessages; ++i )
	{
		if( !m_pSendBuffer->Write( ppMsg[i], pwSize[i] ) ) {
			OnLogString( "m_pSendBuffer->Write fail. data length = %d, %d", m_pSendBuffer->GetLength(), pwSize[i] );
			Remove();
			return FALSE;
		}
	}

	m_dwTotalSendBytes	+= header.size;
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////////
// 接收处理
BOOL CSession::ProcessRecvdPacket( DWORD dwMaxPacketSize )
{
	BYTE			* pPacket;
	PACKET_HEADER	* pHeader;

	// 接收数据包
	while( pPacket = GetRecvBuffer()->GetFirstPacketPtr() )
	{
		pHeader = (PACKET_HEADER*)pPacket;
		if( pHeader->size <= 0 ) {
			OnLogString( "Packet error! size(%d) category(%d) protocol(%d)\n", pHeader->size, *( pPacket + sizeof(PACKET_HEADER) ), *( pPacket + sizeof(PACKET_HEADER) + 1 ) );
			return FALSE;
		}

		if( pHeader->size + sizeof(PACKET_HEADER) > dwMaxPacketSize ) {
			OnLogString( "Packet error : Cur[%d]/Max[%d] Category[%d] Protocol[%d]", pHeader->size, dwMaxPacketSize, *( pPacket + sizeof(PACKET_HEADER) ), *( pPacket + sizeof(PACKET_HEADER) + 1 ) );

			OnLogString( "Packet error : Category[%d] Protocol[%d] Size[%d]", m_iCategory, m_iProtocol, m_iSize );
			return FALSE;
		} 

		// 接收数据
		m_pNetworkObject->OnRecv( pPacket+sizeof(PACKET_HEADER), pHeader->size );
		GetRecvBuffer()->RemoveFirstPacket( sizeof(PACKET_HEADER) + pHeader->size );

		// 超时处理
		ResetTimeOut();

		// 协议加载
		m_iCategory = *( pPacket + sizeof(PACKET_HEADER) );
		m_iProtocol = *( pPacket + sizeof(PACKET_HEADER) + 1 );
		m_iSize = pHeader->size + sizeof(PACKET_HEADER);

		// 包大小
		m_dwTotalRecvBytes	+= pHeader->size + sizeof(PACKET_HEADER);
	}
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////////
// 发送判断
BOOL CSession::OnSend()
{
	OnLogString("[CSession::OnSend]");
	
	m_lockSend.Lock();
	m_bCanSend = TRUE;
	m_lockSend.Unlock();
	return m_bCanSend;
}

/////////////////////////////////////////////////////////////////////////////////
// 预备发送
BOOL CSession::PreSend(CServerHandler* pHandler)
{
	if (!m_bRemove && m_bCanSend && m_pSendBuffer->IsReadyToSend()) {
		struct epoll_event event;
		event.events = 0x800;
		event.data.ptr = this;
		pHandler->AddIoEvent(&event);	
	}
    return TRUE;
}

/////////////////////////////////////////////////////////////////////////////////
// 预备发送
BOOL CSession::DoSend(CServerHandler* pHandler)
{
	// 自动锁
	CThreadGuard gd( m_lockSend );
	
	// 发送配置
	if ( m_bCanSend && m_bRemove == FALSE )
	{
		int len;
		BYTE * buf;
		if ( m_pSendBuffer->GetSendParam( &buf, len ) == FALSE ) {
			return TRUE;
		}
		
		assert(len > 0);
		int ret=send( m_socket, buf, len, 0);
		if (ret == SOCKET_ERROR) 
		{
			if ( errno==EAGAIN ) {
				ret = 0;
			}
			else {
				OnLogString("[CSession::DoSend] send error = %d .", errno);				
				return FALSE;
			}
		}

		if (ret < len) {
			m_bCanSend = FALSE;
			DWORD nEvent = EPOLLIN | EPOLLOUT | EPOLLET | EPOLLERR | EPOLLHUP;
			pHandler->ModEpollEvent(this, nEvent);
			OnLogString("[CSession::DoSend] send ret = %d, len = %d EAGAIN.", ret, len);
		}

		// 发送完成
		m_pSendBuffer->Completion(ret);
	}
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////////
// 接收数据
BOOL CSession::DoRecv()
{
	// 自动锁
	CThreadGuard gd( m_lockRecv );
	
	BYTE* buf;
	int ret = 0, len = 0;
	while ( m_bRemove == FALSE )
	{
		m_pRecvBuffer->GetRecvParam( &buf, len );
		if (len <= 0) {
			OnLogString("[CSession::OnRecv] no more recv buffer." );
			Remove(); 
			return FALSE;		
		}
		
		// 接收数据
		ret = recv(m_socket, buf, len, 0);
		if( ret == SOCKET_ERROR  )
		{
			if ( errno == EAGAIN ) {
				OnLogString("[CSession::OnRecv] recv error = EAGAIN .");
				return TRUE;
			}	
			else {		
				OnLogString("[CSession::OnRecv] recv error = %d .", errno);
				Remove(); 
				return FALSE;
			}
		}
		
		// 移除数据
		if (ret == 0) {
			OnLogString("[CSession::OnRecv] recv ret = 0.");
			Remove();
			return FALSE;
		}
		
		// 接收完成
		m_pRecvBuffer->Completion( ret );
		if ( ret < len ) {
			break;
		}
	}
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////////
// 创建句柄
SOCKET CSession::CreateSocket() 
{
	int nRet		= 0;
	int nZero		= 0;

	SOCKET newSocket = socket(AF_INET, SOCK_STREAM, 0);
	if( newSocket == INVALID_SOCKET )  {
		return newSocket;
	}

	CSocketOpt::Nonblocking( newSocket );
	CSocketOpt::DisableBuffering( newSocket );
	return newSocket;
}

/////////////////////////////////////////////////////////////////////////////////
// 绑定句柄
void CSession::BindNetworkObject( CNetworkObject *pNetworkObject )
{
	m_pNetworkObject = pNetworkObject;
	pNetworkObject->SetSession( this );
}

/////////////////////////////////////////////////////////////////////////////////
// 解绑句柄
void CSession::UnbindNetworkObject()
{
	if( m_pNetworkObject == NULL ) {
		return;
	}

	m_pNetworkObject->SetSession( NULL );
	m_pNetworkObject = NULL;
}

/////////////////////////////////////////////////////////////////////////////////
// 监听数据
void CSession::OnAccept()
{
	ResetKillFlag();
	ResetTimeOut();
	m_pNetworkObject->OnAccept( GetIndex() );
}

/////////////////////////////////////////////////////////////////////////////////
// 设置连接
void CSession::OnConnect( BOOL bSuccess )
{
	Init();
	CNetworkObject *pNetworkObject = m_pNetworkObject;
	if( !bSuccess ) {	
		UnbindNetworkObject();
	}
	pNetworkObject->OnConnect( bSuccess, GetIndex() );
}

/////////////////////////////////////////////////////////////////////////////////
// 打印日志
void CSession::OnLogString( char *pszLog, ... )
{
	if( !m_pNetworkObject ) {
		return;
	}

	char	 szBuffer[512] = "";
	va_list  pArguments;

	va_start( pArguments, pszLog );
	vsprintf( szBuffer, pszLog, pArguments );
	va_end( pArguments );
	
	printf( "%s(%s, %d)\n", szBuffer, GetIP().c_str(), GetIndex() );
	m_pNetworkObject->OnLogString( szBuffer );
}

/////////////////////////////////////////////////////////////////////////////////
// 断开连接
void CSession::Disconnect( BOOL bGracefulDisconnect )
{ 
	if( bGracefulDisconnect )  { 
		OnLogString("[REMOVE][%d] bGracefulDisconnect\n", (int)GetSocket()); 
		Remove(); 
	} 
	else {
		m_bDisconnectOrdered = TRUE; 
	}
}