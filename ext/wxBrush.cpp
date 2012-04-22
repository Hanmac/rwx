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
}




