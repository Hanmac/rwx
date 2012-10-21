/*
 * wxCalendarCtrlBase.cpp
 *
 *  Created on: 08.05.2012
 *      Author: hanmac
 */

#include "wxCalendarCtrlBase.hpp"

VALUE rb_cWXCalendarCtrlBase;

#if wxUSE_TIMEPICKCTRL
#define _self unwrap<wxCalendarCtrlBase*>(self)

namespace RubyWX {
namespace CalendarCtrlBase {

macro_attr(Date,wxDateTime)

}
}
#endif
void Init_WXCalendarCtrlBase(VALUE rb_mWX)
{
#if wxUSE_TIMEPICKCTRL
	using namespace RubyWX::CalendarCtrlBase;
	rb_cWXCalendarCtrlBase = rb_define_class_under(rb_mWX,"CalendarCtrlBase",rb_cWXControl);
	rb_undef_alloc_func(rb_cWXCalendarCtrlBase);

	rb_define_attr_method(rb_cWXCalendarCtrlBase,"date",_getDate,_setDate);

	registerType<wxCalendarCtrlBase>(rb_cWXCalendarCtrlBase);
#endif

}
