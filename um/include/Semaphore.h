#ifndef TCP_INSPECTION_SEMAPHORE
#define TCP_INSPECTION_SEMAPHORE

#include <mutex>
#include <condition_variable>

namespace utils {
namespace sync {

class Semaphore
{
public:
	Semaphore(unsigned long initialValue = 0)
		: m_count(initialValue)
	{
	}

	~Semaphore() = default;

	void notify()
	{
		std::unique_lock<std::mutex> _lock(m_mutex);

		++m_count;
		m_cv.notify_one();
	}

	void wait()
	{
		std::unique_lock<std::mutex> _lock(m_mutex);

		while(m_count == 0) {
			m_cv.wait(_lock);
		}

		--m_count;
	}

	void wait_n(unsigned long n)
	{
		for (unsigned long i = 0; i < n; i++) {
			wait();
		}
	}

	bool try_wait()
	{
		std::unique_lock<std::mutex> _lock(m_mutex);

		if (m_count > 0) {
			--m_count;
			return true;
		}

		return false;
	}

private:
	std::mutex m_mutex;
	std::condition_variable m_cv;
	unsigned long m_count = 0;
};

} // end sync namespace
} // end utils namespace

#endif
