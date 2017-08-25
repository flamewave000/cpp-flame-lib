#pragma once
#ifndef _OBJECT_
#define _OBJECT_ 1

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
#ifndef _STREXT_H
#include "strext.h"
#endif

namespace std {
	class object;

	/*wraps the info and relations for a single runtime type*/
	class type final {
#pragma region internal classes
	public:
		/*contains the class info of a single runtime type*/
		class info final {
#pragma region internal classes
		public:
			/*unique identifier*/
			typedef size_t uid;
#pragma endregion


#pragma region instance variables
		private:
			/*the unique id of the class in the form of a RTTI hash code*/
			const uid _id;
			/*the runtime class name*/
			const char * _name;
			/*true if the class is a protocol; otherwise false*/
			const bool _is_protocol;
#pragma endregion


#pragma region constructors
		public:
			info(const uid id, const char * name, const bool &is_protocol) : _id(id), _name(name), _is_protocol(is_protocol) {}
#pragma endregion


#pragma region properties
		public:
			/*returns the unique id in the form of a RTTI hash code*/
			inline const uid get_id() const { return _id; }
			/*returns the runtime class name*/
			inline const char * get_name() const { return _name; }
			/*returns true if the class is a protocol; otherwise false*/
			inline bool is_protocol() const { return _is_protocol; }
#pragma endregion


#pragma region operator overloads
		public:
			inline bool operator==(const type::info &other) { return this->_id == other._id; }
			inline bool operator!=(const type::info &other) { return this->_id != other._id; }
			inline bool operator==(const type::info &other) const { return this->_id == other._id; }
			inline bool operator!=(const type::info &other) const { return this->_id != other._id; }
			inline bool operator==(const uid &other) { return this->_id == other; }
			inline bool operator!=(const uid &other) { return this->_id != other; }
			inline bool operator==(const uid &other) const { return this->_id == other; }
			inline bool operator!=(const uid &other) const { return this->_id != other; }
#pragma endregion


#pragma region friends
			friend type;
			friend object;
#pragma endregion
		};
#pragma endregion

#pragma region instance variables
	private:
		/*the std::type::info for the runtime host class this std::type represents*/
		info _info;
		/*the base std::type the host class inherits from; null if we are std::object*/
		const std::shared_ptr<type> _base;
		/*the list of protocols the host class implements*/
		std::vector<info> _protocols;
#pragma endregion

#pragma region properties
	public:
		/*returns the std::type::info for the host class*/
		inline const info &get_info() const { return _info; }
		/*returns the std::stype of the base class the host class inherits from*/
		inline const type &get_base_type() const { return *_base; }
#pragma endregion

#pragma region constructors
	private:
		type(const info &type_info, const std::shared_ptr<type> &base) : _info(type_info), _base(base) {}
#pragma endregion

#pragma region public methods
	public:
		/*returns true if the provided std::type::info is a protocol type and is
		registered as being implemented by the host class; otherwise false*/
		bool has_protocol(const info &proto_info) const {
			if (proto_info._is_protocol) {
				for (size_t c = 0, size = _protocols.size(); c < size; c++) {
					if (_protocols[c] == proto_info) {
						return true;
					}
				}
				if (_base != nullptr) {
					return const_cast<type*>(_base.get())->has_protocol(proto_info);
				}
			}
			return false;
		}
		/*returns true if the provided std::type::info is not a protocol and has the
		same std::type::info::uid as the host class or any of its base classes;
		otherwise false*/
		inline bool has_class(const info &type_info) const {
			return !type_info._is_protocol && (_info == type_info || (_base != nullptr && const_cast<type*>(_base.get())->has_class(type_info)));
		}
#pragma endregion

#pragma region operator overloads
	public:
		inline bool operator==(const type &other) { return _info == other._info; }
		inline bool operator!=(const type &other) { return _info != other._info; }
		inline bool operator==(const type &other) const { return _info == other._info; }
		inline bool operator!=(const type &other) const { return _info != other._info; }
#pragma endregion

#pragma region friends
		friend object;
#pragma endregion
	};


	/*helper macro for properly creating an instance of std::type::info using RTTI*/
#define TYPE_INFO(c, p) type::info(typeid(c).hash_code(), typeid(c).name(), p)
	/*returns the runtime std::type::info for the templated class type*/
	template<class _obj>
	inline type::info class_info() { return TYPE_INFO(_obj, false); }
	/*returns the runtime std::type::info for the templated protocol type*/
	template<class _obj>
	inline type::info proto_info() { return TYPE_INFO(_obj, true); }

	/*registers the provided derived type as being the host class*/
#define DERIVED(derived) this->__reg_type<derived>()
	/*registers the list of protocol std::type::info as being implemented by the host class*/
#define IMPLEMENTS(...) this->__reg_protos({__VA_ARGS__});
	/*short hand helper for retrieving the std::type::info of a protocol*/
#define PROTO(protocol) std::proto_info<protocol>()
	/*short hand helper for extending std::object as the base*/
#define ROOT_BASE public std::object

#define IFACE(return_type, method_name, ...) virtual return_type method_name(__VAR_ARGS__) = 0


	/*the root base class of all typed classes*/
	class object {
#pragma region instance variables
	private:
		/*the std::type of the host runtime class*/
		std::shared_ptr<type> _type;
#pragma endregion

#pragma region constructors
	public:
		object() { __reg_type<object>(); }
		virtual ~object() {}
#pragma endregion

#pragma region protected methods
	protected:
		/*registers a single type as the host runtime class*/
		template<class _obj>
		inline void __reg_type() {
			_type.reset(new type(TYPE_INFO(_obj, false), _type));
		}
		/*registers a single protocol that is implemented by the host runtime class*/
		template<class _obj>
		inline void __reg_proto() {
			_type->_protocols.push_back(TYPE_INFO(_obj, true));
		}
		/*registers a list of protocol std::type::info that are implemented by this runtime class*/
		inline void __reg_protos(const std::vector<type::info> &infos) {
			for (size_t c = 0, size = infos.size(); c < size; c++) {
				_type->_protocols.push_back(infos[c]);
			}
		}
#pragma endregion

#pragma region public methods
	public:
		/*returns true if this class is an instance of the provided object*/
		inline bool instanceof(const object *obj) const {
			return instanceof(obj->get_type().get_info());
		}
		/*returns true if this class inherits from the class or proto described
		provided by the std::type::info*/
		inline bool instanceof(const type::info &obj_type) const {
			return (obj_type._is_protocol && _type->has_protocol(obj_type)) || _type->has_class(obj_type);
		}
		const type & get_type() const { return *_type; }
		virtual const char * to_string() const { return _type->_info._name; }
#pragma endregion
	};

#define PROTOCOL(p_name) class p_name{public:virtual~p_name(){}protected:p_name(){}public
#define PROTOCOL_IMP(p_name, ...) class p_name:__VA_ARGS__{public:virtual~p_name(){}protected:p_name(){}public
#define PROTOEND };

#pragma region std::format operator overloads for object
	inline format &operator%(format &fmt, const shared_ptr<object> &obj) {
		return fmt % obj->to_string();
	}
	inline format &operator%(format &fmt, const object * obj) {
		return fmt % obj->to_string();
	}
	inline format &operator%(format &fmt, const object &obj) {
		return fmt % obj.to_string();
	}
#pragma endregion


#undef TYPE_INFO
	}

#endif