#include "Utility.h"
#include "CircuitLock.h"

/////////////////////////////////////////////////////////////////////////////////
/* Linux mutex implementation */
static bool attr_initalized = false;
static pthread_mutexattr_t attr;

/////////////////////////////////////////////////////////////////////////////////
// ���캯��
CCircuitLock::CCircuitLock()
{
	if(!attr_initalized)
	{
		pthread_mutexattr_init(&attr);
		pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE_NP);
		attr_initalized = true;
	}

	pthread_mutex_init(&mutex_, &attr);
}

/////////////////////////////////////////////////////////////////////////////////
// ��������
CCircuitLock::~CCircuitLock() 
{ 
	pthread_mutex_destroy(&mutex_); 
}

/////////////////////////////////////////////////////////////////////////////////
// ��ʱ��
DWORD  GetTickCount()
{	
	static long long t0 = 0;
	struct timeval tv;
	
	gettimeofday(&tv, NULL);
	long long t = (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
	
	if (t0 == 0) {
		t0 = t;
	}
	
	return t - t0;
}

/////////////////////////////////////////////////////////////////////////////////
// ��ʱ��
void ZeroMemory(void* src, size_t size)
{
	memset(src, 0, size);
}	
