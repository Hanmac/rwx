/*
 * wxCalendarCtrlGeneric.cpp
 *
 *  Created on: 08.05.2012
 *      Author: hanmac
 */
#include "wxCalendarCtrlGeneric.hpp"
#include "wxCalendarCtrl.hpp"

VALUE rb_cWXCalendarCtrlGeneric;

#ifdef wxHAS_NATIVE_CALENDARCTRL

#define _self unwrap<wxGenericCalendarCtrl*>(self)

namespace RubyWX {
namespace GenericCalendarCtrl {

APP_PROTECT(wxGenericCalendarCtrl)

/*
 * call-seq:
 *   CalendarCtrlGeneric.new(parent, name, [options])
 *   CalendarCtrlGeneric.new(parent, [options])
 *
 * creates a new CalendarCtrlGeneric widget.
 * ===Arguments
 * * parent of this window or nil
 * * name is a String describing a resource in a loaded xrc
 *
 * *options: Hash with possible options to set:
 *   * date Time
 *
*/
DLL_LOCAL VALUE _initialize(int argc,VALUE *argv,VALUE self)
{
	VALUE parent,name,hash;
	rb_scan_args(argc, argv, "11:",&parent,&name,&hash);
	if(!_created && !rb_obj_is_kind_of(name,rb_cString)) {
		wxWindowID id(wxID_ANY);
		wxDateTime date(wxDefaultDateTime);
		int style(wxCAL_SHOW_HOLIDAYS);

		if(rb_obj_is_kind_of(hash,rb_cHash)) {
			set_hash_option(hash,"id",id,unwrapID);
			set_hash_option(hash,"date",date);
			set_hash_option(hash,"style",style);
		}

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
		VALUE temp;
		set_option(date,Date,wxDateTime)
	}
	return self;
}



}
}
#endif

DLL_LOCAL void Init_WXCalendarCtrlGeneric(VALUE rb_mWX)
{
#if wxUSE_CALENDARCTRL
#ifdef wxHAS_NATIVE_CALENDARCTRL
	using namespace RubyWX::GenericCalendarCtrl;
	rb_cWXCalendarCtrlGeneric = rb_define_class_under(rb_mWX,"CalendarCtrlGeneric",rb_cWXCalendarCtrlBase);
	rb_define_alloc_func(rb_cWXCalendarCtrlGeneric,_alloc);

	rb_define_method(rb_cWXCalendarCtrlGeneric,"initialize",RUBY_METHOD_FUNC(_initialize),-1);

	registerInfo<wxGenericCalendarCtrl>(rb_cWXCalendarCtrlGeneric);
#else
	rb_define_const(rb_mWX,"CalendarCtrlGeneric",rb_cWXCalendarCtrl);
#endif
#endif

}




