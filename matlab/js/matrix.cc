#include "shell.h"
#include "opencv2/core/core.hpp"
#include <iostream>
using namespace v8;

class Matrix {
public:
	void v8_resize(const v8::FunctionCallbackInfo<v8::Value>& args);
	
	void resize(const std::vector<int>& dims);

	cv::Mat matrix;
};

template<int N>
void ArgError(const v8::FunctionCallbackInfo<v8::Value>& args, char const(&str)[N]) {
	Isolate* isolate = args.GetIsolate();
	isolate->ThrowException(
		Exception::TypeError(v8pp::to_v8(isolate, str))
	);
}

void DefineJSMatrix(V8Shell* shell) {
	auto isolate = shell->GetIsolate();

	v8pp::class_<Matrix> matrix_class(isolate);
	matrix_class
		.set("resize", &Matrix::v8_resize);



}






void Matrix::v8_resize(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	int num_args = args.Length();
	Isolate* isolate = args.GetIsolate();
	std::vector<int> dims;

	if (num_args == 0) {
		return ArgError(args, "Expected at least one argument");
	}

	if (args[0]->IsArray()) {
		dims = std::move(
			v8pp::from_v8<std::vector<int>>(args.GetIsolate(), args[0])
		);
	}
	else {
		for (int i = 0; i < num_args; ++i) {
			int t = v8pp::from_v8<int>(isolate, args[i], -1);
			if (t < 0) {
				return ArgError(args, "Expected int");
			}

			dims.push_back(t);
		}
	}
	
	resize(dims);
}

void Matrix::resize(const std::vector<int>& dims)
{
	std::cerr << "resize called: [";
	for (auto d : dims) {
		std::cout << d << ", ";
	}
	std::cout << "]" << std::endl;
}

