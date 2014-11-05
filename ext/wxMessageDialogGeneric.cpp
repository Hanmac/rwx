/*
 * wxMessageDialogGeneric.cpp
 *
 *  Created on: 22.06.2014
 *      Author: hanmac
 */


#include "wxMessageDialogGeneric.hpp"

VALUE rb_cWXMessageDialogGeneric;

#if wxUSE_MSGDLG
#define _self unwrap<wxGenericMessageDialog*>(self)

namespace RubyWX {
namespace MessageDialogGeneric {

#ifdef HAVE_WXGENERICMESSAGEDIALOG
APP_PROTECT(wxGenericMessageDialog)
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

		MessageDialogBase::_set_options(hash, message, caption, style);

		_self->Create(
			unwrap<wxWindow*>(parent),message,caption,style
		);

	}
#else
	MessageDialogBase::_set_options(hash, message, caption, style);

	RTYPEDDATA_DATA(self) = new wxGenericMessageDialog(
		unwrap<wxWindow*>(parent),message,caption,style
	);

#endif

	rb_call_super(argc,argv);
	return self;
}


}
}
#endif
DLL_LOCAL void Init_WXMessageDialogGeneric(VALUE rb_mWX)
{

#if 0
	rb_mWX = rb_define_module("WX");
	rb_cWXWindow = rb_define_class_under(rb_mWX,"Window",rb_cObject);

	rb_cWXTopLevel = rb_define_class_under(rb_mWX,"TopLevel",rb_cWXWindow);
	rb_cWXDialog = rb_define_class_under(rb_mWX,"Dialog",rb_cWXTopLevel);
	rb_cWXMessageDialogBase = rb_define_class_under(rb_mWX,"MessageDialogBase",rb_cWXDialog);
#endif

#if wxUSE_MSGDLG
	using namespace RubyWX::MessageDialogGeneric;
	rb_cWXMessageDialogGeneric = rb_define_class_under(rb_mWX,"MessageDialogGeneric",rb_cWXMessageDialogBase);
	rb_define_alloc_func(rb_cWXMessageDialogGeneric,_alloc);

	rb_define_method(rb_cWXMessageDialogGeneric,"initialize",RUBY_METHOD_FUNC(_initialize),-1);

	registerInfo<wxGenericMessageDialog>(rb_cWXMessageDialogGeneric);
#endif

}
