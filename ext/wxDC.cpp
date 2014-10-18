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

singlereturn(GetSize)

singlereturn(CanDrawBitmap)

singlereturn(GetLogicalOrigin)
singlereturn(GetDeviceOrigin)

DLL_LOCAL VALUE _setLogicalOrigin(VALUE self,VALUE val)
{
	rb_check_frozen(self);
	wxPoint p(unwrap<wxPoint>(val));
	_self->SetLogicalOrigin(p.x,p.y);
	return val;
}

DLL_LOCAL VALUE _setDeviceOrigin(VALUE self,VALUE val)
{
	rb_check_frozen(self);
	wxPoint p(unwrap<wxPoint>(val));
	_self->SetDeviceOrigin(p.x,p.y);
	return val;
}


DLL_LOCAL VALUE _getUserScale(VALUE self)
{
	rb_check_frozen(self);
	double x, y;
	_self->GetUserScale(&x,&y);
	return wrap(wxRealPoint(x,y));
}

DLL_LOCAL VALUE _setUserScale(VALUE self,VALUE val)
{
	rb_check_frozen(self);
	wxRealPoint p(unwrap<wxRealPoint>(val));
	_self->SetUserScale(p.x,p.y);
	return val;
}

DLL_LOCAL VALUE _getLogicalScale(VALUE self)
{
	rb_check_frozen(self);
	double x, y;
	_self->GetLogicalScale(&x,&y);
	return wrap(wxRealPoint(x,y));
}

DLL_LOCAL VALUE _setLogicalScale(VALUE self,VALUE val)
{
	rb_check_frozen(self);
	wxRealPoint p(unwrap<wxRealPoint>(val));
	_self->SetLogicalScale(p.x,p.y);
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

	if(!_self->CanDrawBitmap())
		rb_raise(rb_eArgError, "DC can't draw bitmaps");
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


/* Document-attr: font
 * WX::Font for drawing text
 */

/* Document-attr: pen
 * WX::Pen
 */

/* Document-attr: brush
 * WX::Brush for lines
 */

/* Document-attr: background
 * WX::Brush for filling
 */

/* Document-attr: text_foreground
 * WX::Color for text drawing.
 */

/* Document-attr: text_background
 * WX::Color for text drawing.
 */


/* Document-attr: logical_origin
 * WX::Point
 */

/* Document-attr: device_origin
 * WX::Point the device origin (i.e. the origin in pixels after scaling has been applied).
 */

/* Document-attr: user_scale
 * WX::Point (Float,Float) user scaling factor, useful for applications which require 'zooming'.
 */

/* Document-attr: logical_scale
 * WX::Point
 */


DLL_LOCAL void Init_WXDC(VALUE rb_mWX)
{

	using namespace RubyWX::DC;
	rb_cWXDC = rb_define_class_under(rb_mWX,"DC",rb_cObject);
	rb_undef_alloc_func(rb_cWXDC);

#if 0
	rb_define_attr(rb_cWXDC,"font",1,1);
	rb_define_attr(rb_cWXDC,"pen",1,1);
	rb_define_attr(rb_cWXDC,"brush",1,1);
	rb_define_attr(rb_cWXDC,"background",1,1);
	rb_define_attr(rb_cWXDC,"text_foreground",1,1);
	rb_define_attr(rb_cWXDC,"text_background",1,1);

	rb_define_attr(rb_cWXDC,"logical_origin",1,1);
	rb_define_attr(rb_cWXDC,"device_origin",1,1);
	rb_define_attr(rb_cWXDC,"user_scale",1,1);
	rb_define_attr(rb_cWXDC,"logical_scale",1,1);
#endif

	rb_undef_method(rb_cWXDC,"initialize_copy");
	rb_undef_method(rb_cWXDC,"_load");
	rb_undef_method(rb_cWXDC,"_dump");

	rb_define_method(rb_cWXDC,"draw_rectangle",RUBY_METHOD_FUNC(_DrawRectangle),1);
	rb_define_method(rb_cWXDC,"draw_ellipse",RUBY_METHOD_FUNC(_DrawEllipse),1);
	rb_define_method(rb_cWXDC,"draw_checkmark",RUBY_METHOD_FUNC(_DrawCheckMark),1);

	rb_define_method(rb_cWXDC,"draw_bitmap",RUBY_METHOD_FUNC(_DrawBitmap),3);
	rb_define_method(rb_cWXDC,"draw_text",RUBY_METHOD_FUNC(_DrawText),3);

	rb_define_method(rb_cWXDC,"clear",RUBY_METHOD_FUNC(_Clear),0);
	rb_define_method(rb_cWXDC,"size",RUBY_METHOD_FUNC(_GetSize),0);
	rb_define_method(rb_cWXDC,"draw_bitmap?",RUBY_METHOD_FUNC(_CanDrawBitmap),0);

	rb_define_attr_method(rb_cWXDC,"font",_getFont,_setFont);
	rb_define_attr_method(rb_cWXDC,"pen",_getPen,_setPen);
	rb_define_attr_method(rb_cWXDC,"brush",_getBrush,_setBrush);
	rb_define_attr_method(rb_cWXDC,"background",_getBackground,_setBackground);
	rb_define_attr_method(rb_cWXDC,"text_foreground",_getTextForeground,_setTextForeground);
	rb_define_attr_method(rb_cWXDC,"text_background",_getTextBackground,_setTextBackground);

	rb_define_attr_method(rb_cWXDC,"logical_origin",_GetLogicalOrigin,_setLogicalOrigin);
	rb_define_attr_method(rb_cWXDC,"device_origin",_GetDeviceOrigin,_setDeviceOrigin);

	rb_define_attr_method(rb_cWXDC,"user_scale",_getUserScale,_setUserScale);
	rb_define_attr_method(rb_cWXDC,"logical_scale",_getLogicalScale,_setLogicalScale);


	registerInfo<wxDC>(rb_cWXDC);
}


