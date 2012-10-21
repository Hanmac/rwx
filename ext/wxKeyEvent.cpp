/*
 * wxKeyEvent.cpp
 *
 *  Created on: 21.04.2012
 *      Author: hanmac
 */

#include "wxEvent.hpp"
#include "wxKeyEvent.hpp"

#include "wxEvtHandler.hpp"

#define _self unwrap<wxKeyEvent*>(self)

VALUE rb_cWXKeyEvent;

namespace RubyWX {
namespace KeyEvent {

}
}


DLL_LOCAL void Init_WXKeyEvent(VALUE rb_mWX)
{
	using namespace RubyWX::KeyEvent;
	rb_cWXKeyEvent = rb_define_class_under(rb_cWXEvent,"Key",rb_cWXEvent);
}



