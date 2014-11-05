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

void _set_options(VALUE hash, wxString &message, wxString &caption, int &style) {
	if(rb_obj_is_kind_of(hash,rb_cHash)) {
		set_hash_option(hash,"message",message);
		set_hash_option(hash,"caption",caption);
		set_hash_option(hash,"style",style);
	}
}

macro_attr(Message,wxString)
macro_attr(ExtendedMessage,wxString)

wxMessageDialogBase::ButtonLabel unwrapButtonLabel(VALUE val)
{
	if(FIXNUM_P(val) || SYMBOL_P(val)){
		wxWindowID id = unwrapID(val);
		if(wxIsStockID(id))
			return id;
	}
	return unwrap<wxString>(val);
}

singlereturn(GetYesLabel)
singlereturn(GetNoLabel)
macro_attr_with_func(OKLabel,wrap,unwrapButtonLabel)
singlereturn(GetCancelLabel)
macro_attr_with_func(HelpLabel,wrap,unwrapButtonLabel)


DLL_LOCAL VALUE _SetYesLabel(VALUE self,VALUE val)
{
	rb_check_frozen(self);
	_self->SetYesNoLabels(unwrapButtonLabel(val),_self->GetNoLabel());
	return val;
}

DLL_LOCAL VALUE _SetNoLabel(VALUE self,VALUE val)
{
	rb_check_frozen(self);
	_self->SetYesNoLabels(_self->GetYesLabel(),unwrapButtonLabel(val));
	return val;
}

DLL_LOCAL VALUE _SetCancelLabel(VALUE self,VALUE val)
{
	rb_check_frozen(self);
	_self->SetOKCancelLabels(_self->GetOKLabel(),unwrapButtonLabel(val));
	return val;
}

DLL_LOCAL VALUE _MessageBox(int argc,VALUE *argv,VALUE self)
{
	VALUE parent,message,hash;
	rb_scan_args(argc, argv, "21",&parent,&message,&hash);

	app_protected();

	wxString caption(wxMessageBoxCaptionStr);

	int buttons(wxOK);
	int icon(wxICON_NONE);
	if(rb_obj_is_kind_of(hash,rb_cHash))
	{
		set_hash_option(hash,"caption",caption);
		set_hash_option(hash,"buttons",buttons,unwrap_buttonflag);
		set_hash_option(hash,"icon",icon,(int(*)(const VALUE&))unwrap_iconflag);
	}

	int id = wxMessageBox(unwrap<wxString>(message),caption,buttons | wxCENTRE | icon,unwrap<wxWindow*>(parent));

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



/* Document-attr: message
 * the message of this dialog. String
 */
/* Document-attr: extended_message
 * the message of this dialog. String
 */

/* Document-attr: yes_label
 * the label of the yes button. getter String, setter String/Symbol/Fixnum
 */
/* Document-attr: no_label
 * the label of the no button. getter String, setter String/Symbol/Fixnum
 */
/* Document-attr: ok_label
 * the label of the ok button. getter String, setter String/Symbol/Fixnum
 */
/* Document-attr: cancel_label
 * the label of the cancel button. getter String, setter String/Symbol/Fixnum
 */
/* Document-attr: help_label
 * the label of the help button. getter String, setter String/Symbol/Fixnum
 */

DLL_LOCAL void Init_WXMessageDialogBase(VALUE rb_mWX)
{
#if 0
	rb_mWX = rb_define_module("WX");
	rb_cWXWindow = rb_define_class_under(rb_mWX,"Window",rb_cObject);

	rb_cWXTopLevel = rb_define_class_under(rb_mWX,"TopLevel",rb_cWXWindow);
	rb_cWXDialog = rb_define_class_under(rb_mWX,"Dialog",rb_cWXTopLevel);
#endif

#if wxUSE_MSGDLG
	using namespace RubyWX::MessageDialogBase;
	rb_cWXMessageDialogBase = rb_define_class_under(rb_mWX,"MessageDialogBase",rb_cWXDialog);
	rb_undef_alloc_func(rb_cWXMessageDialogBase);

#if 0
	rb_define_attr(rb_cWXMessageDialogBase,"message",1,1);
	rb_define_attr(rb_cWXMessageDialogBase,"extended_message",1,1);

	rb_define_attr(rb_cWXMessageDialogBase,"yes_label",1,1);
	rb_define_attr(rb_cWXMessageDialogBase,"no_label",1,1);
	rb_define_attr(rb_cWXMessageDialogBase,"ok_label",1,1);
	rb_define_attr(rb_cWXMessageDialogBase,"cancel_label",1,1);
	rb_define_attr(rb_cWXMessageDialogBase,"help_label",1,1);
#endif


	rb_define_attr_method(rb_cWXMessageDialogBase,"message",_getMessage,_setMessage);
	rb_define_attr_method(rb_cWXMessageDialogBase,"extended_message",_getExtendedMessage,_setExtendedMessage);

	rb_define_attr_method(rb_cWXMessageDialogBase,"yes_label",_GetYesLabel,_SetYesLabel);
	rb_define_attr_method(rb_cWXMessageDialogBase,"no_label",_GetNoLabel,_SetNoLabel);
	rb_define_attr_method(rb_cWXMessageDialogBase,"ok_label",_getOKLabel,_setOKLabel);
	rb_define_attr_method(rb_cWXMessageDialogBase,"cancel_label",_GetCancelLabel,_SetCancelLabel);
	rb_define_attr_method(rb_cWXMessageDialogBase,"help_label",_getHelpLabel,_setHelpLabel);


	rb_define_module_function(rb_mWX,"message_box",RUBY_METHOD_FUNC(_MessageBox),-1);
	rb_define_module_function(rb_mWX,"info_message_box",RUBY_METHOD_FUNC(_InfoMessageBox),-1);

	registerType<wxMessageDialogBase>(rb_cWXMessageDialogBase);
#endif

}
