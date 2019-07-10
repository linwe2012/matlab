#pragma once
#include <tuple>
#include "v8.h"
#include <array>
#include <string>
#include "v8pp/convert.hpp"


namespace detail {
	v8::Local<v8::String> Str(v8::Isolate* isolate, const char*c ) {
		using namespace v8;
		return String::NewFromUtf8(isolate, c, NewStringType::kNormal).ToLocalChecked();
	}

	template<typename Args>
	Args GetPropertiesPass(v8::Isolate* isolate, v8::Local<v8::Object> object, const char* c) {
		using namespace v8;
		return
			v8pp::from_v8<Args>(isolate, object->Get(String::NewFromUtf8(isolate, c, NewStringType::kNormal).ToLocalChecked()));
	}

	template<typename... Args, std::size_t... Is>
	std::tuple<Args...> GetProperties(v8::Isolate* isolate, v8::Local<v8::Object> object, const char* arr[sizeof...(Args)], std::index_sequence<Is...>) {
		return {
			GetPropertiesPass<Args>(isolate, object, arr[Is])...
		};
	}
	
	template<typename... Args, std::size_t... Is>
	std::tuple<Args...> GetProperties(v8::Isolate* isolate, v8::Local<v8::Object> object, std::array<const char*, sizeof...(Args)>, std::index_sequence<Is...>) {
		return {
			GetPropertiesPass<Args>(isolate, object, arr[Is])...
		};
	}

}

template<typename... Args>
std::tuple<Args...> GetProperties(v8::Isolate* isolate, v8::Local<v8::Object> object, const char* arr[sizeof...(Args)]) {
	using t = const char* [sizeof...(Args)];

	return detail::GetProperties<Args...>(isolate, object, arr, std::index_sequence_for<Args...>{});
}

template<typename... Args>
std::tuple<Args...> GetProperties(v8::Isolate* isolate, v8::Local<v8::Object> object, std::array<const char*, sizeof...(Args)> arr) {
	using t = const char* [sizeof...(Args)];

	return detail::GetProperties<Args...>(isolate, object, arr, std::index_sequence_for<Args...>{});
}

