#ifndef _PacketHandler_Headfile_
#define _PacketHandler_Headfile_

/////////////////////////////////////////////////////////////////////////////////
#include <PacketStruct.h>
#include <Singleton.h>
#include <map>

/////////////////////////////////////////////////////////////////////////////////
class CServerSession;
class CUserSession;

/////////////////////////////////////////////////////////////////////////////////
// 句柄函数
typedef VOID (*fnHandler)( CServerSession * pSession, MSG_BASE * pMsg, WORD wSize );
typedef VOID (*fnHandler_c)( CUserSession * pSession, MSG_BASE * pMsg, WORD wSize );


/////////////////////////////////////////////////////////////////////////////////
// 数据包控制
class CPacketHandler: public Singleton<CPacketHandler>
{
public:
	// 构造函数
	CPacketHandler(void);
	// 析构函数
	~CPacketHandler(void);

public:
	// 注册消息
	BOOL RegisterHandler();

	// 注册客户端来的消息
	void Register_CA();
	// 注册登录消息
	void Register_AL();
	// 注册游戏消息
	void Register_AG();

	// 添加发送消息
	void AddSendGameSrvMsg(WORD category, WORD protocol);

	// 添加处理消息句柄
	BOOL AddHandler_CA( WORD category, WORD protocol, fnHandler_c fnHandler);
	// 添加处理消息句柄
	BOOL AddHandler_AL( WORD category, WORD protocol, fnHandler fnHandler);
	// 添加处理消息句柄
	BOOL AddHandler_AG( WORD category, WORD protocol, fnHandler fnHandler);

	//解析消息
	VOID ParsePacket_CA( UserSession  * pSession, MSG_BASE * pMsg, WORD wSize );
	//解析消息
	VOID ParsePacket_AL( ServerSession * pSession, MSG_BASE * pMsg, WORD wSize );
	//解析消息
	VOID ParsePacket_AG( ServerSession * pSession, MSG_BASE * pMsg, WORD wSize );

private:

	//客户端<-->网关服务器的消息
	struct FUNC_CA : public BASE_FUNC {
		fnHandler_c	m_fnHandler;
	};

	//网关服务器<-->数据库服务器的消息
	struct FUNC_AL : public BASE_FUNC {
		fnHandler	m_fnHandler;
	};

	//网关服务器<-->监控服务器的消息
	struct FUNC_AG : public BASE_FUNC {
		fnHandler	m_fnHandler;
	};

private:
	//客户端<-->网关服务器的消息
	FunctionMap	*	m_pFuncMap_CA;

	//网关服务器<-->数据库服务器的消息
	FunctionMap	*	m_pFuncMap_AL;

	//网关服务器<-->监控服务器的消息
	FunctionMap	*	m_pFuncMap_AG;

	//需要转发到游戏服务器的消息
	std::map<DWORD, BYTE>	m_mapSendGameSrvMsg;
};

/////////////////////////////////////////////////////////////////////////////////
// 全局包数据
extern CPacketHandler g_PacketHander;

/////////////////////////////////////////////////////////////////////////////////
#endif
