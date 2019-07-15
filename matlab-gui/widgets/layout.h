#pragma once
#include "matlab-gui.h"
#include "qpushbutton.h"
#include "ribbon/ribbon.h"
#include "widget-helper.h"
#include "button.h"
#include "slider.h"
#include "checkbox.h"

class Layout {
public:
	Layout() {

	}
	struct RibbonProxy {
		RibbonProxy() {
			ribbon = new Ribbon;
			ribbon->setParent(GetTargetWidget());
			ribbon->setGeometry(QRect(0, 0, 1000, 120));
		}
		void Add(Button* n, const char* tab_name, const char* group_name) {
			ribbon->setParent(GetTargetWidget());
			ribbon->addButton(
				tab_name,
				group_name,
				n
			);
			ribbon->show();
		}

		void AddSlider(Slider* widget, const char* tab_name, const char* group_name) {
			ribbon->addLayout(
				tab_name,
				group_name,
				widget->GetLayout()
			);
			
			ribbon->show();
		}

		//TODO: support variadic arguments
		template<typename T>
		std::tuple<T*, std::string, std::string> ParseArguments(const FunctionCallbackInfo<Value>& args) {
			int len = args.Length();
			Isolate* isolate = args.GetIsolate();
			if (len != 3) {
				ArgError(args, "Expected at least 2 arguments");
				return { nullptr, std::string{""}, std::string{""} };
			}

			auto obj = args[0]->ToObject(isolate->GetCurrentContext()).ToLocalChecked();
			T* w = static_cast<T*>(
				obj->GetAlignedPointerFromInternalField(0));

			std::string tab_name = v8pp::from_v8<std::string>(isolate, args[1], "");
			std::string group_name = v8pp::from_v8<std::string>(isolate, args[2], "");

			return { w, tab_name, group_name };
		}
		
		void AddWild(const FunctionCallbackInfo<Value>& args) {
			// int len = args.Length();
			// Isolate* isolate = args.GetIsolate();
			// 
			// if (len != 3) {
			// 	return ArgError(args, "Expected at least 2 arguments");
			// }
			// auto obj = args[0]->ToObject(isolate->GetCurrentContext()).ToLocalChecked();
			// QWidget* qw = static_cast<QWidget*>(
			// 	obj->GetAlignedPointerFromInternalField(0));

			//std::string tab_name = v8pp::from_v8<std::string>(isolate, args[1], "");
			//std::string group_name = v8pp::from_v8<std::string>(isolate, args[2], "");
			
			auto [qw, tab_name, group_name] = ParseArguments<QWidget>(args);
			if (qw == nullptr) {
				return;
			}
			ribbon->addWidget(
				tab_name.c_str(),
				group_name.c_str(),
				qw
			);

			ribbon->show();
		}

		void AddGroup(const FunctionCallbackInfo<Value>& args) {
			auto [qw, tab_name, group_name] = ParseArguments<QWidget>(args);
			if (qw == nullptr) {
				return;
			}
			ribbon->addLayout(
				tab_name.c_str(),
				group_name.c_str(),
				qw->layout()
			);
		}

		Ribbon* ribbon;
	};

	struct Window {
		Window(const FunctionCallbackInfo<Value>& args) {
			central_ = new QWidget;
			
			Isolate* isolate = args.GetIsolate();
			js_self_ = args.GetReturnValue().Get()->ToObject(isolate->GetCurrentContext()).ToLocalChecked();
			js_self_->Set(
				MakeStr(isolate, "ribbon"),
				v8pp::class_<RibbonProxy>::create_object(isolate)
			);
		}

		void Hide() {
			central_->hide();
		}

		void Show() {
			central_->show();
		}

		void AddWild(const FunctionCallbackInfo<Value>& args) {
			Isolate* isolate = args.GetIsolate();
			Local<Object> obj =  args[0]->ToObject(isolate->GetCurrentContext()).ToLocalChecked();
			QWidget* widget = static_cast<QWidget*>(obj->GetAlignedPointerFromInternalField(0));
			widget->setParent(central_);
			widget->show();
		}

		static std::map<std::string, Window*> windows_;

		QWidget* central_;
		Local<Object> js_self_;
		static Local<Object> js_super_;
	};
	static void Init(Local<Object> mod, V8Shell* shell) {
		Isolate* isolate = shell->GetIsolate();

		v8pp::class_<RibbonProxy> ribbon(isolate);
		
		ribbon
			.set("add", &RibbonProxy::Add)
			.set("addSlider", &RibbonProxy::AddSlider)
			.set("addWild", &RibbonProxy::AddWild)
			.set("addGroup", &RibbonProxy::AddGroup)
			;

		mod->Set(
			MakeStr(isolate, "ribbon"),
			ribbon.create_object(isolate)
		);

		v8pp::class_<Window> window(isolate);
		window
			.ctor<const FunctionCallbackInfo<Value>&>()
			.set("hide", &Window::Hide)
			.set("show", &Window::Show)
			.set("addWild", &Window::AddWild);

		mod->Set(
			MakeStr(isolate, "Window"),
			window.class_function_template()->GetFunction(isolate->GetCurrentContext()).ToLocalChecked()
		);
	}


	Ribbon* ribbon_;
};