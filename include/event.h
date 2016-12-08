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
	/// <summary>abstract base class used to reference both static or method delegates.</summary>
	/// <typeparam name="...__args">function parameter list.</typeparam>
	template<typename ... _arg_types>
	class __delegate
	{
#pragma region constructors
	protected:
		__delegate() {}
	public:
		virtual ~__delegate() {}
#pragma endregion

#pragma region public methods
	public:
		/// <summary>invokes the function contained inside this delegate.</summary>
		/// <param name="...__args">function parameter list.</param>
		virtual void invoke(_arg_types... __args) = 0;
		/// <summary>compares this delegate to another.</summary>
		/// <param name="other">the other delegate to be compared against.</param>
		/// <returns>true if the delegate is equal to <paramref name="other"/>; otherwise false.</returns>
		virtual bool operator==(const __delegate<_arg_types...> &other) = 0;
		/// <summary>calls std::__delegate::invoke() on each delegate that has subscribed and exists in the subscriber list</summary>
		/// <param name="...__args"></param>
		virtual void operator()(_arg_types... __args) = 0;
#pragma endregion
	};
	/// <summary>a helper type for handling a shared pointer to a std::__delegate.</summary>
	template<typename ... _arg_types>
	using delegate_ptr = std::shared_ptr<__delegate<_arg_types...>>;



	/// <summary>std::__delegate<> implementation that encapsulates static/global function pointers.</summary>
	/// <typeparam name="...__args">function parameter list.</typeparam>
	template<typename ... _arg_types>
	class static_delegate final : public __delegate<_arg_types...>
	{
#pragma region instance variables
	private:
		/// <summary>the wrapped static/global function pointer.</summary>
		void(*_func)(_arg_types...);
#pragma endregion

#pragma region constructors
	public:
		/// <summary>default constructor.</summary>
		/// <param name="func">function pointer to be managed by this <seealso cref="static_delegate"/></param>
		static_delegate(void(*func)(_arg_types...)) : _func(func) {}
#pragma endregion

#pragma region public methods
	public:
		/// <summary>invokes the static/global function contained inside this delegate</summary>
		/// <param name="...__args">list of arguments to be sent to the underlying function.</param>
		inline virtual void invoke(_arg_types... __args) { _func(__args...); }
		/// <summary>compares this std::__delegate to another using the std::static_delegate comparison</summary>
		/// <param name="other">the other delegate to be compared against.</param>
		/// <returns>true if the delegate is equal to <paramref name="other"/>; otherwise false.</returns>
		virtual bool operator==(const __delegate<_arg_types...> &other) {
			if (const static_delegate* pother = dynamic_cast<const static_delegate*>(&other)) {
				return operator==(*pother);
			}
			return false;
		}
		/// <summary>compares this std::__delegate to another based on the address of the wrapped static/global function</summary>
		/// <param name="other">the other delegate to be compared against.</param>
		/// <returns>true if the delegate is equal to <paramref name="other"/>; otherwise false.</returns>
		inline virtual bool operator==(const static_delegate &other) { return _func == other._func; }
		/// <summary>calls std::__delegate::invoke() on each delegate that has subscribed and exists in the subscriber list</summary>
		/// <param name="...__args"></param>
		inline virtual void operator()(_arg_types... __args) { invoke(__args...); }
#pragma endregion
	};



	/// <summary>std::__delegate implementation that encapsulates object instance methods</summary>
	/// <typeparam name="_type">class type that hosts the method.</typeparam>
	/// <typeparam name="...__args">function parameter list.</typeparam>
	template<class _type, typename ... _arg_types>
	class method_delegate final : public __delegate<_arg_types...>
	{
#pragma region instance variables
	private:
		/// <summary>a pointer to the instance the method is bound to</summary>
		_type* _instance;
		/// <summary>the wrapped method pointer.</summary>
		void(_type::*_method)(_arg_types...);
#pragma endregion

#pragma region constructors
	public:
		method_delegate(_type * instance, void(_type::*method)(_arg_types...))
			: _instance(instance), _method(method) {}
#pragma endregion

#pragma region public methods
	public:
		/// <summary>invokes the class method contained within this delegate</summary>
		/// <param name="...__args">parameters to be passed to the underlying function calls.</param>
		inline virtual void invoke(_arg_types... __args) { (*_instance.*_method)(__args...); }
		/// <summary>compares this std::__delegate to another using the std::method_delegate comparison</summary>
		/// <param name="other">the other delegate to be compared against.</param>
		/// <returns>true if the delegate is equal to <paramref name="other"/>; otherwise false.</returns>
		inline virtual bool operator==(const __delegate<_arg_types...> &other) {
			if (const method_delegate* pother = dynamic_cast<const method_delegate*>(&other)) {
				return operator==(*pother);
			}
			return false;
		}
		/// <summary>compares this std::__delegate to another based on the addresses of the wrapped instance, and class method pointers</summary>
		/// <param name="other">the other delegate to be compared against.</param>
		/// <returns>true if the delegate is equal to <paramref name="other"/>; otherwise false.</returns>
		inline virtual bool operator==(const method_delegate &other) {
			return _instance == other._instance && _method == other._method;
		}
		/// <summary>calls std::__delegate::invoke() on each delegate that has subscribed and exists in the subscriber list</summary>
		/// <param name="...__args"></param>
		inline virtual void operator()(_arg_types... __args) { invoke(__args...); }
#pragma endregion
	};



	/// <summary>
	/// <para>a specialized std::__delegate that contains a list of "subscribed" delegates</para>
	/// <para>which will all be invoked in the same order in which they subscribed when</para>
	/// <para>std::__delegate::invoke() is called.</para>
	/// </summary>
	/// <example>
	/// <code>
	/// void func(int a) {
	///		cout &lt;&lt; "a = " &lt;&lt; a &lt;&lt; endl;
	/// }
	/// 
	/// event&lt;int&gt; myEvent;
	/// myEvent += new static_delegate&lt;int&gt(func);
	/// 
	/// if(myEvent != nullptr) {
	///		myEvent(42);
	/// }
	/// </code>
	/// </example>
	template<typename ... _arg_types>
	class event final : public __delegate<_arg_types...> {
#pragma region instance variables
	private:
		/// <summary>list of subscribers</summary>
		vector<delegate_ptr<_arg_types...>> _subscribers;
#pragma endregion

#pragma region constructors
	public:
		event() {}
		/// <summary>can take a null pointer as an initializer for verbosity.</summary>
		/// <param name="nptr">null pointer.</param>
		event(const nullptr_t&nptr) {}
#pragma endregion

#pragma region overrides
	public:
		/// <summary>calls std::__delegate::invoke() on each delegate that has subscribed and exists in the subscriber list</summary>
		/// <param name="...__args"></param>
		virtual void invoke(_arg_types... __args) {
			size_t size = _subscribers.size();
			if (size == 0) throw std::runtime_error("std::event<> is null");
			for (size_t c = 0; c < size; c++) {
				_subscribers[c]->invoke(__args...);
			}
		}
		/// <summary>will throw a std::logic_error of "not implemented".</summary>
		inline virtual bool operator==(const __delegate<_arg_types...> &other) { throw logic_error("not implemented"); }
#pragma endregion

#pragma region operator overloads
	public:
		/// <summary>can be assigned the value of null which will clear all subscribers.</summary>
		/// <param name="nptr">null pointer.</param>
		inline void operator=(const std::nullptr_t &nptr) { _subscribers.clear(); }
		/// <summary>
		/// <para>will add the provided delegate pointer to the list of subscribers</para>
		/// <para>throws std::invalid_argument if sub is null.</para>
		/// </summary>
		/// <param name="sub">subscribing delegate to be added to list.</param>
		inline void operator+=(const delegate_ptr<_arg_types...> &sub) {
			if (sub != nullptr)
				_subscribers.push_back(sub);
			else
				throw std::invalid_argument("const delegate_ptr<_arg_types...> &sub cannot be null");
		}
		/// <summary>
		/// <para>will add the provided delegate pointer to the list of subscribers</para>
		/// <para>throws std::invalid_argument if sub is null</para>
		/// </summary>
		/// <param name="sub">subscribing delegate to be added to list.</param>
		inline void operator+=(__delegate<_arg_types...>* sub) {
			if (sub != nullptr)
				_subscribers.push_back(delegate_ptr<_arg_types...>(sub));
			else
				throw std::invalid_argument("__delegate<_arg_types...>* sub cannot be null");
		}
		/// <summary>
		/// <para>finds and removes all entries of the provided delegate pointer in the list of subscribers</para>
		/// <para>throws std::invalid_argument if sub is null.</para>
		/// </summary>
		/// <param name="sub">unsubscribing delegate to be removed from the list.</param>
		inline void operator-=(__delegate<_arg_types...>* sub) { operator-=(delegate_ptr<_arg_types...>(sub)); }
		/// <summary>
		/// <para>finds and removes all entries of the provided delegate pointer in the list of subscribers</para>
		/// <para>throws std::invalid_argument if sub is null.</para>
		/// </summary>
		/// <param name="sub">unsubscribing delegate to be removed from the list.</param>
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
		/// <summary>checking equality to null will return true if there are no subscriber; otherwise false</summary>
		/// <param name="nptr">null pointer.</param>
		/// <returns>true if there are no subscriber; otherwise false</returns>
		inline bool operator==(const std::nullptr_t&nptr) { return _subscribers.size() == 0; }
		/// <summary>checking non-equality to null will return true if there are subscriber; otherwise false</summary>
		/// <param name="nptr">null pointer.</param>
		/// <returns>true if there are subscriber; otherwise false</returns>
		inline bool operator!=(const std::nullptr_t&nptr) { return _subscribers.size() != 0; }
		/// <summary>calls std::__delegate::invoke() on each delegate that has subscribed and exists in the subscriber list</summary>
		/// <param name="...__args"></param>
		inline virtual void operator()(_arg_types... __args) { invoke(__args...); }
#pragma endregion
	};
}
#endif
