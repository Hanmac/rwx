/*
 * wxControl.cpp
 *
 *  Created on: 12.02.2012
 *      Author: hanmac
 */

#include "wxWindow.hpp"

#if wxUSE_CONTROLS
VALUE rb_cWXControl;
#define _self wrap<wxControl*>(self)

namespace RubyWX {
namespace Control {

macro_attr(LabelText,wxString)

VALUE _alloc(VALUE self)
{
	return wrap(new wxControl(NULL,wxID_ANY),self);
}

}
}

#endif
void Init_WXControl(VALUE rb_mWX)
{
#if wxUSE_CONTROLS
	using namespace RubyWX::Control;
	rb_cWXControl = rb_define_class_under(rb_mWX,"Control",rb_cWXWindow);
	rb_define_alloc_func(rb_cWXControl,_alloc);
#endif
}
