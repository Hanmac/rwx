/*
 * wxCalendarCtrlBase.cpp
 *
 *  Created on: 08.05.2012
 *      Author: hanmac
 */

#include "wxCalendarCtrlBase.hpp"

VALUE rb_cWXCalendarCtrlBase;

#if wxUSE_CALENDARCTRL
#define _self unwrap<wxCalendarCtrlBase*>(self)

template <>
wxCalendarCtrlBase* unwrap<wxCalendarCtrlBase*>(const VALUE &arg)
{
	return unwrapPtr<wxCalendarCtrlBase>(arg,rb_cWXCalendarCtrlBase);
}

namespace RubyWX {
namespace CalendarCtrlBase {

macro_attr(Date,wxDateTime)

}
}
#endif


/* Document-attr: date
 * the date of the CalendarCtrl. Time
 */
DLL_LOCAL void Init_WXCalendarCtrlBase(VALUE rb_mWX)
{
#if 0
	rb_mWX = rb_define_module("WX");
	rb_cWXWindow = rb_define_class_under(rb_mWX,"Window",rb_cObject);

	rb_cWXControl = rb_define_class_under(rb_mWX,"Control",rb_cWXWindow);

	rb_define_attr(rb_cWXCalendarCtrlBase,"date",1,1);
#endif

#if wxUSE_CALENDARCTRL
	using namespace RubyWX::CalendarCtrlBase;
	rb_cWXCalendarCtrlBase = rb_define_class_under(rb_mWX,"CalendarCtrlBase",rb_cWXControl);
	rb_undef_alloc_func(rb_cWXCalendarCtrlBase);

	rb_define_attr_method(rb_cWXCalendarCtrlBase,"date",_getDate,_setDate);

	registerType<wxCalendarCtrlBase>(rb_cWXCalendarCtrlBase);
#endif

}
