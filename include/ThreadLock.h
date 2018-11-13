#ifndef _TreadLock_Headfile_
#define _TreadLock_Headfile_

/////////////////////////////////////////////////////////////////////////////////
#pragma once
#include "Utility.h"
#include <pthread.h>

/////////////////////////////////////////////////////////////////////////////////
// 循环锁存器

class  CThreadLock
{
	// 约束
	friend class CThreadCond;
	
	// 变量
protected:
	/////////////////////////////////////////////////////////////////////////////////
	// pthread struct used in system calls
	pthread_mutex_t mutex_;

public:
	// 构造函数
	CThreadLock();
	// 析构函数
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
// 条件约束

class CThreadCond
{
	// 参数变量
private:
	// 约束变量
	pthread_cond_t cond_;

public:
	// 构造函数
	inline CThreadCond() {
		pthread_cond_init(&cond_, NULL);
	}
	// 析构函数
	inline ~CThreadCond() {
		pthread_cond_destroy(&cond_);
	}
	// 发送信息
	inline void Signal() {
		pthread_cond_signal(&cond_);
	}
	// 广播信息
	inline void Broadcast() {
		pthread_cond_broadcast(&cond_);
	}
	// 等待解锁
	inline void Wait(CThreadLock* lock) {
		pthread_cond_wait(&cond_, &lock->mutex_);
	}
	// 等待时间
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
// 锁存守护

// 锁存守护
class  CThreadGuard
{
	// 参数变量
protected:
	// 约束变量
	CThreadLock& lock_;

	// 基本函数
public:
	// 构造函数
	CThreadGuard(CThreadLock& lock) : lock_(lock) {
		lock_.Lock();
	}
	// 析构函数
	~CThreadGuard() {
		lock_.Unlock();
	}
};

/////////////////////////////////////////////////////////////////////////////////
#endif // _ThreadLock_Headfile_
