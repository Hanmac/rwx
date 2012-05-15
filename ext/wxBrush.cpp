/*
 * wxBrush.cpp
 *
 *  Created on: 21.04.2012
 *      Author: hanmac
 */

#include "wxBrush.hpp"
#include "wxColor.hpp"

#define _self wrap<wxBrush*>(self)

VALUE rb_cWXBrush;

template <>
wxBrush* nullPtr<wxBrush*>(){ return &wxNullBrush;}
template <>
wxBrush nullPtr<wxBrush>(){ return wxNullBrush;}

template <>
wxBrushStyle wrap< wxBrushStyle >(const VALUE &style )
{
	return wxBRUSHSTYLE_INVALID;
}

template <>
VALUE wrap< wxBrushStyle >(const wxBrushStyle &style )
{
	ID id;
	switch(style)
	{
	case wxBRUSHSTYLE_SOLID:
		id = rb_intern("solid");
		break;
	case wxBRUSHSTYLE_TRANSPARENT:
		id = rb_intern("transparent");
		break;
	case wxBRUSHSTYLE_STIPPLE_MASK_OPAQUE:
		id = rb_intern("stipple_mask_opaque");
		break;
	case wxBRUSHSTYLE_STIPPLE_MASK:
		id = rb_intern("stipple_mask");
		break;
	case wxBRUSHSTYLE_STIPPLE:
		id = rb_intern("stipple");
		break;
	case wxBRUSHSTYLE_BDIAGONAL_HATCH:
		id = rb_intern("bdiagona_hatch");
		break;
	case wxBRUSHSTYLE_CROSSDIAG_HATCH:
		id = rb_intern("crossdiag_hatch");
		break;
	case wxBRUSHSTYLE_FDIAGONAL_HATCH:
		id = rb_intern("fdiagonal_hatch");
		break;
	case wxBRUSHSTYLE_CROSS_HATCH:
		id = rb_intern("cross_hatch");
		break;
	case wxBRUSHSTYLE_HORIZONTAL_HATCH:
		id = rb_intern("horizontal_hatch");
		break;
	case wxBRUSHSTYLE_VERTICAL_HATCH:
		id = rb_intern("vertical_hatch");
		break;
	default:
		return Qnil;
	};
	return ID2SYM(id);
}


namespace RubyWX {
namespace Brush {
VALUE _alloc(VALUE self) {
	return wrap(new wxBrush(*wxBLACK));
}

macro_attr(Colour,wxColour)
macro_attr(Style,wxBrushStyle)

}
}

void Init_WXBrush(VALUE rb_mWX)
{
	using namespace RubyWX::Brush;
	rb_cWXBrush = rb_define_class_under(rb_mWX,"Brush",rb_cObject);
	rb_define_alloc_func(rb_cWXBrush,_alloc);

//	rb_define_method(rb_cWXBrush,"initialize",RUBY_METHOD_FUNC(_initialize),-1);
//	rb_define_private_method(rb_cWXBrush,"initialize_copy",RUBY_METHOD_FUNC(_initialize_copy),1);
//
//
	rb_define_attr_method(rb_cWXBrush,"color",_getColour,_setColour);

	rb_define_attr_method(rb_cWXBrush,"style",_getStyle,_setStyle);
//
//	rb_define_method(rb_cWXBrush,"to_s",RUBY_METHOD_FUNC(_tos),0);
//	rb_define_method(rb_cWXBrush,"inspect",RUBY_METHOD_FUNC(_inspect),0);

	registerInfo<wxBrush>(rb_cWXBrush);
}




