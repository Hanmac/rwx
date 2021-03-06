/*
 * wxCalendarCtrl.cpp
 *
 *  Created on: 08.05.2012
 *      Author: hanmac
 */

#include "wxCalendarCtrl.hpp"

VALUE rb_cWXCalendarCtrl;

#if wxUSE_CALENDARCTRL
#define _self unwrap<wxCalendarCtrl*>(self)

namespace RubyWX {
namespace CalendarCtrl {

APP_PROTECT(wxCalendarCtrl)

/*
 * call-seq:
 *   CalendarCtrl.new(parent, name, [options])
 *   CalendarCtrl.new(parent, [options])
 *
 * creates a new CalendarCtrl widget.
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

		CalendarCtrlBase::_set_options(hash,id, date, style);

		if(nil_check(parent)) {
			_self->Create(
				unwrap<wxWindow*>(parent),id,date,
				wxDefaultPosition,wxDefaultSize,
				style
			);
		}
	}

	rb_call_super(argc,argv);

	return self;
}


}
}
#endif
DLL_LOCAL void Init_WXCalendarCtrl(VALUE rb_mWX)
{
#if 0
	rb_mWX = rb_define_module("WX");
	rb_cWXWindow = rb_define_class_under(rb_mWX,"Window",rb_cObject);

	rb_cWXControl = rb_define_class_under(rb_mWX,"Control",rb_cWXWindow);
	rb_cWXCalendarCtrlBase = rb_define_class_under(rb_mWX,"CalendarCtrlBase",rb_cWXControl);
#endif
#if wxUSE_CALENDARCTRL
	using namespace RubyWX::CalendarCtrl;
	rb_cWXCalendarCtrl = rb_define_class_under(rb_mWX,"CalendarCtrl",rb_cWXCalendarCtrlBase);
	rb_define_alloc_func(rb_cWXCalendarCtrl,_alloc);

	rb_define_method(rb_cWXCalendarCtrl,"initialize",RUBY_METHOD_FUNC(_initialize),-1);

	registerInfo<wxCalendarCtrl>(rb_cWXCalendarCtrl);
#endif

}
