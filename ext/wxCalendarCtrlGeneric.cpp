/*
 * wxCalendarCtrlGeneric.cpp
 *
 *  Created on: 08.05.2012
 *      Author: hanmac
 */
#include "wxCalendarCtrlGeneric.hpp"

VALUE rb_cWXCalendarCtrlGeneric;

#if wxUSE_CALENDARCTRL
#define _self unwrap<wxGenericCalendarCtrl*>(self)

namespace RubyWX {
namespace GenericCalendarCtrl {

APP_PROTECT(wxGenericCalendarCtrl)

VALUE _initialize(int argc,VALUE *argv,VALUE self)
{
	VALUE parent,hash;
	rb_scan_args(argc, argv, "11",&parent,&hash);
	_self->Create(unwrap<wxWindow*>(parent),wxID_ANY);
	rb_call_super(argc,argv);
	return self;
}


}
}
#endif
void Init_WXCalendarCtrlGeneric(VALUE rb_mWX)
{
#if wxUSE_CALENDARCTRL
	using namespace RubyWX::GenericCalendarCtrl;
#ifdef wxHAS_NATIVE_CALENDARCTRL
	rb_cWXCalendarCtrlGeneric = rb_define_class_under(rb_mWX,"CalendarCtrlGeneric",rb_cWXCalendarCtrlBase);
	rb_define_alloc_func(rb_cWXCalendarCtrlGeneric,_alloc);

	rb_define_method(rb_cWXCalendarCtrlGeneric,"initialize",RUBY_METHOD_FUNC(_initialize),-1);

	registerInfo<wxGenericCalendarCtrl>(rb_cWXCalendarCtrlGeneric);
#else
	rb_define_const(rb_mWX,"CalendarCtrlGeneric",rb_cWXCalendarCtrl);
#endif
#endif

}




