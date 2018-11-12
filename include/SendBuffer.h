#ifndef _SendBuffer_Headfile_
#define _SendBuffer_Headfile_

#include "CircuitQueue.h"
/////////////////////////////////////////////////////////////////////////////////

//发送缓存
class SendBuffer
{
	//变量数据
private:
	BOOL						m_bComplete;						// 参数队列
	CircuitQueue<BYTE>			*m_pQueue;							// 参数队列

public:
	// 构造函数
	SendBuffer() { 
		m_pQueue = NULL;
	}
	// 析构函数
	virtual ~SendBuffer() {
		if( m_pQueue ) {
			delete m_pQueue; 
		}
	}
	// 创建数据块
	inline void Create( int nBufferSize, DWORD dwExtraBuffeSize ) {
		if( m_pQueue ) {
			delete m_pQueue;
		}
		m_pQueue = new CircuitQueue<BYTE>;
		m_pQueue->Create( nBufferSize, dwExtraBuffeSize );
		m_bComplete = TRUE;
	}
	//清理数据块
	inline void Clear() { 
		m_pQueue->Clear();
		m_bComplete = TRUE; 
	}
	// 数据完成
	inline void Completion( int nBytesSend ) {
		m_pQueue->Dequeue( NULL, nBytesSend ); 
		m_bComplete = TRUE;
	}
	// 获取指针
	inline BOOL	GetSendParam( BYTE **ppSendPtr, int &nLength ) 
	{
		// 发送判断
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
	// 写入数据
	inline BOOL Write( tagPACKET_HEADER *pHeader, BYTE *pMsg )	{
		if( !m_pQueue->Enqueue( (BYTE*)pHeader, sizeof(PACKET_HEADER)) ) {
			return FALSE;
		}
		if( !m_pQueue->Enqueue( pMsg, pHeader->size ) )	{
			return FALSE;
		}
		return TRUE; 
	}
	// 写入数据
	inline BOOL Write( BYTE *pMsg, WORD wSize ) {
		if( !m_pQueue->Enqueue( pMsg, wSize ) ) {
			return FALSE;
		}
		return TRUE; 
	}
	// 取得长度
	inline DWORD GetLength() { 
		return m_pQueue->GetLength(); 
	}
	// 可读发送
	inline BOOL IsReadyToSend() {
		return ( m_bComplete && m_pQueue->GetLength() > 0 ) ? TRUE : FALSE;
	}
};

#endif