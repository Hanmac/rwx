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

/*
 * call-seq:
 *   ListBookCtrl.new(parent, name, [options])
 *   ListBookCtrl.new(parent, [options])
 *
 * creates a new ListBookCtrl widget.
 * ===Arguments
 * * parent of this window or nil
 * * name is a String describing a resource in a loaded xrc
 *
 * *options: Hash with possible options to set
*/
DLL_LOCAL VALUE _initialize(int argc,VALUE *argv,VALUE self)
{
	VALUE parent,name,hash;

	rb_scan_args(argc, argv, "11:",&parent,&name,&hash);
	if(!_created && !rb_obj_is_kind_of(name,rb_cString))
	{
		wxWindowID id(wxID_ANY);
		int style(0);

		if(rb_obj_is_kind_of(hash,rb_cHash))
		{
			set_hash_option(hash,"id",id,unwrapID);
			set_hash_option(hash,"style",style);
		}
		
		if(nil_check(parent))
			_self->Create(unwrap<wxWindow*>(parent),id,
				wxDefaultPosition,wxDefaultSize,style
			);

	}

	rb_call_super(argc,argv);

	return self;
}

}
}
#endif
DLL_LOCAL void Init_WXListBookCtrl(VALUE rb_mWX)
{
#if 0
	rb_mWX = rb_define_module("WX");
	rb_mWXEvtHandler = rb_define_module_under(rb_mWX,"EvtHandler");

	rb_cWXWindow = rb_define_class_under(rb_mWX,"Window",rb_cObject);
	rb_cWXControl = rb_define_class_under(rb_mWX,"Control",rb_cWXWindow);
	rb_cWXBookCtrlBase = rb_define_class_under(rb_mWX,"BookCtrl",rb_cWXControl);
#endif

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





