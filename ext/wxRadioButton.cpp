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

DLL_LOCAL VALUE _initialize(int argc,VALUE *argv,VALUE self)
{
	VALUE parent,hash;
	int style = 0;
	bool group = false;
	rb_scan_args(argc, argv, "11",&parent,&hash);
	if(rb_obj_is_kind_of(hash,rb_cHash))
	{
		set_hash_option(hash,"style",style);
		set_hash_option(hash,"group",group);
		if(group)
			style |= wxRB_GROUP;
	}

	if(!rb_obj_is_kind_of(hash,rb_cString))
	{
		_self->Create(unwrap<wxWindow*>(parent),wxID_ANY,wxEmptyString,wxDefaultPosition,wxDefaultSize,style);
		
	}

	rb_call_super(argc,argv);
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
	rb_cWXControl = rb_define_class_under(rb_mWX,"Control",rb_cWXWindow);

	rb_define_attr(rb_cWXRadioButton,"value",1,1);
#endif

#if wxUSE_RADIOBTN
	using namespace RubyWX::RadioButton;
	rb_cWXRadioButton = rb_define_class_under(rb_mWX,"RadioButton",rb_cWXControl);
	rb_define_alloc_func(rb_cWXRadioButton,_alloc);

	rb_define_method(rb_cWXRadioButton,"initialize",RUBY_METHOD_FUNC(_initialize),-1);

	rb_define_attr_method(rb_cWXRadioButton,"value",_getValue,_setValue);

	rb_define_const(rb_cWXRadioButton,"GROUP",INT2NUM(wxRB_GROUP));

	registerInfo<wxRadioButton>(rb_cWXRadioButton);
	registerEventType("radiobutton",wxEVT_RADIOBUTTON);
#endif

}
