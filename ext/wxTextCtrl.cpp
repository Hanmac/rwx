/*
 * wxTextCtrl.cpp
 *
 *  Created on: 27.04.2012
 *      Author: hanmac
 */

#include "wxTextCtrl.hpp"

VALUE rb_cWXTextCtrl;

#if wxUSE_TEXTCTRL
#define _self wrap<wxTextCtrl*>(self)

namespace RubyWX {
namespace TextCtrl {

APP_PROTECT(wxTextCtrl)

VALUE _initialize(int argc,VALUE *argv,VALUE self)
{
	VALUE parent,hash;
	rb_scan_args(argc, argv, "11",&parent,&hash);
	_self->Create(wrap<wxWindow*>(parent),wxID_ANY);
	_created = true;
	rb_call_super(argc,argv);
	return self;
}

}
}
#endif
void Init_WXTextCtrl(VALUE rb_mWX)
{
#if wxUSE_TEXTCTRL
	using namespace RubyWX::TextCtrl;
	rb_cWXTextCtrl = rb_define_class_under(rb_mWX,"TextCtrl",rb_cWXControl);
	rb_define_alloc_func(rb_cWXTextCtrl,_alloc);

	rb_define_method(rb_cWXTextCtrl,"initialize",RUBY_METHOD_FUNC(_initialize),-1);

	registerInfo<wxTextCtrl>(rb_cWXTextCtrl);
#endif

}
