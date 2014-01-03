/*
 * wxDC.cpp
 *
 *  Created on: 12.03.2012
 *      Author: hanmac
 */

#include "wxDC.hpp"
#include "wxRect.hpp"
#include "wxBitmap.hpp"
#include "wxPoint.hpp"
#include "wxPen.hpp"
#include "wxBrush.hpp"
#include "wxColor.hpp"
#include "wxFont.hpp"

#define _self unwrap<wxDC*>(self)

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

DLL_LOCAL VALUE _setLogicalOrigin(VALUE self,VALUE val)
{
	rb_check_frozen(self);
	wxPoint *p = unwrap<wxPoint*>(val);
	_self->SetLogicalOrigin(p->x,p->y);
	return val;
}

DLL_LOCAL VALUE _setDeviceOrigin(VALUE self,VALUE val)
{
	rb_check_frozen(self);
	wxPoint *p = unwrap<wxPoint*>(val);
	_self->SetDeviceOrigin(p->x,p->y);
	return val;
}

DLL_LOCAL VALUE _doc(VALUE self,VALUE message)
{
	rb_check_frozen(self);
	_self->StartDoc(unwrap<wxString>(message));
	rb_yield(self);
	_self->EndDoc();
	return self;
}

DLL_LOCAL VALUE _page(VALUE self)
{
	rb_check_frozen(self);
	_self->StartPage();
	rb_yield(self);
	_self->EndPage();
	return self;
}

DLL_LOCAL VALUE _DrawRectangle(VALUE self,VALUE rect)
{
	_self->DrawRectangle(unwrap<wxRect>(rect));
	return self;
}

DLL_LOCAL VALUE _DrawEllipse(VALUE self,VALUE rect)
{
	_self->DrawEllipse(unwrap<wxRect>(rect));
	return self;
}

DLL_LOCAL VALUE _DrawCheckMark(VALUE self,VALUE rect)
{
	_self->DrawCheckMark(unwrap<wxRect>(rect));
	return self;
}


DLL_LOCAL VALUE _DrawBitmap(VALUE self,VALUE bitmap,VALUE x,VALUE y)
{

	_self->DrawBitmap(unwrap<wxBitmap>(bitmap),NUM2INT(x),NUM2INT(y));
	return self;
}

DLL_LOCAL VALUE _DrawText(VALUE self,VALUE bitmap,VALUE x,VALUE y)
{

	_self->DrawText(unwrap<wxString>(bitmap),NUM2INT(x),NUM2INT(y));
	return self;
}


DLL_LOCAL VALUE _DrawLines(int argc,VALUE *argv,VALUE self)
{
	VALUE x,y,points;
	rb_scan_args(argc, argv, "2*",&x,&y,&points);
	_self->DrawLines(unwrap<wxPointList*>(points),NUM2INT(x),NUM2INT(y));
	return self;
}


}
}

DLL_LOCAL void Init_WXDC(VALUE rb_mWX)
{
#if 0
	rb_define_attr_method(rb_cWXDC,"font",_getFont,_setFont);
	rb_define_attr_method(rb_cWXDC,"pen",_getPen,_setPen);
	rb_define_attr_method(rb_cWXDC,"brush",_getBrush,_setBrush);
	rb_define_attr_method(rb_cWXDC,"background",_getBackground,_setBackground);
	rb_define_attr_method(rb_cWXDC,"text_foreground",_getTextForeground,_setTextForeground);
	rb_define_attr_method(rb_cWXDC,"text_background",_getTextBackground,_setTextBackground);

#endif

	using namespace RubyWX::DC;
	rb_cWXDC = rb_define_class_under(rb_mWX,"DC",rb_cObject);
	rb_undef_alloc_func(rb_cWXDC);

	rb_undef_method(rb_cWXDC,"initialize_copy");
	rb_undef_method(rb_cWXDC,"_load");
	rb_undef_method(rb_cWXDC,"_dump");

	rb_define_method(rb_cWXDC,"draw_rectangle",RUBY_METHOD_FUNC(_DrawRectangle),1);
	rb_define_method(rb_cWXDC,"draw_ellipse",RUBY_METHOD_FUNC(_DrawEllipse),1);
	rb_define_method(rb_cWXDC,"draw_checkmark",RUBY_METHOD_FUNC(_DrawCheckMark),1);

	rb_define_method(rb_cWXDC,"draw_bitmap",RUBY_METHOD_FUNC(_DrawBitmap),3);
	rb_define_method(rb_cWXDC,"draw_text",RUBY_METHOD_FUNC(_DrawText),3);

	rb_define_method(rb_cWXDC,"clear",RUBY_METHOD_FUNC(_Clear),0);

	rb_define_attr_method(rb_cWXDC,"font",_getFont,_setFont);
	rb_define_attr_method(rb_cWXDC,"pen",_getPen,_setPen);
	rb_define_attr_method(rb_cWXDC,"brush",_getBrush,_setBrush);
	rb_define_attr_method(rb_cWXDC,"background",_getBackground,_setBackground);
	rb_define_attr_method(rb_cWXDC,"text_foreground",_getTextForeground,_setTextForeground);
	rb_define_attr_method(rb_cWXDC,"text_background",_getTextBackground,_setTextBackground);

	registerInfo<wxDC>(rb_cWXDC);
}


