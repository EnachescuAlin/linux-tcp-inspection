#ifndef TCP_INSPECTION_NON_COPYABLE
#define TCP_INSPECTION_NON_COPYABLE

namespace utils {

class NonCopyable
{
public:
	NonCopyable(const NonCopyable&) = delete;
	NonCopyable& operator=(const NonCopyable&) = delete;

	NonCopyable() = default;
	~NonCopyable() = default;
};

} // end utils namespace

#endif
