#ifndef _Session_Headfile_
#define _Session_Headfile_

/////////////////////////////////////////////////////////////////////////////////
#pragma once
class SendBuffer;
class RecvBuffer;

/////////////////////////////////////////////////////////////////////////////////
// 会话类型

class Session
{
	// 友元类型
	friend void* io_thread( void* param );
	
	// 参数变量
public:
	SendBuffer				*m_pSendBuffer;					// 发送缓存	
	RecvBuffer				*m_pRecvBuffer;					// 接收缓存
	SOCKET					m_socket;						// 网络句柄
	SOCKADDR_IN				m_sockaddr;						// 网络地址
	DWORD					m_dwLastSyncTick;				// 导步时间
	volatile int			m_bRemove;						// 是否移除
	DWORD					m_dwTimeOut;					// 超时变量
	DWORD					m_dwIndex;						// 会话下标
	BOOL					m_bAcceptSocket;				// 是否监听
	BOOL					m_bDisconnectOrdered;			// 是否断开
	int						m_iCategory;					// 协议类型
	int						m_iProtocol;					// 协议参数
	int						m_iSize;						// 字节大小
	TLock					m_lockRecv;						// 接收锁
	TLock					m_lockSend;						// 发送锁
	BOOL					m_bCanSend;						// 可否发送
	UINT64					m_dwTotalRecvBytes;				// 总共接收到的数据字节数
	UINT64					m_dwTotalSendBytes;				// 总共接收到的数据字节数

public:
	// 构造函数
	Session( DWORD dwSendBufferSize, DWORD dwRecvBufferSize, DWORD dwMaxPacketSize, DWORD dwTimeOut );
	// 析构函数
	virtual ~Session();

	// 初始化数据
	void					Init();
	// 发送数据
	BOOL					Send( BYTE *pMsg, WORD wSize );
	// 扩展发送
	BOOL					SendEx( DWORD dwNumberOfMessages, BYTE **ppMsg, WORD *pwSize );
	// 开始发送
	BOOL					OnSend();
	// 准备发送
	BOOL 					PreSend(IoHandler* pIoHandler);
	// 发送数据
	BOOL					DoSend(IoHandler* pIoHandler);
	// 接收数据
	BOOL					DoRecv();
	// 创建网络句柄
	SOCKET					CreateSocket();
	// 接收数数据包
	BOOL					ProcessRecvdPacket( DWORD dwMaxPacketSize );
	// 绑定句柄
	void					BindNetworkObject( NetworkObject *pNetworkObject );
	// 解绑句柄
	void					UnbindNetworkObject();
	// 设置为监听
	void					OnAccept();
	// 设置连接
	void					OnConnect( BOOL bSuccess );
	void					OnLogString( char *pszLog, ... );
	// 地址数
	inline void				SetSocket( SOCKET socket ) {
		m_socket = socket;
	}
	// 地址数据
	inline void	SetSockAddr( SOCKADDR_IN& sockaddr ) {
		m_sockaddr = sockaddr; }
	// 关闭地址
	inline void	CloseSocket() { 
		SocketOpt::CloseSocket( m_socket ); 
		m_socket = INVALID_SOCKET; 
	}
	// 网络接口
	inline NetworkObject* GetNetworkObject() { 
		return m_pNetworkObject; 
	}
	// 发送缓存
	inline SendBuffer*	GetSendBuffer() { 
		return m_pSendBuffer; 
	}
	// 接收缓存
	inline RecvBuffer*	GetRecvBuffer() { 
		return m_pRecvBuffer;
	} 
	// 网络句柄
	inline SOCKET GetSocket() { 
		return m_socket;
	}
	// 网络地址
	inline SOCKADDR_IN*	GetSockAddr() { 
		return &m_sockaddr; 
	}
	// 获取IP地址
	inline std::string GetIP() {
		return inet_ntoa( m_sockaddr.sin_addr ); 
	}
	// 获取超时 
	inline DWORD GetIdleTick() const { 
		return m_dwTimeOut? m_dwLastSyncTick + m_dwTimeOut : 0; 
	}
	// 是否超时
	inline BOOL	IsOnIdle() { 
		return m_dwTimeOut ? m_dwLastSyncTick + m_dwTimeOut < GetTickCount() : FALSE; 
	}
	// 获取下标
	inline DWORD GetIndex() { 
		return m_dwIndex; 
	}
	// 是否监听句柄
	inline BOOL IsAcceptSocket() { 
		return m_bAcceptSocket;
	}
	// 设置监听 
	inline void SetAcceptSocketFlag() { 
		m_bAcceptSocket = TRUE; 
	}
	// 移除参数
	void Remove() { 
		__sync_fetch_and_or (&m_bRemove, 1);
	}
	// 重置参数
	inline void ResetKillFlag() { 
		__sync_fetch_and_and (&m_bRemove, 0);
	}
	// 是否移除
	inline BOOL ShouldBeRemoved() { 
		return m_bRemove; 
	}
	// 断开连接
	void Disconnect( BOOL bGracefulDisconnect );
	// 判断连接
	inline BOOL HasDisconnectOrdered() { 
		return m_bDisconnectOrdered; 
	}

private:
	// 设置下标
	void SetIndex( DWORD index ) {
		m_dwIndex = index; 
	}
	// 重置时间
	inline void ResetTimeOut() { 
		m_dwLastSyncTick = GetTickCount(); 
	}
};