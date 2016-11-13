#ifndef _EVENT_H
#define _EVENT_H 1

#ifndef _GLIBCXX_VECTOR
#ifndef _VECTOR_
#include <vector>
#endif
#endif
#ifndef _GLIBCXX_MEMORY
#ifndef _MEMORY_
#include <memory>
#endif
#endif
#ifndef _GLIBCXX_ALGORITHM
#ifndef _ALGORITHM_
#include <algorithm>
#endif
#endif

namespace std
{
	template<typename ... _arg_types>
	class __delegate {
#pragma region constructors
	protected:
		__delegate() {}
	public:
		virtual ~__delegate() {}
#pragma endregion


#pragma region public methods
	public:
		virtual void invoke(_arg_types... __args) = 0;
		virtual bool operator==(const __delegate &other) = 0;
#pragma endregion
	};

	template<typename ... _arg_types>
	class static_delegate final : public __delegate<_arg_types...> {
#pragma region instance variables
	private:
		void(*_method)(_arg_types...);
#pragma endregion


#pragma region constructors
	public:
		static_delegate(void(*method)(_arg_types...)) : _method(method) {}
#pragma endregion


#pragma region public methods
	public:
		virtual void invoke(_arg_types... __args) { _method(__args...); }
		virtual bool operator==(const __delegate &other) {
			if (const static_delegate* pother = dynamic_cast<const static_delegate*>(&other)) {
				return operator==(*pother);
			}
			return false;
		}
		virtual bool operator==(const static_delegate &other) { return _method == other._method; }
#pragma endregion
	};

	template<class _type, typename ... _arg_types>
	class method_delegate final : public __delegate<_arg_types...> {
#pragma region instance variables
	private:
		_type* _instance;
		void(_type::*_method)(_arg_types...);
#pragma endregion


#pragma region constructors
	public:
		method_delegate(_type * instance, void(_type::*method)(_arg_types...))
			: _instance(instance), _method(method) {}
#pragma endregion


#pragma region public methods
	public:
		virtual void invoke(_arg_types... __args) {
			(*_instance.*_method)(__args...);
		}
		virtual bool operator==(const __delegate &other) {
			if (const method_delegate* pother = dynamic_cast<const method_delegate*>(&other)) {
				return operator==(*pother);
			}
			return false;
		}
		virtual bool operator==(const method_delegate &other) {
			return _instance == other._instance && _method == other._method;
		}
#pragma endregion
	};

	template<typename ... _arg_types>
	using delegate_ptr = std::shared_ptr<__delegate<_arg_types...>>;

	template<typename ... _arg_types>
	class event final : public __delegate<_arg_types...> {
#pragma region instance variables
	private:
		vector<delegate_ptr<_arg_types...>> _subscribers;
#pragma endregion


#pragma region constructors
	public:
		event() {}
		event(const nullptr_t&) {}
#pragma endregion


#pragma region overrides
	public:
		virtual void invoke(_arg_types... __args) {
			size_t size = _subscribers.size();
			if (size == 0) throw std::runtime_error("std::event<> is null");
			for (size_t c = 0; c < size; c++) {
				_subscribers[c]->invoke(__args...);
			}
		}
		virtual bool operator==(const __delegate &other) { throw logic_error("not implemented"); }
#pragma endregion


#pragma region operator overloads
	public:
		inline void operator=(const std::nullptr_t &) { _subscribers.clear(); }
		inline void operator+=(const delegate_ptr<_arg_types...> &sub) { _subscribers.push_back(sub); }
		inline void operator+=(__delegate<_arg_types...>* sub) { _subscribers.push_back(delegate_ptr<_arg_types...>(sub)); }
		inline void operator-=(__delegate<_arg_types...>* sub) { operator-=(delegate_ptr<_arg_types...>(sub)); }
		void operator-=(const delegate_ptr<_arg_types...> &sub) {
			const auto end = _subscribers.end();
			_subscribers.erase(
				std::remove_if(_subscribers.begin(), end,
					[&sub](const delegate_ptr<_arg_types...> &l)-> bool { return *l == *sub; }),
				end);
		}
		inline bool operator==(const std::nullptr_t&) { return _subscribers.size() == 0; }
		inline bool operator!=(const std::nullptr_t&) { return _subscribers.size() != 0; }
#pragma endregion
	};
} // namespace std
#endif
