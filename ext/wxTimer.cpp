/*
 * wxTimer.cpp
 *
 *  Created on: 11.03.2012
 *      Author: hanmac
 */

#include "wxEvtHandler.hpp"

VALUE rb_cWXTimer;
#if wxUSE_TIMER
#define _self wrap<wxTimer*>(self)

namespace RubyWX {
namespace Timer {

VALUE _alloc(VALUE self)
{
	if(ruby_app_inited)
		return getEvtObj(new wxTimer,self);
	else
		rb_raise(rb_eArgError,"%s is not running.",rb_class2name(rb_cWXApp));
	return Qnil;

}

VALUE _initialize(VALUE self)
{
	if(rb_block_given_p()){
		VALUE proc = rb_block_proc();
#ifdef wxHAS_EVENT_BIND
		_self->Bind(wxEVT_TIMER,RubyFunctor(proc),_self->GetId());
#else
		_self->Connect(_self->GetId(),wxEVT_TIMER,(wxObjectEventFunction)&RubyFunctor::operator(),NULL,new RubyFunctor(proc));
#endif
	}

	return self;
}

VALUE _start(VALUE self,VALUE msec)
{
	return wrap(_self->Start(NUM2UINT(msec)));
}

singlefunc(Stop)
singlereturn(GetOwner)

VALUE _setOwner(VALUE self,VALUE val)
{
	_self->SetOwner(wrap<wxEvtHandler*>(val),_self->GetId());
	return val;
}

}
}

#endif

void Init_WXTimer(VALUE rb_mWX)
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
#endif
}

