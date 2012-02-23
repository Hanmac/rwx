/*
 * wxAnyButton.cpp
 *
 *  Created on: 13.02.2012
 *      Author: hanmac
 */



#include "wxWindow.hpp"

VALUE rb_cWXAnyButton;

#define _self wrap<wxAnyButton*>(self)

RubyAnyButton::RubyAnyButton(VALUE klass)
: wxAnyButton(), RubyControl::RubyControl(klass)
{}

namespace RubyWX {
namespace AnyButton {

macro_attr(LabelText,wxString)

VALUE _alloc(VALUE self)
{
	return wrap(new RubyAnyButton(self));
}

}
}

void Init_WXAnyButton(VALUE rb_mWX)
{
	using namespace RubyWX::AnyButton;
	rb_cWXAnyButton = rb_define_class_under(rb_mWX,"AnyButton",rb_cWXTopLevel);
	rb_define_alloc_func(rb_cWXAnyButton,_alloc);
}


