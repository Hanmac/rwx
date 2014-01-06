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
 *   CalendarCtrlGeneric.new(parent, options)
 *
 * creates a new CalendarCtrlGeneric widget.
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




