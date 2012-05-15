/*
 * wxScrollEvent.cpp
 *
 *  Created on: 08.04.2012
 *      Author: hanmac
 */
#include "wxScrollEvent.hpp"
#include "wxEvtHandler.hpp"
#define _self wrap< wxScrollEvent* >(self)

VALUE rb_cWXScrollEvent;

namespace RubyWX {
namespace ScrollEvent {

macro_attr(Position,int)

}
}

void Init_WXScrollEvent(VALUE rb_mWX)
{
	using namespace RubyWX::ScrollEvent;

	rb_cWXScrollEvent = rb_define_class_under(rb_cWXEvent,"Scroll",rb_cWXEvent);

	registerEventType("scroll_top", wxEVT_SCROLL_TOP, rb_cWXScrollEvent);
	registerEventType("scroll_bottom", wxEVT_SCROLL_BOTTOM, rb_cWXScrollEvent);
	registerEventType("scroll_lineup", wxEVT_SCROLL_LINEUP, rb_cWXScrollEvent);
	registerEventType("scroll_linedown", wxEVT_SCROLL_LINEDOWN, rb_cWXScrollEvent);
	registerEventType("scroll_pageup", wxEVT_SCROLL_PAGEUP, rb_cWXScrollEvent);
	registerEventType("scroll_pagedown", wxEVT_SCROLL_PAGEDOWN, rb_cWXScrollEvent);
	registerEventType("scroll_thumbtrack", wxEVT_SCROLL_THUMBTRACK, rb_cWXScrollEvent);
	registerEventType("scroll_thumbrelease", wxEVT_SCROLL_THUMBRELEASE, rb_cWXScrollEvent);
	registerEventType("scroll_changed", wxEVT_SCROLL_CHANGED, rb_cWXScrollEvent);

	rb_define_attr_method(rb_cWXScrollEvent,"position",_getPosition,_setPosition);

}
