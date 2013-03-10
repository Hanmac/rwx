/*
 * wxInfobar.cpp
 *
 *  Created on: 09.03.2013
 *      Author: hanmac
 */

#include "wxInfoBar.hpp"

VALUE rb_cWXInfoBar;

#if wxUSE_INFOBAR
#define _self unwrap<wxInfoBar*>(self)

namespace RubyWX {
namespace InfoBar {

APP_PROTECT(wxInfoBar)

VALUE _initialize(int argc,VALUE *argv,VALUE self)
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
void Init_WXInfoBar(VALUE rb_mWX)
{
#if wxUSE_CALENDARCTRL
	using namespace RubyWX::InfoBar;
	rb_cWXInfoBar = rb_define_class_under(rb_mWX,"InfoBar",rb_cWXInfoBarBase);
	rb_define_alloc_func(rb_cWXInfoBar,_alloc);

	rb_define_method(rb_cWXInfoBar,"initialize",RUBY_METHOD_FUNC(_initialize),-1);

	registerInfo<wxInfoBar>(rb_cWXInfoBar);
#endif

}

