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
DLL_LOCAL void Init_WXCalendarCtrl(VALUE rb_mWX)
{
#if wxUSE_CALENDARCTRL
	using namespace RubyWX::CalendarCtrl;
	rb_cWXCalendarCtrl = rb_define_class_under(rb_mWX,"CalendarCtrl",rb_cWXCalendarCtrlBase);
	rb_define_alloc_func(rb_cWXCalendarCtrl,_alloc);

	rb_define_method(rb_cWXCalendarCtrl,"initialize",RUBY_METHOD_FUNC(_initialize),-1);

	registerInfo<wxCalendarCtrl>(rb_cWXCalendarCtrl);
#endif

}
