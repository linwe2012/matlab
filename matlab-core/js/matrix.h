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
	cv::Vec3b v8_getColor(const v8::FunctionCallbackInfo<v8::Value>& args);
	int v8_getRows(const v8::FunctionCallbackInfo<v8::Value>& args);
	int v8_getCols(const v8::FunctionCallbackInfo<v8::Value>& args);

	void fill(std::vector<std::vector<float>>);
	void resize(const std::vector<int>& dims);
	void openFile(Isolate* isolate, const char* filename);
	void write(Isolate* isolate, const char* filename);
	void rotate(const double degree);
	void togray();
	void tobin();
	void equalizeHist();
	void linear(double alpha, int beta);
	void face();
	void conv(cv::Mat& ker); /*example:
							 * cv::Mat kernel = (cv::Mat_<char>(3, 3) << 0, -1, 0,
							 *  										 -1, 5, -1,
							 *  										 0, -1, 0);
							 * conv(kernel);
							 */
	void setColor(int x, int y, cv::Vec3b& color); /*example:
												   * cv::Vec3b foo;
												   * foo[0] = 255;
												   * foo[1] = 0;
												   * foo[2] = 0;
												   * for (int i = 0; i < 114; ++i)
												   * 	for (int j = 0; j < 514; ++j)
												   * 		setColor(i, j, foo);
												   */
	cv::Vec3b getColor(int x, int y); /*example:
									  * cv::Vec3b foo = getColor(11, 51);
									  */
	int getRows();
	int getCols();


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