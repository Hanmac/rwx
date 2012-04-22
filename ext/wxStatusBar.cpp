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

VALUE _alloc(VALUE self)
{
	return wrap(new wxStatusBar,self);
}

}
}
#endif
void Init_WXStatusBar(VALUE rb_mWX)
{
#if wxUSE_STATUSBAR
	using namespace RubyWX::StatusBar;
	rb_cWXStatusBar = rb_define_class_under(rb_mWX,"StatusBar",rb_cWXControl);
	rb_define_alloc_func(rb_cWXStatusBar,_alloc);
#endif
//	rb_define_method(rb_cWXMenu,"each",RUBY_METHOD_FUNC(_each),0);

}




