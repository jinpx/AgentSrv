#ifndef _PacketStruct_Headfile_
#define _PacketStruct_Headfile_

#pragma pack(push,1)
/////////////////////////////////////////////////////////////////////////////////
// ��Ϣ��������

/*
1) (Request)						_REQ ������Ϣ
2) (Answer)							_ANS Ӧ����Ϣ
3) Client -> Server (Sync)			_SYN ͬ����Ϣ
4) Server -> Client (Command)		-CMD ������Ϣ
5) Server -> Client Broadcasting	_BRD �㲥��Ϣ
6) Server -> Client (Notify)		_NTF ������֪ͨ��Ϣ
7) 									_ACK ����
8) 									_NAK ʧ��
*/

/////////////////////////////////////////////////////////////////////////////////
// ��Ϣ����
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

	WORD	m_byCategory;	//��Ϣ����
	WORD	m_byProtocol;	//��ϢID	
};

/////////////////////////////////////////////////////////////////////////////////
// ��������
struct MSG_HEARTBEAT : public MSG_BASE
{
	MSG_HEARTBEAT() {
		memset(this, 0, sizeof(MSG_BASE));
	}
};

/////////////////////////////////////////////////////////////////////////////////
#endif
