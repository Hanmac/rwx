/*
 * wxTimer.cpp
 *
 *  Created on: 11.03.2012
 *      Author: hanmac
 */

#include "wxEvtHandler.hpp"
#include "wxTimer.hpp"
#include "wxApp.hpp"

VALUE rb_cWXTimer,rb_cWXTimerEvent;
#if wxUSE_TIMER
#define _self unwrap<wxTimer*>(self)

namespace RubyWX {
namespace Timer {

APP_PROTECT(wxTimer)

DLL_LOCAL VALUE _initialize(VALUE self)
{
	if(rb_block_given_p()){
		VALUE proc = rb_block_proc();

		_self->Bind(wxEVT_TIMER,RubyFunctor(proc),_self->GetId());

	}

	return self;
}

DLL_LOCAL VALUE _start(VALUE self,VALUE msec)
{
	return wrap(_self->Start(NUM2UINT(msec)));
}

singlefunc(Stop)
singlereturn(GetOwner)

singlereturn(GetInterval)
singlereturn(IsRunning)

DLL_LOCAL VALUE _setOwner(VALUE self,VALUE val)
{
	_self->SetOwner(unwrap<wxEvtHandler*>(val),_self->GetId());
	return val;
}

namespace Event {

#undef _self
#define _self unwrapTypedPtr<wxTimerEvent>(self,rb_cWXTimerEvent)

singlereturn(GetInterval)

}

}
}

#endif



DLL_LOCAL void Init_WXTimer(VALUE rb_mWX)
{
#if 0
	rb_mWXEvtHandler = rb_define_module_under(rb_mWX,"EvtHandler");
#endif

#if wxUSE_TIMER
	using namespace RubyWX::Timer;

	rb_cWXTimer = rb_define_class_under(rb_mWX,"Timer",rb_cObject);
	rb_define_alloc_func(rb_cWXTimer,_alloc);

#if 0
	rb_define_attr(rb_cWXTimer,"owner",1,1);
#endif

	rb_undef_method(rb_cWXTimer,"initialize_copy");
	rb_undef_method(rb_cWXTimer,"_load");
	rb_undef_method(rb_cWXTimer,"_dump");

	rb_define_method(rb_cWXTimer,"initialize",RUBY_METHOD_FUNC(_initialize),0);

	rb_include_module(rb_cWXTimer,rb_mWXEvtHandler);

	rb_define_method(rb_cWXTimer,"start",RUBY_METHOD_FUNC(_start),1);
	rb_define_method(rb_cWXTimer,"stop",RUBY_METHOD_FUNC(_Stop),0);

	rb_define_method(rb_cWXTimer,"running?",RUBY_METHOD_FUNC(_IsRunning),0);

	rb_define_method(rb_cWXTimer,"interval",RUBY_METHOD_FUNC(_GetInterval),0);

	rb_define_attr_method(rb_cWXTimer,"owner",_GetOwner,_setOwner);

	rb_cWXTimerEvent = rb_define_class_under(rb_cWXEvent,"Timer",rb_cWXEvent);

	rb_define_method(rb_cWXTimerEvent,"interval",RUBY_METHOD_FUNC(Event::_GetInterval),0);

	registerEventType("timer",wxEVT_TIMER,rb_cWXTimerEvent);

	registerInfo<wxTimer>(rb_cWXTimer);
#endif
}

