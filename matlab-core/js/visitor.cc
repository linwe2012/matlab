#include "visitor.h"

using namespace v8;

#define LIST(V) \
V(String)       \
V(StringObject) \
V(BigInt)\
V(BigIntObject)\
V(Symbol)\
V(SymbolObject)\
V(Number)\
V(NumberObject) \
V(Boolean)\
V(BooleanObject) \
V(Array)\
V(Proxy)\
V(Date)\
V(Function)\
V(RegExp)\
V(Object)


#define VISIT(t) \
if(v->Is##t()) {\
	Visit(Local<t>::Cast(v));\
	return;\
}

void JSVisitor::Dispatch(v8::Isolate* i, Local<v8::Value> v)
{
	auto c = i->GetCurrentContext();
	if (v->IsNullOrUndefined()) {
		Visit(nullptr);
		return;
	}

	LIST(VISIT);

}
