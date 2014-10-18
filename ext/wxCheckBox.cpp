/*
 * wxCheckBox.cpp
 *
 *  Created on: 01.05.2012
 *      Author: hanmac
 */


#include "wxControl.hpp"

VALUE rb_cWXCheckBox;

#if wxUSE_CHECKBOX
#define _self unwrap<wxCheckBox*>(self)

namespace RubyWX {
namespace CheckBox {

macro_attr(Value,bool)
macro_attr_enum_con(3StateValue,wxCheckBoxState,Is3State)

singlereturn(Is3State)
singlereturn(Is3rdStateAllowedForUser)

APP_PROTECT(wxCheckBox)

/*
 * call-seq:
 *   CheckBox.new(parent, name, [options])
 *   CheckBox.new(parent, [options])
 *
 * creates a new CheckBox widget.
 * ===Arguments
 * * parent of this window or nil
 * * name is a String describing a resource in a loaded xrc
 *
 * *options: Hash with possible options to set:
 *   * value true/false says if this element is checked
 *
*/
DLL_LOCAL VALUE _initialize(int argc,VALUE *argv,VALUE self)
{
	VALUE parent,name,hash;

	rb_scan_args(argc, argv, "11:",&parent,&name,&hash);
	if(!_created && !rb_obj_is_kind_of(name,rb_cString))
	{
		wxWindowID id(wxID_ANY);
		int style(0);
		wxString label(wxEmptyString);

		if(rb_obj_is_kind_of(hash,rb_cHash))
		{
			set_hash_option(hash,"id",id,unwrapID);
			set_hash_option(hash,"style",style);
			set_hash_option(hash,"label",label);

			set_hash_flag_option(hash,"third_state",wxCHK_3STATE,style);
			set_hash_flag_option(hash,"third_state_user",wxCHK_ALLOW_3RD_STATE_FOR_USER,style);

			//add wxCHK_3STATE style if for user is used
			if((style & wxCHK_ALLOW_3RD_STATE_FOR_USER) != 0)
			{
				style |= wxCHK_3STATE;
			}

		}
		if(nil_check(parent)) {
			_self->Create(unwrap<wxWindow*>(parent),id,label,
				wxDefaultPosition,wxDefaultSize,style
			);
		}
	}
	
	rb_call_super(argc,argv);

	if(rb_obj_is_kind_of(hash,rb_cHash))
	{
		VALUE temp;
		set_option(value,Value,bool)
	}

	return self;
}

}
}
#endif


/* Document-method: third_state?
 * call-seq:
 *   third_state? -> true/false
 *
 * Returns whether or not the checkbox is a 3-state checkbox.
 * ===Return value
 * true/false
*/
/* Document-method: third_state_user?
 * call-seq:
 *   third_state_user? -> true/false
 *
 * Returns whether or not the user can set the checkbox to the third state.
 * ===Return value
 * true/false
*/

/* Document-attr: value
 * the value of the CheckBox. bool
 */
/* Document-attr: third_state
 * the value of the CheckBox then third-state is used. Symbol
 */

/* Document-const: THIRD_STATE
 *   Create a 3-state checkbox.
 */
/* Document-const: THIRD_STATE_USER
 *   Using this flags allows the user to set the checkbox to the third state by clicking.
 */
DLL_LOCAL void Init_WXCheckBox(VALUE rb_mWX)
{
#if 0
	rb_mWX = rb_define_module("WX");
	rb_cWXWindow = rb_define_class_under(rb_mWX,"Window",rb_cObject);

	rb_cWXControl = rb_define_class_under(rb_mWX,"Control",rb_cWXWindow);
#endif

#if wxUSE_CHECKBOX
	using namespace RubyWX::CheckBox;
	rb_cWXCheckBox = rb_define_class_under(rb_mWX,"CheckBox",rb_cWXControl);
	rb_define_alloc_func(rb_cWXCheckBox,_alloc);

#if 0
	rb_define_attr(rb_cWXCheckBox,"value",1,1);
	rb_define_attr(rb_cWXCheckBox,"third_state",1,1);
#endif

	rb_define_method(rb_cWXCheckBox,"initialize",RUBY_METHOD_FUNC(_initialize),-1);

	rb_define_attr_method(rb_cWXCheckBox,"value",_getValue,_setValue);
	rb_define_attr_method(rb_cWXCheckBox,"third_state",_get3StateValue,_set3StateValue);

	rb_define_method(rb_cWXCheckBox,"third_state?",RUBY_METHOD_FUNC(_Is3State),0);
	rb_define_method(rb_cWXCheckBox,"third_state_user?",RUBY_METHOD_FUNC(_Is3rdStateAllowedForUser),0);

	rb_define_const(rb_cWXCheckBox,"THIRD_STATE",INT2NUM(wxCHK_3STATE));
	rb_define_const(rb_cWXCheckBox,"THIRD_STATE_USER",INT2NUM(wxCHK_ALLOW_3RD_STATE_FOR_USER));

	registerInfo<wxCheckBox>(rb_cWXCheckBox);
	registerEventType("checkbox",wxEVT_CHECKBOX);

	registerEnum<wxCheckBoxState>("wxCheckBoxState")
		->add(wxCHK_UNCHECKED,"unchecked")
		->add(wxCHK_CHECKED,"checked")
		->add(wxCHK_UNDETERMINED,"undetermined");

#endif

}

