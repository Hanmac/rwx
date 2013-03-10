/*
 * wxInfobarBase.cpp
 *
 *  Created on: 09.03.2013
 *      Author: hanmac
 */


#include "wxInfoBarBase.hpp"

VALUE rb_cWXInfoBarBase;

#if wxUSE_INFOBAR
#define _self unwrap<wxInfoBarBase*>(self)

namespace RubyWX {
namespace InfoBarBase {

singlefunc(Dismiss)

VALUE _showMessage(int argc,VALUE *argv,VALUE self)
{
	VALUE text,icon;
	rb_scan_args(argc, argv, "11",&text,&icon);
	int flags = wxICON_INFORMATION;
	_self->ShowMessage(unwrap<wxString>(text),flags);
	return self;

}

}
}
#endif
void Init_WXInfoBarBase(VALUE rb_mWX)
{
#if wxUSE_TIMEPICKCTRL
	using namespace RubyWX::InfoBarBase;
	rb_cWXInfoBarBase = rb_define_class_under(rb_mWX,"InfoBarBase",rb_cWXControl);
	rb_undef_alloc_func(rb_cWXInfoBarBase);

	rb_define_method(rb_cWXInfoBarBase,"dismiss",RUBY_METHOD_FUNC(_Dismiss),0);
	rb_define_method(rb_cWXInfoBarBase,"show_message",RUBY_METHOD_FUNC(_showMessage),-1);

	registerType<wxInfoBarBase>(rb_cWXInfoBarBase);
#endif

}


