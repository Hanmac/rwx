/*
 * wxPen.hpp
 *
 *  Created on: 16.03.2012
 *      Author: hanmac
 */


#include "wxPen.hpp"
#include "wxColor.hpp"

#define _self wrap<wxPen*>(self)

VALUE rb_cWXPen;


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


