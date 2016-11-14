#ifndef _EVENT_
#define _EVENT_ 1

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
	/*abstract base class used to reference both static or method delegates*/
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
		/*invokes the function contained inside this delegate*/
		virtual void invoke(_arg_types... __args) = 0;
		/*compares this delegate to another*/
		virtual bool operator==(const __delegate &other) = 0;
#pragma endregion
	};
	/*a helper type for handling a shared pointer to a std::__delegate*/
	template<typename ... _arg_types>
	using delegate_ptr = std::shared_ptr<__delegate<_arg_types...>>;



	/*std::__delegate<> implementation that encapsulates static/global function pointers*/
	template<typename ... _arg_types>
	class static_delegate final : public __delegate<_arg_types...> {
#pragma region instance variables
	private:
		/*the wrapped static/global function pointer*/
		void(*_func)(_arg_types...);
#pragma endregion

#pragma region constructors
	public:
		static_delegate(void(*func)(_arg_types...)) : _func(func) {}
#pragma endregion

#pragma region public methods
	public:
		/*invokes the static/global function contained inside this delegate*/
		virtual void invoke(_arg_types... __args) { _func(__args...); }
		/*compares this std::__delegate to another using the std::static_delegate comparison*/
		virtual bool operator==(const __delegate &other) {
			if (const static_delegate* pother = dynamic_cast<const static_delegate*>(&other)) {
				return operator==(*pother);
			}
			return false;
		}
		/*compares this std::__delegate to another based on the address of the wrapped static/global function*/
		virtual bool operator==(const static_delegate &other) { return _func == other._func; }
#pragma endregion
	};



	/*std::__delegate implementation that encapsulates object instance methods*/
	template<class _type, typename ... _arg_types>
	class method_delegate final : public __delegate<_arg_types...> {
#pragma region instance variables
	private:
		/*a pointer to the instance the method is bound to*/
		_type* _instance;
		/*the wrapped method pointer*/
		void(_type::*_method)(_arg_types...);
#pragma endregion

#pragma region constructors
	public:
		method_delegate(_type * instance, void(_type::*method)(_arg_types...))
			: _instance(instance), _method(method) {}
#pragma endregion

#pragma region public methods
	public:
		/*invokes the class method contained within this delegate*/
		virtual void invoke(_arg_types... __args) {
			(*_instance.*_method)(__args...);
		}
		/*compares this std::__delegate to another using the std::method_delegate comparison*/
		virtual bool operator==(const __delegate &other) {
			if (const method_delegate* pother = dynamic_cast<const method_delegate*>(&other)) {
				return operator==(*pother);
			}
			return false;
		}
		/*compares this std::__delegate to another based on the addresses of the wrapped instance, and class method pointers*/
		virtual bool operator==(const method_delegate &other) {
			return _instance == other._instance && _method == other._method;
		}
#pragma endregion
	};



	/*a specialized std::__delegate that contains a list of "subscribed" delegates
	which will all be invoked in the same order in which they subscribed when
	std::__delegate::invoke() is called*/
	template<typename ... _arg_types>
	class event final : public __delegate<_arg_types...> {
#pragma region instance variables
	private:
		/*list of subscribers*/
		vector<delegate_ptr<_arg_types...>> _subscribers;
#pragma endregion

#pragma region constructors
	public:
		event() {}
		/*can take a null pointer as an initializer for verbosity*/
		event(const nullptr_t&) {}
#pragma endregion

#pragma region overrides
	public:
		/*calls std::__delegate::invoke() on each delegate that has subscribed and exists in the subscriber list*/
		virtual void invoke(_arg_types... __args) {
			size_t size = _subscribers.size();
			if (size == 0) throw std::runtime_error("std::event<> is null");
			for (size_t c = 0; c < size; c++) {
				_subscribers[c]->invoke(__args...);
			}
		}
		/*will throw a std::logic_error of "not implemented"*/
		virtual bool operator==(const __delegate &other) { throw logic_error("not implemented"); }
#pragma endregion

#pragma region operator overloads
	public:
		/*can be assigned the value of null which will clear all subscribers*/
		inline void operator=(const std::nullptr_t &) { _subscribers.clear(); }
		/*will add the provided delegate pointer to the list of subscribers
		throws std::invalid_argument if sub is null*/
		inline void operator+=(const delegate_ptr<_arg_types...> &sub) {
			if (sub != nullptr)
				_subscribers.push_back(sub);
			else
				throw std::invalid_argument("const delegate_ptr<_arg_types...> &sub cannot be null");
		}
		/*will add the provided delegate pointer to the list of subscribers
		throws std::invalid_argument if sub is null*/
		inline void operator+=(__delegate<_arg_types...>* sub) {
			if (sub != nullptr)
				_subscribers.push_back(delegate_ptr<_arg_types...>(sub));
			else
				throw std::invalid_argument("__delegate<_arg_types...>* sub cannot be null");
		}
		/*finds and removes all entries of the provided delegate pointer in the list of subscribers
		throws std::invalid_argument if sub is null*/
		inline void operator-=(__delegate<_arg_types...>* sub) { operator-=(delegate_ptr<_arg_types...>(sub)); }
		/*finds and removes all entries of the provided delegate pointer in the list of subscribers
		throws std::invalid_argument if sub is null*/
		void operator-=(const delegate_ptr<_arg_types...> &sub) {
			if (sub == nullptr) {
				throw std::invalid_argument("__delegate<_arg_types...>* sub cannot be null");
				return;
			}
			const auto end = _subscribers.end();
			_subscribers.erase(
				std::remove_if(_subscribers.begin(), end,
					[&sub](const delegate_ptr<_arg_types...> &l)-> bool { return *l == *sub; }),
				end);
		}
		/*checking equality to null will return true if there are no subscriber; otherwise false*/
		inline bool operator==(const std::nullptr_t&) { return _subscribers.size() == 0; }
		/*checking non-equality to null will return true if there are subscriber; otherwise false*/
		inline bool operator!=(const std::nullptr_t&) { return _subscribers.size() != 0; }
#pragma endregion
	};
} // namespace std
#endif
