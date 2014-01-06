/*
 * wxProgressDialog.cpp
 *
 *  Created on: 11.03.2012
 *      Author: hanmac
 */

#include "wxProgressDialog.hpp"

VALUE rb_cWXProgressDialog;
#if wxUSE_PROGRESSDLG
#define _self unwrap<wxProgressDialog*>(self)

namespace RubyWX {
namespace ProgressDialog {

macro_attr(Range,int)

singlereturn(GetValue)
singlereturn(GetMessage)

DLL_LOCAL VALUE _alloc(VALUE self)
{
	app_protected();

	return wrapPtr(new wxProgressDialog(wxEmptyString,wxEmptyString),self);
}

DLL_LOCAL VALUE _initialize(int argc,VALUE *argv,VALUE self)
{
	VALUE parent,hash;
	rb_scan_args(argc, argv, "11",&parent,&hash);
	if(!NIL_P(parent))
		_self->Reparent(unwrap<wxWindow*>(parent));
	
	rb_call_super(argc,argv);
	return self;
}

DLL_LOCAL VALUE _update(int argc,VALUE *argv,VALUE self)
{
	VALUE val,message;
	rb_scan_args(argc, argv, "11",&val,&message);
	return wrap(_self->Update(NUM2INT(val),unwrap<wxString>(message)));
}

DLL_LOCAL VALUE _pulse(int argc,VALUE *argv,VALUE self)
{
	VALUE message;
	rb_scan_args(argc, argv, "01",&message);

	return wrap(_self->Pulse(unwrap<wxString>(message)));
}


}
}
#endif

DLL_LOCAL void Init_WXProgressDialog(VALUE rb_mWX)
{
#if wxUSE_PROGRESSDLG
	using namespace RubyWX::ProgressDialog;
	rb_cWXProgressDialog = rb_define_class_under(rb_mWX,"ProgressDialog",rb_cWXDialog);
	rb_define_alloc_func(rb_cWXProgressDialog,_alloc);
	rb_define_method(rb_cWXProgressDialog,"initialize",RUBY_METHOD_FUNC(_initialize),-1);

	rb_define_method(rb_cWXProgressDialog,"update",RUBY_METHOD_FUNC(_update),-1);
	rb_define_method(rb_cWXProgressDialog,"pulse",RUBY_METHOD_FUNC(_pulse),-1);

	registerInfo<wxProgressDialog>(rb_cWXProgressDialog);
#endif
}
