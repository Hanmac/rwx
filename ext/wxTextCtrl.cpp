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

void set_style_flags(VALUE hash,int& style)
{
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

			set_style_flags(hash,style);
		}

		if(nil_check(parent)) {
			_self->Create(unwrap<wxWindow*>(parent),id,value,
				wxDefaultPosition,wxDefaultSize,style
			);
		}
	}

	rb_call_super(argc,argv);

	if(rb_obj_is_kind_of(name,rb_cString) &&
		rb_obj_is_kind_of(hash,rb_cHash))
	{
		set_obj_option(hash,"value", &wxTextCtrl::SetValue, _self);
	}

	return self;
}

}
}
#endif


/* Document-attr: value
 * the value of the TextCtrl. String
 */

/* Document-method: single_line?
 * call-seq:
 *   single_line? -> true/false
 *
 * Returns true if this is a single line edit control and false otherwise.
 * ===Return value
 * true/false
*/
/* Document-method: multi_line?
 * call-seq:
 *   multi_line? -> true/false
 *
 * Returns true if this is a multi line edit control and false otherwise.
 * ===Return value
 * true/false
*/

/* Document-const: READONLY
 *   The text will not be user-editable.
 */
/* Document-const: MULTILINE
 *   The text control allows multiple lines. If this style is not specified, line break characters should not be used in the controls value.
 */
/* Document-const: PROCESS_TAB
 *   The control will receive :char events for TAB pressed - normally, TAB is used for passing to the next control in a dialog instead. For the control created with this style, you can still use Ctrl-Enter to pass to the next control from the keyboard.
 */
/* Document-const: PROCESS_ENTER
 *  The control will generate the event :text_enter (otherwise pressing Enter key is either processed internally by the control or used for navigation between dialog controls).
 */
/* Document-const: PASSWORD
 *   The text will be echoed as asterisks.
 */
DLL_LOCAL void Init_WXTextCtrl(VALUE rb_mWX)
{
#if 0
	rb_mWX = rb_define_module("WX");
	rb_cWXWindow = rb_define_class_under(rb_mWX,"Window",rb_cObject);

	rb_cWXControl = rb_define_class_under(rb_mWX,"Control",rb_cWXWindow);

	rb_mWXTextArea = rb_define_module_under(rb_mWX,"TextArea");
	rb_mWXTextEntry = rb_define_module_under(rb_mWX,"TextEntry");
#endif

#if wxUSE_TEXTCTRL
	using namespace RubyWX::TextCtrl;
	rb_cWXTextCtrl = rb_define_class_under(rb_mWX,"TextCtrl",rb_cWXControl);
	rb_define_alloc_func(rb_cWXTextCtrl,_alloc);

#if 0
	rb_define_attr(rb_cWXTextCtrl,"value",1,1);
#endif

	rb_include_module(rb_cWXTextCtrl,rb_mWXTextArea);
	rb_include_module(rb_cWXTextCtrl,rb_mWXTextEntry);

	rb_define_method(rb_cWXTextCtrl,"initialize",RUBY_METHOD_FUNC(_initialize),-1);

	rb_define_method(rb_cWXTextCtrl,"single_line?",RUBY_METHOD_FUNC(_IsSingleLine),0);
	rb_define_method(rb_cWXTextCtrl,"multi_line?",RUBY_METHOD_FUNC(_IsMultiLine),0);

	rb_define_attr_method(rb_cWXTextCtrl,"value",_getValue,_setValue);

	rb_define_const(rb_cWXTextCtrl,"READONLY",RB_INT2NUM(wxTE_READONLY));
	rb_define_const(rb_cWXTextCtrl,"MULTILINE",RB_INT2NUM(wxTE_MULTILINE));
	rb_define_const(rb_cWXTextCtrl,"PROCESS_TAB",RB_INT2NUM(wxTE_PROCESS_TAB));
	rb_define_const(rb_cWXTextCtrl,"PROCESS_ENTER",RB_INT2NUM(wxTE_PROCESS_ENTER));
	rb_define_const(rb_cWXTextCtrl,"PASSWORD",RB_INT2NUM(wxTE_PASSWORD));

	registerEventType("text",wxEVT_TEXT,rb_cWXCommandEvent);
	registerEventType("text_enter",wxEVT_TEXT_ENTER,rb_cWXCommandEvent);
	registerEventType("text_url",wxEVT_TEXT_URL,rb_cWXCommandEvent);
	registerEventType("text_maxlen",wxEVT_TEXT_MAXLEN,rb_cWXCommandEvent);

	registerInfo<wxTextCtrl>(rb_cWXTextCtrl);
#endif

}
