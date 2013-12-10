/*
 * wxToggleButton.cpp
 *
 *  Created on: 10.12.2013
 *      Author: hanmac
 */

#include "wxToggleButton.hpp"

VALUE rb_cWXToggleButton;
#if wxUSE_TOGGLEBTN
#define _self unwrap<wxToggleButton*>(self)

namespace RubyWX {
namespace ToggleButton {

APP_PROTECT(wxToggleButton)

macro_attr(Value,int)

DLL_LOCAL VALUE _initialize(int argc,VALUE *argv,VALUE self)
{
	VALUE parent,hash;
	rb_scan_args(argc, argv, "11",&parent,&hash);

	if(!_created) {
		_self->Create(unwrap<wxWindow*>(parent),wxID_ANY,wxEmptyString);
		_created = true;
	}

	rb_call_super(argc,argv);
	return self;
}

}
}

#endif

DLL_LOCAL void Init_WXToggleButton(VALUE rb_mWX)
{
#if 0
	rb_define_attr(rb_cWXToggleButton,"value",1,1);
#endif

#if wxUSE_TOGGLEBTN
	using namespace RubyWX::ToggleButton;
	rb_cWXToggleButton = rb_define_class_under(rb_mWX,"ToggleButton",rb_cWXAnyButton);
	rb_define_alloc_func(rb_cWXToggleButton,_alloc);

	rb_define_method(rb_cWXToggleButton,"initialize",RUBY_METHOD_FUNC(_initialize),-1);

	rb_define_attr_method(rb_cWXToggleButton,"value",_getValue,_setValue);

	registerInfo<wxToggleButton>(rb_cWXToggleButton);

	registerEventType<wxCommandEvent>("toogle_button",wxEVT_TOGGLEBUTTON);

#endif
}
