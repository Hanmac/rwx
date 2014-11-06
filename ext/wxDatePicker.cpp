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
 *   DatePicker.new(parent, name, [options])
 *   DatePicker.new(parent, [options])
 *
 * creates a new DatePicker widget.
 * ===Arguments
 * * parent of this window or nil
 * * name is a String describing a resource in a loaded xrc
 *
 * *options: Hash with possible options to set
 *
*/
DLL_LOCAL VALUE _initialize(int argc,VALUE *argv,VALUE self)
{
	VALUE parent,name,hash;
	rb_scan_args(argc, argv, "11:",&parent,&name,&hash);
	if(!_created && !rb_obj_is_kind_of(name,rb_cString)) {
		wxWindowID id(wxID_ANY);
		wxDateTime date(wxDefaultDateTime);
		int style(wxDP_DEFAULT | wxDP_SHOWCENTURY);

		if(rb_obj_is_kind_of(hash,rb_cHash)) {
			set_hash_option(hash,"id",id,unwrapID);
			set_hash_option(hash,"date",date);
			set_hash_option(hash,"style",style);
		}
		if(nil_check(parent))
			_self->Create(
				unwrap<wxWindow*>(parent),id,date,
				wxDefaultPosition,wxDefaultSize,
				style
			);

	}

	rb_call_super(argc,argv);

	if(rb_obj_is_kind_of(name,rb_cString) &&
		rb_obj_is_kind_of(hash,rb_cHash))
	{
		set_obj_option(hash, "date", &wxDatePickerCtrl::SetValue, _self);
	}
	
	rb_call_super(argc,argv);
	return self;
}

}
}
#endif


/* Document-attr: value
 * the value of the DatePicker. Time
 */
DLL_LOCAL void Init_WXDatePicker(VALUE rb_mWX)
{
#if 0
	rb_mWX = rb_define_module("WX");
	rb_cWXWindow = rb_define_class_under(rb_mWX,"Window",rb_cObject);

	rb_cWXControl = rb_define_class_under(rb_mWX,"Control",rb_cWXWindow);
#endif

#if wxUSE_DATEPICKCTRL
	using namespace RubyWX::DatePicker;
	rb_cWXDatePicker = rb_define_class_under(rb_mWX,"DatePicker",rb_cWXControl);
	rb_define_alloc_func(rb_cWXDatePicker,_alloc);

#if 0
	rb_define_attr(rb_cWXDatePicker,"value",1,1);
#endif

	rb_define_method(rb_cWXDatePicker,"initialize",RUBY_METHOD_FUNC(_initialize),-1);

	rb_define_attr_method(rb_cWXDatePicker,"value",_getValue,_setValue);

	registerInfo<wxDatePickerCtrl>(rb_cWXDatePicker);
#endif

}


