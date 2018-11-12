#ifndef _SendBuffer_Headfile_
#define _SendBuffer_Headfile_

#include "CircuitQueue.h"
/////////////////////////////////////////////////////////////////////////////////

//���ͻ���
class SendBuffer
{
	//��������
private:
	BOOL						m_bComplete;						// ��������
	CircuitQueue<BYTE>			*m_pQueue;							// ��������

public:
	// ���캯��
	SendBuffer() { 
		m_pQueue = NULL;
	}
	// ��������
	virtual ~SendBuffer() {
		if( m_pQueue ) {
			delete m_pQueue; 
		}
	}
	// �������ݿ�
	inline void Create( int nBufferSize, DWORD dwExtraBuffeSize ) {
		if( m_pQueue ) {
			delete m_pQueue;
		}
		m_pQueue = new CircuitQueue<BYTE>;
		m_pQueue->Create( nBufferSize, dwExtraBuffeSize );
		m_bComplete = TRUE;
	}
	//�������ݿ�
	inline void Clear() { 
		m_pQueue->Clear();
		m_bComplete = TRUE; 
	}
	// �������
	inline void Completion( int nBytesSend ) {
		m_pQueue->Dequeue( NULL, nBytesSend ); 
		m_bComplete = TRUE;
	}
	// ��ȡָ��
	inline BOOL	GetSendParam( BYTE **ppSendPtr, int &nLength ) 
	{
		// �����ж�
		if( !IsReadyToSend() ) 
		{ 
			nLength = 0; 
			return FALSE; 
		}
		*ppSendPtr	= m_pQueue->GetReadPtr();
		nLength		= m_pQueue->GetReadableLen();
		m_bComplete = FALSE;
		return TRUE; 
	}
	// д������
	inline BOOL Write( tagPACKET_HEADER *pHeader, BYTE *pMsg )	{
		if( !m_pQueue->Enqueue( (BYTE*)pHeader, sizeof(PACKET_HEADER)) ) {
			return FALSE;
		}
		if( !m_pQueue->Enqueue( pMsg, pHeader->size ) )	{
			return FALSE;
		}
		return TRUE; 
	}
	// д������
	inline BOOL Write( BYTE *pMsg, WORD wSize ) {
		if( !m_pQueue->Enqueue( pMsg, wSize ) ) {
			return FALSE;
		}
		return TRUE; 
	}
	// ȡ�ó���
	inline DWORD GetLength() { 
		return m_pQueue->GetLength(); 
	}
	// �ɶ�����
	inline BOOL IsReadyToSend() {
		return ( m_bComplete && m_pQueue->GetLength() > 0 ) ? TRUE : FALSE;
	}
};

#endif