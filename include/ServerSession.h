#ifndef _ServerSeesion_Headfile_
#define _ServerSeesion_Headfile_

#pragma once
/////////////////////////////////////////////////////////////////////////////////
#include <Network.h>

/////////////////////////////////////////////////////////////////////////////////
#include <iostream>
using namespace std;

/////////////////////////////////////////////////////////////////////////////////
// 服务器类型

enum eSERVER_TYPE
{
	UNKNOWN_SERVER 			= 0,
	TEMP_SERVER				,
	LOGON_SERVER			,		//登录服务器
	GAME_SERVER				,		//游戏服务器
	DB_SERVER				,		//数据库服务
};

/////////////////////////////////////////////////////////////////////////////////
// 服务器会话类型

class CServerSession : public CNetworkObject
{
	// 服务器友元
	// friend class CAgentSrv;
	
	// 参数变量
private:
	DWORD					m_dwSessionIndex;
	std::string				m_strConnectIP;
	WORD					m_wConnectPort;
	BOOL					m_bForConnect;
	DWORD					m_dwLastHeartbeatTick;
	DWORD					m_bConnection;

public:
	// 构造函数
	CServerSession();
	// 析构函数
	~CServerSession();

	// 清理数据
	virtual VOID Clear();
	// 初始数据
	virtual VOID Init();
	// 释放数据
	virtual VOID Release();
	// 更新数据
	virtual VOID Update();
	// 监听网络
	virtual VOID OnAccept( DWORD dwNetworkIndex );
	// 断开连接
	virtual VOID OnDisconnect();
	// 设置连接
	virtual VOID OnConnect( BOOL bSuccess, DWORD dwNetworkIndex );
	// 接收数据
	virtual VOID OnRecv( BYTE *pMsg, WORD wSize );
	// 打印日志
	virtual VOID OnLogString( char *pszLog );
	// 服务类型
	virtual eSERVER_TYPE GetServerType() { 
		return UNKNOWN_SERVER;
	}
	// 会话标识
	DWORD GetSessionIndex() { 
		return m_dwSessionIndex; 
	}
	// 设置标识
	VOID SetSessionIndex( DWORD dwIdx ) { 
		m_dwSessionIndex = dwIdx; 
	}
	// 发送类型
	VOID SendServerType();
	// 设置地址
	VOID SetAddr( char *pszIP, WORD wPort );
	// 尝试连接
	VOID TryToConnect();
	// 连接IP
	inline std::string & GetConnectIP() { 
		return m_strConnectIP; 
	}
	// 连接端口
	inline WORD GetConnectPort() { 
		return m_wConnectPort; 
	}
	// 连接问题
	inline BOOL IsConnected() { 
		return m_bConnection; 
	}
	// 设置连接
	inline VOID SetForConnect( BOOL bForConnect ) { 
		m_bForConnect = bForConnect; 
	}
	// 判断连接
	inline BOOL IsForConnect() { 
		return m_bForConnect;
	}
};

/////////////////////////////////////////////////////////////////////////////////
#endif // __SERVER_SESSION_H__
