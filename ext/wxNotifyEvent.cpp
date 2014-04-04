/*
 * wxNotifyEvent.cpp
 *
 *  Created on: 08.11.2012
 *      Author: hanmac
 */

#include "wxEvent.hpp"
#include "wxWindow.hpp"

#define _self unwrap<wxNotifyEvent*>(self)

VALUE rb_cWXNotifyEvent;

namespace RubyWX {
namespace NotifyEvent {

singlefunc(IsAllowed)
singlefunc(Allow)
singlefunc(Veto)

}
}


DLL_LOCAL void Init_WXNotifyEvent(VALUE rb_mWX)
{
#if 0
	rb_mWX = rb_define_module("WX");
	rb_cWXEvent = rb_define_class_under(rb_mWX,"Event",rb_cObject);
	rb_cWXCommandEvent = rb_define_class_under(rb_cWXEvent,"Command",rb_cWXEvent);
#endif

	using namespace RubyWX::NotifyEvent;

	rb_cWXNotifyEvent = rb_define_class_under(rb_cWXEvent,"Notify",rb_cWXCommandEvent);

	rb_define_method(rb_cWXNotifyEvent,"allowed?",RUBY_METHOD_FUNC(_IsAllowed),0);
	rb_define_method(rb_cWXNotifyEvent,"allow",RUBY_METHOD_FUNC(_Allow),0);
	rb_define_method(rb_cWXNotifyEvent,"veto",RUBY_METHOD_FUNC(_Veto),0);

	registerInfo<wxNotifyEvent>(rb_cWXNotifyEvent);
}


