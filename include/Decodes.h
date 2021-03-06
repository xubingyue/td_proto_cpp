#ifndef _TD_PROTO_DECODE_H_
#define _TD_PROTO_DECODE_H_

#include "Values.h"

namespace td_proto {

#define CHECK_RETURN_BUFFER_VAILD(value) if (!buffer.isVaild()) { \
	return value; \
}

#define CHECK_BREAK_BUFFER_VAILD() if (!buffer.isVaild()) { \
	break; \
}

	static Values decode_field(Buffer& buffer, Config& config);

	static Values decode_number(Buffer& buffer, u16 pattern) {
		CHECK_RETURN_BUFFER_VAILD(Values());
		switch (pattern)
		{
		case TYPE_U8:
			return Values(buffer.read<u8>());
		case TYPE_I8:
			return Values(buffer.read<i8>());
		case TYPE_U16:
			return Values(buffer.read<u16>());
		case TYPE_I16:
			return Values(buffer.read<i16>());
		case TYPE_U32:
			return Values(buffer.read<u32>());
		case TYPE_I32:
			return Values(buffer.read<i32>());
		case TYPE_FLOAT: {
			float value = (float)(buffer.read<i32>() / 1000.0);
			return Values(value);
		}
		default:
			std::terminate();
			break;
		}
		return Values();
	}
	
	static Field read_field(Buffer& buffer) {
		CHECK_RETURN_BUFFER_VAILD(Field(0, STR_TYPE_NIL));
		auto index = decode_number(buffer, TYPE_U16)._u16;
		auto pattern = decode_number(buffer, TYPE_U16)._u16;
		return Field(index, get_name_by_type(pattern));
	}

	static Values decode_str_raw(Buffer& buffer, u16 pattern) {
		CHECK_RETURN_BUFFER_VAILD(Values());
		switch (pattern)
		{
		case TYPE_STR: {
			u16 len = decode_number(buffer, TYPE_U16)._u16;
			if( len == 0 ) {
				return Values(new std::string());
			}
			
			u8* value = new u8[len];
			buffer.read(value, len);
			std::string* str = new std::string((const char*)value, len);
			delete[] value;
			return Values(str);
		}
		case TYPE_RAW: {
			u16 len = decode_number(buffer, TYPE_U16)._u16;
			if (len == 0) {
				return Values(new std::vector<char>());
			}

			auto vecs = new std::vector<char>(len);
			u8* value = new u8[len];
			buffer.read(value, len);
			memcpy(&vecs[0], value, len);
			return Values(vecs);
		}
		default:
			std::terminate();
			break;
		}
		return Values();
	}

	static Values decode_map(Buffer& buffer, Config& config) {
		CHECK_RETURN_BUFFER_VAILD(Values());
		auto map = new std::map<std::string, Values>();
		while (true) {
			auto field = read_field(buffer);
			if (field.is_nil_type()) {
				return Values(map);
			}
			CHECK_RETURN_BUFFER_VAILD(Values(map));
			auto sub_value = decode_field(buffer, config);
			auto name = config.get_field_index_name(field.index);
			if ( name.size() == 0 ) {
				continue;
			}
			map->insert(std::make_pair(name, std::move(sub_value)));
		}
		return Values();
	}

	static Values decode_arrays(Buffer& buffer, Config& config, u16 pattern, u16 sub_type) {
		CHECK_RETURN_BUFFER_VAILD(Values());
		auto value = new std::vector<Values>();
		while (true) {
			auto sub_value = decode_field(buffer, config);
			if (sub_value.sub_type == TYPE_NIL) {
				break;
			}
			if (sub_value.sub_type != sub_type) {
				buffer.setVaild(false);
				break;
			}
			CHECK_BREAK_BUFFER_VAILD();
			value->push_back(std::move(sub_value));
		}
		return Values(value, (u8)pattern);
	}

	static Values decode_by_field(Buffer& buffer, Config& config, Field& field) {
		CHECK_RETURN_BUFFER_VAILD(Values());
		auto t = get_type_by_name(field.pattern.c_str());
		switch (t)
		{
		case TYPE_U8:
		case TYPE_I8:
		case TYPE_U16:
		case TYPE_I16:
		case TYPE_U32:
		case TYPE_I32:
		case TYPE_FLOAT:
			return decode_number(buffer, t);
		case TYPE_STR:
		case TYPE_RAW:
			return decode_str_raw(buffer, t);
		case TYPE_MAP:
			return decode_map(buffer, config);
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
			return decode_arrays(buffer, config, t, t + TYPE_U8 - TYPE_AU8);
		default:
			std::terminate();
		}
	}

	static Values decode_field(Buffer& buffer, Config& config) {
		CHECK_RETURN_BUFFER_VAILD(Values());
		auto field = read_field(buffer);
		if (field.is_nil_type()) {
			return Values();
		}
		return decode_by_field(buffer, config, field);
	}

	static std::string decode_proto(Buffer& buffer, Config& config, std::vector<Values>& result) {
		auto name_value = decode_str_raw(buffer, TYPE_STR);
		while (true)
		{
			auto sub_value = decode_field(buffer, config);
			if (sub_value.sub_type == TYPE_NIL) {
				break;
			}
			CHECK_BREAK_BUFFER_VAILD();
			result.push_back(std::move(sub_value));
		}

		auto proto = config.get_proto_by_name(*name_value._str);
		if (proto == NULL) {
			buffer.setVaild(false);
			return *name_value._str;
		}
		if (proto->args.size() != result.size()) {
			buffer.setVaild(false);
			return *name_value._str;
		}
		return *name_value._str;

	}

}
#endif