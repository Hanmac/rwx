/*
 * wxScrollEvent.cpp
 *
 *  Created on: 08.04.2012
 *      Author: hanmac
 */
#include "wxScrollEvent.hpp"
#include "wxCommandEvent.hpp"
#include "wxEvtHandler.hpp"
#define _self unwrap< wxScrollEvent* >(self)

VALUE rb_cWXScrollEvent;

namespace RubyWX {
namespace ScrollEvent {

macro_attr(Position,int)

}
}

DLL_LOCAL void Init_WXScrollEvent(VALUE rb_mWX)
{
#if 0
	rb_mWX = rb_define_module("WX");
	rb_cWXEvent = rb_define_class_under(rb_mWX,"Event",rb_cObject);
#endif
	using namespace RubyWX::ScrollEvent;

	rb_cWXScrollEvent = rb_define_class_under(rb_cWXEvent,"Scroll",rb_cWXCommandEvent);

	registerEventType<wxScrollEvent>("scroll_top", wxEVT_SCROLL_TOP, rb_cWXScrollEvent);
	registerEventType<wxScrollEvent>("scroll_bottom", wxEVT_SCROLL_BOTTOM, rb_cWXScrollEvent);
	registerEventType<wxScrollEvent>("scroll_lineup", wxEVT_SCROLL_LINEUP, rb_cWXScrollEvent);
	registerEventType<wxScrollEvent>("scroll_linedown", wxEVT_SCROLL_LINEDOWN, rb_cWXScrollEvent);
	registerEventType<wxScrollEvent>("scroll_pageup", wxEVT_SCROLL_PAGEUP, rb_cWXScrollEvent);
	registerEventType<wxScrollEvent>("scroll_pagedown", wxEVT_SCROLL_PAGEDOWN, rb_cWXScrollEvent);
	registerEventType<wxScrollEvent>("scroll_thumbtrack", wxEVT_SCROLL_THUMBTRACK, rb_cWXScrollEvent);
	registerEventType<wxScrollEvent>("scroll_thumbrelease", wxEVT_SCROLL_THUMBRELEASE, rb_cWXScrollEvent);
	registerEventType<wxScrollEvent>("scroll_changed", wxEVT_SCROLL_CHANGED, rb_cWXScrollEvent);

	rb_define_attr_method(rb_cWXScrollEvent,"position",_getPosition,_setPosition);
}
