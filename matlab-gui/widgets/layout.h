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

		void AddWild(QWidget* w, const char* tab_name, const char* group_name) {
			ribbon->addWidget(
				tab_name,
				group_name,
				w
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