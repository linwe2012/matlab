#pragma once
#include <QFileDialog>
#include <qtreewidget.h>
#include <QStandardItemModel>
#include "../matlab-core/matlab.h"

class JSObjVeiwer : public QTreeWidget {
	Q_OBJECT
public:
	template<typename T>
	using Local = v8::Local<T>;

	using Value = v8::Value;

	JSObjVeiwer() {
		model_ = new QStandardItemModel(this);
		model_->setHorizontalHeaderLabels(QStringList() << QStringLiteral("Name") << QStringLiteral("Data"));
		
	}
	void Update() {
		QStandardItem* item = new QStandardItem(QStringLiteral(""));

	}
	class JSDispayer : public SimpleJSVisitor {
	public:
		template<typename T>
		using Local = v8::Local<T>;

		void SetRight(const char* s) {
			parent->setChild(
				current->index().row(),
				new QStandardItem(s)
			);
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
			MaybeLocal<Array> arr_maybe = v->GetPropertyNames(context);
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
				Local<Value> value = v->Get(context, key).ToLocalChecked();

				MaybeLocal<String> key_str_maybe = key->ToString(context);
				if (key_str_maybe.IsEmpty()) {
					current = new QStandardItem("<unkown>");
					parent->appendRow(current);
					continue;
				}
				
				String::Utf8Value utf8(isolate, key_str_maybe.ToLocalChecked());
				current = new QStandardItem(*utf8);
				parent->appendRow(current);
				row_id = i;
				dispatcher_->Dispatch(isolate, value);
			}
			current = parent;
			parent = last_parent;
		}

		void Init(Local<Object> v) {

		}
		QStandardItem* parent;
		QStandardItem* current;
		
		int row_id;
		Local<Context> context;
		Isolate* isolate;
		SimpleJSVisitorDispatcher* dispatcher_;
	};

	void Display(v8::Local<v8::Value>) {
		
	}
	
	
	
	struct TreeNode {
		QStandardItem* self;
		QStandardItem* parent;
		std::map<Local<Value>, TreeNode*> childrens;
	};

private slots:


	QStandardItemModel* model_;
};

bool operator<(const v8::Local<v8::Value>& l, const v8::Local<v8::Value>& r) {
	return l.operator->() < r.operator->();
}