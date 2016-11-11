#ifndef _BOOL_SET_H
#define _BOOL_SET_H
#include <cstddef>
#include <cstdint>

namespace std
{
	template<size_t _L = 1>
	class bool_set {
	private:
		uint8_t _bits[_L];
	public:
		inline size_t size() const { return _L * 8u; }
		inline bool operator[](const size_t &index) const { return get(index); }
		inline bool get(const size_t &index) const { return (_bits[index / 8u] & (1 << (index % 8u))) != 0; }
		void set(const size_t &index, const bool &value) {
			if (value)
				_bits[index / 8u] |= 1u << (index % 8u);
			else
				_bits[index / 8u] &= 0xFFu - (1u << (index % 8u));
		}
	};
}

#endif
