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

singlereturn(GetAsBitmap)

singlereturn(CanDrawBitmap)
singlereturn(CanGetTextExtent)

singlereturn_if(GetCharHeight, _self->GetFont().IsOk())
singlereturn_if(GetCharWidth, _self->GetFont().IsOk())

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

/*
 * call-seq:
 *   text_extent(text) -> WX::Size
 *
 * returns the size of the text
 * ===Arguments
 * * text String
 * ===Return value
 * WX::Size
 */
DLL_LOCAL VALUE _GetTextExtent(VALUE self,VALUE text)
{
	if(!_self->CanGetTextExtent())
		rb_raise(rb_eArgError, "DC can't get TextExtent");

	if(!_self->GetFont().IsOk())
		rb_raise(rb_eRuntimeError, "need valid WX::Font");

	return wrap(_self->GetTextExtent(unwrap<wxString>(text)));
}

/*
 * call-seq:
 *   mulit_line_text_extent(text) -> WX::Size
 *
 * returns the size of the multi line text
 * ===Arguments
 * * text String
 * ===Return value
 * WX::Size
 */
DLL_LOCAL VALUE _GetMultiLineTextExtent(VALUE self,VALUE text)
{
	if(!_self->CanGetTextExtent())
		rb_raise(rb_eArgError, "DC can't get TextExtent");

	if(!_self->GetFont().IsOk())
		rb_raise(rb_eRuntimeError, "need valid WX::Font");

	return wrap(_self->GetMultiLineTextExtent(unwrap<wxString>(text)));
}

/*
 * call-seq:
 *   doc(message) {|self| } -> self
 *
 * creates a doc.
 * ===Arguments
 * * message is a String
 * ===Return value
 * self
 */
DLL_LOCAL VALUE _doc(VALUE self,VALUE message)
{
	rb_check_frozen(self);
	_self->StartDoc(unwrap<wxString>(message));
	rb_yield(self);
	_self->EndDoc();
	return self;
}

/*
 * call-seq:
 *   page {|self| } -> self
 *
 * creates a page.
 * ===Arguments
 * * message is a String
 * ===Return value
 * self
 */
DLL_LOCAL VALUE _page(VALUE self)
{
	rb_check_frozen(self);
	_self->StartPage();
	rb_yield(self);
	_self->EndPage();
	return self;
}

/*
 * call-seq:
 *   draw_rectangle(rect) -> self
 *
 * draw a rectangle.
 * ===Arguments
 * * rect is a WX::Rect
 * ===Return value
 * self
 */
DLL_LOCAL VALUE _DrawRectangle(VALUE self,VALUE rect)
{
	rb_check_frozen(self);
	_self->DrawRectangle(unwrap<wxRect>(rect));
	return self;
}

/*
 * call-seq:
 *   draw_rounded_rectangle(rect, angle) -> self
 *
 * draw a rounded rectangle.
 * ===Arguments
 * * rect is a WX::Rect
 * * angle is a Float
 * ===Return value
 * self
 */
DLL_LOCAL VALUE _DrawRoundedRectangle(VALUE self,VALUE rect, VALUE angle)
{
	rb_check_frozen(self);
	_self->DrawRoundedRectangle(unwrap<wxRect>(rect), NUM2DBL(angle));
	return self;
}

/*
 * call-seq:
 *   draw_ellipse(rect) -> self
 *
 * draw a ellipse.
 * ===Arguments
 * * rect is a WX::Rect
 * ===Return value
 * self
 */
DLL_LOCAL VALUE _DrawEllipse(VALUE self,VALUE rect)
{
	rb_check_frozen(self);
	_self->DrawEllipse(unwrap<wxRect>(rect));
	return self;
}

/*
 * call-seq:
 *   draw_ellipse_arc(rect, start, end) -> self
 *
 * draw a ellipse arc.
 * ===Arguments
 * * rect is a WX::Rect
 * * start and end are Float
 * ===Return value
 * self
 */
DLL_LOCAL VALUE _DrawEllipseArc(VALUE self,VALUE rect, VALUE start, VALUE end)
{
	rb_check_frozen(self);
	wxRect crect(unwrap<wxRect>(rect));
	_self->DrawEllipticArc(crect.GetPosition(), crect.GetSize(), NUM2DBL(start), NUM2DBL(end));
	return self;
}
/*
 * call-seq:
 *   draw_checkmark(rect) -> self
 *
 * draw a checkmark.
 * ===Arguments
 * * rect is a WX::Rect
 * ===Return value
 * self
 */
DLL_LOCAL VALUE _DrawCheckMark(VALUE self,VALUE rect)
{
	rb_check_frozen(self);
	_self->DrawCheckMark(unwrap<wxRect>(rect));
	return self;
}

/*
 * call-seq:
 *   draw_point(rect) -> self
 *
 * draw a point.
 * ===Arguments
 * * point is a WX::Rect
 * ===Return value
 * self
 */
DLL_LOCAL VALUE _DrawPoint(VALUE self,VALUE point)
{
	rb_check_frozen(self);
	_self->DrawPoint(unwrap<wxPoint>(point));
	return self;
}

