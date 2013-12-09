/*
 * wxListBook.cpp
 *
 *  Created on: 09.12.2012
 *      Author: hanmac
 */

#include "wxListBook.hpp"
#include "wxBookCtrl.hpp"

VALUE rb_cWXListbook;

#if wxUSE_LISTBOOK
#define _self unwrap<wxListbook*>(self)

namespace RubyWX {
namespace Listbook {

APP_PROTECT(wxListbook)

DLL_LOCAL VALUE _initialize(int argc,VALUE *argv,VALUE self)
{
	VALUE parent,hash;
	rb_scan_args(argc, argv, "11",&parent,&hash);

	if(!_created)
	{
		_self->Create(unwrap<wxWindow*>(parent),wxID_ANY);
		_created = true;
	}
	rb_call_super(argc,argv);
	return self;
}

}
}
#endif
DLL_LOCAL void Init_WXListBookCtrl(VALUE rb_mWX)
{
#if wxUSE_LISTBOOK
	using namespace RubyWX::Listbook;
	rb_cWXListbook = rb_define_class_under(rb_mWX,"ListBookCtrl",rb_cWXBookCtrlBase);
	rb_define_alloc_func(rb_cWXListbook,_alloc);

	rb_define_method(rb_cWXListbook,"initialize",RUBY_METHOD_FUNC(_initialize),-1);

	registerInfo<wxListbook>(rb_cWXListbook);

	registerEventType<wxBookCtrlEvent>("listbook_page_changed",wxEVT_LISTBOOK_PAGE_CHANGED);
	registerEventType<wxBookCtrlEvent>("listbook_page_changing",wxEVT_LISTBOOK_PAGE_CHANGING);
#endif

}





