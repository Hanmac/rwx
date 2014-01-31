/*
 * wxTextCtrl.cpp
 *
 *  Created on: 27.04.2012
 *      Author: hanmac
 */

#include "wxTextCtrl.hpp"

#include "wxTextArea.hpp"
#include "wxTextEntry.hpp"

VALUE rb_cWXTextCtrl;

#if wxUSE_TEXTCTRL
#define _self unwrap<wxTextCtrl*>(self)

namespace RubyWX {
namespace TextCtrl {

macro_attr(Value,wxString)

singlereturn(IsSingleLine)
singlereturn(IsMultiLine)

APP_PROTECT(wxTextCtrl)


/*
 * call-seq:
 *   TextCtrl.new(parent, name, [options])
 *   TextCtrl.new(parent, [options])
 *
 * creates a new TextCtrl widget.
 * ===Arguments
 * * parent of this window or nil
 * * name is a String describing a resource in a loaded xrc
 *
 * *options: Hash with possible options to set:
 *   * value String
 *
*/
DLL_LOCAL VALUE _initialize(int argc,VALUE *argv,VALUE self)
{
	VALUE parent, name, hash;
	rb_scan_args(argc, argv, "11:",&parent,&name,&hash);
	if(!_created && !rb_obj_is_kind_of(name,rb_cString))
	{
		wxWindowID id(wxID_ANY);
		wxString value(wxEmptyString);
		int style(0);

		if(rb_obj_is_kind_of(hash,rb_cHash))
		{
			set_hash_option(hash,"id",id,unwrapID);
			set_hash_option(hash,"value",value);
			set_hash_option(hash,"style",style);

			set_hash_flag_option(hash,"readonly",wxTE_READONLY,style);
			set_hash_flag_option(hash,"multiline",wxTE_MULTILINE,style);
			set_hash_flag_option(hash,"password",wxTE_PASSWORD,style);
			set_hash_flag_option(hash,"process_tab",wxTE_PROCESS_TAB,style);
			set_hash_flag_option(hash,"process_enter",wxTE_PROCESS_ENTER,style);

			if(((style & wxTE_MULTILINE) != 0) && ((style & (wxTE_PASSWORD|wxTE_PROCESS_ENTER)) != 0) )
			{
				rb_raise(rb_eArgError,"password style and process enter style are for singleline only");
			}
		}

		_self->Create(unwrap<wxWindow*>(parent),id,value,wxDefaultPosition,wxDefaultSize,style);
	}

	rb_call_super(argc,argv);

	if(rb_obj_is_kind_of(name,rb_cString) &&
		rb_obj_is_kind_of(hash,rb_cHash))
	{
		VALUE temp;
		set_option(value,Value,wxString)
	}

	return self;
}

}
}
#endif


/* Document-attr: value
 * the value of the TextCtrl. String
 */
DLL_LOCAL void Init_WXTextCtrl(VALUE rb_mWX)
{
#if 0
	rb_cWXControl = rb_define_class_under(rb_mWX,"Control",rb_cWXWindow);

	rb_mWXTextArea = rb_define_module_under(rb_mWX,"TextArea");
	rb_mWXTextEntry = rb_define_module_under(rb_mWX,"TextEntry");

	rb_define_attr(rb_cWXTextCtrl,"value",1,1);
#endif

#if wxUSE_TEXTCTRL
	using namespace RubyWX::TextCtrl;
	rb_cWXTextCtrl = rb_define_class_under(rb_mWX,"TextCtrl",rb_cWXControl);
	rb_define_alloc_func(rb_cWXTextCtrl,_alloc);

	rb_include_module(rb_cWXTextCtrl,rb_mWXTextArea);
	rb_include_module(rb_cWXTextCtrl,rb_mWXTextEntry);

	rb_define_method(rb_cWXTextCtrl,"initialize",RUBY_METHOD_FUNC(_initialize),-1);

	rb_define_method(rb_cWXTextCtrl,"single_line?",RUBY_METHOD_FUNC(_IsSingleLine),0);
	rb_define_method(rb_cWXTextCtrl,"multi_line?",RUBY_METHOD_FUNC(_IsMultiLine),0);

	rb_define_attr_method(rb_cWXTextCtrl,"value",_getValue,_setValue);

	rb_define_const(rb_cWXTextCtrl,"READONLY",INT2NUM(wxTE_READONLY));
	rb_define_const(rb_cWXTextCtrl,"MULTILINE",INT2NUM(wxTE_MULTILINE));
	rb_define_const(rb_cWXTextCtrl,"PROCESS_TAB",INT2NUM(wxTE_PROCESS_TAB));
	rb_define_const(rb_cWXTextCtrl,"PROCESS_ENTER",INT2NUM(wxTE_PROCESS_ENTER));
	rb_define_const(rb_cWXTextCtrl,"PASSWORD",INT2NUM(wxTE_PASSWORD));

	registerInfo<wxTextCtrl>(rb_cWXTextCtrl);
#endif

}