/*
 * call-seq:
 *   draw_bitmap(bitmap, point) -> self
 *   draw_bitmap(bitmap, x, y) -> self
 *
 * draw a bitmap
 * ===Arguments
 * * bitmap is a WX::Bitmap
 * * point is a WX::Point
 * * x, y and i are Integer
 * ===Return value
 * self
 * === Exceptions
 * [ArgumentError]
 * * raise an error if DC can't draw bitmaps
 */
DLL_LOCAL VALUE _DrawBitmap(int argc,VALUE *argv,VALUE self)
{
	VALUE bitmap,x,y;
	rb_scan_args(argc, argv, "21", &bitmap, &x, &y);

	if(!_self->CanDrawBitmap())
		rb_raise(rb_eArgError, "DC can't draw bitmaps");

	wxBitmap bit(unwrap<wxBitmap>(bitmap));

	if(bit.IsOk())
		rb_raise(rb_eArgError, "can't draw invalid bitmap");

	rb_check_frozen(self);

	if(NIL_P(y)) {
		wxPoint point(unwrap<wxPoint>(x));
		_self->DrawBitmap(bit, point.x, point.y);
	} else {
		_self->DrawBitmap(bit, RB_NUM2INT(x), RB_NUM2INT(y));
	}

	return self;
}

/*
 * call-seq:
 *   draw_text(text, x, y) -> self
 *
 * draw a bitmap
 * ===Arguments
 * * text is a String
 * * x and y are Integer
 * ===Return value
 * self
 */
DLL_LOCAL VALUE _DrawText(int argc, VALUE *argv, VALUE self)
{
	VALUE text,x,y;
	rb_scan_args(argc, argv, "21", &text, &x, &y);

	rb_check_frozen(self);

	if(!_self->GetFont().IsOk())
		rb_raise(rb_eRuntimeError, "need valid WX::Font");

	if(NIL_P(y)) {
		wxPoint point(unwrap<wxPoint>(x));
		_self->DrawText(unwrap<wxString>(text), point.x, point.y);
	} else {
		_self->DrawText(unwrap<wxString>(text), RB_NUM2INT(x), RB_NUM2INT(y));
	}

	return self;
}

/*
 * call-seq:
 *   draw_rotated_text(text, angle, x, y) -> self
 *
 * draw a bitmap
 * ===Arguments
 * * text is a String
 * * angle is a Float
 * * x and y are Integer
 * ===Return value
 * self
 */
DLL_LOCAL VALUE _DrawRotatedText(int argc, VALUE *argv, VALUE self)
{
	VALUE text,angle,x,y;
	rb_scan_args(argc, argv, "31", &text, &angle, &x, &y);

	rb_check_frozen(self);

	if(!_self->GetFont().IsOk())
		rb_raise(rb_eRuntimeError, "need valid WX::Font");

	if(NIL_P(y)) {
		wxPoint point(unwrap<wxPoint>(x));
		_self->DrawRotatedText(unwrap<wxString>(text), NUM2DBL(angle), point.x, point.y);
	} else {
		_self->DrawRotatedText(unwrap<wxString>(text), NUM2DBL(angle), RB_NUM2INT(x), RB_NUM2INT(y));
	}

	return self;
}

/*
 * call-seq:
 *   draw_lines(x, y, *points) -> self
 *
 * draw a polyline
 * ===Arguments
 * * x and y are Integer
 * * points is a Array of WX::Point
 * ===Return value
 * self
 */
DLL_LOCAL VALUE _DrawLines(int argc,VALUE *argv,VALUE self)
{
	VALUE x,y,points;
	rb_scan_args(argc, argv, "2*",&x,&y,&points);
	rb_check_frozen(self);
	_self->DrawLines(unwrap<wxPointList*>(points), RB_NUM2INT(x), RB_NUM2INT(y));
	return self;
}

#if wxUSE_SPLINES
/*
 * call-seq:
 *   draw_spline(*points) -> self
 *
 * draw a spline
 * ===Arguments
 * * points is a Array of WX::Point
 * ===Return value
 * self
 */
DLL_LOCAL VALUE _DrawSpline(int argc,VALUE *argv,VALUE self)
{
	VALUE points;
	rb_scan_args(argc, argv, "*", &points);
	rb_check_frozen(self);
	_self->DrawSpline(unwrap<wxPointList*>(points));
	return self;
}
#endif // wxUSE_SPLINES

/*
 * call-seq:
 *   draw_polygon(x, y, *points) -> self
 *
 * draw a polygon
 * ===Arguments
 * * x and y are Integer
 * * points is a Array of WX::Point
 * ===Return value
 * self
 */
DLL_LOCAL VALUE _DrawPolygon(int argc,VALUE *argv,VALUE self)
{
	VALUE x,y,points;
	rb_scan_args(argc, argv, "2*",&x,&y,&points);
	rb_check_frozen(self);
	_self->DrawPolygon(unwrap<wxPointList*>(points), RB_NUM2INT(x), RB_NUM2INT(y));
	return self;
}

