/*
 * wxProgressDialog.cpp
 *
 *  Created on: 11.03.2012
 *      Author: hanmac
 */

#include "wxProgressDialog.hpp"

VALUE rb_cWXProgressDialog;
#if wxUSE_PROGRESSDLG

#ifdef HAVE_WXPROGRESSDIALOG
#define _self unwrap<wxProgressDialog*>(self)
#else
#define _self unwrap<wxGenericProgressDialog*>(self)
#endif


namespace RubyWX {
namespace ProgressDialog {

macro_attr(Range,int)

singlereturn(GetValue)
singlereturn(GetMessage)

#ifdef HAVE_WXPROGRESSDIALOG
APP_PROTECT(wxProgressDialog)
#else
APP_PROTECT(wxGenericProgressDialog)
#endif

DLL_LOCAL VALUE _initialize(int argc,VALUE *argv,VALUE self)
{
	VALUE parent,name,hash;
	rb_scan_args(argc, argv, "11:",&parent,&name,&hash);
	if(!_created && !rb_obj_is_kind_of(name,rb_cString))
	{
		wxString title(wxEmptyString);
		wxString message(wxEmptyString);
		int max(100);
		int style(wxPD_APP_MODAL | wxPD_AUTO_HIDE);

		if(rb_obj_is_kind_of(hash,rb_cHash))
		{
			set_hash_option(hash,"title",title);
			set_hash_option(hash,"message",message);

			set_hash_option(hash,"max",max);

			set_hash_option(hash,"style",style);

			TopLevel::set_style_flags(hash,style);

			set_hash_flag_option(hash,"can_abort",wxPD_CAN_ABORT,style);
			set_hash_flag_option(hash,"app_modal",wxPD_APP_MODAL,style);
			set_hash_flag_option(hash,"auto_hide",wxPD_AUTO_HIDE,style);
			set_hash_flag_option(hash,"smooth",wxPD_SMOOTH,style);
			set_hash_flag_option(hash,"can_skip",wxPD_CAN_SKIP,style);
			set_hash_flag_option(hash,"elapsed_time",wxPD_ELAPSED_TIME,style);
			set_hash_flag_option(hash,"estimated_time",wxPD_ESTIMATED_TIME,style);
			set_hash_flag_option(hash,"remaining_time",wxPD_REMAINING_TIME,style);

		}
		_self->Create(title,message,max,unwrap<wxWindow*>(parent),style);

	}
	rb_call_super(argc,argv);

	return self;
}

DLL_LOCAL VALUE _update(int argc,VALUE *argv,VALUE self)
{
	VALUE val,message;
	rb_scan_args(argc, argv, "11",&val,&message);

	int value = NUM2INT(val);

	if(check_index(value,_self->GetRange()))
		return wrap(_self->Update(value,unwrap<wxString>(message)));
	return Qnil;
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


/* Document-const: APP_MODAL
 *   Make the progress dialog modal.
 *   If this flag is not given, it is only "locally" modal
 *   - that is the input to the parent window is disabled,
 *   but not to the other ones.
 */
/* Document-const: CAN_ABORT
 *   This flag tells the dialog that it should have a "Cancel" button which the user may press.
 *   If this happens, the next call to update() will return false
 */
/* Document-const: CAN_SKIP
 *   This flag tells the dialog that it should have a "Skip" button which the user may press.
 *   If this happens, the next call to Update() will return true in its skip parameter.
 */
/* Document-const: AUTO_HIDE
 *   Causes the progress dialog to disappear from screen as soon as the maximum value of the progress meter has been reached.
 *   If this style is not present, the dialog will become a modal dialog (see WX::Dialog#show_modal) once the maximum value has been reached and wait for the user to dismiss it.
 */
/* Document-const: SMOOTH
 *   Causes smooth progress of the gauge control (uses a WX::Gauge with the WX::Gauge::SMOOTH style).
 */
/* Document-const: ELAPSED_TIME
 *   The text will not be user-editable.
 */
/* Document-const: ESTIMATED_TIME
 *   The text will not be user-editable.
 */
/* Document-const: REMAINING_TIME
 *   The text will not be user-editable.
 */


DLL_LOCAL void Init_WXProgressDialog(VALUE rb_mWX)
{
#if 0
	rb_mWX = rb_define_module("WX");
	rb_cWXWindow = rb_define_class_under(rb_mWX,"Window",rb_cObject);

	rb_cWXTopLevel = rb_define_class_under(rb_mWX,"TopLevel",rb_cWXWindow);
	rb_cWXDialog = rb_define_class_under(rb_mWX,"Dialog",rb_cWXTopLevel);

#endif

#if wxUSE_PROGRESSDLG
	using namespace RubyWX::ProgressDialog;
	rb_cWXProgressDialog = rb_define_class_under(rb_mWX,"ProgressDialog",rb_cWXDialog);
	rb_define_alloc_func(rb_cWXProgressDialog,_alloc);
	rb_define_method(rb_cWXProgressDialog,"initialize",RUBY_METHOD_FUNC(_initialize),-1);

	rb_define_method(rb_cWXProgressDialog,"update",RUBY_METHOD_FUNC(_update),-1);
	rb_define_method(rb_cWXProgressDialog,"pulse",RUBY_METHOD_FUNC(_pulse),-1);


	rb_define_const(rb_cWXProgressDialog,"CAN_ABORT",INT2NUM(wxPD_CAN_ABORT));
	rb_define_const(rb_cWXProgressDialog,"APP_MODAL",INT2NUM(wxPD_APP_MODAL));
	rb_define_const(rb_cWXProgressDialog,"AUTO_HIDE",INT2NUM(wxPD_AUTO_HIDE));
	rb_define_const(rb_cWXProgressDialog,"ELAPSED_TIME",INT2NUM(wxPD_ELAPSED_TIME));
	rb_define_const(rb_cWXProgressDialog,"ESTIMATED_TIME",INT2NUM(wxPD_ESTIMATED_TIME));
	rb_define_const(rb_cWXProgressDialog,"SMOOTH",INT2NUM(wxPD_SMOOTH));
	rb_define_const(rb_cWXProgressDialog,"REMAINING_TIME",INT2NUM(wxPD_REMAINING_TIME));
	rb_define_const(rb_cWXProgressDialog,"CAN_SKIP",INT2NUM(wxPD_CAN_SKIP));

#ifdef HAVE_WXPROGRESSDIALOG
	registerInfo<wxProgressDialog>(rb_cWXProgressDialog);
#else
	registerInfo<wxGenericProgressDialog>(rb_cWXProgressDialog);
	registerInfo<wxProgressDialog>(rb_cWXProgressDialog);
#endif


#endif
}
