#include <Connector.h>
#include <SocketOpt.h>
#include <Session.h>

/////////////////////////////////////////////////////////////////////////////////
// �����߳�
void * connect_thread(void * param )
{
	CConnector *pConnector = (CConnector*)param;
	while( true )
	{
		sem_wait(&pConnector->m_semConnect);
		if ( pConnector->m_bShutdown )
		{
			printf ("\nconnect_thread 0x%x exit\n", pthread_self ()); 
			break; 
		}
		
		// ��������
		CSession * pSession = pConnector->m_pSession;
		int err = connect( pSession->GetSocket(), (SOCKADDR*)( pSession->GetSockAddr() ), sizeof(SOCKADDR_IN) );
		if( err == SOCKET_ERROR )
		{
			printf ("connect fail, errno = %d. %s:%d \n", errno,
				inet_ntoa(pSession->GetSockAddr()->sin_addr), 
				ntohs(pSession->GetSockAddr()->sin_port) ); 
		}
		else
		{
			CSocketOpt::Nonblocking( pSession->GetSocket() );
			CSocketOpt::DisableBuffering( pSession->GetSocket() );
		}
	}

	printf ("connect_thread 0x%x exit\n", pthread_self ()); 
	pthread_exit(0);
}

/////////////////////////////////////////////////////////////////////////////////
// ���캯��
CConnector::CConnector()
{
	m_pSession			= 0L;
	m_bShutdown			= FALSE;
	m_hThread			= 0;
}

/////////////////////////////////////////////////////////////////////////////////
// ��������
CConnector::~CConnector()
{
	if( !m_bShutdown ) {
		Shutdown();
	}
}

/////////////////////////////////////////////////////////////////////////////////
// ��ʼ��
void CConnector::Init( CServerHandler *pHandler )
{
	// �����ź�
	if ( sem_init(&m_semConnect,0,0)!=0 ) {
  		perror("Semaphore initialization failed");
 	}

	// �����߳�
	pthread_create( &m_hThread, NULL, connect_thread, (void*)this );
}

/////////////////////////////////////////////////////////////////////////////////
// ��������
void CConnector::Connect( CSession * pSession )
{
	// ��������
	m_pSession = m_pSession;

	// �����߳�
	sem_post( &m_semConnect );
}

/////////////////////////////////////////////////////////////////////////////////
// �ر�����
void CConnector::Shutdown()
{
	// �ر�����
	m_bShutdown = TRUE;
	
	// wake up connect_thread to exit
	sem_post(&m_semConnect);
	pthread_cancel(m_hThread);
	pthread_join(m_hThread, NULL); 
	sem_destroy(&m_semConnect);
}
