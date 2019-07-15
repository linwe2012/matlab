#include "matrix.h"

namespace fs = std::filesystem;

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
		.set("getColor", &Matrix::v8_clone)
		.set("setColor", &Matrix::v8_clone)
		.set("getRows", &Matrix::v8_clone)
		.set("getCols", &Matrix::v8_clone)
		.set("fill", &Matrix::fill)
		.set("conv", &Matrix::v8_conv)
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
	auto obj = (*gMatrixClass).create_object(
		isolate, (*gMatrixClass).unwrap_object(args.GetIsolate(), args.This())->matrix.clone(),
		shell_
	);
	Local<Array> keys = args.This()->GetOwnPropertyNames(isolate->GetCurrentContext()).ToLocalChecked();
	int len = keys->Length();
	for (int i = 0; i < len; ++i) {

		auto key = v8pp::from_v8<Local<String>>(isolate, keys->Get(i));
		auto value = obj->Get(isolate->GetCurrentContext(), keys->Get(i));

		obj->Set(
			isolate->GetCurrentContext(),
			keys->Get(i),
			args.This()->Get(isolate->GetCurrentContext(), keys->Get(i)).ToLocalChecked()
		);
	}
	args.GetReturnValue().Set(
		obj
	);
}

cv::Vec3b Matrix::v8_getColor(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	int row = v8pp::from_v8<int>(args.GetIsolate(), args[0], INT_MAX);
	int col = v8pp::from_v8<int>(args.GetIsolate(), args[1], INT_MAX);

	return getColor(row, col);
}

int Matrix::v8_getRows(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	return getRows();
	return_this(args);
}

int Matrix::v8_getCols(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	return getCols();
	return_this(args);
}

void Matrix::v8_conv(Matrix* mat)
{
	conv(mat->matrix);
}

void Matrix::fill(std::vector<std::vector<float>> mat)
{
	matrix = cv::Mat(mat.size(), mat[0].size(), CV_32FC1);

	for (int i = 0; i < matrix.rows; ++i)
		for (int j = 0; j < matrix.cols; ++j)
			matrix.at<float>(i, j) = mat.at(i).at(j);
}


void Matrix::resize(const std::vector<int>& dims)
{
	cv::Mat temp = matrix.clone();
	cv::resize(temp, matrix, cv::Size(temp.cols * dims.at(0), temp.rows * dims.at(1)), 0, 0, cv::INTER_LINEAR);
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
	/*
	cv::Mat* temp = new cv::Mat(matrix);
	auto oldrows = temp->rows;
	auto oldcols = temp->cols;

	cv::Point2f center((temp->cols - 1) / 2.0, (temp->rows - 1) / 2.0);
	cv::Mat rot = cv::getRotationMatrix2D(center, degree, 1.0);

	cv::Rect2f bbox = cv::RotatedRect(cv::Point2f(), temp->size(), degree).boundingRect2f();

	rot.at<double>(0, 2) += bbox.width / 2.0 - temp->cols / 2.0;
	rot.at<double>(1, 2) += bbox.height / 2.0 - temp->rows / 2.0;

	cv::warpAffine(*temp, matrix, rot, bbox.size());

	cv::Mat beforeResize = matrix.clone();
	cv::resize(beforeResize, matrix, cv::Size(oldcols, oldrows), 0, 0, cv::INTER_LINEAR);

	std::cout << "Done: rotate " << degree << " degrees." << std::endl;
	*/

	
	cv::Mat moveImage(matrix.rows, matrix.cols, CV_8UC3, cv::Scalar(0, 0, 0));
	cv::Point2f center(matrix.cols/2, matrix.rows/2);
	cv::Mat M = getRotationMatrix2D(center, degree, 1);
	cv::warpAffine(matrix, moveImage, M, cv::Size(matrix.cols, matrix.rows));
	cv::circle(moveImage, center, 2, cv::Scalar(255,0,0));
	matrix = moveImage;
	

	/*
	cv::Mat outimg;
	int rotatedWidth = ceil(matrix.rows * fabs(sin(degree * CV_PI / 180)) + matrix.cols * fabs(cos(degree * CV_PI / 180)));
	int rotatedHeight = ceil(matrix.cols * fabs(sin(degree * CV_PI / 180)) + matrix.rows * fabs(cos(degree * CV_PI / 180)));

	cv::Point2f center(matrix.cols / 2, matrix.rows / 2);
	cv::Mat rotateMatrix = getRotationMatrix2D(center, 45, 1.0);

	rotateMatrix.at<double>(0, 2) += (rotatedWidth - matrix.cols) / 2;
	rotateMatrix.at<double>(1, 2) += (rotatedHeight - matrix.rows) / 2;
	
	cv::warpAffine(matrix, outimg, rotateMatrix, cv::Size(rotatedWidth, rotatedHeight), cv::INTER_LINEAR, 0, cv::Scalar(255, 255, 255));
	matrix = outimg;
	*/
	std::cout << "Done: rotate " << degree << " degrees." << std::endl;
}

void Matrix::togray()
{	
	if (matrix.type() == CV_8UC3) {
		cv::Mat temp = matrix.clone();
		cv::cvtColor(temp, matrix, cv::COLOR_BGR2GRAY);
		std::cout << "Done: convert to gray picture." << std::endl;
	} else {
		std::cout << "Err: the image is already a gray one." << std::endl;
	}
}

void Matrix::tobin()
{
	if (matrix.type() != CV_8UC1) {
		togray();
	}
	cv::Mat temp = matrix.clone();
	cv::threshold(temp, matrix, 50, 150.0, 8);

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

void Matrix::conv(cv::Mat& ker)
{
	cv::Mat temp = matrix.clone();
	cv::filter2D(temp, matrix, -1, ker);
}

void Matrix::setColor(int x, int y, cv::Vec3b& color)
{
	matrix.at<cv::Vec3b>(x, y)[0] = color[0]; // B
	matrix.at<cv::Vec3b>(x, y)[1] = color[1]; // G
	matrix.at<cv::Vec3b>(x, y)[2] = color[2]; // R
}

cv::Vec3b Matrix::getColor(int x, int y)
{
	cv::Vec3b color;
	color[0] = matrix.at<cv::Vec3b>(x, y)[0]; // B
	color[1] = matrix.at<cv::Vec3b>(x, y)[1]; // G
	color[2] = matrix.at<cv::Vec3b>(x, y)[2]; // R
	return color;
}

int Matrix::getRows()
{
	return matrix.cols;
}

int Matrix::getCols()
{
	return matrix.cols;
}