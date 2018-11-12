#ifndef _RecvBuffer_Headfile_
#define _RecvBuffer_Headfile_

#include "CircuitQueue.h"
/////////////////////////////////////////////////////////////////////////////////

//���ջ���
class RecvBuffer
{
	//��������
private:
	CircuitQueue<BYTE>				*m_pQueue;						// ��������

public:
	// ���캯��
	RecvBuffer() { 
		m_pQueue = NULL;
	}
	// ��������
	virtual ~RecvBuffer() {
		if( m_pQueue ) {
			delete m_pQueue; 
		}
	}
	// �������ݿ�
	inline void	Create( int nBufferSize, DWORD dwExtraBufferSize ) {
		if( m_pQueue ) {
			delete m_pQueue;
		}
		m_pQueue = new CircuitQueue<BYTE>;
		m_pQueue->Create( nBufferSize, dwExtraBufferSize ); 
	}
	// �������
	inline void	Completion( int nBytesRecvd ) {
		m_pQueue->Enqueue( NULL, nBytesRecvd ); 
	}
	// ��ȡָ��
	inline void	Clear() { 
		m_pQueue->Clear(); 
	}
	// ��ȡ����
	inline void	GetRecvParam( BYTE **ppRecvPtr, int &nLength ) {
		*ppRecvPtr	= m_pQueue->GetWritePtr();		
		nLength		= m_pQueue->GetWritableLen(); 
	}
	// ����ָ��
	inline BYTE* GetFirstPacketPtr() {
		PACKET_HEADER   header;
		int nPktSize = sizeof(PACKET_HEADER);

		// ��ȡ����
		if(!m_pQueue->Peek( (BYTE*)&header, nPktSize )) {
			return NULL;
		}

		// �ٴ���ʾ
		nPktSize += header.size;
		if(m_pQueue->GetLength() < nPktSize) {
			return NULL;
		}
		
		// ���ݸ���
		if(m_pQueue->GetBackDataCount() < nPktSize) {
			m_pQueue->CopyHeadDataToExtraBuffer( nPktSize - m_pQueue->GetBackDataCount() ); 
		}
		return  m_pQueue->GetReadPtr();
	}
	// �Ƴ����ݰ�
	inline void	RemoveFirstPacket( WORD wSize ) { 
		m_pQueue->Dequeue( NULL, wSize ); 
	}
};

#endif
