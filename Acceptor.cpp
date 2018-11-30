#include <Acceptor.h>
#include <RecvBuffer.h>

/////////////////////////////////////////////////////////////////////////////////
// �����߳�
void * accept_thread(void * param )
{
	Acceptor *pAcceptor = (Acceptor*)param;
	while( !pAcceptor->m_bShutdown )
	{
		struct sockaddr_in addr;
		int len = sizeof(struct sockaddr_in);
		SOCKET	sock = accept(pAcceptor->m_listenSocket, (sockaddr*)&addr, (socklen_t*)&len);
		if( sock == -1)
			continue; 

		SocketOpt::Nonblocking( sock );
		SocketOpt::DisableBuffering( sock );
	}

	printf ("accept_thread 0x%x exit\n", pthread_self ()); 
	pthread_exit(0);
}

/////////////////////////////////////////////////////////////////////////////////
// ���캯��
CAcceptor::CAcceptor()
{
	m_listenSocket	= INVALID_SOCKET;
	m_hAcceptThread = 0;
	m_bShutdown = 0;
}

/////////////////////////////////////////////////////////////////////////////////
// ��������
CAcceptor::~CAcceptor()
{
	Shutdown();
}

/////////////////////////////////////////////////////////////////////////////////
// ��ʼ��
void CAcceptor::Init( CServerHandler *pHandler )
{
	m_pHandler = pHandler;
}

/////////////////////////////////////////////////////////////////////////////////
// ��ʼ��������
BOOL CAcceptor::StartListen( char *pIP, WORD wPort )
{
	if( m_listenSocket != INVALID_SOCKET ) {
		return FALSE;
	}

	m_listenSocket = socket(AF_INET, SOCK_STREAM, 0);
	if( m_listenSocket == INVALID_SOCKET ) {
		printf("\ncreate socket fail");
		return FALSE;
	}

	SocketOpt::ReuseAddr(m_listenSocket);
//	SocketOpt::Nonblocking(m_listenSocket);
//	SocketOpt::SetTimeout(m_listenSocket, 30);

	memset( &m_sockaddr, 0, sizeof( m_sockaddr ) );
	m_sockaddr.sin_family		= AF_INET;
	m_sockaddr.sin_addr.s_addr	= ( pIP == NULL || strlen( pIP ) == 0 ) ? htonl(INADDR_ANY) : inet_addr(pIP);
	m_sockaddr.sin_port			= htons( wPort );

	int err = bind( m_listenSocket, (SOCKADDR *)&m_sockaddr, sizeof(m_sockaddr) );
	if( err == SOCKET_ERROR ) {
		SocketOpt::CloseSocket(m_listenSocket);
		printf("\nbind fail %s:%d\n", pIP, wPort);
		return FALSE;
	}

	err = listen( m_listenSocket, 5);//SOMAXCONN );
	if( err == SOCKET_ERROR ) {
		SocketOpt::CloseSocket(m_listenSocket);
		printf("\n[Acceptor::CreateListenSocket] socket listen fail!");
		return FALSE;
	}

	pthread_create( &m_hAcceptThread, NULL, accept_thread, (void*)this);
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////////
// �ر�����
void CAcceptor::Shutdown()
{
	if( m_listenSocket != INVALID_SOCKET )		
	{
		SocketOpt::CloseSocket( m_listenSocket );
		m_bShutdown = 1;
		pthread_cancel(m_hAcceptThread); 
		pthread_join(m_hAcceptThread, NULL);   
		printf ("accept_thread 0x%x exit\n", m_hAcceptThread); 
	}

	m_listenSocket	= INVALID_SOCKET;
	m_hAcceptThread = 0;
	m_bShutdown = 0;
}
