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
#ifdef wxHAS_EVENT_BIND
		_self->Bind(wxEVT_TIMER,RubyFunctor(proc),_self->GetId());
#else
		_self->Connect(_self->GetId(),wxEVT_TIMER,wxTimerEventHandler(RubyFunctor::operator()),NULL,new RubyFunctor(proc));
#endif
	}

	return self;
}

DLL_LOCAL VALUE _start(VALUE self,VALUE msec)
{
	return wrap(_self->Start(NUM2UINT(msec)));
}

singlefunc(Stop)
singlereturn(GetOwner)

DLL_LOCAL VALUE _setOwner(VALUE self,VALUE val)
{
	_self->SetOwner(unwrap<wxEvtHandler*>(val),_self->GetId());
	return val;
}

}
}

#endif

DLL_LOCAL void Init_WXTimer(VALUE rb_mWX)
{
#if wxUSE_TIMER
	using namespace RubyWX::Timer;

	rb_cWXTimer = rb_define_class_under(rb_mWX,"Timer",rb_cObject);
	rb_define_alloc_func(rb_cWXTimer,_alloc);

	rb_define_method(rb_cWXTimer,"initialize",RUBY_METHOD_FUNC(_initialize),0);

	rb_include_module(rb_cWXTimer,rb_mWXEvtHandler);

	rb_define_method(rb_cWXTimer,"start",RUBY_METHOD_FUNC(_start),1);
	rb_define_method(rb_cWXTimer,"stop",RUBY_METHOD_FUNC(_Stop),0);

	rb_define_attr_method(rb_cWXTimer,"owner",_GetOwner,_setOwner);

	rb_cWXTimerEvent = rb_define_class_under(rb_cWXEvent,"Timer",rb_cWXEvent);
	registerEventType("timer",wxEVT_TIMER,rb_cWXTimerEvent);

	//rb_define_attr_method(rb_cWXFileDirPickerEvent,"path",
	//		Event::_getPath,Event::_setPath);

	registerInfo<wxTimer>(rb_cWXTimer);
#endif
}

