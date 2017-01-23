#pragma once
#ifndef _SAFEVAR_H
#define _SAFEVAR_H 1

#ifndef _MEMORY_
#include <memory>
#endif
#ifndef _MUTEX_
#include <mutex>
#endif
#ifndef _EXCEPTION_
#include <exception>
#endif

namespace var_types
{
	template<typename _var_type>
	using var_ptr = std::shared_ptr<_var_type>;
	typedef std::recursive_mutex var_mtx;
	typedef std::lock_guard<var_mtx> var_lock;
	typedef std::shared_ptr<var_lock> var_lock_ptr;
}

template<class _var_type>
class var_ref
{
private:
	var_types::var_ptr<_var_type> _ptr;
	var_types::var_lock_ptr _lock;
public:
	var_ref(const var_types::var_ptr<_var_type> &ptr, var_types::var_mtx &mtx) : _ptr(ptr), _lock(new var_types::var_lock(mtx)) {}
	var_ref(var_ref &&_Right) : _ptr(std::move(_Right._ptr)), _lock(std::move(_Right._lock)) {}
	var_ref(const var_ref&) = delete;
	var_ref& operator=(const var_ref&) = delete;
	virtual ~var_ref() { dispose(); }
public:
	_var_type &operator*() const _NOEXCEPT { return *_ptr.get(); }
	_var_type *operator->() const _NOEXCEPT { return _ptr.get(); }
public:
	inline void dispose() { _ptr = nullptr; _lock = nullptr; }
};

template<class _var_type>
class safe_var
{
#pragma region instance variables
private:
	var_types::var_ptr<_var_type> _ptr;
	var_types::var_mtx _mtx;
	std::weak_ptr<var_ref<_var_type>> _var_ref;
#pragma endregion


#pragma region constructors
public:
	explicit safe_var() : _ptr(nullptr) {}
	safe_var(var_types::var_ptr<_var_type> &ptr) : _ptr(ptr) {}
	safe_var(safe_var<_var_type> &&_Right) : _ptr(std::move(_Right._ptr)), _mtx(std::move(_Right._mtx)) {}
	virtual ~safe_var() {}
#pragma endregion


public:
	std::shared_ptr<var_ref<_var_type>> lock() {
		var_types::var_lock __lock(_mtx);
		std::shared_ptr<var_ref<_var_type>> sptr = _var_ref.lock();
		if (sptr == nullptr) {
			sptr = std::make_shared<var_ref<_var_type>>(_ptr, _mtx);
			_var_ref = sptr;
		}
		return std::move(sptr);
	}
};
#endif
