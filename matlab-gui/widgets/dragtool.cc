#include "dragtool.h"
#include "widget-helper.h"
#include "registry.h"

DragTool::DragTool(const FunctionCallbackInfo<Value>& args)
  : QToolButton(GetTargetWidget())
{

  INIT_OBJECT(args);

  GET(std::string, text);
  GET(Local<Function>, onpress);
  GET(Local<Function>, onmove);
  GET(Local<Function>, onrelease);
  GET_IF(std::string, icon, "");

  

  if (!icon.empty()) {
    this->setIcon(QIcon(icon.c_str()));
  }
  this->setText(text.c_str());
  auto sz = GetCurrentLayout().width();
  sz += this->size().width() + 20;
  GetCurrentLayout().setWidth(sz);
  this->setGeometry(QRect(sz, 530, 160, 22));
  this->setCheckable(true);

  text_ = text;
  onpress_ = onpress;
  onmove_ = onmove;
  onrelease_ = onrelease;

  connect(this, SIGNAL(clicked(bool)), this, SLOT(ClickSlot(bool)));
  GetImageView()->installEventFilter(this);

  isolate_ = isolate;
  context_ = isolate->GetCurrentContext();

  js_self_ = args.GetReturnValue().Get()->ToObject(context_).ToLocalChecked();
  js_self_->Set(MakeStr(isolate, "onpress_func"), (onpress));
  js_self_->Set(MakeStr(isolate, "onmove_func"), (onmove));
  js_self_->Set(MakeStr(isolate, "onrelease_func"), (onrelease));
}

bool DragTool::eventFilter(QObject* target, QEvent* event)
{
  if (this->isChecked()&&target== GetImageView())
  {
    js_self_ = v8pp::class_<DragTool>::find_object(isolate_, this);
    QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
    QPoint pos = mouseEvent->pos();
    Local<Integer> arg1 = Integer::New(isolate_, pos.x());
    Local<Integer> arg2 = Integer::New(isolate_, pos.y());
    Local<Value> argv[] = { arg1, arg2 };
    switch (event->type()) 
    {
    case QEvent::MouseButtonPress:
      onpress_ = Local<Function>::Cast(js_self_->Get(MakeStr(isolate_, "onpress_func")));
      onpress_->CallAsFunction(isolate_->GetCurrentContext(), js_self_, 2, argv);
      break;
    case QEvent::MouseButtonRelease:
      onrelease_ = Local<Function>::Cast(js_self_->Get(MakeStr(isolate_, "onrelease_func")));
      onrelease_->CallAsFunction(isolate_->GetCurrentContext(), js_self_, 2, argv);
      break;
    case QEvent::MouseMove:
      onmove_ = Local<Function>::Cast(js_self_->Get(MakeStr(isolate_, "onmove_func")));
      onmove_->CallAsFunction(isolate_->GetCurrentContext(), js_self_, 2, argv);
      break;
    default:
      break;
    }
  }
  return QToolButton::eventFilter(target, event);
}

void DragTool::ClickSlot(bool checked)
{
  if (checked)
    active_->setChecked(false);
  else
    active_ = nullptr;
}

Local<FunctionTemplate> DragTool::me_class_;
v8pp::class_<DragTool>* DragTool::self_class_ = nullptr;
DragTool* DragTool::active_ = nullptr;

void DragTool::Init(Local<Object> mod, V8Shell* shell)
{
  self_class_ = new
    v8pp::class_<DragTool>(shell->GetIsolate());
  self_class_
    ->ctor<const FunctionCallbackInfo<Value>&>()
    .set("text", &DragTool::text_)
    .set("onpress", &DragTool::onpress_)
    .set("onmove", &DragTool::onmove_)
    .set("onrelease", &DragTool::onrelease_);

  mod->Set(MakeStr(shell->GetIsolate(), "DragTool"), self_class_->js_function_template()->GetFunction(shell->GetIsolate()->GetCurrentContext()).ToLocalChecked());
}