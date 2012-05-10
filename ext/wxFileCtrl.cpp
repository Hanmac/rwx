/*
 * wxFileCtrl.cpp
 *
 *  Created on: 16.03.2012
 *      Author: hanmac
 */

#include "wxFileCtrl.hpp"

VALUE rb_cWXFileCtrl;

#if wxUSE_FILECTRL
#define _self wrap<wxFileCtrl*>(self)

namespace RubyWX {
namespace FileCtrl {

APP_PROTECT(wxFileCtrl)

VALUE _initialize(int argc,VALUE *argv,VALUE self)
{
	VALUE parent,hash;
	rb_scan_args(argc, argv, "11",&parent,&hash);
	_self->Create(wrap<wxWindow*>(parent),wxID_ANY);
	rb_call_super(argc,argv);
	return self;
}


}
}
#endif
void Init_WXFileCtrl(VALUE rb_mWX)
{
#if wxUSE_FILECTRL
	using namespace RubyWX::FileCtrl;
	rb_cWXFileCtrl = rb_define_class_under(rb_mWX,"FileCtrl",rb_cWXFileCtrlBase);
	rb_define_alloc_func(rb_cWXFileCtrl,_alloc);

	rb_define_method(rb_cWXFileCtrl,"initialize",RUBY_METHOD_FUNC(_initialize),-1);

	registerType<wxFileCtrl>(rb_cWXFileCtrl);
#endif

}




