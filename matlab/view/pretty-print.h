#pragma once

#include "js/shell.h"
#include "js/visitor.h"

class PrettyPrint : public JSVisitor {
public:
	void Visit(Local<v8::String> v) override {
		
	}



};