/*
 * wxSearchCtrl.cpp
 *
 *  Created on: 14.05.2012
 *      Author: hanmac
 */

#include "wxSearchCtrl.hpp"
#include "wxMenu.hpp"
#include "wxBitmap.hpp"
#include "wxBitmapButton.hpp"
#include "wxTextCtrl.hpp"

VALUE rb_cWXSearchCtrl;

#if wxUSE_SEARCHCTRL
#define _self unwrap<wxSearchCtrl*>(self)

namespace RubyWX {
namespace SearchCtrl {

APP_PROTECT(wxSearchCtrl)

#if wxUSE_MENUS
macro_attr(Menu,wxMenu*)
#endif

VALUE _initialize(int argc,VALUE *argv,VALUE self)
{
	VALUE parent,hash;
	rb_scan_args(argc, argv, "11",&parent,&hash);
	_self->Create(unwrap<wxWindow*>(parent),wxID_ANY);
	_created = true;
	rb_call_super(argc,argv);
	return self;
}

}
}
#endif
void Init_WXSearchCtrl(VALUE rb_mWX)
{
#if wxUSE_SEARCHCTRL
	using namespace RubyWX::SearchCtrl;
	rb_cWXSearchCtrl = rb_define_class_under(rb_mWX,"SearchCtrl",rb_cWXControl);
	rb_define_alloc_func(rb_cWXSearchCtrl,_alloc);

	rb_define_method(rb_cWXSearchCtrl,"initialize",RUBY_METHOD_FUNC(_initialize),-1);

	registerInfo<wxSearchCtrl>(rb_cWXSearchCtrl);
#endif

}
