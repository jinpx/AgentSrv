#include "CircuitLock.h"
#include "Network.h"
#include "RecvBuffer.h"
#include "SendBuffer.h"
#include "Session.h"

/////////////////////////////////////////////////////////////////////////////////
// ���캯��
Session::Session( DWORD dwSendBufferSize, DWORD dwRecvBufferSize, DWORD dwMaxPacketSize, DWORD dwTimeOut )
{
	m_pSendBuffer = new SendBuffer;
	m_pSendBuffer->Create( dwSendBufferSize, dwMaxPacketSize );

	m_pRecvBuffer = new RecvBuffer;
	m_pRecvBuffer->Create( dwRecvBufferSize, dwMaxPacketSize );

	m_dwTimeOut			= dwTimeOut;
	m_socket			= INVALID_SOCKET;
	m_bAcceptSocket		= FALSE;
	m_bCanSend			= true;
}

/////////////////////////////////////////////////////////////////////////////////
// ��������
Session::~Session()
{
	CloseSocket();

	// ɾ����������
	if( m_pSendBuffer ) {
		delete m_pSendBuffer;
	}

	// ɾ����������
	if( m_pRecvBuffer ) {
		delete m_pRecvBuffer;
	}
}

/////////////////////////////////////////////////////////////////////////////////
// ��������
BOOL Session::Send( BYTE *pMsg, WORD wSize )
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
// ��������
BOOL Session::SendEx( DWORD dwNumberOfMessages, BYTE **ppMsg, WORD *pwSize )
{
	PACKET_HEADER header;
	header.size = 0;
	DWORD i;
	
	for( i = 0; i < dwNumberOfMessages; ++i )
	{
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
// ���մ���
BOOL Session::ProcessRecvdPacket( DWORD dwMaxPacketSize )
{
	BYTE			*pPacket;
	PACKET_HEADER	*pHeader;

	// �������ݰ�
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

		// ��������
		m_pNetworkObject->OnRecv( pPacket+sizeof(PACKET_HEADER), pHeader->size );
		GetRecvBuffer()->RemoveFirstPacket( sizeof(PACKET_HEADER) + pHeader->size );

		// ��ʱ����
		ResetTimeOut();

		// Э�����
		m_iCategory = *( pPacket + sizeof(PACKET_HEADER) );
		m_iProtocol = *( pPacket + sizeof(PACKET_HEADER) + 1 );
		m_iSize = pHeader->size + sizeof(PACKET_HEADER);

		// ����С
		m_dwTotalRecvBytes	+= pHeader->size + sizeof(PACKET_HEADER);
	}
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////////
// �����ж�
BOOL Session::OnSend()
{
	OnLogString("[Session::OnSend]");
	
	m_lockSend.Lock();
	m_bCanSend = TRUE;
	m_lockSend.Unlock();
	return m_bCanSend;
}

/////////////////////////////////////////////////////////////////////////////////
// Ԥ������
BOOL Session::PreSend()
{
	if (!m_bRemove && m_bCanSend && m_pSendBuffer->IsReadyToSend()) {
		struct epoll_event event;
		event.events = 0x800;
		event.data.ptr = this;
		pIoHandler->AddIoEvent(&event);	
	}
    return TRUE;
}

/////////////////////////////////////////////////////////////////////////////////
// Ԥ������
BOOL Session::DoSend(IoHandler* pIoHandler)
{
	// �Զ���
	CCircuitlGuard gd( m_lockSend );
	
	// ��������
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
				OnLogString("[Session::DoSend] send error = %d .", errno);				
				return FALSE;
			}
		}

		if (ret < len) {
			m_bCanSend = FALSE;
			DWORD event = EPOLLIN | EPOLLOUT | EPOLLET | EPOLLERR | EPOLLHUP;
			pIoHandler->ModEpollEvent(this,  event);
			OnLogString("[Session::DoSend] send ret = %d, len = %d EAGAIN.", ret, len);
		}

		// �������
		m_pSendBuffer->Completion(ret);
	}
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////////
// ��������
BOOL Session::DoRecv()
{
	// �Զ���
	CCircuitlGuard gd( m_lockRecv );
	
	BYTE* buf;
	int ret = 0, len = 0;
	while ( m_bRemove == FALSE )
	{
		m_pRecvBuffer->GetRecvParam( &buf, len );
		if (len <= 0) {
			OnLogString("[Session::OnRecv] no more recv buffer." );
			Remove(); 
			return FALSE;		
		}
		
		// ��������
		ret = recv(m_socket, buf, len, 0);
		if( ret == SOCKET_ERROR  )
		{
			if ( errno == EAGAIN ) {
				OnLogString("[Session::OnRecv] recv error = EAGAIN .");
				return TRUE;
			}	
			else {		
				OnLogString("[Session::OnRecv] recv error = %d .", errno);
				Remove(); 
				return FALSE;
			}
		}
		
		// �Ƴ�����
		if (ret == 0) {
			OnLogString("[Session::OnRecv] recv ret = 0.");
			Remove();
			return FALSE;
		}
		
		// �������
		m_pRecvBuffer->Completion( ret );
		if ( ret < len ) {
			break;
		}
	}
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////////
// �������
SOCKET Session::CreateSocket() 
{
	int nRet		= 0;
	int nZero		= 0;

	SOCKET newSocket = socket(AF_INET, SOCK_STREAM, 0);
	if( newSocket == INVALID_SOCKET )  {
		return newSocket;
	}

	SocketOpt::Nonblocking( newSocket );
	SocketOpt::DisableBuffering( newSocket );
	return newSocket;
}

/////////////////////////////////////////////////////////////////////////////////
// �󶨾��
void Session::BindNetworkObject( NetworkObject *pNetworkObject )
{
	m_pNetworkObject = pNetworkObject;
	pNetworkObject->SetSession( this );
}

/////////////////////////////////////////////////////////////////////////////////
// �����
void Session::UnbindNetworkObject()
{
	if( m_pNetworkObject == NULL ) {
		return;
	}

	m_pNetworkObject->SetSession( NULL );
	m_pNetworkObject = NULL;
}

/////////////////////////////////////////////////////////////////////////////////
// ��������
void Session::OnAccept()
{
	ResetKillFlag();
	ResetTimeOut();
	m_pNetworkObject->OnAccept( GetIndex() );
}

/////////////////////////////////////////////////////////////////////////////////
// ��������
void Session::OnConnect( BOOL bSuccess )
{
	Init();
	NetworkObject *pNetworkObject = m_pNetworkObject;
	if( !bSuccess ) {	
		UnbindNetworkObject();
	}
	pNetworkObject->OnConnect( bSuccess, GetIndex() );
}

/////////////////////////////////////////////////////////////////////////////////
// ��ӡ��־
void Session::OnLogString( char *pszLog, ... )
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
// �Ͽ�����
void Session::Disconnect( BOOL bGracefulDisconnect )
{ 
	if( bGracefulDisconnect )  { 
		OnLogString("[REMOVE][%d] bGracefulDisconnect\n", (int)GetSocket()); 
		Remove(); 
	} 
	else {
		m_bDisconnectOrdered = TRUE; 
	}
}