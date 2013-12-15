/*
 * wxDatePickerCtrl.cpp
 *
 *  Created on: 02.03.2012
 *      Author: hanmac
 */


#include "wxDatePicker.hpp"

VALUE rb_cWXDatePicker;

#if wxUSE_DATEPICKCTRL
#define _self unwrap<wxDatePickerCtrl*>(self)

namespace RubyWX {
namespace DatePicker {

macro_attr(Value,wxDateTime)

APP_PROTECT(wxDatePickerCtrl)

/*
 * call-seq:
 *   DatePicker.new(parent, [options])
 *
 * creates a new DatePicker widget.
 * ===Arguments
 * * parent of this window or nil
 *
 * *options: Hash with possible options to set
 *
*/
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
DLL_LOCAL void Init_WXDatePicker(VALUE rb_mWX)
{
#if 0
	rb_define_attr(rb_cWXDatePicker,"value",1,1);
#endif

#if wxUSE_DATEPICKCTRL
	using namespace RubyWX::DatePicker;
	rb_cWXDatePicker = rb_define_class_under(rb_mWX,"DatePicker",rb_cWXControl);
	rb_define_alloc_func(rb_cWXDatePicker,_alloc);

	rb_define_method(rb_cWXDatePicker,"initialize",RUBY_METHOD_FUNC(_initialize),-1);

	rb_define_attr_method(rb_cWXDatePicker,"value",_getValue,_setValue);

	registerInfo<wxDatePickerCtrl>(rb_cWXDatePicker);
#endif

}


