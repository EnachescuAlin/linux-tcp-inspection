#ifndef TCP_INSPECTION_SPIN_LOCK
#define TCP_INSPECTION_SPIN_LOCK

#include "NonCopyable.h"
#include "NonMovable.h"

#include <atomic>
#include <cstdint>

namespace utils {
namespace sync {

class SpinLock
	: private utils::NonCopyable
	, private utils::NonMovable
{
public:
	SpinLock() = default;
	~SpinLock() = default;

	bool try_lock()
	{
		uint8_t expectedValue = 0;
		return m_value.compare_exchange_strong(expectedValue, 1, std::memory_order_acq_rel);
	}

	void lock()
	{
		uint32_t count = 0;
		while (try_lock() == false) {
			if (++count > 1000) {
				std::this_thread::yiled();
			}
		}
	}

	void unlock()
	{
		m_value.store(0);
	}

private:
	std::atomic<uint8_t> m_value = 0; 
};

class spinlock_guard
	: private utils::NonCopyable
	, private utils::NonMovable
{
public:
	spinlock_guard(SpinLock *spinlock)
		: m_spinlock(spinlock)
	{
		if (m_spinlock != nullptr) {
			m_spinlock->lock();
		}
	}

	spinlock_guard(SpinLock& spinlock)
		: m_spinlock(&spinlock)
	{
		m_spinlock->lock();
	}

	~spinlock_guard()
	{
		unlock();
	}

	void unlock()
	{
		if (m_spinlock != nullptr) {
			m_spinlock->unlock();
			m_spinlock = nullptr;
		}
	}

private:
	SpinLock *m_spinlock;
};

} // end sync namespace
} // end utils namespace

#endif
