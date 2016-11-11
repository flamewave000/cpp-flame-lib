#pragma once
#ifndef _OBJECT_H
#define _OBJECT_H 1

#include <memory>
#include <vector>

namespace flame {
	class object;

	class type final {
#pragma region internal classes
	public:
		class info final {
#pragma region internal classes
		public:
			typedef size_t uid;
#pragma endregion


#pragma region instance variables
		private:
			const uid _id;
			const char * _name;
			const bool _is_protocol;
#pragma endregion


#pragma region constructors
		public:
			info(const uid id, const char * name, const bool &is_protocol) : _id(id), _name(name), _is_protocol(is_protocol) {}
#pragma endregion


#pragma region properties
		public:
			inline const uid get_id() const { return _id; }
			inline const char * get_name() const { return _name; }
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
		info _info;
		const std::shared_ptr<type> _base;
		std::vector<info> _protocols;
#pragma endregion


#pragma region properties
	public:
		inline const info &get_info() const { return _info; }
		inline const type &get_base_type() const { return *_base; }
#pragma endregion


#pragma region constructors
	private:
		type(const info &type_info, const std::shared_ptr<type> &base) : _info(type_info), _base(base) {}
#pragma endregion


#pragma region public methods
	public:
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



#define TYPE_INFO(c, p) type::info(typeid(c).hash_code(), typeid(c).name(), p)
	template<class _obj>
	inline type::info class_info() { return TYPE_INFO(_obj, false); }
	template<class _obj>
	inline type::info proto_info() { return TYPE_INFO(_obj, true); }


#define DERIVED(derived) this->__reg_type<derived>()
#define IMPLEMENTS(...) this->__reg_protos({__VA_ARGS__});
#define PROTO(protocol) type::info(typeid(protocol).hash_code(), typeid(protocol).name(), true)

	class object {
#pragma region instance variables
	private:
		std::shared_ptr<type> _type;
#pragma endregion


#pragma region constructors
	public:
		object() {
			__reg_type<object>();
		}
		virtual ~object() {}
#pragma endregion


#pragma region protected methods
	protected:
		template<class _obj>
		inline void __reg_type() {
			_type.reset(new type(TYPE_INFO(_obj, false), _type));
		}
		template<class _obj>
		inline void __reg_proto() {
			_type->_protocols.push_back(TYPE_INFO(_obj, true));
		}
		inline void __reg_protos(const std::vector<type::info> &infos) {
			for (size_t c = 0, size = infos.size(); c < size; c++) {
				_type->_protocols.push_back(infos[c]);
			}
		}
#pragma endregion


#pragma region public methods
	public:
		inline bool instanceof(const object *obj) const {
			return instanceof(obj->get_type().get_info());
		}
		inline bool instanceof(const type::info &obj_type) const {
			return (obj_type._is_protocol && _type->has_protocol(obj_type)) || _type->has_class(obj_type);
		}
		const type & get_type() const { return *_type; }
		virtual const char * to_string() const { return _type->_info._name; }
#pragma endregion
	};


	class protocol {
#pragma region constructors
	protected:
		protocol() {
		}
	public:
		virtual ~protocol() {}
#pragma endregion


#pragma region protected methods
	protected:
		inline const char * to_string() const { return dynamic_cast<const object*>(this)->to_string(); }
#pragma endregion
	};

#undef TYPE_INFO

}

#endif