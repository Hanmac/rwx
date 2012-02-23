/*
 * wxControl.cpp
 *
 *  Created on: 12.02.2012
 *      Author: hanmac
 */


#include "wxWindow.hpp"

VALUE rb_cWXControl;

#define _self wrap<wxControl*>(self)

RubyControl::RubyControl(VALUE klass)
: RubyWindow::RubyWindow(klass)
{}

namespace RubyWX {
namespace Control {

macro_attr(LabelText,wxString)

VALUE _alloc(VALUE self)
{
	return wrap(new RubyControl(self));
}

}
}

void Init_WXControl(VALUE rb_mWX)
{
	using namespace RubyWX::Control;
	rb_cWXControl = rb_define_class_under(rb_mWX,"Control",rb_cWXTopLevel);
	rb_define_alloc_func(rb_cWXControl,_alloc);
}
