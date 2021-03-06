#ifndef _TD_PROTO_VALUES_H_
#define _TD_PROTO_VALUES_H_
#include "TDMacro.h"
#include <string>
#include <vector>
#include <Map>

namespace td_proto {

	static const u16 TYPE_NIL = 0;
	static const u16 TYPE_U8 = 1;
	static const u16 TYPE_I8 = 2;
	static const u16 TYPE_U16 = 3;
	static const u16 TYPE_I16 = 4;
	static const u16 TYPE_U32 = 5;
	static const u16 TYPE_I32 = 6;
	static const u16 TYPE_FLOAT = 7;
	static const u16 TYPE_STR = 8;
	static const u16 TYPE_RAW = 9;
	static const u16 TYPE_MAP = 10;
	static const u16 TYPE_AU8 = 21;
	static const u16 TYPE_AI8 = 22;
	static const u16 TYPE_AU16 = 23;
	static const u16 TYPE_AI16 = 24;
	static const u16 TYPE_AU32 = 25;
	static const u16 TYPE_AI32 = 26;
	static const u16 TYPE_AFLOAT = 27;
	static const u16 TYPE_ASTR = 28;
	static const u16 TYPE_ARAW = 29;
	static const u16 TYPE_AMAP = 30;

	static const char* STR_TYPE_NIL = "nil";
	static const char* STR_TYPE_U8 = "u8";
	static const char* STR_TYPE_I8 = "i8";
	static const char* STR_TYPE_U16 = "u16";
	static const char* STR_TYPE_I16 = "i16";
	static const char* STR_TYPE_U32 = "u32";
	static const char* STR_TYPE_I32 = "i32";
	static const char* STR_TYPE_FLOAT = "float";
	static const char* STR_TYPE_STR = "str";
	static const char* STR_TYPE_RAW = "raw";
	static const char* STR_TYPE_MAP = "map";
	static const char* STR_TYPE_AU8 = "u8[]";
	static const char* STR_TYPE_AI8 = "i8[]";
	static const char* STR_TYPE_AU16 = "u16[]";
	static const char* STR_TYPE_AI16 = "i16[]";
	static const char* STR_TYPE_AU32 = "u32[]";
	static const char* STR_TYPE_AI32 = "i32[]";
	static const char* STR_TYPE_AFLOAT = "float[]";
	static const char* STR_TYPE_ASTR = "str[]";
	static const char* STR_TYPE_ARAW = "raw[]";
	static const char* STR_TYPE_AMAP = "map[]";

	struct cmp_str
	{
		bool operator()(char const *a, char const *b) const
		{
			return strcmp(a, b) < 0;
		}
	};

	static std::map<const char*, u16, cmp_str> HASH_STR_INT = {
		{ STR_TYPE_U8, TYPE_U8 },
		{ STR_TYPE_I8, TYPE_I8 },
		{ STR_TYPE_U16, TYPE_U16 },
		{ STR_TYPE_I16, TYPE_I16 },
		{ STR_TYPE_U32, TYPE_U32 },
		{ STR_TYPE_I32, TYPE_I32 },
		{ STR_TYPE_FLOAT, TYPE_FLOAT },
		{ STR_TYPE_STR, TYPE_STR },
		{ STR_TYPE_RAW, TYPE_RAW },
		{ STR_TYPE_MAP, TYPE_MAP },
		{ STR_TYPE_AU8, TYPE_AU8 },
		{ STR_TYPE_AI8, TYPE_AI8 },
		{ STR_TYPE_AU16, TYPE_AU16 },
		{ STR_TYPE_AI16, TYPE_AI16 },
		{ STR_TYPE_AU32, TYPE_AU32 },
		{ STR_TYPE_AI32, TYPE_AI32 },
		{ STR_TYPE_AFLOAT, TYPE_AFLOAT },
		{ STR_TYPE_ASTR, TYPE_ASTR },
		{ STR_TYPE_ARAW, TYPE_ARAW },
		{ STR_TYPE_AMAP, TYPE_AMAP },
	};

	static std::map<u16, const char*> HASH_INT_STR;

	static u16 get_type_by_name(const char* name) {
		auto iter = HASH_STR_INT.find(name);
		if (iter == HASH_STR_INT.end()) {
			return 0;
		}
		return iter->second;
	}

	static const char* get_name_by_type(u16 index) {
		if (HASH_INT_STR.empty()) {
			for (auto iter : HASH_STR_INT)
			{
				HASH_INT_STR.insert(std::make_pair(iter.second, iter.first));
			}
		}
		auto iter = HASH_INT_STR.find(index);
		if (iter == HASH_INT_STR.end()) {
			return STR_TYPE_NIL;
		}
		return iter->second;
	}

