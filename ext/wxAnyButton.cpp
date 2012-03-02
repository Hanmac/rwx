/*
 * wxAnyButton.cpp
 *
 *  Created on: 13.02.2012
 *      Author: hanmac
 */



#include "wxWindow.hpp"

VALUE rb_cWXAnyButton;

#define _self wrap<wxAnyButton*>(self)

namespace RubyWX {
namespace AnyButton {

macro_attr(LabelText,wxString)
macro_attr(BitmapLabel,wxBitmap)
macro_attr(BitmapPressed,wxBitmap)
macro_attr(BitmapDisabled,wxBitmap)
macro_attr(BitmapCurrent,wxBitmap)
macro_attr(BitmapFocus,wxBitmap)

VALUE _alloc(VALUE self)
{
	return wrap(new wxAnyButton,self);
}

}
}

void Init_WXAnyButton(VALUE rb_mWX)
{
	using namespace RubyWX::AnyButton;
	rb_cWXAnyButton = rb_define_class_under(rb_mWX,"AnyButton",rb_cWXControl);
	rb_define_alloc_func(rb_cWXAnyButton,_alloc);
}


