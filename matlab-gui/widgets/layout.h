#pragma once
#include "matlab-gui.h"
#include "qpushbutton.h"
#include "ribbon/ribbon.h"
#include "widget-helper.h"
#include "button.h"
#include "slider.h"

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
			//ribbon->setParent(GetTargetWidget());
			/*
			for (auto w : widget->GetAllWidget()) {
				ribbon->addWidget(
					tab_name,
					group_name,
					w
				);
			}
			*/
			ribbon->addLayout(
				tab_name,
				group_name,
				widget->GetLayout()
			);
			
			ribbon->show();
		}

		//TODO: support variadic arguments
		void AddWild(const FunctionCallbackInfo<Value>& args) {
			int len = args.Length();
			Isolate* isolate = args.GetIsolate();

			if (len != 3) {
				return ArgError(args, "Expected at least 2 arguments");
			}

			QWidget* qw = static_cast<QWidget*>(
			args.This()->GetAlignedPointerFromInternalField(0));

			std::string tab_name = v8pp::from_v8<std::string>(isolate, args[1], "");
			std::string group_name = v8pp::from_v8<std::string>(isolate, args[2], "");

			ribbon->addWidget(
				tab_name.c_str(),
				group_name.c_str(),
				qw
			);

			ribbon->show();
		}


		Ribbon* ribbon;
	};
	static void Init(Local<Object> mod, V8Shell* shell) {
		Isolate* isolate = shell->GetIsolate();

		v8pp::class_<RibbonProxy> ribbon(isolate);
		ribbon
			.set("add", &RibbonProxy::Add)
			.set("addSlider", &RibbonProxy::AddSlider)
			.set("addWild", &RibbonProxy::AddWild)
			;

		mod->Set(
			MakeStr(isolate, "ribbon"),
			ribbon.create_object(isolate)
		);
	}


	Ribbon* ribbon_;
};