/*
 * wxRearrangeDialog.cpp
 *
 *  Created on: 14.03.2013
 *      Author: hanmac
 */

#include "wxRearrangeDialog.hpp"

VALUE rb_cWXRearrangeDialog;

#if wxUSE_REARRANGECTRL

namespace RubyWX {
namespace RearrangeDialog {
#define _self unwrap<wxRearrangeDialog*>(self)

APP_PROTECT(wxRearrangeDialog)

VALUE _initialize(int argc,VALUE *argv,VALUE self)
{
	VALUE parent,hash;
	rb_scan_args(argc, argv, "11",&parent,&hash);
	if(!_created)
	{
		_self->Create(unwrap<wxWindow*>(parent),wxEmptyString,wxEmptyString,wxArrayInt(),wxArrayString());
		_created = true;
	}
	rb_call_super(argc,argv);
	return self;
}

}
}

#endif
void Init_WXRearrangeDialog(VALUE rb_mWX)
{
#if wxUSE_REARRANGECTRL
	using namespace RubyWX::RearrangeDialog;
	rb_cWXRearrangeDialog = rb_define_class_under(rb_mWX,"RearrangeDialog",rb_cWXPanel);
	rb_define_alloc_func(rb_cWXRearrangeDialog,_alloc);

	rb_define_method(rb_cWXRearrangeDialog,"initialize",RUBY_METHOD_FUNC(_initialize),-1);

	registerInfo<wxRearrangeDialog>(rb_cWXRearrangeDialog);
#endif

}


