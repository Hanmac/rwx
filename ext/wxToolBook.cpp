/*
 * wxToolBook.cpp
 *
 *  Created on: 14.04.2012
 *      Author: hanmac
 */

#include "wxToolBook.hpp"
#include "wxBookCtrl.hpp"

VALUE rb_cWXToolbook;

#if wxUSE_TOOLBOOK
#define _self wrap<wxToolbook*>(self)

namespace RubyWX {
namespace Toolbook {

APP_PROTECT(wxToolbook)

VALUE _initialize(int argc,VALUE *argv,VALUE self)
{
	VALUE parent,hash;
	rb_scan_args(argc, argv, "11",&parent,&hash);

	if(!_created)
	{
		_self->Create(wrap<wxWindow*>(parent),wxID_ANY);
		_created = true;
	}
	rb_call_super(argc,argv);
	return self;
}

}
}
#endif
void Init_WXToolBookCtrl(VALUE rb_mWX)
{
#if wxUSE_TOOLBOOK
	using namespace RubyWX::Toolbook;
	rb_cWXToolbook = rb_define_class_under(rb_mWX,"ToolBookCtrl",rb_cWXBookCtrlBase);
	rb_define_alloc_func(rb_cWXToolbook,_alloc);

	rb_define_method(rb_cWXToolbook,"initialize",RUBY_METHOD_FUNC(_initialize),-1);

	registerType<wxToolbook>(rb_cWXToolbook);
#endif

}






