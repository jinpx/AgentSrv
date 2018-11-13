#ifndef _TreadLock_Headfile_
#define _TreadLock_Headfile_

/////////////////////////////////////////////////////////////////////////////////
#pragma once
#include "Utility.h"
#include <pthread.h>

/////////////////////////////////////////////////////////////////////////////////
// ѭ��������

class  CThreadLock
{
	// Լ��
	friend class CThreadCond;
	
	// ����
protected:
	/////////////////////////////////////////////////////////////////////////////////
	// pthread struct used in system calls
	pthread_mutex_t mutex_;

public:
	// ���캯��
	CThreadLock();
	// ��������
	virtual ~CThreadLock();

	/////////////////////////////////////////////////////////////////////////////////
	// Acquires this mutex. If it cannot be acquired immediately, it will block.
	inline void Lock() {
		pthread_mutex_lock(&mutex_);
	}

	/////////////////////////////////////////////////////////////////////////////////
	// Releases this mutex. No error checking performed
	inline void Unlock() {
		pthread_mutex_unlock(&mutex_);
	}

	/////////////////////////////////////////////////////////////////////////////////
	// Attempts to acquire this mutex. If it cannot be acquired (held by another thread)
	inline bool TryLock() {
		return (pthread_mutex_trylock(&mutex_) == 0);
	}
};


/////////////////////////////////////////////////////////////////////////////////
// ����Լ��

class CThreadCond
{
	// ��������
private:
	// Լ������
	pthread_cond_t cond_;

public:
	// ���캯��
	inline CThreadCond() {
		pthread_cond_init(&cond_, NULL);
	}
	// ��������
	inline ~CThreadCond() {
		pthread_cond_destroy(&cond_);
	}
	// ������Ϣ
	inline void Signal() {
		pthread_cond_signal(&cond_);
	}
	// �㲥��Ϣ
	inline void Broadcast() {
		pthread_cond_broadcast(&cond_);
	}
	// �ȴ�����
	inline void Wait(CThreadLock* lock) {
		pthread_cond_wait(&cond_, &lock->mutex_);
	}
	// �ȴ�ʱ��
	inline bool Wait(CThreadLock* lock, int seconds)
	{
		timespec tv;
		tv.tv_nsec = 0;
		tv.tv_sec = seconds;
		if(pthread_cond_timedwait(&cond_, &lock->mutex_, &tv) == 0) {
			return true;
		}
		else {
			return false;
		}
	}
};


/////////////////////////////////////////////////////////////////////////////////
// �����ػ�

// �����ػ�
class  CThreadGuard
{
	// ��������
protected:
	// Լ������
	CThreadLock& lock_;

	// ��������
public:
	// ���캯��
	CThreadGuard(CThreadLock& lock) : lock_(lock) {
		lock_.Lock();
	}
	// ��������
	~CThreadGuard() {
		lock_.Unlock();
	}
};

/////////////////////////////////////////////////////////////////////////////////
#endif // _ThreadLock_Headfile_