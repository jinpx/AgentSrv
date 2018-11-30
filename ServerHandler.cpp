#include <Session.h>
#include <ServerHandler.h>

/////////////////////////////////////////////////////////////////////////////////
// eopll监听线程
void* epoll_thread(void* param ) 
{
	CServerHandler *pServerHandler = (CServerHandler*)param;

	pServerHandler->m_epoll = epoll_create(SOCKET_HOLDER_SIZE);
    if(pServerHandler->m_epoll == -1) {
        printf("Could not create epoll fd (/dev/epoll).");
        pthread_exit(0);
    }
    
	// 创建事件头
	struct epoll_event *events = new struct epoll_event [SOCKET_HOLDER_SIZE];

	while( !pServerHandler->m_bShutdown )
	{
        int fd_count = epoll_wait(pServerHandler->m_epoll, events, SOCKET_HOLDER_SIZE, 5000);
        for ( int i = 0; i < fd_count; i ++ )
        {
        	CSession* pSession = (CSession*)events[i].data.ptr;
        	if( events[i].events & EPOLLHUP || events[i].events & EPOLLERR) {
				pSession->Remove();
	            continue;
		    }
			
			// 发送事件
        	if(events[i].events & EPOLLOUT) {
				pSession->OnSend();
				pServerHandler->ModEpollEvent(pSession,  EPOLLIN | EPOLLET | EPOLLERR | EPOLLHUP);
        	}

        	// 接收处理事件
        	if(events[i].events & EPOLLIN) {
        		pServerHandler->AddIoEvent(&events[i]);        	
        	}
		}
	}

	// 删除根事件
	delete [] events;

	printf ("epoll_thread 0x%x exit\n", pthread_self ()); 
	pthread_exit(0);
}

/////////////////////////////////////////////////////////////////////////////////
// 添加事件
void CServerHandler::AddIoEvent(struct epoll_event* pEvent)
{
    m_lockEvents.Lock();        	
    m_pEvents->Enqueue(pEvent, 1);
    m_lockEvents.Unlock();

    // wake up a io_thread  
    m_condEvents.Signal();
}

/////////////////////////////////////////////////////////////////////////////////
// 修改事件
int CServerHandler::ModEpollEvent(CSession *pSession, DWORD nEvent )
{
	// Add epoll event based on socket activity.
	struct epoll_event ev;
	memset(&ev, 0, sizeof(epoll_event));

	// use edge-triggered instead of level-triggered because we're using nonblocking sockets 
	ev.events = nEvent | EPOLLET; 
	//ev.data.fd = pSession->GetSocket();
	ev.data.ptr = pSession;
    
	if( epoll_ctl(m_epoll, EPOLL_CTL_MOD, pSession->GetSocket(), &ev) != 0) {
		pSession->OnLogString("Epoll could not add event to epoll set on fd %u", pSession->GetSocket());
		return FALSE;
	}
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////////
// 修改事件
int CServerHandler::AddEpollEvent(CSession *pSession )
{
	// Add epoll event based on socket activity.
	struct epoll_event ev;
	memset(&ev, 0, sizeof(epoll_event));

	// use edge-triggered instead of level-triggered because we're using nonblocking sockets 
	ev.events = EPOLLIN | EPOLLET | EPOLLERR | EPOLLHUP; 
	//ev.events |= EPOLLOUT ;
	//ev.data.fd = pSession->GetSocket();
	ev.data.ptr = pSession;
    
	if( epoll_ctl(m_epoll, EPOLL_CTL_ADD, pSession->GetSocket(), &ev) != 0)
	{
		pSession->OnLogString("Epoll could not add event to epoll set on fd %u", pSession->GetSocket());
		return FALSE;
	}
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////////
// 删除事件
void CServerHandler::DelEpollEvent(CSession *pSession )
{
    // Remove from epoll list.
    struct epoll_event ev;
	memset(&ev, 0, sizeof(epoll_event));
	//ev.data.fd = pSession->GetSocket();
	ev.data.ptr = pSession;
    ev.events = EPOLLIN | EPOLLOUT | EPOLLERR | EPOLLHUP | EPOLLONESHOT;

    if(epoll_ctl(m_epoll, EPOLL_CTL_DEL, pSession->GetSocket(), &ev)) {
		pSession->OnLogString("Epoll could not remove fd %u from epoll set", pSession->GetSocket());
	}
}