/*
 * call-seq:
 *   with_brush(brush) {|self| } -> self
 *
 * uses block with brush
 * ===Arguments
 * * brush is a WX::Brush
 * ===Return value
 * self
 */
DLL_LOCAL VALUE _with_brush(VALUE self,VALUE brush)
{
	rb_check_frozen(self);
	wxDCBrushChanger changer(*_self, unwrap<wxBrush>(brush));
	rb_yield(self);
	return self;
}

/*
 * call-seq:
 *   with_pen(pen) {|self| } -> self
 *
 * uses block with pen
 * ===Arguments
 * * pen is a WX::Pem
 * ===Return value
 * self
 */
DLL_LOCAL VALUE _with_pen(VALUE self,VALUE pen)
{
	rb_check_frozen(self);
	wxDCPenChanger changer(*_self, unwrap<wxPen>(pen));
	rb_yield(self);
	return self;
}

/*
 * call-seq:
 *   with_color(color) {|self| } -> self
 *
 * uses block with text color
 * ===Arguments
 * * color is a WX::Color
 * ===Return value
 * self
 */
DLL_LOCAL VALUE _with_text_color(VALUE self,VALUE color)
{
	rb_check_frozen(self);
	wxDCTextColourChanger changer(*_self, unwrap<wxColour>(color));
	rb_yield(self);
	return self;
}

/*
 * call-seq:
 *   with_font(brush) {|self| } -> self
 *
 * uses block with font
 * ===Arguments
 * * font is a WX::Font
 * ===Return value
 * self
 */
DLL_LOCAL VALUE _with_font(VALUE self,VALUE font)
{
	rb_check_frozen(self);
	wxDCFontChanger changer(*_self, unwrap<wxFont>(font));
	rb_yield(self);
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
	rb_define_method(rb_cWXDC,"draw_rounded_rectangle",RUBY_METHOD_FUNC(_DrawRoundedRectangle),2);
	rb_define_method(rb_cWXDC,"draw_ellipse",RUBY_METHOD_FUNC(_DrawEllipse),1);
	rb_define_method(rb_cWXDC,"draw_ellipse_arc",RUBY_METHOD_FUNC(_DrawEllipseArc),3);
	rb_define_method(rb_cWXDC,"draw_checkmark",RUBY_METHOD_FUNC(_DrawCheckMark),1);
	rb_define_method(rb_cWXDC,"draw_point",RUBY_METHOD_FUNC(_DrawPoint),1);

	rb_define_method(rb_cWXDC,"draw_bitmap",RUBY_METHOD_FUNC(_DrawBitmap),-1);
	rb_define_method(rb_cWXDC,"draw_text",RUBY_METHOD_FUNC(_DrawText),-1);
	rb_define_method(rb_cWXDC,"draw_rotated_text",RUBY_METHOD_FUNC(_DrawRotatedText),-1);

	rb_define_method(rb_cWXDC,"draw_lines",RUBY_METHOD_FUNC(_DrawLines),-1);
#if wxUSE_SPLINES
	rb_define_method(rb_cWXDC,"draw_spline",RUBY_METHOD_FUNC(_DrawSpline),-1);
#endif // wxUSE_SPLINES
	rb_define_method(rb_cWXDC,"draw_polygon",RUBY_METHOD_FUNC(_DrawPolygon),-1);

	rb_define_method(rb_cWXDC,"clear",RUBY_METHOD_FUNC(_Clear),0);
	rb_define_method(rb_cWXDC,"size",RUBY_METHOD_FUNC(_GetSize),0);

	rb_define_method(rb_cWXDC,"as_bitmap",RUBY_METHOD_FUNC(_GetAsBitmap),0);

	rb_define_method(rb_cWXDC,"draw_bitmap?",RUBY_METHOD_FUNC(_CanDrawBitmap),0);
	rb_define_method(rb_cWXDC,"text_extent?",RUBY_METHOD_FUNC(_CanGetTextExtent),0);

	rb_define_method(rb_cWXDC,"text_extent",RUBY_METHOD_FUNC(_GetTextExtent),1);
	rb_define_method(rb_cWXDC,"multi_line_text_extent",RUBY_METHOD_FUNC(_GetMultiLineTextExtent),1);

	rb_define_method(rb_cWXDC,"char_height",RUBY_METHOD_FUNC(_GetCharHeight),0);
	rb_define_method(rb_cWXDC,"char_width",RUBY_METHOD_FUNC(_GetCharWidth),0);

	rb_define_method(rb_cWXDC,"doc",RUBY_METHOD_FUNC(_doc),1);
	rb_define_method(rb_cWXDC,"page",RUBY_METHOD_FUNC(_page),0);

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

	rb_define_method(rb_cWXDC,"with_brush",RUBY_METHOD_FUNC(_with_brush),1);
	rb_define_method(rb_cWXDC,"with_pen",RUBY_METHOD_FUNC(_with_pen),1);
	rb_define_method(rb_cWXDC,"with_text_color",RUBY_METHOD_FUNC(_with_text_color),1);
	rb_define_method(rb_cWXDC,"with_font",RUBY_METHOD_FUNC(_with_text_color),1);

	registerInfo<wxDC>(rb_cWXDC);
}


