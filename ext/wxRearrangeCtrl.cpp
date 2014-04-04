/*
 * wxRearrangeCtrl.cpp
 *
 *  Created on: 14.03.2013
 *      Author: hanmac
 */

#include "wxRearrangeCtrl.hpp"

VALUE rb_cWXRearrangeCtrl;

#if wxUSE_REARRANGECTRL

namespace RubyWX {
namespace RearrangeCtrl {
#define _self unwrap<wxRearrangeCtrl*>(self)

APP_PROTECT(wxRearrangeCtrl)


singlereturn(GetList)

/*
 * call-seq:
 *   RearrangeCtrl.new(parent, name, [options])
 *   RearrangeCtrl.new(parent, [options])
 *
 * creates a new RearrangeCtrl widget.
 * ===Arguments
 * * parent of this window or nil
 * * name is a String describing a resource in a loaded xrc
 *
 * *options: Hash with possible options to set
 *   * items [string]
 *   * order [Integer]
 *
*/
DLL_LOCAL VALUE _initialize(int argc,VALUE *argv,VALUE self)
{
	VALUE parent,name,hash;
	rb_scan_args(argc, argv, "11:",&parent,&name,&hash);
	if(!_created && !rb_obj_is_kind_of(name,rb_cString))
	{
		wxWindowID id(wxID_ANY);
		wxArrayString items;
		wxArrayInt order;
		int style(0);

		if(rb_obj_is_kind_of(hash,rb_cHash))
		{
			set_hash_option(hash,"items",items);
			set_hash_option(hash,"order",order);
			order.resize(items.size());
			set_hash_option(hash,"style",style);
		}

		_self->Create(unwrap<wxWindow*>(parent),id,wxDefaultPosition,wxDefaultSize,order,items,style);
		
	}
	rb_call_super(argc,argv);
	return self;
}

}
}

#endif
DLL_LOCAL void Init_WXRearrangeCtrl(VALUE rb_mWX)
{
#if 0
	rb_mWX = rb_define_module("WX");
	rb_cWXWindow = rb_define_class_under(rb_mWX,"Window",rb_cObject);

	rb_cWXPanel = rb_define_class_under(rb_mWX,"Panel",rb_cWXWindow);
#endif

#if wxUSE_REARRANGECTRL
	using namespace RubyWX::RearrangeCtrl;
	rb_cWXRearrangeCtrl = rb_define_class_under(rb_mWX,"RearrangeCtrl",rb_cWXPanel);
	rb_define_alloc_func(rb_cWXRearrangeCtrl,_alloc);

	rb_define_method(rb_cWXRearrangeCtrl,"initialize",RUBY_METHOD_FUNC(_initialize),-1);

	registerInfo<wxRearrangeCtrl>(rb_cWXRearrangeCtrl);
#endif

}


