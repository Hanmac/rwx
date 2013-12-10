/*
 * wxMessageDialogBase.cpp
 *
 *  Created on: 25.11.2012
 *      Author: hanmac
 */


#include "wxMessageDialogBase.hpp"
#include "wxApp.hpp"

VALUE rb_cWXMessageDialogBase;

#if wxUSE_MSGDLG
#define _self unwrap<wxMessageDialogBase*>(self)

namespace RubyWX {
namespace MessageDialogBase {

macro_attr(Message,wxString)
macro_attr(ExtendedMessage,wxString)

singlereturn(GetYesLabel)
singlereturn(GetNoLabel)
singlereturn(GetOKLabel)
singlereturn(GetCancelLabel)
singlereturn(GetHelpLabel)

DLL_LOCAL VALUE _SetOKLabel(VALUE self,VALUE val)
{
	if(FIXNUM_P(val) || SYMBOL_P(val))
		_self->SetOKLabel(unwrapID(val));
	else
		_self->SetOKLabel(unwrap<wxString>(val));
	return val;
}

DLL_LOCAL VALUE _SetYesLabel(VALUE self,VALUE val)
{
	if(FIXNUM_P(val) || SYMBOL_P(val))
		_self->SetYesNoLabels(unwrapID(val),_self->GetNoLabel());
	else
		_self->SetYesNoLabels(unwrap<wxString>(val),_self->GetNoLabel());
	return val;
}

DLL_LOCAL VALUE _SetNoLabel(VALUE self,VALUE val)
{
	if(FIXNUM_P(val) || SYMBOL_P(val))
		_self->SetYesNoLabels(_self->GetYesLabel(),unwrapID(val));
	else
		_self->SetYesNoLabels(_self->GetYesLabel(),unwrap<wxString>(val));
	return val;
}

DLL_LOCAL VALUE _SetCancelLabel(VALUE self,VALUE val)
{
	if(FIXNUM_P(val) || SYMBOL_P(val))
		_self->SetOKCancelLabels(_self->GetOKLabel(),unwrapID(val));
	else
		_self->SetOKCancelLabels(_self->GetOKLabel(),unwrap<wxString>(val));
	return val;
}


DLL_LOCAL VALUE _SetHelpLabel(VALUE self,VALUE val)
{
	if(FIXNUM_P(val) || SYMBOL_P(val))
		_self->SetHelpLabel(unwrapID(val));
	else
		_self->SetHelpLabel(unwrap<wxString>(val));
	return val;
}

DLL_LOCAL VALUE _MessageBox(int argc,VALUE *argv,VALUE self)
{
	VALUE parent,message,hash;
	rb_scan_args(argc, argv, "21",&parent,&message,&hash);

	app_protected();

	wxString caption(wxMessageBoxCaptionStr);

	if(rb_obj_is_kind_of(hash,rb_cHash))
	{
		set_hash_option(hash,"caption",caption);
	}

	int id = wxMessageBox(unwrap<wxString>(message),caption,wxOK | wxCENTRE,unwrap<wxWindow*>(parent));

	//wrap the ids
	std::map<int,wxWindowID> keys;

	keys[wxYES] = wxID_YES;
	keys[wxOK] = wxID_OK;
	keys[wxNO] = wxID_NO;

	keys[wxCANCEL] = wxID_CANCEL;
	keys[wxAPPLY] = wxID_APPLY;
	keys[wxCLOSE] = wxID_CLOSE;

	return wrapID(keys[id]);
}

DLL_LOCAL VALUE _InfoMessageBox(int argc,VALUE *argv,VALUE self)
{
	VALUE parent;
	rb_scan_args(argc, argv, "01",&parent);
	app_protected();
	wxInfoMessageBox(unwrap<wxWindow*>(parent));
	return Qnil;
}

}
}
#endif
DLL_LOCAL void Init_WXMessageDialogBase(VALUE rb_mWX)
{
#if wxUSE_MSGDLG
	using namespace RubyWX::MessageDialogBase;
	rb_cWXMessageDialogBase = rb_define_class_under(rb_mWX,"MessageDialogBase",rb_cWXDialog);
	rb_undef_alloc_func(rb_cWXMessageDialogBase);

	rb_define_attr_method(rb_cWXMessageDialogBase,"message",_getMessage,_setMessage);
	rb_define_attr_method(rb_cWXMessageDialogBase,"extended_message",_getExtendedMessage,_setExtendedMessage);

	rb_define_attr_method(rb_cWXMessageDialogBase,"yes_label",_GetYesLabel,_SetYesLabel);
	rb_define_attr_method(rb_cWXMessageDialogBase,"no_label",_GetNoLabel,_SetNoLabel);
	rb_define_attr_method(rb_cWXMessageDialogBase,"ok_label",_GetOKLabel,_SetOKLabel);
	rb_define_attr_method(rb_cWXMessageDialogBase,"cancel_label",_GetCancelLabel,_SetCancelLabel);
	rb_define_attr_method(rb_cWXMessageDialogBase,"help_label",_GetHelpLabel,_SetHelpLabel);


	rb_define_module_function(rb_mWX,"message_box",RUBY_METHOD_FUNC(_MessageBox),-1);
	rb_define_module_function(rb_mWX,"info_message_box",RUBY_METHOD_FUNC(_InfoMessageBox),-1);

	registerType<wxMessageDialogBase>(rb_cWXMessageDialogBase);
#endif

}
