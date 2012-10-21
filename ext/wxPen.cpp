/*
 * wxPen.hpp
 *
 *  Created on: 16.03.2012
 *      Author: hanmac
 */


#include "wxPen.hpp"
#include "wxColor.hpp"

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
wxPenStyle unwrap< wxPenStyle >(const VALUE &style )
{
	return wxPENSTYLE_INVALID;
}

template <>
VALUE wrap< wxPenStyle >(const wxPenStyle &style )
{
	ID id;
	switch(style)
	{
	case wxPENSTYLE_SOLID:
		id = rb_intern("solid");
		break;
	case wxPENSTYLE_DOT:
		id = rb_intern("dot");
		break;
	case wxPENSTYLE_LONG_DASH:
		id = rb_intern("long_dash");
		break;
	case wxPENSTYLE_SHORT_DASH:
		id = rb_intern("short_dash");
		break;
	case wxPENSTYLE_DOT_DASH:
		id = rb_intern("dot_dash");
		break;
	case wxPENSTYLE_USER_DASH:
		id = rb_intern("user_dash");
		break;
	case wxPENSTYLE_TRANSPARENT:
		id = rb_intern("transparent");
		break;
	case wxPENSTYLE_STIPPLE_MASK_OPAQUE:
		id = rb_intern("stipple_mask_opaque");
		break;
	case wxPENSTYLE_STIPPLE_MASK:
		id = rb_intern("stipple_mask");
		break;
	case wxPENSTYLE_STIPPLE:
		id = rb_intern("stipple");
		break;
	case wxPENSTYLE_BDIAGONAL_HATCH:
		id = rb_intern("bdiagona_hatch");
		break;
	case wxPENSTYLE_CROSSDIAG_HATCH:
		id = rb_intern("crossdiag_hatch");
		break;
	case wxPENSTYLE_FDIAGONAL_HATCH:
		id = rb_intern("fdiagonal_hatch");
		break;
	case wxPENSTYLE_CROSS_HATCH:
		id = rb_intern("cross_hatch");
		break;
	case wxPENSTYLE_HORIZONTAL_HATCH:
		id = rb_intern("horizontal_hatch");
		break;
	case wxPENSTYLE_VERTICAL_HATCH:
		id = rb_intern("vertical_hatch");
		break;
	default:
		return Qnil;
	};
	return ID2SYM(id);
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
VALUE _alloc(VALUE self) {
	return wrap(new wxPen(*wxBLACK));
}

macro_attr(Width,int)
macro_attr(Colour,wxColour)
macro_attr(Style,wxPenStyle)

}
}

void Init_WXPen(VALUE rb_mWX)
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
//	rb_define_attr_method(rb_cWXPen,"green",_getGreen,_setGreen);
//	rb_define_attr_method(rb_cWXPen,"blue",_getBlue,_setBlue);
//	rb_define_attr_method(rb_cWXPen,"alpha",_getAlpha,_setAlpha);
//
//	rb_define_method(rb_cWXPen,"to_s",RUBY_METHOD_FUNC(_tos),0);
//	rb_define_method(rb_cWXPen,"inspect",RUBY_METHOD_FUNC(_inspect),0);
}


