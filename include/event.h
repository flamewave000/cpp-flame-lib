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
	template<typename ... _ArgTypes>
	class event
	{
	public:
		// Expected function pointer for this iteration of event
		typedef function<void(_ArgTypes...)> delegate;
		// Expected delegate pointer for this iteration of event
		typedef shared_ptr<delegate> delegate_ptr;
	private:
		vector<delegate_ptr> _listeners;
	public:
		// invokes the event
		virtual void operator()(void * owner, _ArgTypes... __args)
		{
			invoke(owner, __args...);
		}
		virtual void operator+=(delegate_ptr listener) {
			_listeners.push_back(listener);
		}
		virtual void operator-=(delegate_ptr listener) {
			for (auto it = _listeners.begin(), end = _listeners.end(); it != end; it++) {
				if (*it == listener) {
					_listeners.erase(it);
					break;
				}
			}
		}
		virtual void invoke(void * owner, _ArgTypes... __args) {
			for (auto listener : this->_listeners) {
				(*listener)(__args...);
			}
		}
	};
	template<typename ... _ArgTypes>
	class thread_safe_event : public event<_ArgTypes...>
	{
	private:
		recursive_mutex _mutex;
	public:
		// invokes the event
		virtual void operator()(void * owner, _ArgTypes... __args)
		{
			lock_guard<recursive_mutex> lock(_mutex);
			event<_ArgTypes...>::operator()(owner, __args...);
		}
		virtual void operator+=(typename event<_ArgTypes...>::delegate_ptr listener)
		{
			lock_guard<recursive_mutex> lock(_mutex);
			event<_ArgTypes...>::operator+=(listener);
		}
		virtual void operator-=(typename event<_ArgTypes...>::delegate_ptr listener)
		{
			lock_guard<recursive_mutex> lock(_mutex);
			event<_ArgTypes...>::operator-=(listener);
		}
		virtual void invoke(void * owner, _ArgTypes... __args)
		{
			lock_guard<recursive_mutex> lock(_mutex);
			event<_ArgTypes...>::invoke(owner, __args...);
		}
	};
} // namespace std
#endif
