#ifndef _CircuitLock_Headfile_
#define _CircuitLock_Headfile_

/////////////////////////////////////////////////////////////////////////////////
#pragma once
#include "Utility.h"
#include <pthread.h>

/////////////////////////////////////////////////////////////////////////////////
// 循环锁存器

class  CCircuitLock
{
	// 约束
	friend class CCircuitCond;
	
	// 变量
protected:
	/////////////////////////////////////////////////////////////////////////////////
	// pthread struct used in system calls
	pthread_mutex_t mutex_;

public:
	// 构造函数
	CCircuitLock();
	// 析构函数
	virtual ~CCircuitLock();

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

class CCircuitCond
{
	// 参数变量
private:
	// 约束变量
	pthread_cond_t cond_;

public:
	// 构造函数
	inline CCircuitCond() {
		pthread_cond_init(&cond_, NULL);
	}
	// 析构函数
	inline ~CCircuitCond() {
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
	inline void Wait(CCircuitLock* lock) {
		pthread_cond_wait(&cond_, &lock->mutex_);
	}
	// 等待时间
	inline bool Wait(CCircuitLock* lock, int seconds)
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
class  CCircuitGuard
{
	// 参数变量
protected:
	// 约束变量
	CCircuitLock& lock_;

	// 基本函数
public:
	// 构造函数
	CCircuitGuard(CCircuitLock& lock) : lock_(lock) {
		lock_.Lock();
	}
	// 析构函数
	~TGuard() {
		lock_.Unlock();
	}
};

/////////////////////////////////////////////////////////////////////////////////
#endif // _CircuitLock_Headfile_
