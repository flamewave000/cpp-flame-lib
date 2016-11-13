#ifndef _EVENT_H
#define _EVENT_H 1

#ifndef _GLIBCXX_VECTOR
#include <vector>
#endif
#ifndef _GLIBCXX_FUNCTIONAL
#include <functional>
#endif
#ifndef _GLIBCXX_MEMORY
#include <memory>
#endif
#ifndef _GLIBCXX_MUTEX
#include <mutex>
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
				operator==(*pother);
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
	class event : public __delegate<_arg_types...> {
#pragma region instance variables
	private:
		vector<shared_ptr<__delegate<_arg_types...>>> _subscribers;
#pragma endregion


#pragma region overrides
	public:
		virtual void invoke(_arg_types... __args) {
			for (size_t c = 0, size = _subscribers.size(); c < size; c++) {
				_subscribers[c]->invoke(__args...);
			}
		}
		virtual bool operator==(const __delegate &other) throw() { throw logic_error("not implemented"); }
#pragma endregion


	public:
		void operator+=(const shared_ptr<__delegate> &sub) {
			_subscribers.push_back(sub);
		}
		void operator-=(const shared_ptr<__delegate> &sub) {
			for (int c = 0, size = _subscribers.size(); c < size; c++) {
				auto ind = _subscribers.begin() + c;
				if (**ind == *sub) {
					_subscribers.erase(ind);
				}
			}
		}
	};
} // namespace std
#endif
