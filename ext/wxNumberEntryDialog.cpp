/*
 * wxNumberEntryDialog.cpp
 *
 *  Created on: 24.12.2013
 *      Author: hanmac
 */


#include "wxNumberEntryDialog.hpp"
#include "wxPoint.hpp"
#include "wxApp.hpp"

VALUE rb_cWXNumberEntryDialog;
#if wxUSE_NUMBERDLG
#define _self unwrap<wxNumberEntryDialog*>(self)

namespace RubyWX {
namespace NumberEntryDialog {

APP_PROTECT(wxNumberEntryDialog)

singlereturn(GetValue)

/*
 * call-seq:
 *   NumberEntryDialog.new(parent, [options])
 *
 * creates a new NumberEntryDialog widget.
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
		wxString message,prompt,caption;
		long value = 0, min = 0, max = 0;
		if(rb_obj_is_kind_of(hash,rb_cHash))
		{
			set_hash_option(hash,"message",message);
			set_hash_option(hash,"prompt",prompt);
			set_hash_option(hash,"caption",caption);

			set_hash_option(hash,"value",value);
			set_hash_option(hash,"min",min);
			set_hash_option(hash,"max",max);
		}
		_self->Create(unwrap<wxWindow*>(parent),
				message,prompt,caption,value,min,max);
		_created = true;
	}
	rb_call_super(argc,argv);

	return self;
}

DLL_LOCAL VALUE _getNumber(int argc,VALUE *argv,VALUE self)
{
	VALUE parent,hash;
	rb_scan_args(argc, argv, "02",&parent,&hash);

	app_protected();

	wxString message,prompt,caption;
	long value = 0, min = 0, max = 100;
	wxPoint pos = wxDefaultPosition;

	if(rb_obj_is_kind_of(hash,rb_cHash))
	{
		set_hash_option(hash,"message",message);
		set_hash_option(hash,"prompt",prompt);
		set_hash_option(hash,"caption",caption);

		set_hash_option(hash,"value",value);
		set_hash_option(hash,"min",min);
		set_hash_option(hash,"max",max);

		set_hash_option(hash,"pos",pos);
	}

	return wrap(wxGetNumberFromUser(
			message,prompt,caption,
			value,min,max,
			unwrap<wxWindow*>(parent),pos));
}

}
}

#endif

DLL_LOCAL void Init_WXNumberEntryDialog(VALUE rb_mWX)
{
#if wxUSE_NUMBERDLG
	using namespace RubyWX::NumberEntryDialog;
	rb_cWXNumberEntryDialog = rb_define_class_under(rb_mWX,"NumberEntryDialog",rb_cWXDialog);
	rb_define_alloc_func(rb_cWXNumberEntryDialog,_alloc);
	rb_define_method(rb_cWXNumberEntryDialog,"initialize",RUBY_METHOD_FUNC(_initialize),-1);

	rb_define_method(rb_cWXNumberEntryDialog,"value",RUBY_METHOD_FUNC(_GetValue),0);

	rb_define_module_function(rb_mWX,"number_dialog",RUBY_METHOD_FUNC(_getNumber),-1);

	registerInfo<wxNumberEntryDialog>(rb_cWXNumberEntryDialog);
#endif
}
