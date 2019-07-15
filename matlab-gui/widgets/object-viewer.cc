#include "object-viewer.h"

void JSObjVeiwer::Init(Local<v8::Object> mod, V8Shell* shell) {
	using namespace v8;
	v8pp::class_<JSObjVeiwer> obj_viewer(shell->GetIsolate());
	obj_viewer
		.ctor<const FunctionCallbackInfo<Value>&>()
		.set("refreshAll", &JSObjVeiwer::RefreshAll)
		;
	mod->Set(
		MakeStr(shell->GetIsolate(), "ObjectViewer"),
		obj_viewer.js_function_template()->GetFunction(shell->GetIsolate()->GetCurrentContext()).ToLocalChecked());
}

JSObjVeiwer::JSObjVeiwer(const FunctionCallbackInfo<Value>& args)
{
	model_ = new QStandardItemModel(this);
	this->setParent(GetTargetWidget());
	model_->setHorizontalHeaderLabels(QStringList() << QStringLiteral("Name") << QStringLiteral("Data"));
	isolate = args.GetIsolate();
	DisplayGlobal();
	this->setGeometry(QRect(850, 120, 250, 800));
	this->show();
}

void JSObjVeiwer::DisplayGlobal() {
	if (model_ == nullptr) {
		model_ = new QStandardItemModel(this);
	}
	if (displayer_ == nullptr) {
		displayer_ = new JSDispayer;
		dispater_ = new SimpleJSVisitorDispatcher;
		dispater_->Bind(displayer_, isolate);
		displayer_->dispatcher_ = dispater_;
		displayer_->isolate = isolate;
	}

	displayer_->current = new QStandardItem("<global>");

	int rnct = model_->rowCount();
	model_->appendRow(displayer_->current);

	displayer_->context = isolate->GetCurrentContext();

	displayer_->Visit(displayer_->context->Global());

	this->setModel(model_);
}

void JSObjVeiwer::RefreshAll()
{
	//delete model_;
	delete displayer_;
	delete dispater_;
	//model_ = nullptr;
	displayer_ = nullptr;
	dispater_ = nullptr;
	DisplayGlobal();
}
