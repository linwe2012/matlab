#include "shell.h"
#include "opencv2/opencv.hpp"
#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;
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
	void v8_capcha(const v8::FunctionCallbackInfo<v8::Value>& args);
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

static v8pp::class_<Matrix> *gMatrixClass = nullptr;


template<int N>
void ArgError(const v8::FunctionCallbackInfo<v8::Value>& args, char const(&str)[N]) {
	Isolate* isolate = args.GetIsolate();
	isolate->ThrowException(
		Exception::TypeError(v8pp::to_v8(isolate, str))
	);
}

void DefineJSMatrix(V8Shell* shell) {
	if (gMatrixClass != nullptr) {
		//TODO: Log warning
		return;
	}
	auto isolate = shell->GetIsolate();
	gMatrixClass = new v8pp::class_<Matrix>(isolate);
	// v8pp::class_<Matrix> matrix_class(isolate);
	// matrix_class
	(*gMatrixClass)
		.ctor <> ()
		// values
		.set("matrix", &Matrix::matrix)
		// functions
		.set("resize", &Matrix::v8_resize)
		.set("read", &Matrix::v8_read)
		.set("write", &Matrix::v8_write)
		.set("rotate", &Matrix::v8_rotate)
		.set("togray", &Matrix::v8_togray)
		.set("tobin", &Matrix::v8_tobin)
		.set("equalizeHist", &Matrix::v8_equalizeHist)
		.set("linear", &Matrix::v8_linear)
		.set("face", &Matrix::v8_face)
		.set("clone", &Matrix::v8_clone)
		;

	shell->RegisterClasses(
		{
			{ "Matrix", (*gMatrixClass).js_function_template() }
		}
	);
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
	return_this(args);
}


void Matrix::v8_read(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	int num_args = args.Length();

	if (num_args == 0) {
		return ArgError(args, "Expected at least one argument");
	}

	openFile(args.GetIsolate(), v8pp::from_v8<const char*>(args.GetIsolate(), args[0]));
	return_this(args);
}

void Matrix::v8_write(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	int num_args = args.Length();

	if (num_args == 0) {
		return ArgError(args, "Expected at least one argument");
	}

	if (!args[0]->IsString()) {
		return ArgError(args, "Expected string as filename");
	}

	write(args.GetIsolate(), v8pp::from_v8<const char*>(args.GetIsolate(), args[0]));
	return_this(args);
}

void Matrix::v8_rotate(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	int num_args = args.Length();

	if (num_args == 0) {
		return ArgError(args, "Expected at least one argument");
	}

	double deg = v8pp::from_v8<double>(args.GetIsolate(), args[0], INT_MAX);
	if (deg == INT_MAX) {
		return ArgError(args, "Expected int");
	}

	rotate(deg);
	return_this(args);
}

void Matrix::v8_togray(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	togray();
	return_this(args);
}

void Matrix::v8_tobin(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	tobin();
	return_this(args);
}

void Matrix::v8_equalizeHist(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	equalizeHist();
	return_this(args);
}

void Matrix::v8_linear(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	int num_args = args.Length();
	if (num_args < 2) {
		return ArgError(args, "Expected at least one argument");
	}

	double alpha = v8pp::from_v8<double>(args.GetIsolate(), args[0]);
	int beta = v8pp::from_v8<int>(args.GetIsolate(), args[1]);
	
	linear(alpha, beta);
	return_this(args);
}

void Matrix::v8_face(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	face();
	return_this(args);
}

void Matrix::v8_clone(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	Isolate* isolate = args.GetIsolate();
	args.GetReturnValue().Set(
		(*gMatrixClass).create_object(
			isolate, (*gMatrixClass).unwrap_object(args.GetIsolate(), args.This())->matrix.clone(),
			shell_
		)
	);
}


void Matrix::resize(const std::vector<int>& dims)
{
	std::cerr << "resize called: [";
	for (auto d : dims) {
		std::cout << d << ", ";
	}
	std::cout << "]" << std::endl;
}

void Matrix::openFile(Isolate* isolate, const char* filename)
{
	if (!GetShell(isolate)) {
		//TODO: Log error
		return;
	}
	fs::path p(shell_->GetCwd());
	p /= filename;
	matrix = cv::imread(p.string().c_str());
	// std::cout << fileName << " loaded." << std::endl;
}

void Matrix::write(Isolate* isolate, const char* filename)
{
	if (!GetShell(isolate)) {
		//TODO: Log
		return;
	}
	//std::cout << matrix << std::endl;
	fs::path p(shell_->GetCwd());
	p /= filename;
	cv::imwrite(p.string().c_str(), matrix);
	// std::cout << "Output file is output.png." << std::endl;
	//cv::imshow("Pic", matrix);
}

void Matrix::rotate(const double degree)
{
	cv::Mat* temp = new cv::Mat(matrix);

	cv::Point2f center((temp->cols - 1) / 2.0, (temp->rows - 1) / 2.0);
	cv::Mat rot = cv::getRotationMatrix2D(center, degree, 1.0);

	cv::Rect2f bbox = cv::RotatedRect(cv::Point2f(), temp->size(), degree).boundingRect2f();

	rot.at<double>(0, 2) += bbox.width / 2.0 - temp->cols / 2.0;
	rot.at<double>(1, 2) += bbox.height / 2.0 - temp->rows / 2.0;

	cv::warpAffine(*temp, matrix, rot, bbox.size());

	std::cout << "Done: rotate " << degree << " degrees." << std::endl;
}

void Matrix::togray()
{
	cv::Mat temp = matrix.clone();
	cv::cvtColor(temp, matrix, cv::COLOR_BGR2GRAY);
	std::cout << "Done: convert to gray picture." << std::endl;
}

void Matrix::tobin()
{
	togray();
	cv::Mat temp = matrix.clone();
	cv::threshold(temp, matrix, 15, 100.0, 8);

	std::cout << "Done: convert to binary picture." << std::endl;
}

void Matrix::equalizeHist()
{
	cv::Mat RGB[3];
	cv::Mat temp = matrix.clone();

	cv::split(temp, RGB);
	for (int i = 0; i < 3; ++i) {
		cv::equalizeHist(RGB[i], RGB[i]);
	}
	cv::merge(RGB, 3, matrix);

	std::cout << "Done: equalize." << std::endl;
}

void Matrix::linear(double alpha, int beta)
{
	cv::Mat temp = matrix.clone();
	temp.convertTo(matrix, -1, alpha, beta);
}

void Matrix::face()
{
	cv::Mat temp = matrix.clone();
	cv::Mat gray;
	cv::cvtColor(temp, gray, cv::COLOR_BGR2GRAY);
	cv::equalizeHist(gray, gray);

	cv::CascadeClassifier cascade;
	cascade.load("haarcascade_frontalface_alt.xml");
	std::vector<cv::Rect> faces;
	cascade.detectMultiScale(gray, faces, 1.1, 2, 0 | cv::CASCADE_SCALE_IMAGE, cv::Size(30, 30));

	for (auto it = faces.begin(); it != faces.end(); ++it) {
		cv::rectangle(matrix, *it, cv::Scalar(0, 0, 255), 3, 4, 0);
	}
}