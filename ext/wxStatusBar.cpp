/*
 * wxStatusBar.cpp
 *
 *  Created on: 26.02.2012
 *      Author: hanmac
 */


#include "wxWindow.hpp"


VALUE rb_cWXStatusBar;

#if wxUSE_STATUSBAR
#define _self wrap<wxStatusBar*>(self)

namespace RubyWX {
namespace StatusBar {
//
//VALUE _alloc(VALUE self)
//{
//	return wrap(new wxStatusBar(NULL,wxID_ANY),self);
//}

}
}
#endif
void Init_WXStatusBar(VALUE rb_mWX)
{
#if wxUSE_STATUSBAR
	using namespace RubyWX::StatusBar;
	rb_cWXStatusBar = rb_define_class_under(rb_mWX,"StatusBar",rb_cWXControl);
	rb_undef_alloc_func(rb_cWXStatusBar);
#endif
//	rb_define_method(rb_cWXMenu,"each",RUBY_METHOD_FUNC(_each),0);

}




