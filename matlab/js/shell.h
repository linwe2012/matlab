#pragma once
#include "v8.h"
#include "v8pp/module.hpp"
#include "v8pp/class.hpp"

#include <string>
#include <functional>

struct V8Exception {
	std::string exception;

	bool has_detail;
	std::string filename;
	std::string sourceline;
	int sourceline_begin;
	int sourceline_end;
	int line_number = -1;
	std::string stack_trace;

	V8Exception(v8::Isolate* isolate, v8::TryCatch* try_catch);
};

class V8Shell {
public:

	using UncaughtExceptionHandler = std::function<void(v8::Isolate* isolate, v8::TryCatch* try_catch)>;
	using ResultCallback = std::function<void(v8::Local<v8::Value>)>;

	V8Shell(int argc, char* argv[], std::ostream& os, const char* shell_name = "(shell)");

	bool Execute(const std::string& str, ResultCallback callback = ResultCallback());

	void SetUncaughtExceptionHandler(UncaughtExceptionHandler handler);

	bool Closed();

	v8::Isolate* GetIsolate();

private:

	void ReportException(v8::TryCatch* try_catch);

	struct Data; // serve as firewall between v8 and us
	Data* data_;
};