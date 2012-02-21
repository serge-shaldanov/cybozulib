#pragma once
/**
	@file
	@brief critical section

	Copyright (C) 2007-2012 Cybozu Labs, Inc., all rights reserved.
	@author MITSUNARI Shigeo
*/
#include <cybozu/mutex.hpp>

namespace cybozu {

class ConditionVariableCs;

namespace thread {

#ifdef _WIN32
typedef CRITICAL_SECTION CsHandle;
inline void CsInit(CsHandle& cs) { InitializeCriticalSection(&cs); }
inline void CsLock(CsHandle& cs) { EnterCriticalSection(&cs); }
inline void CsUnlock(CsHandle& cs) { LeaveCriticalSection(&cs); }
inline void CsTerm(CsHandle& cs) { DeleteCriticalSection(&cs); }
#else
typedef pthread_mutex_t CsHandle;
inline void CsInit(CsHandle& cs) { pthread_mutex_init(&cs, NULL); }
inline void CsLock(CsHandle& cs) { pthread_mutex_lock(&cs); }
inline void CsUnlock(CsHandle& cs) { pthread_mutex_unlock(&cs); }
inline void CsTerm(CsHandle& cs) { pthread_mutex_destroy(&cs); }
#endif

} // cybozu::thread

class CriticalSection {
	friend class cybozu::ConditionVariableCs;
public:
	CriticalSection()
	{
		thread::CsInit(hdl_);
	}
	~CriticalSection()
	{
		thread::CsTerm(hdl_);
	}
	inline void lock()
	{
		thread::CsLock(hdl_);
	}
	inline void unlock()
	{
		thread::CsUnlock(hdl_);
	}
private:
	CriticalSection(const CriticalSection&);
	CriticalSection& operator=(const CriticalSection&);
	thread::CsHandle hdl_;
};

typedef cybozu::thread::AutoLockT<cybozu::CriticalSection> AutoLockCs; //!< auto lock critical section

} // cybozu
