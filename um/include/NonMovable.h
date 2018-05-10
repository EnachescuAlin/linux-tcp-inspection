#ifndef TCP_INSPECTION_NON_MOVABLE
#define TCP_INSPECTION_NON_MOVABLE

namespace utils {

class NonMovable
{
public:
	NonMovable(NonMovable&&) = delete;
	NonMovable& operator=(NonMovable&&) = delete;

	NonMovable() = default;
	~NonMovable() = default;
};

} // end utils namespace

#endif
