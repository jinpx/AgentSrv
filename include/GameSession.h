#ifndef _GameSession_Headfile_
#define _GameSession_Headfile_

/////////////////////////////////////////////////////////////////////////////////
#include "ServerSession.h"

/////////////////////////////////////////////////////////////////////////////////
// 游戏服务器

class CGameSession : public CServerSession
{
public:
	// 构造函数
	CGameSession(void);
	// 析构函数
	~CGameSession(void);
	// 接收数据
	virtual VOID OnRecv( BYTE * pMsg, WORD wSize );
	// 获取类型
	eSERVER_TYPE GetServerType() {
		return GAME_SERVER;
	}

protected:
	// 连接数据
	virtual VOID OnDisconnect();
	// 打印日志
	virtual VOID OnLogString( char *pszLog );
	// 初始化
	virtual VOID Init();
};

#endif
