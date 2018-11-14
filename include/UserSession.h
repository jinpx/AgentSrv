#ifndef _UserSession_Headfile_
#define _UserSession_Headfile_

/////////////////////////////////////////////////////////////////////////////////
enum eUSER_TYPE { UT_TEMP_USER, UT_USER, };

/////////////////////////////////////////////////////////////////////////////////
// 用户会话

class CUserSession : public CServerSession
{
	// 参数变量
protected:
	eUSER_TYPE				m_eUserType;						// 用户类型
	DWORD					m_dwUserId;							// 用户标识
	TCHAR					m_pszID[MAX_ID_LEN+1];				// 帐户类型
	DWORD					m_dwUserKey;						// 用户键值
	TCHAR					m_pszLoginTime[MAX_TIMEDATA_SIZE];	// 登录时间
	ServerSession *			m_pGameServer;

	// 基本函数
public:
	// 构造函数
	CUserSession(void);
	// 析构函数
	~CUserSession(void);

	// 用户参数
public:
	// 用户标识
	DWORD GetUserID() { 
		return m_dwUserId; 
	}
	// 用户标识
	VOID SetUserID( DWORD dwUserID ) {
		m_dwUserId = dwUserID;
	}
	// 设置用户名称
	VOID SetUsername( const TCHAR * ps );	
	// 获取用户名称
	const TCHAR * GetUsername() { 
		return m_pszID;
	}
	// 用户键值
	VOID SetUserKey( DWORD dwKey ) { 
		m_dwUserKey = dwKey; 
	}
	// 用户键值
	DWORD GetUserKey() const { 
		return m_dwUserKey; 
	}
	// 设置IP数据
	VOID SetClientIP( const TCHAR * ps ) {
		strcpy(m_pszClientIP, ps);
	}
	// 登录时间
	VOID SetLoginTime( const TCHAR * ps ) { 
		strncpy( m_pszLoginTime, ps, MAX_TIMEDATA_SIZE ); 
		m_pszID[MAX_ID_LEN-1]=0; 
	}
	// 获取时间
	const TCHAR * GetLoginTime() { 
		return m_pszLoginTime; 
	}
	// 游戏服务器
	VOID UnGameServer();

	// 基类函数
public:
	// 发送数据
	virtual BOOL SendPacket( MSG_BASE * pMsg, WORD wSize ); 
	// 接收数据
	virtual VOID OnRecv( BYTE * pMsg, WORD wSize );
	// 游戏服务器
	ServerSession *	GetGameServer() {
		return m_pGameServer;
	}
	// 获取类型
	eSERVER_TYPE GetServerType() {
		return GAME_SERVER;
	}
	//用户类型
	virtual eUSER_TYPE	getUserType() { 
		return m_eUserType; 
	}
	// 用户类型
	virtual VOID setUserType( eUSER_TYPE type ) {
		m_eUserType = type; 
	}

protected:
	// 打印日志
	virtual VOID OnLogString( char *pszLog );
	// 初始化
	virtual VOID Init();
	// 释放参数
	virtual VOID release();
	// 连接数据
	virtual VOID OnDisconnect();

};

/////////////////////////////////////////////////////////////////////////////////
#endif
