/*
 * wxEvent.cpp
 *
 *  Created on: 18.02.2012
 *      Author: hanmac
 */



#include "wxEvent.hpp"

#define _self wrap<wxEvent*>(self)

VALUE rb_cWXEvent;


namespace RubyWX {
namespace Event {

}
}


void Init_WXEvent(VALUE rb_mWX)
{
	using namespace RubyWX::Event;
	rb_cWXEvent = rb_define_class_under(rb_mWX,"Event",rb_cObject);
	rb_undef_alloc_func(rb_cWXEvent);
}


