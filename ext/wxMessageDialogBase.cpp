/*
 * wxMessageDialogBase.cpp
 *
 *  Created on: 25.11.2012
 *      Author: hanmac
 */


#include "wxMessageDialogBase.hpp"

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

VALUE _SetOKLabel(VALUE self,VALUE val)
{
	if(FIXNUM_P(val) || SYMBOL_P(val))
		_self->SetOKLabel(unwrapID(val));
	else
		_self->SetOKLabel(unwrap<wxString>(val));
	return val;
}

VALUE _SetYesLabel(VALUE self,VALUE val)
{
	if(FIXNUM_P(val) || SYMBOL_P(val))
		_self->SetYesNoLabels(unwrapID(val),_self->GetNoLabel());
	else
		_self->SetYesNoLabels(unwrap<wxString>(val),_self->GetNoLabel());
	return val;
}

VALUE _SetNoLabel(VALUE self,VALUE val)
{
	if(FIXNUM_P(val) || SYMBOL_P(val))
		_self->SetYesNoLabels(_self->GetYesLabel(),unwrapID(val));
	else
		_self->SetYesNoLabels(_self->GetYesLabel(),unwrap<wxString>(val));
	return val;
}

VALUE _SetCancelLabel(VALUE self,VALUE val)
{
	if(FIXNUM_P(val) || SYMBOL_P(val))
		_self->SetOKCancelLabels(_self->GetOKLabel(),unwrapID(val));
	else
		_self->SetOKCancelLabels(_self->GetOKLabel(),unwrap<wxString>(val));
	return val;
}


VALUE _SetHelpLabel(VALUE self,VALUE val)
{
	if(FIXNUM_P(val) || SYMBOL_P(val))
		_self->SetHelpLabel(unwrapID(val));
	else
		_self->SetHelpLabel(unwrap<wxString>(val));
	return val;
}

VALUE _MessageBox(int argc,VALUE *argv,VALUE self)
{
	VALUE parent,message;
	rb_scan_args(argc, argv, "20",&parent,&message);

	return unwrapID(wxMessageBox(unwrap<wxString>(message),wxMessageBoxCaptionStr,wxOK | wxCENTRE,unwrap<wxWindow*>(parent)));
}

VALUE _InfoMessageBox(int argc,VALUE *argv,VALUE self)
{
	VALUE parent;
	rb_scan_args(argc, argv, "01",&parent);

	wxInfoMessageBox(unwrap<wxWindow*>(parent));
	return Qnil;
}

}
}
#endif
void Init_WXMessageDialogBase(VALUE rb_mWX)
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