	class Values
	{
	public:
		u8 sub_type;
		union {
			u8 _u8;
			i8 _i8;
			u16 _u16;
			i16 _i16;
			u32 _u32;
			i32 _i32;
			float _f;
			std::string* _str;
			std::vector<char>* _raw;
			std::map<std::string, Values>* _map;
			std::vector<Values>* _array;
		};

		explicit Values() : sub_type(TYPE_NIL) {

		}
		explicit Values(u8 _u8) : sub_type(TYPE_U8), _u8(_u8) {
		}
		explicit Values(i8 _i8) : sub_type(TYPE_I8), _i8(_i8) {
		}
		explicit Values(u16 _u16) : sub_type(TYPE_U16), _u16(_u16) {
		}
		explicit Values(i16 _i16) : sub_type(TYPE_I16), _i16(_i16) {
		}
		explicit Values(u32 _u32) : sub_type(TYPE_U32), _u32(_u32) {
		}
		explicit Values(i32 _i32) : sub_type(TYPE_I32), _i32(_i32) {
		}
		explicit Values(float _f) : sub_type(TYPE_FLOAT), _f(_f) {
		}
		explicit Values(std::string* _str) : sub_type(TYPE_STR), _str(_str) {
		}
		explicit Values(std::vector<char>* _raw) : sub_type(TYPE_RAW), _raw(_raw) {
		}
		explicit Values(std::map<std::string, Values>* _map) : sub_type(TYPE_MAP), _map(_map) {
		}
		explicit Values(std::vector<Values>* arrays, u8 sub_type) : sub_type(sub_type) {
			switch (sub_type)
			{
			case TYPE_AU8:
			case TYPE_AI8:
			case TYPE_AU16:
			case TYPE_AI16:
			case TYPE_AU32:
			case TYPE_AI32:
			case TYPE_AFLOAT:
			case TYPE_ASTR:
			case TYPE_ARAW:
			case TYPE_AMAP:
				_array = arrays;
				break;
			default:
				sub_type = TYPE_NIL;
				break;
			}
		}
		~Values() {
			free();
		}

		void move(Values& other)
		{
			free();
			this->sub_type = other.sub_type;
			switch (other.sub_type)
			{
			case TYPE_U8:
				this->_u8 = other._u8;
				break;
			case TYPE_I8:
				this->_i8 = other._i8;
				break;
			case TYPE_U16:
				this->_u16 = other._u16;
				break;
			case TYPE_I16:
				this->_i16 = other._i16;
				break;
			case TYPE_U32:
				this->_u32 = other._u32;
				break;
			case TYPE_I32:
				this->_i32 = other._i32;
				break;
			case TYPE_FLOAT:
				this->_f = other._f;
				break;
			case TYPE_STR:
				this->_str = other._str;
				break;
			case TYPE_RAW:
				this->_raw = other._raw;
				break;
			case TYPE_MAP:
				this->_map = other._map;
				break;
			case TYPE_AU8:
			case TYPE_AI8:
			case TYPE_AU16:
			case TYPE_AI16:
			case TYPE_AU32:
			case TYPE_AI32:
			case TYPE_AFLOAT:
			case TYPE_ASTR:
			case TYPE_ARAW:
			case TYPE_AMAP:
				this->_array = other._array;
				break;
			default:
				break;
			}
			other.sub_type = TYPE_NIL;
		}

		Values(Values&& other) : sub_type(0)
		{
			move(other);
		}
		Values& operator= (Values&& other) {
			move(other);
			return *this;
		}

		void free() {
			switch (this->sub_type)
			{
			case TYPE_STR:
				delete _str;
				break;
			case TYPE_RAW:
				delete _raw;
				break;
			case TYPE_MAP:
				delete _map;
				break;
			case TYPE_AU8:
			case TYPE_AI8:
			case TYPE_AU16:
			case TYPE_AU32:
			case TYPE_AI32:
			case TYPE_AFLOAT:
			case TYPE_ASTR:
			case TYPE_ARAW:
			case TYPE_AMAP:
				delete _array;
				break;
			default:
				break;
			}
			this->sub_type = TYPE_NIL;
		}

		//当指针引用的栈变量的时候，我们不能析构这个指针
		void unfree() {
			this->sub_type = TYPE_NIL;
		}
	};

}
#endif