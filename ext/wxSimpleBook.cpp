/*
 * wxSimpleBook.cpp
 *
 *  Created on: 30.03.2012
 *      Author: hanmac
 */

#include "wxSimpleBook.hpp"
#include "wxBookCtrl.hpp"

VALUE rb_cWXSimplebook;

#if wxUSE_BOOKCTRL
#define _self unwrap<wxSimplebook*>(self)

namespace RubyWX {
namespace Simplebook {

APP_PROTECT(wxSimplebook)

/*
 * call-seq:
 *   SimpleBookCtrl.new(parent, name, [options])
 *   SimpleBookCtrl.new(parent, [options])
 *
 * creates a new SimpleBookCtrl widget.
 * ===Arguments
 * * parent of this window or nil
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

		if(nil_check(parent)) {
			_self->Create(unwrap<wxWindow*>(parent),id,
				wxDefaultPosition,wxDefaultSize,style
			);
		}
		
	}

	rb_call_super(argc,argv);

	return self;
}

}
}
#endif
DLL_LOCAL void Init_WXSimpleBookCtrl(VALUE rb_mWX)
{
#if 0
	rb_mWX = rb_define_module("WX");
	rb_mWXEvtHandler = rb_define_module_under(rb_mWX,"EvtHandler");

	rb_cWXWindow = rb_define_class_under(rb_mWX,"Window",rb_cObject);
	rb_cWXControl = rb_define_class_under(rb_mWX,"Control",rb_cWXWindow);
	rb_cWXBookCtrlBase = rb_define_class_under(rb_mWX,"BookCtrl",rb_cWXControl);
#endif

#if wxUSE_BOOKCTRL
	using namespace RubyWX::Simplebook;
	rb_cWXSimplebook = rb_define_class_under(rb_mWX,"SimpleBookCtrl",rb_cWXBookCtrlBase);
	rb_define_alloc_func(rb_cWXSimplebook,_alloc);

	rb_define_method(rb_cWXSimplebook,"initialize",RUBY_METHOD_FUNC(_initialize),-1);

	registerInfo<wxSimplebook>(rb_cWXSimplebook);

	//registerEventType<wxBookCtrlEvent>("notebook_page_changed",wxEVT_NOTEBOOK_PAGE_CHANGED);
	//registerEventType<wxBookCtrlEvent>("notebook_page_changing",wxEVT_NOTEBOOK_PAGE_CHANGING);
#endif

}





