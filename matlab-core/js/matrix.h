#pragma once
#include "shell.h"
#include "opencv2/opencv.hpp"
#include <iostream>
#include <filesystem>

using namespace v8;

class Matrix {
public:
	Matrix() {}

	Matrix(cv::Mat m, V8Shell* shell)
		:matrix(m), shell_(shell)
	{}

	void v8_resize(const v8::FunctionCallbackInfo<v8::Value>& args);
	void v8_read(const v8::FunctionCallbackInfo<v8::Value>& args);
	void v8_write(const v8::FunctionCallbackInfo<v8::Value>& args); // Output pic, to file `output.png`. Just for test.
	void v8_rotate(const v8::FunctionCallbackInfo<v8::Value>& args);
	void v8_togray(const v8::FunctionCallbackInfo<v8::Value>& args);
	void v8_tobin(const v8::FunctionCallbackInfo<v8::Value>& args);
	void v8_equalizeHist(const v8::FunctionCallbackInfo<v8::Value>& args);
	void v8_linear(const v8::FunctionCallbackInfo<v8::Value>& args);
	void v8_face(const v8::FunctionCallbackInfo<v8::Value>& args);
	void v8_clone(const v8::FunctionCallbackInfo<v8::Value>& args);
	void return_this(const v8::FunctionCallbackInfo<v8::Value>& args) {
		args.GetReturnValue().Set(args.This());
	}

	void resize(const std::vector<int>& dims);
	void openFile(Isolate* isolate, const char* filename);
	void write(Isolate* isolate, const char* filename);
	void rotate(const double degree);
	void togray();
	void tobin();
	void equalizeHist();
	void linear(double alpha, int beta);
	void face();



	cv::Mat matrix;
private:
	bool GetShell(Isolate* isolate) {
		if (shell_ != nullptr) {
			return true;
		}
		shell_ = V8Shell::GetShell(isolate);
		return shell_;
	}

	V8Shell* shell_ = nullptr;
};


void DefineJSMatrix(V8Shell* shell);