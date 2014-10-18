/*
 * wxRadioButton.cpp
 *
 *  Created on: 03.03.2012
 *      Author: hanmac
 */


#include "wxRadioButton.hpp"

VALUE rb_cWXRadioButton;

#if wxUSE_RADIOBTN
#define _self unwrap<wxRadioButton*>(self)

namespace RubyWX {
namespace RadioButton {

macro_attr(Value,bool)

APP_PROTECT(wxRadioButton)

/*
 * call-seq:
 *   RadioButton.new(parent, name, [options])
 *   RadioButton.new(parent, [options])
 *
 * creates a new RadioButton widget.
 * ===Arguments
 * * parent of this window or nil
 * * name is a String describing a resource in a loaded xrc
 *
 * *options: Hash with possible options to set:
 *   * group true/false says to start a new Radio group
 *   * value true/false says if this element is checked
 *
*/
DLL_LOCAL VALUE _initialize(int argc,VALUE *argv,VALUE self)
{
	VALUE parent,name,hash;
	rb_scan_args(argc, argv, "11:",&parent,&name,&hash);
	if(!rb_obj_is_kind_of(name,rb_cString))
	{
		wxWindowID id(wxID_ANY);
		int style(0);

		wxString label(wxEmptyString);

		if(rb_obj_is_kind_of(hash,rb_cHash))
		{
			set_hash_option(hash,"id",id,unwrapID);
			set_hash_option(hash,"style",style);

			set_hash_option(hash,"label",label);

			set_hash_flag_option(hash,"group",wxRB_GROUP,style);
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


/* Document-attr: value
 * the value of the RadioButton. bool
 */
DLL_LOCAL void Init_WXRadioButton(VALUE rb_mWX)
{
#if 0
	rb_mWX = rb_define_module("WX");
	rb_cWXWindow = rb_define_class_under(rb_mWX,"Window",rb_cObject);

	rb_cWXControl = rb_define_class_under(rb_mWX,"Control",rb_cWXWindow);
#endif

#if wxUSE_RADIOBTN
	using namespace RubyWX::RadioButton;
	rb_cWXRadioButton = rb_define_class_under(rb_mWX,"RadioButton",rb_cWXControl);
	rb_define_alloc_func(rb_cWXRadioButton,_alloc);

#if 0
	rb_define_attr(rb_cWXRadioButton,"value",1,1);
#endif

	rb_define_method(rb_cWXRadioButton,"initialize",RUBY_METHOD_FUNC(_initialize),-1);

	rb_define_attr_method(rb_cWXRadioButton,"value",_getValue,_setValue);

	rb_define_const(rb_cWXRadioButton,"GROUP",INT2NUM(wxRB_GROUP));

	registerInfo<wxRadioButton>(rb_cWXRadioButton);
	registerEventType("radiobutton",wxEVT_RADIOBUTTON);
#endif

}
