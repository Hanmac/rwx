/*
 * wxEvent.cpp
 *
 *  Created on: 18.02.2012
 *      Author: hanmac
 */



#include "wxEvent.hpp"
#include "wxWindow.hpp"

#define _self wrap<wxEvent*>(self)

VALUE rb_cWXEvent;

namespace RubyWX {
namespace Event {

macro_attr_with_func(Id,wrapID,unwrapID)
macro_attr_with_func(Timestamp,LONG2NUM,NUM2LONG)

}
}


void Init_WXEvent(VALUE rb_mWX)
{
	using namespace RubyWX::Event;
	rb_cWXEvent = rb_define_class_under(rb_mWX,"Event",rb_cObject);
	rb_undef_alloc_func(rb_cWXEvent);
	rb_define_attr_method(rb_cWXEvent,"id",_getId,_setId);
	rb_define_attr_method(rb_cWXEvent,"timestamp",_getTimestamp,_setTimestamp);
}


