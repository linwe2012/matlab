#pragma once
#include <QFileDialog>
#include <qtreewidget.h>
#include <QStandardItemModel>
#include "../matlab-core/matlab.h"
#include "widget-helper.h"
#include "registry.h"
#include <QSplitter>

class JSObjVeiwer : public QTreeView {
public:
	template<typename T>
	using Local = v8::Local<T>;

	using Value = v8::Value;
	static void Init(Local<v8::Object> mod, V8Shell* shell);


	JSObjVeiwer(const FunctionCallbackInfo<Value>& args);

	void Update() {
		QStandardItem* item = new QStandardItem(QStringLiteral(""));

	}
	class JSDispayer : public SimpleJSVisitor {
	public:
		template<typename T>
		using Local = v8::Local<T>;

		void SetRight(const char* s) {
			if (parent) {
				parent->setChild(
					current->index().row(), 1,
					new QStandardItem(s)
				);
			}
		}

		void Visit(nullptr_t) override {
			SetRight("null");
		}

		void Visit(const char* c) override {
			SetRight(c);
		}

		void Visit(Local<v8::BigInt> v) override {
			String::Utf8Value utf8(isolate, v);
			SetRight(*utf8);
		}

		void Visit(double d) override {
			std::string str = std::to_string(d);
			SetRight(str.c_str());
		}

		void Visit(bool v) override {
			SetRight(v ? "true" : "false");
		}

		void Visit(Local<v8::Array> v) override {
			int len = v->Length();
			std::string r = "<array> size: " + std::to_string(len);
			SetRight(r.c_str());
			QStandardItem* last_parent = parent;

			for (int i = 0; i < len; ++i) {
				Local<Value> value = v->Get(i);
				std::string arr_key = std::to_string(i);
				current = new QStandardItem(arr_key.c_str());
				parent->appendRow(current);
				row_id = i;
				dispatcher_->Dispatch(isolate, value);
			}
		}

		void Visit(Local<v8::Object> v) override {
			MaybeLocal<Array> arr_maybe = v->GetOwnPropertyNames(context);
			if (arr_maybe.IsEmpty()) {
				return;
			}
			SetRight("<object>");
			Local<Array> arr = arr_maybe.ToLocalChecked();
			int len = arr->Length();
			QStandardItem* last_parent = parent;
			parent = current;
			for (int i = 0; i < len; ++i) {
				Local<Value> key = arr->Get(i);

				MaybeLocal<String> key_str_maybe = key->ToString(context);
				if (key_str_maybe.IsEmpty()) {
					current = new QStandardItem("<unkown>");
					parent->appendRow(current);
					continue;
				}
				String::Utf8Value utf8(isolate, key_str_maybe.ToLocalChecked());
				current = new QStandardItem(*utf8);

				MaybeLocal value_maybe = v->Get(context, key);
				if (value_maybe.IsEmpty()) {
					SetRight("<unkown>");
				}

				Local<Value> value = value_maybe.ToLocalChecked();

				parent->appendRow(current);
				row_id = i;
				dispatcher_->Dispatch(isolate, value);
			}
			
			current = parent;
			parent = last_parent;
		}

		QStandardItem* parent = nullptr;
		QStandardItem* current;
		
		int row_id = 0;;
		Local<Context> context;
		Isolate* isolate = nullptr;
		SimpleJSVisitorDispatcher* dispatcher_ = nullptr;
	};

	void DisplayGlobal();
	
	void RefreshAll();

	struct TreeNode {
		QStandardItem* self;
		QStandardItem* parent;
		std::map<Local<Value>, TreeNode*> childrens;
	};

	
private:

	v8::Isolate* isolate;
	QStandardItemModel* model_ = nullptr;
	JSDispayer* displayer_ = nullptr;
	SimpleJSVisitorDispatcher* dispater_ = nullptr;
};

inline bool operator<(const v8::Local<v8::Value>& l, const v8::Local<v8::Value>& r) {
	return l.operator->() < r.operator->();
}