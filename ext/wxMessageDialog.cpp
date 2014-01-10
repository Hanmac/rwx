/*
 * wxMessageDialog.cpp
 *
 *  Created on: 25.11.2012
 *      Author: hanmac
 */


#include "wxMessageDialog.hpp"

VALUE rb_cWXMessageDialog;

#if wxUSE_FILECTRL
#define _self unwrap<wxMessageDialog*>(self)

namespace RubyWX {
namespace MessageDialog {

//APP_PROTECT(wxMessageDialog)

DLL_LOCAL VALUE _initialize(int argc,VALUE *argv,VALUE self)
{
//	VALUE parent,hash;
//	rb_scan_args(argc, argv, "11",&parent,&hash);
//	if(!rb_obj_is_kind_of(hash,rb_cString))
//	{
//		_self->Create(unwrap<wxWindow*>(parent),wxID_ANY,wxEmptyString);
//
//	}
//	rb_call_super(argc,argv);
	return self;
}


}
}
#endif
DLL_LOCAL void Init_WXMessageDialog(VALUE rb_mWX)
{
#if wxUSE_FILECTRL
	using namespace RubyWX::MessageDialog;
	rb_cWXMessageDialog = rb_define_class_under(rb_mWX,"MessageDialog",rb_cWXMessageDialogBase);
	//rb_define_alloc_func(rb_cWXMessageDialog,_alloc);

	rb_define_method(rb_cWXMessageDialog,"initialize",RUBY_METHOD_FUNC(_initialize),-1);

	registerInfo<wxMessageDialog>(rb_cWXMessageDialog);
#endif

}
