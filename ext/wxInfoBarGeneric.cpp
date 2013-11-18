/*
 * wxInfoBarGeneric.cpp
 *
 *  Created on: 09.03.2013
 *      Author: hanmac
 */

#include "wxInfoBarGeneric.hpp"
#include "wxInfoBar.hpp"

VALUE rb_cWXInfoBarGeneric;

#if wxUSE_INFOBAR
#define _self unwrap<wxInfoBarGeneric*>(self)

namespace RubyWX {
namespace GenericInfoBar {

APP_PROTECT(wxInfoBarGeneric)

DLL_LOCAL VALUE _initialize(int argc,VALUE *argv,VALUE self)
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
DLL_LOCAL void Init_WXInfoBarGeneric(VALUE rb_mWX)
{
#if wxUSE_INFOBAR
	using namespace RubyWX::GenericInfoBar;
#ifdef wxHAS_NATIVE_INFOBAR
	rb_cWXInfoBarGeneric = rb_define_class_under(rb_mWX,"InfoBarGeneric",rb_cWXInfoBarBase);
	rb_define_alloc_func(rb_cWXInfoBarGeneric,_alloc);

	rb_define_method(rb_cWXInfoBarGeneric,"initialize",RUBY_METHOD_FUNC(_initialize),-1);

	registerInfo<wxInfoBarGeneric>(rb_cWXInfoBarGeneric);
#else
	rb_define_const(rb_mWX,"InfoBarGeneric",rb_cWXInfoBar);
#endif
#endif

}



