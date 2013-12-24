/*
 * wxPasswordEntryDialog.cpp
 *
 *  Created on: 24.12.2013
 *      Author: hanmac
 */


#include "wxPasswordEntryDialog.hpp"
#include "wxTextEntryDialog.hpp"
#include "wxPoint.hpp"
#include "wxApp.hpp"

VALUE rb_cWXPasswordEntryDialog;
#if wxUSE_TEXTDLG
#define _self unwrap<wxPasswordEntryDialog*>(self)

namespace RubyWX {
namespace PasswordEntryDialog {

APP_PROTECT(wxPasswordEntryDialog)

/*
 * call-seq:
 *   PasswordEntryDialog.new(parent, [options])
 *
 * creates a new PasswordEntryDialog widget.
 * ===Arguments
 * * parent of this window or nil
 *
 * *options: Hash with possible options to set:
 * * *path String default path
 * * *message String
 *
*/
DLL_LOCAL VALUE _initialize(int argc,VALUE *argv,VALUE self)
{
	VALUE parent,hash;
	rb_scan_args(argc, argv, "11",&parent,&hash);
	if(!rb_obj_is_kind_of(hash,rb_cString))
	{
		wxString message;
		wxString caption(wxGetPasswordFromUserPromptStr);
		wxString value;
		long style = wxTextEntryDialogStyle;

		if(rb_obj_is_kind_of(hash,rb_cHash))
		{
			set_hash_option(hash,"message",message);
			set_hash_option(hash,"caption",caption);

			set_hash_option(hash,"value",value);

			set_hash_option(hash,"style",style);
		}
		_self->Create(unwrap<wxWindow*>(parent),
				message,caption,value,style);
		_created = true;
	}
	rb_call_super(argc,argv);

	return self;
}

DLL_LOCAL VALUE _getPassword(int argc,VALUE *argv,VALUE self)
{
	VALUE parent,hash;
	rb_scan_args(argc, argv, "02",&parent,&hash);

	app_protected();

	wxString message;
	wxString caption(wxGetTextFromUserPromptStr);
	wxString value;
	wxPoint pos = wxDefaultPosition;
	bool centre(true);

	if(rb_obj_is_kind_of(hash,rb_cHash))
	{
		set_hash_option(hash,"message",message);
		set_hash_option(hash,"caption",caption);

		set_hash_option(hash,"value",value);

		set_hash_option(hash,"pos",pos);
		set_hash_option(hash,"center",centre);
	}

	return wrap(wxGetPasswordFromUser(
		message,caption,
		value, unwrap<wxWindow*>(parent),
		pos.x,pos.y,centre));
}

}
}

#endif

DLL_LOCAL void Init_WXPasswordEntryDialog(VALUE rb_mWX)
{
#if 0
	rb_cWXTextEntryDialog = rb_define_class_under(rb_mWX,"TextEntryDialog",rb_cWXDialog);

	rb_define_attr(rb_cWXPasswordEntryDialog,"value",1,1);
#endif
#if wxUSE_TEXTDLG
	using namespace RubyWX::PasswordEntryDialog;
	rb_cWXPasswordEntryDialog = rb_define_class_under(rb_mWX,"PasswordEntryDialog",rb_cWXTextEntryDialog);
	rb_define_alloc_func(rb_cWXPasswordEntryDialog,_alloc);
	rb_define_method(rb_cWXPasswordEntryDialog,"initialize",RUBY_METHOD_FUNC(_initialize),-1);

	rb_define_module_function(rb_mWX,"password_dialog",RUBY_METHOD_FUNC(_getPassword),-1);

	registerInfo<wxPasswordEntryDialog>(rb_cWXPasswordEntryDialog);
#endif
}
