/*
 * wxPen.hpp
 *
 *  Created on: 16.03.2012
 *      Author: hanmac
 */


#include "wxPen.hpp"
#include "wxColor.hpp"
#include "wxBitmap.hpp"

#define _self unwrap<wxPen*>(self)

VALUE rb_cWXPen;


template <>
VALUE wrap< wxPen >(wxPen *bitmap )
{
	if(bitmap->IsOk())
		return wrapPtr(bitmap,rb_cWXPen);
	return Qnil;
}

template <>
VALUE wrap< wxPen >(const wxPen &bitmap )
{
	if(bitmap.IsOk())
		return wrapPtr(const_cast<wxPen*>(&bitmap),rb_cWXPen);
	return Qnil;
}

template <>
wxPen* unwrap< wxPen* >(const VALUE &vbitmap)
{
	if(NIL_P(vbitmap))
		return &wxNullPen;
	return unwrapPtr<wxPen>(vbitmap, rb_cWXPen);
}

template <>
wxPen unwrap< wxPen >(const VALUE &vbitmap)
{
	if(NIL_P(vbitmap))
		return wxNullPen;
	return *unwrap<wxPen*>(vbitmap);
}


namespace RubyWX {
namespace Pen {
DLL_LOCAL VALUE _alloc(VALUE self) {
	return wrap(new wxPen(*wxBLACK));
}

macro_attr(Width,int)
macro_attr(Colour,wxColour)
macro_attr_enum(Style,wxPenStyle)
macro_attr(Stipple,wxBitmap)

}
}

DLL_LOCAL void Init_WXPen(VALUE rb_mWX)
{
	using namespace RubyWX::Pen;
	rb_cWXPen = rb_define_class_under(rb_mWX,"Pen",rb_cObject);
	rb_define_alloc_func(rb_cWXPen,_alloc);

//	rb_define_method(rb_cWXPen,"initialize",RUBY_METHOD_FUNC(_initialize),-1);
//	rb_define_private_method(rb_cWXPen,"initialize_copy",RUBY_METHOD_FUNC(_initialize_copy),1);
//
//
	rb_define_attr_method(rb_cWXPen,"width",_getWidth,_setWidth);
	rb_define_attr_method(rb_cWXPen,"color",_getColour,_setColour);

	rb_define_attr_method(rb_cWXPen,"style",_getStyle,_setStyle);
	rb_define_attr_method(rb_cWXPen,"stipple",_getStipple,_setStipple);

//
//	rb_define_method(rb_cWXPen,"to_s",RUBY_METHOD_FUNC(_tos),0);
//	rb_define_method(rb_cWXPen,"inspect",RUBY_METHOD_FUNC(_inspect),0);

	registerEnum<wxPenStyle>("WX::PenStyle",wxPENSTYLE_TRANSPARENT)
		->add(wxPENSTYLE_SOLID,"solid")
		->add(wxPENSTYLE_TRANSPARENT,"transparent")
		->add(wxPENSTYLE_STIPPLE_MASK_OPAQUE,"stipple_mask_opaque")
		->add(wxPENSTYLE_STIPPLE_MASK,"stipple_mask")
		->add(wxPENSTYLE_STIPPLE,"stipple")
		->add(wxPENSTYLE_BDIAGONAL_HATCH,"bdiagona_hatch")
		->add(wxPENSTYLE_CROSSDIAG_HATCH,"crossdiag_hatch")
		->add(wxPENSTYLE_FDIAGONAL_HATCH,"fdiagonal_hatch")
		->add(wxPENSTYLE_CROSS_HATCH,"cross_hatch")
		->add(wxPENSTYLE_HORIZONTAL_HATCH,"horizontal_hatch")
		->add(wxPENSTYLE_VERTICAL_HATCH,"vertical_hatch");

}


