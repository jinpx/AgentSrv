#ifndef _PacketStruct_Headfile_
#define _PacketStruct_Headfile_

#pragma pack(push,1)
/////////////////////////////////////////////////////////////////////////////////
// 消息命名规则

/*
1) (Request)						_REQ 请求消息
2) (Answer)							_ANS 应答消息
3) Client -> Server (Sync)			_SYN 同步消息
4) Server -> Client (Command)		-CMD 命令消息
5) Server -> Client Broadcasting	_BRD 广播消息
6) Server -> Client (Notify)		_NTF 服务器通知消息
7) 									_ACK 允许
8) 									_NAK 失败
*/

/////////////////////////////////////////////////////////////////////////////////
// 消息基类
struct MSG_BASE
{
	MSG_BASE()
	{
		memset(this, 0, sizeof(MSG_BASE));
	}
	void Init(WORD wCMDType, WORD wCMDID)
	{
		m_byCategory = wCMDType;
		m_byProtocol = wCMDID;
	}

	WORD	m_byCategory;	//消息种类
	WORD	m_byProtocol;	//消息ID	
};

/////////////////////////////////////////////////////////////////////////////////
// 心跳类型
struct MSG_HEARTBEAT : public MSG_BASE
{
	MSG_HEARTBEAT() {
		memset(this, 0, sizeof(MSG_BASE));
	}
};

/////////////////////////////////////////////////////////////////////////////////
#endif
