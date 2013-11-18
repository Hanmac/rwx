/*
 * wxBrush.cpp
 *
 *  Created on: 21.04.2012
 *      Author: hanmac
 */

#include "wxBrush.hpp"
#include "wxColor.hpp"
#include "wxBitmap.hpp"

#define _self unwrap<wxBrush*>(self)

VALUE rb_cWXBrush;

template <>
wxBrush* nullPtr<wxBrush*>(){ return &wxNullBrush;}
template <>
wxBrush nullPtr<wxBrush>(){ return wxNullBrush;}

namespace RubyWX {
namespace Brush {
DLL_LOCAL VALUE _alloc(VALUE self) {
	return wrap(new wxBrush(*wxBLACK));
}

macro_attr(Colour,wxColour)
macro_attr_enum(Style,wxBrushStyle)
macro_attr(Stipple,wxBitmap)

}
}

DLL_LOCAL void Init_WXBrush(VALUE rb_mWX)
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
	rb_define_attr_method(rb_cWXBrush,"stipple",_getStipple,_setStipple);

//
//	rb_define_method(rb_cWXBrush,"to_s",RUBY_METHOD_FUNC(_tos),0);
//	rb_define_method(rb_cWXBrush,"inspect",RUBY_METHOD_FUNC(_inspect),0);

	registerInfo<wxBrush>(rb_cWXBrush);
	registerEnum<wxBrushStyle>("WX::BrushStyle",wxBRUSHSTYLE_TRANSPARENT)
		->add(wxBRUSHSTYLE_SOLID,"solid")
		->add(wxBRUSHSTYLE_TRANSPARENT,"transparent")
		->add(wxBRUSHSTYLE_STIPPLE_MASK_OPAQUE,"stipple_mask_opaque")
		->add(wxBRUSHSTYLE_STIPPLE_MASK,"stipple_mask")
		->add(wxBRUSHSTYLE_STIPPLE,"stipple")
		->add(wxBRUSHSTYLE_BDIAGONAL_HATCH,"bdiagona_hatch")
		->add(wxBRUSHSTYLE_CROSSDIAG_HATCH,"crossdiag_hatch")
		->add(wxBRUSHSTYLE_FDIAGONAL_HATCH,"fdiagonal_hatch")
		->add(wxBRUSHSTYLE_CROSS_HATCH,"cross_hatch")
		->add(wxBRUSHSTYLE_HORIZONTAL_HATCH,"horizontal_hatch")
		->add(wxBRUSHSTYLE_VERTICAL_HATCH,"vertical_hatch");

}




