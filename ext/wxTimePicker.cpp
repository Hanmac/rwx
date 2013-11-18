/*
 * wxTimePickerCtrl.cpp
 *
 *  Created on: 02.03.2012
 *      Author: hanmac
 */


#include "wxTimePicker.hpp"

VALUE rb_cWXTimePicker;

#if wxUSE_TIMEPICKCTRL
#define _self unwrap<wxTimePickerCtrl*>(self)

namespace RubyWX {
namespace TimePicker {

macro_attr(Value,wxDateTime)

APP_PROTECT(wxTimePickerCtrl)

DLL_LOCAL VALUE _initialize(int argc,VALUE *argv,VALUE self)
{
	VALUE parent,hash;
	rb_scan_args(argc, argv, "11",&parent,&hash);
	_self->Create(unwrap<wxWindow*>(parent),wxID_ANY);
	_created = true;
	rb_call_super(argc,argv);
	return self;
}

}
}
#endif
DLL_LOCAL void Init_WXTimePicker(VALUE rb_mWX)
{
#if wxUSE_TIMEPICKCTRL
	using namespace RubyWX::TimePicker;
	rb_cWXTimePicker = rb_define_class_under(rb_mWX,"TimePicker",rb_cWXControl);
	rb_define_alloc_func(rb_cWXTimePicker,_alloc);

	rb_define_method(rb_cWXTimePicker,"initialize",RUBY_METHOD_FUNC(_initialize),-1);

	rb_define_attr_method(rb_cWXTimePicker,"value",_getValue,_setValue);

	registerInfo<wxTimePickerCtrl>(rb_cWXTimePicker);
#endif

}


