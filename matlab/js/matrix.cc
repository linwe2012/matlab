#include "shell.h"
#include "opencv2/opencv.hpp"
#include <iostream>
using namespace v8;

class Matrix {
public:
	Matrix() {}

	void v8_resize(const v8::FunctionCallbackInfo<v8::Value>& args);
	void v8_openFile(const v8::FunctionCallbackInfo<v8::Value>& args);
	void v8_showimg(const v8::FunctionCallbackInfo<v8::Value>& args); // Output pic, to file `output.png`. Just for test.
	void v8_rotate(const v8::FunctionCallbackInfo<v8::Value>& args);
	void v8_togray(const v8::FunctionCallbackInfo<v8::Value>& args);
	void v8_tobin(const v8::FunctionCallbackInfo<v8::Value>& args);
	
	void resize(const std::vector<int>& dims);
	void openFile(const std::string& fileName);
	void showimg();
	void rotate(const int degree);
	void togray();
	void tobin();

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
	v8pp::module mylib(isolate);

	v8pp::class_<Matrix> matrix_class(isolate);
	matrix_class
		.ctor<>()
		.set("resize", &Matrix::v8_resize)
		.set("matrix", &Matrix::matrix)
		.set("openFile", &Matrix::openFile)
		.set("showimg", &Matrix::showimg)
		.set("rotate", &Matrix::rotate)
		.set("togray", &Matrix::togray)
		.set("tobin", &Matrix::tobin);

	mylib.set("Matrix", matrix_class);

	isolate->GetCurrentContext()->Global()->Set(
		v8::String::NewFromUtf8(isolate, "mylib"), mylib.new_instance());
	
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


void Matrix::v8_openFile(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	int num_args = args.Length();

	if (num_args == 0) {
		return ArgError(args, "Expected at least one argument");
	}

	std::string fileName = v8pp::from_v8<std::string>(args.GetIsolate(), args[0]);
	openFile(fileName);
}

void Matrix::v8_showimg(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	int num_args = args.Length();

	if (num_args == 0) {
		return ArgError(args, "Expected at least one argument");
	}

	showimg();
}

void Matrix::v8_rotate(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	int num_args = args.Length();

	if (num_args == 0) {
		return ArgError(args, "Expected at least one argument");
	}

	int deg = v8pp::from_v8<int>(args.GetIsolate(), args[0]);
	rotate(deg);
}

void Matrix::v8_togray(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	togray();
}

void Matrix::v8_tobin(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	tobin();
}

void Matrix::resize(const std::vector<int>& dims)
{
	std::cerr << "resize called: [";
	for (auto d : dims) {
		std::cout << d << ", ";
	}
	std::cout << "]" << std::endl;
}

void Matrix::openFile(const std::string& fileName)
{
	matrix = cv::imread(fileName);
	std::cout << fileName << "loaded." << std::endl;
}

void Matrix::showimg()
{
	//std::cout << matrix << std::endl;
	cv::imwrite("output.png", matrix);
	//cv::imshow("Pic", matrix);
}

void Matrix::rotate(const int degree)
{
	cv::Mat* temp = new cv::Mat(matrix);

	cv::Point2f center((temp->cols - 1) / 2.0, (temp->rows - 1) / 2.0);
	cv::Mat rot = cv::getRotationMatrix2D(center, degree, 1.0);

	cv::Rect2f bbox = cv::RotatedRect(cv::Point2f(), temp->size(), degree).boundingRect2f();

	rot.at<double>(0, 2) += bbox.width / 2.0 - temp->cols / 2.0;
	rot.at<double>(1, 2) += bbox.height / 2.0 - temp->rows / 2.0;

	cv::warpAffine(*temp, matrix, rot, bbox.size());
}

void Matrix::togray()
{
	cv::Mat temp = matrix.clone();
	cv::cvtColor(temp, matrix, cv::COLOR_BGR2GRAY);
}

void Matrix::tobin()
{
	togray();
	cv::Mat temp = matrix.clone();
	cv::threshold(temp, matrix, 15, 100.0, 8);
}
