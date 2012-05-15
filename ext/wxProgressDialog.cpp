/*
 * wxProgressDialog.cpp
 *
 *  Created on: 11.03.2012
 *      Author: hanmac
 */

#include "wxProgressDialog.hpp"

VALUE rb_cWXProgressDialog;
#if wxUSE_PROGRESSDLG
#define _self wrap<RubyProgressDialog*>(self)

#if wxUSE_TIMER
void RubyProgressDialog::onTimer(wxTimerEvent &evt)
{
	rb_thread_schedule();
}
#endif

namespace RubyWX {
namespace ProgressDialog {

macro_attr(Range,int)

singlereturn(GetValue)
singlereturn(GetMessage)

APP_PROTECT(RubyProgressDialog)

#if wxUSE_TIMER
struct rt_holder
{
	rt_holder(VALUE b,VALUE p) : block(b),pd(p) {};
	VALUE block;
	VALUE pd;
};

VALUE thread_code(rt_holder *val)
{
	rb_gc_disable();
	rb_funcall(val->block,rb_intern("call"),1,val->pd);
	rb_gc_enable();
	return Qnil;
}
#endif

VALUE _initialize(int argc,VALUE *argv,VALUE self)
{
	VALUE parent,hash;
	rb_scan_args(argc, argv, "11",&parent,&hash);
	if(!NIL_P(parent))
		_self->Reparent(wrap<wxWindow*>(parent));
	_created = true;
	rb_call_super(argc,argv);
#if wxUSE_TIMER
	if(rb_block_given_p())
	{
		_self->mTimer = new wxTimer;
#ifdef wxHAS_EVENT_BIND
		_self->mTimer->Bind(wxEVT_TIMER,wxTimerEventHandler(RubyProgressDialog::onTimer),_self,_self->mTimer->GetId());
#else
		_self->mTimer->Connect(_self->mTimer->GetId(),wxEVT_TIMER,wxTimerEventHandler(RubyProgressDialog::onTimer),NULL,_self);
#endif
		_self->mTimer->Start(10);
		rb_thread_create((VALUE (*)(ANYARGS))&thread_code,new rt_holder(rb_block_proc(),self));
	}
#endif
	return self;
}

VALUE _update(int argc,VALUE *argv,VALUE self)
{
	VALUE val,message;
	rb_scan_args(argc, argv, "11",&val,&message);
#if wxUSE_TIMER

	if(!_self->mTimer && NUM2INT(val) >= _self->GetRange())
		_self->mTimer->Stop();
#endif
	return wrap(_self->Update(NUM2INT(val),wrap<wxString>(message)));
}

VALUE _pulse(int argc,VALUE *argv,VALUE self)
{
	VALUE message;
	rb_scan_args(argc, argv, "01",&message);

	return wrap(_self->Pulse(wrap<wxString>(message)));
}

#endif
}
}

DLL_LOCAL void Init_WXProgressDialog(VALUE rb_mWX)
{
#if wxUSE_PROGRESSDLG
	using namespace RubyWX::ProgressDialog;
	rb_cWXProgressDialog = rb_define_class_under(rb_mWX,"ProgressDialog",rb_cWXDialog);
	rb_define_alloc_func(rb_cWXProgressDialog,_alloc);
	rb_define_method(rb_cWXProgressDialog,"initialize",RUBY_METHOD_FUNC(_initialize),-1);

	rb_define_method(rb_cWXProgressDialog,"update",RUBY_METHOD_FUNC(_update),-1);
	rb_define_method(rb_cWXProgressDialog,"pulse",RUBY_METHOD_FUNC(_pulse),-1);

	registerInfo<wxProgressDialog>(rb_cWXProgressDialog);
	registerInfo<RubyProgressDialog>(rb_cWXProgressDialog);
#endif
}
