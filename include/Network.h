#ifndef _Network_Headfile_
#define _Network_Headfile_

#pragma pack(push,1)
/////////////////////////////////////////////////////////////////////////////////
// 包头

typedef struct tagPACKET_HEADER
{
	WORD		size;			// 数据包大小
} PACKET_HEADER;

/////////////////////////////////////////////////////////////////////////////////
class CSession;
class CServerHandler;

/////////////////////////////////////////////////////////////////////////////////
// 网络对象

class CNetworkObject
{
	// 会话参数
	friend class CSession;
	// 控制句柄
	friend class CServerHandler;
	
	// 参数变量
private:
	CSession *					m_pSession;			// 会话参数

public:
	// 构造函数
	CNetworkObject();
	// 析构函数
	virtual ~CNetworkObject();
	// 关闭连接
	void Disconnect( BOOL bGracefulDisconnect = TRUE );
	// 发送数据
	BOOL Send( BYTE *pMsg, WORD wSize );
	// 发送数据
	BOOL SendEx( DWORD dwNumberOfMessages, BYTE **ppMsg, WORD *pwSize );
	// 绑定参数
	void Redirect( CNetworkObject *pNetworkObject );
	// 获取地址
	SOCKADDR_IN* GetSockAddr();
	// 获取地址
	std::string GetIP();

	// 虚拟参数
private:
	// 监听函数
	virtual void OnAccept( DWORD dwNetworkIndex ) {}
	// 关闭连接
	virtual void OnDisconnect() {}
	// 接收数据
	virtual	void OnRecv( BYTE *pMsg, WORD wSize ) = 0;
	// 连接参数
	virtual void OnConnect( BOOL bSuccess, DWORD dwNetworkIndex ) {}
	// 打印日志
	virtual void OnLogString( char *pszLog ) {}
	// 设置会话
	inline void	SetSession( CSession *pSession ) { 
		m_pSession = pSession; 
	}
};


/////////////////////////////////////////////////////////////////////////////////
#pragma pack(pop)
#endif
