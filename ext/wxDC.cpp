/*
 * wxDC.cpp
 *
 *  Created on: 12.03.2012
 *      Author: hanmac
 */

#include "wxDC.hpp"
#define _self wrap<wxDC*>(self)

VALUE rb_cWXDC;


namespace RubyWX {
namespace DC {

macro_attr(Font,wxFont)
macro_attr(Pen,wxPen)
macro_attr(Brush,wxBrush)
macro_attr(Background,wxBrush)

macro_attr(TextForeground,wxColour)
macro_attr(TextBackground,wxColour)


singlefunc(Clear)
singlereturn(GetLogicalOrigin)
singlereturn(GetDeviceOrigin)

VALUE _setLogicalOrigin(VALUE self,VALUE val)
{
	wxPoint *p = wrap<wxPoint*>(val);
	_self->SetLogicalOrigin(p->x,p->y);
	return val;
}

VALUE _setDeviceOrigin(VALUE self,VALUE val)
{
	wxPoint *p = wrap<wxPoint*>(val);
	_self->SetDeviceOrigin(p->x,p->y);
	return val;
}

VALUE _doc(VALUE self,VALUE message)
{
	_self->StartDoc(wrap<wxString>(message));
	rb_yield(self);
	_self->EndDoc();
	return self;
}

VALUE _page(VALUE self)
{
	_self->StartPage();
	rb_yield(self);
	_self->EndPage();
	return self;
}

}
}

void Init_WXDC(VALUE rb_mWX)
{
	using namespace RubyWX::DC;
	rb_cWXDC = rb_define_class_under(rb_mWX,"DC",rb_cObject);
	rb_undef_alloc_func(rb_cWXDC);


}


