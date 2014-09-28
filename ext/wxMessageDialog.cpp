/*
 * wxMessageDialog.cpp
 *
 *  Created on: 25.11.2012
 *      Author: hanmac
 */


#include "wxMessageDialog.hpp"

VALUE rb_cWXMessageDialog;

#if wxUSE_MSGDLG
#define _self unwrap<wxMessageDialog*>(self)

namespace RubyWX {
namespace MessageDialog {

#ifdef HAVE_WXMESSAGEDIALOG
APP_PROTECT(wxMessageDialog)
#else
APP_PROTECT_NULL
#endif

DLL_LOCAL VALUE _initialize(int argc,VALUE *argv,VALUE self)
{
	VALUE parent,name,hash;
	rb_scan_args(argc, argv, "11:",&parent,&name,&hash);

	wxString message;
	wxString caption(wxMessageBoxCaptionStr);
	int style(wxOK|wxCENTRE);

#ifdef HAVE_WXMESSAGEDIALOG
	if(!_created && rb_obj_is_kind_of(name,rb_cString)) {

		if(rb_obj_is_kind_of(hash,rb_cHash)) {
			set_hash_option(hash,"message",message);
			set_hash_option(hash,"caption",caption);
			set_hash_option(hash,"style",style);
		}

		_self->Create(
			unwrap<wxWindow*>(parent),message,caption,style
		);

	}
#else
	if(rb_obj_is_kind_of(hash,rb_cHash)) {
		set_hash_option(hash,"message",message);
		set_hash_option(hash,"caption",caption);
		set_hash_option(hash,"style",style);
	}

	RTYPEDDATA_DATA(self) = new wxMessageDialog(
		unwrap<wxWindow*>(parent),message,caption,style
	);

#endif

	rb_call_super(argc,argv);
	return self;
}


}
}
#endif
DLL_LOCAL void Init_WXMessageDialog(VALUE rb_mWX)
{

#if 0
	rb_mWX = rb_define_module("WX");
	rb_cWXWindow = rb_define_class_under(rb_mWX,"Window",rb_cObject);

	rb_cWXTopLevel = rb_define_class_under(rb_mWX,"TopLevel",rb_cWXWindow);
	rb_cWXDialog = rb_define_class_under(rb_mWX,"Dialog",rb_cWXTopLevel);
	rb_cWXMessageDialogBase = rb_define_class_under(rb_mWX,"MessageDialogBase",rb_cWXDialog);
#endif

#if wxUSE_MSGDLG
	using namespace RubyWX::MessageDialog;
	rb_cWXMessageDialog = rb_define_class_under(rb_mWX,"MessageDialog",rb_cWXMessageDialogBase);
	rb_define_alloc_func(rb_cWXMessageDialog,_alloc);

	rb_define_method(rb_cWXMessageDialog,"initialize",RUBY_METHOD_FUNC(_initialize),-1);

	registerInfo<wxMessageDialog>(rb_cWXMessageDialog);
#endif

}
