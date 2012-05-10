/*
 * wxMouseEvent.cpp
 *
 *  Created on: 21.04.2012
 *      Author: hanmac
 */

#include "wxEvent.hpp"
#include "wxMouseEvent.hpp"

#include "wxEvtHandler.hpp"

#define _self wrap<wxMouseEvent*>(self)

VALUE rb_cWXMouseEvent;

namespace RubyWX {
namespace MouseEvent {

}
}


DLL_LOCAL void Init_WXMouseEvent(VALUE rb_mWX)
{
	using namespace RubyWX::MouseEvent;
	rb_cWXMouseEvent = rb_define_class_under(rb_cWXEvent,"Mouse",rb_cWXEvent);

	registerEventType("left_down", wxEVT_LEFT_DOWN, rb_cWXMouseEvent);
	registerEventType("left_up", wxEVT_LEFT_UP, rb_cWXMouseEvent);
	registerEventType("middle_down", wxEVT_MIDDLE_DOWN, rb_cWXMouseEvent);
	registerEventType("middle_up", wxEVT_MIDDLE_UP, rb_cWXMouseEvent);
	registerEventType("right_down", wxEVT_RIGHT_DOWN, rb_cWXMouseEvent);
	registerEventType("right_up", wxEVT_RIGHT_UP, rb_cWXMouseEvent);
	registerEventType("motion", wxEVT_MOTION, rb_cWXMouseEvent);
	registerEventType("enter_window", wxEVT_ENTER_WINDOW, rb_cWXMouseEvent);
	registerEventType("leave_window", wxEVT_LEAVE_WINDOW, rb_cWXMouseEvent);
	registerEventType("left_dclick", wxEVT_LEFT_DCLICK, rb_cWXMouseEvent);
	registerEventType("middle_dclick", wxEVT_MIDDLE_DCLICK, rb_cWXMouseEvent);
	registerEventType("right_dclick", wxEVT_RIGHT_DCLICK, rb_cWXMouseEvent);
//	registerEventType("set_focus", wxEVT_SET_FOCUS, wxFocusEvent);
//	registerEventType("kill_focus", wxEVT_KILL_FOCUS, wxFocusEvent);
//	registerEventType("child_focus", wxEVT_CHILD_FOCUS, wxChildFocusEvent);
	registerEventType("mousewheel", wxEVT_MOUSEWHEEL, rb_cWXMouseEvent);
	registerEventType("aux1_down", wxEVT_AUX1_DOWN, rb_cWXMouseEvent);
	registerEventType("aux1_up", wxEVT_AUX1_UP, rb_cWXMouseEvent);
	registerEventType("aux1_dclick", wxEVT_AUX1_DCLICK, rb_cWXMouseEvent);
	registerEventType("aux2_down", wxEVT_AUX2_DOWN, rb_cWXMouseEvent);
	registerEventType("aux2_up", wxEVT_AUX2_UP, rb_cWXMouseEvent);
	registerEventType("aux2_dclick", wxEVT_AUX2_DCLICK, rb_cWXMouseEvent);


}


