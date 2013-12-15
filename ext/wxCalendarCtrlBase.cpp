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

/*
 * call-seq:
 *   CalendarCtrlBase.new(parent, options)
 *
 * creates a new CalendarCtrlBase widget.
 * ===Arguments
 * * parent of this window or nil
 *
 * *options: Hash with possible options to set:
 * * * date Time default time
 *
*/
DLL_LOCAL VALUE _initialize(int argc,VALUE *argv,VALUE self)
{
	VALUE parent,hash;
	rb_scan_args(argc, argv, "11",&parent,&hash);

	rb_call_super(argc,argv);

	if(rb_obj_is_kind_of(hash,rb_cHash))
	{
		VALUE temp;
		set_option(date,Date,wxDateTime)
	}
	return self;
}

}
}
#endif
DLL_LOCAL void Init_WXCalendarCtrlBase(VALUE rb_mWX)
{
#if 0
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
