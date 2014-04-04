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

// wxPasswordEntryDialog can not be used directly try to fake it
#ifdef HAVE_WXPASSWORDENTRYDIALOG
#define _self unwrap<wxPasswordEntryDialog*>(self)
#else
#define _self unwrap<wxTextEntryDialog*>(self)
#endif

namespace RubyWX {
namespace PasswordEntryDialog {

#ifdef HAVE_WXPASSWORDENTRYDIALOG
APP_PROTECT(wxPasswordEntryDialog)
#else
APP_PROTECT(wxTextEntryDialog)
#endif
/*
 * call-seq:
 *   PasswordEntryDialog.new(parent, name, [options])
 *   PasswordEntryDialog.new(parent, [options])
 *
 * creates a new PasswordEntryDialog widget.
 * ===Arguments
 * * parent of this window or nil
 * * name is a String describing a resource in a loaded xrc
 *
 * *options: Hash with possible options to set:
 *   * path String default path
 *   * message String
 *
*/
DLL_LOCAL VALUE _initialize(int argc,VALUE *argv,VALUE self)
{
	VALUE parent,name,hash;
	rb_scan_args(argc, argv, "11:",&parent,&name,&hash);
	if(!_created && !rb_obj_is_kind_of(name,rb_cString))
	{
		wxString message;
		wxString caption(wxGetPasswordFromUserPromptStr);
		wxString value;
		long style(wxTextEntryDialogStyle);

		if(rb_obj_is_kind_of(hash,rb_cHash))
		{
			set_hash_option(hash,"message",message);
			set_hash_option(hash,"caption",caption);

			set_hash_option(hash,"value",value);

			set_hash_option(hash,"style",style);
		}
#ifdef HAVE_WXPASSWORDENTRYDIALOG
		_self->Create(unwrap<wxWindow*>(parent),
				message,caption,value,style);
#else
		_self->Create(unwrap<wxWindow*>(parent),
				message,caption,value,style | wxTE_PASSWORD);
#endif
		
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
	wxPoint pos(wxDefaultPosition);
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
	rb_mWX = rb_define_module("WX");
	rb_cWXWindow = rb_define_class_under(rb_mWX,"Window",rb_cObject);

	rb_cWXTopLevel = rb_define_class_under(rb_mWX,"TopLevel",rb_cWXWindow);
	rb_cWXDialog = rb_define_class_under(rb_mWX,"Dialog",rb_cWXTopLevel);

	rb_cWXTextEntryDialog = rb_define_class_under(rb_mWX,"TextEntryDialog",rb_cWXDialog);
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
