/*
 * wxMessageDialog.cpp
 *
 *  Created on: 22.06.2014
 *      Author: hanmac
 */


#include "wxRichMessageDialog.hpp"

VALUE rb_cWXRichMessageDialog;

#if wxUSE_RICHMSGDLG
#define _self unwrap<wxRichMessageDialog*>(self)

namespace RubyWX {
namespace RichMessageDialog {

#ifdef HAVE_WXRICHMESSAGEDIALOG
APP_PROTECT(wxRichMessageDialog)
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

#ifdef HAVE_WXRICHMESSAGEDIALOG
	if(!_created && rb_obj_is_kind_of(name,rb_cString)) {

		MessageDialogBase::_set_options(hash, message, caption, style);

		_self->Create(
			unwrap<wxWindow*>(parent),message,caption,style
		);

	}
#else
	MessageDialogBase::_set_options(hash, message, caption, style);

	RTYPEDDATA_DATA(self) = new wxRichMessageDialog(
		unwrap<wxWindow*>(parent),message,caption,style
	);

#endif

	rb_call_super(argc,argv);
	return self;
}


}
}
#endif
DLL_LOCAL void Init_WXRichMessageDialog(VALUE rb_mWX)
{

#if 0
	rb_mWX = rb_define_module("WX");
	rb_cWXWindow = rb_define_class_under(rb_mWX,"Window",rb_cObject);

	rb_cWXTopLevel = rb_define_class_under(rb_mWX,"TopLevel",rb_cWXWindow);
	rb_cWXDialog = rb_define_class_under(rb_mWX,"Dialog",rb_cWXTopLevel);
	rb_cWXMessageDialogBase = rb_define_class_under(rb_mWX,"MessageDialogBase",rb_cWXDialog);
#endif

#if wxUSE_RICHMSGDLG
	using namespace RubyWX::RichMessageDialog;
	rb_cWXRichMessageDialog = rb_define_class_under(rb_mWX,"RichMessageDialog",rb_cWXMessageDialogBase);
	rb_define_alloc_func(rb_cWXRichMessageDialog,_alloc);

	rb_define_method(rb_cWXRichMessageDialog,"initialize",RUBY_METHOD_FUNC(_initialize),-1);

	registerInfo<wxRichMessageDialog>(rb_cWXRichMessageDialog);
#endif

}
