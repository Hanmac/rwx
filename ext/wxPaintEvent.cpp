/*
 * wxPaintEvent.cpp
 *
 *  Created on: 04.03.2016
 *      Author: hanmac
 */



#include "wxPaintEvent.hpp"
#include "wxEvent.hpp"
#include "wxWindow.hpp"

#include "wxDC.hpp"

VALUE rb_cWXPaintEvent;

template <>
wxPaintEvent* unwrap<wxPaintEvent*>(const VALUE &arg)
{
	return unwrapTypedPtr<wxPaintEvent>(arg,rb_cWXPaintEvent);
}


#define _self unwrap<wxPaintEvent*>(self)

namespace RubyWX {
namespace PaintEvent {


DLL_LOCAL VALUE _draw(VALUE self)
{
	wxDC *dc;

	wxWindow *win = dynamic_cast<wxWindow*>(_self->GetEventObject());
	
	if(!win)
	{
		rb_raise(rb_eArgError, "draw needs valid window");
		return Qnil;
	}

	wxPaintDC *mdc = new wxPaintDC(win);
	win->PrepareDC(*mdc);
#if wxUSE_GRAPHICS_CONTEXT
	dc = new wxGCDC(*mdc);
	win->PrepareDC(*dc);
#else
	dc = mdc;
#endif

	dc->Clear();
	rb_yield(wrap(dc));
	return self;
}

}
}


DLL_LOCAL void Init_WXPaintEvent(VALUE rb_mWX)
{
#if 0
	rb_mWX = rb_define_module("WX");
	rb_cWXEvent = rb_define_class_under(rb_mWX,"Event",rb_cObject);
#endif

	using namespace RubyWX::PaintEvent;
	rb_cWXPaintEvent = rb_define_class_under(rb_cWXEvent,"Paint",rb_cWXEvent);

	rb_define_method(rb_cWXEvent,"draw",RUBY_METHOD_FUNC(_draw),0);

	registerEventType("paint",wxEVT_PAINT, rb_cWXPaintEvent);
}


