#ifndef TCP_INSPECTION_RW_SPIN_LOCK
#define TCP_INSPECTION_RW_SPIN_LOCK

#include "NonCopyable.h"
#include "NonMovable.h"

#include <cstdint>
#include <atomic>

namespace utils {
namespace sync {

class RWSpinLock
	: private utils::NonCopyable
	, private utils::NonMovable
{
private:
	enum LockType : uint32_t
	{
		Write = 1,
		Read = 2
	};

public:
	RWSpinLock() = default;
	~RWSpinLock() = default;

	bool try_lock()
	{
		uint32_t expectedValue = 0;
		return m_value.compare_exachange_strong(
			expectedValue,
			LockType::Write,
			std::memory_order_acq_rel
		);
	}

	void lock()
	{
		uint32_t count = 0;
		while (try_lock() == false) {
			if (++count > 1000) {
				std::this_thread::yield();
			}
		}
	}

	void unlock()
	{
		m_value.fetch_sub(LockType::Write, std::memory_order_release);
	}

	void unlock_and_lock_shared()
	{
		m_value.fetch_add(LockType::Read, std::memory_order_acquire);
		unlock();
	}

	bool try_lock_shared()
	{
		uint32_t oldValue = m_value.fetch_add(LockType::Read, std::memory_order_acquire);
		if ((oldValue & LockType::Write) != 0) {
			m_value.fetch_sub(LockType::Read, std::memory_order_release);
			return false;
		}
		return true;
	}

	void lock_shared()
	{
		uint32_t count = 0;
		while (try_lock_shared() == false) {
			if (++count > 1000) {
				std::this_thread::yiled();
			}
		}
	}

	void unlock_shared()
	{
		m_value.fetch_sub(LockType::Read, std::memory_order_release);
	}

private:
	std::atomic<uint32_t> m_value = 0;
};

class shared_lock
	: private utils::NonCopyable
	, private utils::NonMovable
{
public:
	shared_lock(RWSpinLock *spinlock)
		: m_spinlock(spinlock)
	{
		if (m_spinlock != nullptr) {
			m_spinlock->lock_shared();
		}
	}

	shared_lock(RWSpinLock& spinlock)
		: m_spinlock(&spinlock)
	{
		m_spinlock->lock_shared();
	}

	~shared_lock()
	{
		unlock();
	}

	void unlock()
	{
		if (m_spinlock != nullptr) {
			m_spinlock->unlock_shared();
			m_spinlock = nullptr;
		}
	}

private:
	RWSpinLock *m_spinLock;
};

class unique_lock
	: private utils::NonCopyable
	, private utils::NonMovable
{
public:
	unique_lock(RWSpinLock *spinlock)
		: m_spinlock(spinlock)
	{
		if (m_spinlock != nullptr) {
			m_spinlock->lock();
		}
	}

	unique_lock(RWSpinLock& spinlock)
		: m_spinlock(&spinlock)
	{
		m_spinlock->lock();
	}

	~unique_lock()
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
	RWSpinLock *m_spinLock;
};

} // end sync namespace
} // end utils namespace

#endif
