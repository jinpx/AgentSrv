#ifndef _CircuitQueue_Headfile_
#define _CircuitQueue_Headfile_

#include <stdio.h>
#include <assert.h>
#include <CircuitLock.h>

/////////////////////////////////////////////////////////////////////////////////
// ѭ������

template<typename T>
class CircuitQueue
{
	//���������
protected:
	CCircuitLock		m_cs;				
	T					*m_pData;									// ѭ����
	int					m_nLength;									// ���ݳ���
	int					m_nSize;									// ���ݴ�С
	int					m_nHead;									// ͷָ��
	int					m_nTail;									// βָ�� 
	int					m_nExtraSize;								// ������

	// ��������
public:
	// ���캯��
	CircuitQueue()
		: m_pData(NULL)
		, m_nLength(0)
		, m_nSize(0)
		, m_nHead(0)
		, m_nTail(0)
	{ }

	// ��������
	virtual ~CircuitQueue() {
		if (m_pData) {
			delete[] m_pData;
		}
	}

	// ���º���
public:
	// ��������
	void Create( int nSize, int nExtraSize = 0)
	{
		m_cs.Lock();
		if( m_pData ) {
			delete [] m_pData;
		}

		m_pData			= new T[nSize + nExtraSize];
		m_nSize			= nSize;
		m_nExtraSize	= nExtraSize;
		m_cs.Unlock();
	}
	// ��������
	inline void Clear()
	{
		m_cs.Lock();

		m_nLength       = 0;
		m_nHead         = 0;
		m_nTail         = 0;
		m_cs.Unlock();
	}
	// ȡ�ÿռ��С
	inline int GetSpace()
	{
		int iRet;

		m_cs.Lock();
		iRet = m_nSize - m_nLength;		
		m_cs.Unlock();
		return iRet;
	}
	// ���ݳ���
	inline int GetLength()
	{
		int iRet;

		m_cs.Lock();
		iRet = m_nLength;
		m_cs.Unlock();
		return iRet;
	}
	// ȡ�ÿ����ݳ���
	inline int GetBackDataCount()
	{
		int iRet;

		m_cs.Lock();
		iRet = m_nSize - m_nHead;
		m_cs.Unlock();
		return iRet;
	}
	// ��ȡָ��
	inline T* GetReadPtr()
	{
		T *pRet;

		m_cs.Lock();
		pRet = m_pData + m_nHead;

		int nSplitFirstDataCount;
		if( m_nHead > m_nTail && ( nSplitFirstDataCount = m_nSize - m_nHead ) < m_nExtraSize ) {
			memcpy( m_pData + m_nSize, m_pData, sizeof(T) * ( m_nExtraSize - nSplitFirstDataCount ) );
		}

		m_cs.Unlock();
		return pRet;
	}
	// д����
	inline T* GetWritePtr()
	{
		T *pRet;

		m_cs.Lock();
		pRet = m_pData + m_nTail;
		m_cs.Unlock();
		return pRet;
	}
	// �ɶ�ȡ����
	inline int GetReadableLen()
	{
		int iRet;

		m_cs.Lock();
		if( m_nHead == m_nTail ) {
			iRet = GetLength() > 0 ? m_nSize - m_nHead: 0;
		}
		else if( m_nHead < m_nTail ) {
			iRet = m_nTail - m_nHead;
		}
		else {
			iRet = m_nSize - m_nHead;
		}
		m_cs.Unlock();
		return iRet;
	}
	// ��д�볤��
	inline int GetWritableLen()
	{
		int iRet;

		m_cs.Lock();
		if( m_nHead == m_nTail ) {
			iRet = GetLength() > 0 ? 0 : m_nSize - m_nTail;
		}
		else if( m_nHead < m_nTail ) {
			iRet = m_nSize - m_nTail;
		}
		else {
			iRet = m_nHead - m_nTail;
		}
		m_cs.Unlock();
		return iRet;
	}
	// �������
	inline BOOL Enqueue( T *pSrc, int nSize )
	{
		// �жϴ�С
		m_cs.Lock();
		if( GetSpace() < nSize ) {
			m_cs.Unlock();
			return FALSE;
		}

		// ��ȡ����
		if( pSrc )
		{
			if( m_nHead <= m_nTail )
			{
				int nBackSpaceCount = m_nSize - m_nTail;
				if( nBackSpaceCount >= nSize ) {
					memcpy( m_pData + m_nTail, pSrc, sizeof(T) * nSize );
				}
				else {
					memcpy( m_pData + m_nTail, pSrc, sizeof(T) * nBackSpaceCount );
					memcpy( m_pData, pSrc + nBackSpaceCount, sizeof(T) * ( nSize - nBackSpaceCount ) );
				}
			}
			else {
				memcpy( m_pData + m_nTail, pSrc, sizeof(T) * nSize );
			}
		}

		m_nTail		+= nSize;
		m_nTail		%= m_nSize;
		m_nLength	+= nSize;
		m_cs.Unlock();
		return TRUE;
	}
	// ѹ�����
	inline BOOL Dequeue( T *pTar, int nSize )
	{    
		m_cs.Lock();

		if( !Peek( pTar, nSize ) ) {
			m_cs.Unlock();
			return FALSE;
		}

		m_nHead		+= nSize;
		m_nHead		%= m_nSize;
		m_nLength	-= nSize;
		m_cs.Unlock();
		return TRUE;
	}
	// ѹ������㷨
	inline BOOL Peek( T *pTar, int nSize )
	{
		m_cs.Lock();
		if( m_nLength < nSize ) {
			m_cs.Unlock();
			return FALSE;
		}

		//  ѹ������
		if( pTar!=NULL )
		{
			if( m_nHead<m_nTail ) {
				memcpy( pTar, m_pData + m_nHead, sizeof(T) * nSize );
			}
			else {
				if( GetBackDataCount() >= nSize ) {
					memcpy( pTar, m_pData + m_nHead, sizeof(T) * nSize );                           
				}
				else {
					memcpy( pTar, m_pData + m_nHead, sizeof(T) * GetBackDataCount() );
					memcpy( pTar + GetBackDataCount(), m_pData, sizeof(T)*(nSize-GetBackDataCount()));
				}
			}
		}

		m_cs.Unlock();
		return TRUE;
	}
	// ��������
	inline void CopyHeadDataToExtraBuffer( int nSize )
	{
		assert( nSize<=m_nExtraSize );
		m_cs.Lock();

		// ��������
		memcpy( m_pData+m_nSize, m_pData, nSize );
		m_cs.Unlock();
	}
};

#endif